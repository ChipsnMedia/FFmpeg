/*
 * MPEG-1/2 decoder
 * Copyright (c) 2000, 2001 Fabrice Bellard
 * Copyright (c) 2002-2004 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * MPEG-1/2 decoder
 */

#define UNCHECKED_BITSTREAM_READER 1

#include "libavutil/attributes.h"
#include "libavutil/avassert.h"
#include "libavutil/timecode.h"
#include "libavutil/thread.h"

#include "internal.h"
#include "avcodec.h"
#include "mpegvideo.h"
#include "error_resilience.h"
#include "mpeg12.h"
#include "mpeg12data.h"
#include "mpegvideodata.h"
#include "bytestream.h"
#include "thread.h"

static const uint8_t table_mb_ptype[7][2] = {
    { 3, 5 }, // 0x01 MB_INTRA
    { 1, 2 }, // 0x02 MB_PAT
    { 1, 3 }, // 0x08 MB_FOR
    { 1, 1 }, // 0x0A MB_FOR|MB_PAT
    { 1, 6 }, // 0x11 MB_QUANT|MB_INTRA
    { 1, 5 }, // 0x12 MB_QUANT|MB_PAT
    { 2, 5 }, // 0x1A MB_QUANT|MB_FOR|MB_PAT
};

static const uint8_t table_mb_btype[11][2] = {
    { 3, 5 }, // 0x01 MB_INTRA
    { 2, 3 }, // 0x04 MB_BACK
    { 3, 3 }, // 0x06 MB_BACK|MB_PAT
    { 2, 4 }, // 0x08 MB_FOR
    { 3, 4 }, // 0x0A MB_FOR|MB_PAT
    { 2, 2 }, // 0x0C MB_FOR|MB_BACK
    { 3, 2 }, // 0x0E MB_FOR|MB_BACK|MB_PAT
    { 1, 6 }, // 0x11 MB_QUANT|MB_INTRA
    { 2, 6 }, // 0x16 MB_QUANT|MB_BACK|MB_PAT
    { 3, 6 }, // 0x1A MB_QUANT|MB_FOR|MB_PAT
    { 2, 5 }, // 0x1E MB_QUANT|MB_FOR|MB_BACK|MB_PAT
};

av_cold void ff_init_2d_vlc_rl(RLTable *rl, unsigned static_size, int flags)
{
    int i;
    VLC_TYPE table[680][2] = {{0}};
    VLC vlc = { .table = table, .table_allocated = static_size };
    av_assert0(static_size <= FF_ARRAY_ELEMS(table));
    init_vlc(&vlc, TEX_VLC_BITS, rl->n + 2, &rl->table_vlc[0][1], 4, 2, &rl->table_vlc[0][0], 4, 2, INIT_VLC_USE_NEW_STATIC | flags);

    for (i = 0; i < vlc.table_size; i++) {
        int code = vlc.table[i][0];
        int len  = vlc.table[i][1];
        int level, run;

        if (len == 0) { // illegal code
            run   = 65;
            level = MAX_LEVEL;
        } else if (len<0) { //more bits needed
            run   = 0;
            level = code;
        } else {
            if (code == rl->n) { //esc
                run   = 65;
                level = 0;
            } else if (code == rl->n+1) { //eob
                run   = 0;
                level = 127;
            } else {
                run   = rl->table_run  [code] + 1;
                level = rl->table_level[code];
            }
        }
        rl->rl_vlc[0][i].len   = len;
        rl->rl_vlc[0][i].level = level;
        rl->rl_vlc[0][i].run   = run;
    }
}

av_cold void ff_mpeg12_common_init(MpegEncContext *s)
{

    s->y_dc_scale_table =
    s->c_dc_scale_table = ff_mpeg2_dc_scale_table[s->intra_dc_precision];

}

void ff_mpeg1_clean_buffers(MpegEncContext *s)
{
    //+clair remove 2021-11-11
    //s->last_dc[0] = 1 << (7 + s->intra_dc_precision);
    //-clair remove

    //+clair add 2021-11-11
    // [OKA-396] C&M IDCT reset value
    s->last_dc[0] = 0;
    //-clair add
    s->last_dc[1] = s->last_dc[0];
    s->last_dc[2] = s->last_dc[0];
    memset(s->last_mv, 0, sizeof(s->last_mv));
}


/******************************************/
/* decoding */

VLC ff_mv_vlc;

VLC ff_dc_lum_vlc;
VLC ff_dc_chroma_vlc;

VLC ff_mbincr_vlc;
VLC ff_mb_ptype_vlc;
VLC ff_mb_btype_vlc;
VLC ff_mb_pat_vlc;

static av_cold void mpeg12_init_vlcs(void)
{
    INIT_VLC_STATIC(&ff_dc_lum_vlc, DC_VLC_BITS, 12,
                    ff_mpeg12_vlc_dc_lum_bits, 1, 1,
                    ff_mpeg12_vlc_dc_lum_code, 2, 2, 512);
    INIT_VLC_STATIC(&ff_dc_chroma_vlc,  DC_VLC_BITS, 12,
                    ff_mpeg12_vlc_dc_chroma_bits, 1, 1,
                    ff_mpeg12_vlc_dc_chroma_code, 2, 2, 514);
    INIT_VLC_STATIC(&ff_mv_vlc, MV_VLC_BITS, 17,
                    &ff_mpeg12_mbMotionVectorTable[0][1], 2, 1,
                    &ff_mpeg12_mbMotionVectorTable[0][0], 2, 1, 266);
    INIT_VLC_STATIC(&ff_mbincr_vlc, MBINCR_VLC_BITS, 36,
                    &ff_mpeg12_mbAddrIncrTable[0][1], 2, 1,
                    &ff_mpeg12_mbAddrIncrTable[0][0], 2, 1, 538);
    INIT_VLC_STATIC(&ff_mb_pat_vlc, MB_PAT_VLC_BITS, 64,
                    &ff_mpeg12_mbPatTable[0][1], 2, 1,
                    &ff_mpeg12_mbPatTable[0][0], 2, 1, 512);

    INIT_VLC_STATIC(&ff_mb_ptype_vlc, MB_PTYPE_VLC_BITS, 7,
                    &table_mb_ptype[0][1], 2, 1,
                    &table_mb_ptype[0][0], 2, 1, 64);
    INIT_VLC_STATIC(&ff_mb_btype_vlc, MB_BTYPE_VLC_BITS, 11,
                    &table_mb_btype[0][1], 2, 1,
                    &table_mb_btype[0][0], 2, 1, 64);

    INIT_2D_VLC_RL(ff_rl_mpeg1, 680, 0);
    INIT_2D_VLC_RL(ff_rl_mpeg2, 674, 0);
}

av_cold void ff_mpeg12_init_vlcs(void)
{
    static AVOnce init_static_once = AV_ONCE_INIT;
    ff_thread_once(&init_static_once, mpeg12_init_vlcs);
}

/**
 * Find the end of the current frame in the bitstream.
 * @return the position of the first byte of the next frame, or -1
 */
int ff_mpeg1_find_frame_end(ParseContext *pc, const uint8_t *buf, int buf_size, AVCodecParserContext *s)
{
    int i;
    uint32_t state = pc->state;

    /* EOF considered as end of frame */
    if (buf_size == 0)
        return 0;

/*
 0  frame start         -> 1/4
 1  first_SEQEXT        -> 0/2
 2  first field start   -> 3/0
 3  second_SEQEXT       -> 2/0
 4  searching end
*/

    for (i = 0; i < buf_size; i++) {
        av_assert1(pc->frame_start_found >= 0 && pc->frame_start_found <= 4);
        if (pc->frame_start_found & 1) {
            if (state == EXT_START_CODE && (buf[i] & 0xF0) != 0x80)
                pc->frame_start_found--;
            else if (state == EXT_START_CODE + 2) {
                if ((buf[i] & 3) == 3)
                    pc->frame_start_found = 0;
                else
                    pc->frame_start_found = (pc->frame_start_found + 1) & 3;
            }
            state++;
        } else {
            i = avpriv_find_start_code(buf + i, buf + buf_size, &state) - buf - 1;
            if (pc->frame_start_found == 0 && state >= SLICE_MIN_START_CODE && state <= SLICE_MAX_START_CODE) {
                i++;
                pc->frame_start_found = 4;
            }
            if (state == SEQ_END_CODE) {
                pc->frame_start_found = 0;
                pc->state=-1;
                return i+1;
            }
            if (pc->frame_start_found == 2 && state == SEQ_START_CODE)
                pc->frame_start_found = 0;
            if (pc->frame_start_found  < 4 && state == EXT_START_CODE)
                pc->frame_start_found++;
            if (pc->frame_start_found == 4 && (state & 0xFFFFFF00) == 0x100) {
                if (state < SLICE_MIN_START_CODE || state > SLICE_MAX_START_CODE) {
                    pc->frame_start_found = 0;
                    pc->state             = -1;
                    return i - 3;
                }
            }
            if (pc->frame_start_found == 0 && s && state == PICTURE_START_CODE) {
                ff_fetch_timestamp(s, i - 3, 1, i > 3);
            }
        }
    }
    pc->state = state;
    return END_NOT_FOUND;
}

#define MAX_INDEX (64 - 1)

int ff_mpeg1_decode_block_intra(GetBitContext *gb,
                                const uint16_t *quant_matrix,
                                uint8_t *const scantable, int last_dc[3],
                                int16_t *block, int index, int qscale)
{
    int dc, diff, i = 0, component;
    RLTable *rl = &ff_rl_mpeg1;

    /* DC coefficient */
    component = index <= 3 ? 0 : index - 4 + 1;

    diff = decode_dc(gb, component);
    if (diff >= 0xffff)
        return AVERROR_INVALIDDATA;

    dc  = last_dc[component];
    dc += diff;
    last_dc[component] = dc;

    block[0] = dc * quant_matrix[0];

    {
        OPEN_READER(re, gb);
        UPDATE_CACHE(re, gb);
        if (((int32_t)GET_CACHE(re, gb)) <= (int32_t)0xBFFFFFFF)
            goto end;

        /* now quantify & encode AC coefficients */
        while (1) {
            int level, run, j;

            GET_RL_VLC(level, run, re, gb, rl->rl_vlc[0],
                       TEX_VLC_BITS, 2, 0);

            if (level != 0) {
                i += run;
                if (i > MAX_INDEX)
                    break;

                j = scantable[i];
                level = (level * qscale * quant_matrix[j]) >> 4;
                level = (level - 1) | 1;
                level = (level ^ SHOW_SBITS(re, gb, 1)) -
                        SHOW_SBITS(re, gb, 1);
                SKIP_BITS(re, gb, 1);
            } else {
                /* escape */
                run = SHOW_UBITS(re, gb, 6) + 1;
                LAST_SKIP_BITS(re, gb, 6);
                UPDATE_CACHE(re, gb);
                level = SHOW_SBITS(re, gb, 8);
                SKIP_BITS(re, gb, 8);

                if (level == -128) {
                    level = SHOW_UBITS(re, gb, 8) - 256;
                    SKIP_BITS(re, gb, 8);
                } else if (level == 0) {
                    level = SHOW_UBITS(re, gb, 8);
                    SKIP_BITS(re, gb, 8);
                }

                i += run;
                if (i > MAX_INDEX)
                    break;

                j = scantable[i];
                if (level < 0) {
                    level = -level;
                    level = (level * qscale * quant_matrix[j]) >> 4;
                    level = (level - 1) | 1;
                    level = -level;
                } else {
                    level = (level * qscale * quant_matrix[j]) >> 4;
                    level = (level - 1) | 1;
                }
            }

            block[j] = level;
            if (((int32_t)GET_CACHE(re, gb)) <= (int32_t)0xBFFFFFFF)
               break;

            UPDATE_CACHE(re, gb);
        }
end:
        LAST_SKIP_BITS(re, gb, 2);
        CLOSE_READER(re, gb);
    }

    if (i > MAX_INDEX)
        i = AVERROR_INVALIDDATA;

    return i;
}
