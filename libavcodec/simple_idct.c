/*
 * Simple IDCT
 *
 * Copyright (c) 2001 Michael Niedermayer <michaelni@gmx.at>
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
 * simpleidct in C.
 */

#include "libavutil/intreadwrite.h"
#include "avcodec.h"
#include "mathops.h"
#include "simple_idct.h"

#define IN_IDCT_DEPTH 16

#define BIT_DEPTH 8
#include "simple_idct_template.c"
#undef BIT_DEPTH

#define BIT_DEPTH 10
#include "simple_idct_template.c"

#define EXTRA_SHIFT  2
#include "simple_idct_template.c"

#undef EXTRA_SHIFT
#undef BIT_DEPTH

#define BIT_DEPTH 12
#include "simple_idct_template.c"
#undef BIT_DEPTH
#undef IN_IDCT_DEPTH

#define IN_IDCT_DEPTH 32
#define BIT_DEPTH 10
#include "simple_idct_template.c"
#undef BIT_DEPTH
#undef IN_IDCT_DEPTH

/* 2x4x8 idct */

#define CN_SHIFT 12
#define C_FIX(x) ((int)((x) * (1 << CN_SHIFT) + 0.5))
#define C1 C_FIX(0.6532814824)
#define C2 C_FIX(0.2705980501)

/* row idct is multiple by 16 * sqrt(2.0), col idct4 is normalized,
   and the butterfly must be multiplied by 0.5 * sqrt(2.0) */
#define C_SHIFT (4+1+12)

static inline void idct4col_put(uint8_t *dest, ptrdiff_t line_size, const int16_t *col)
{
    int c0, c1, c2, c3, a0, a1, a2, a3;

    a0 = col[8*0];
    a1 = col[8*2];
    a2 = col[8*4];
    a3 = col[8*6];
    c0 = ((a0 + a2) * (1 << CN_SHIFT - 1)) + (1 << (C_SHIFT - 1));
    c2 = ((a0 - a2) * (1 << CN_SHIFT - 1)) + (1 << (C_SHIFT - 1));
    c1 = a1 * C1 + a3 * C2;
    c3 = a1 * C2 - a3 * C1;
    dest[0] = av_clip_uint8((c0 + c1) >> C_SHIFT);
    dest += line_size;
    dest[0] = av_clip_uint8((c2 + c3) >> C_SHIFT);
    dest += line_size;
    dest[0] = av_clip_uint8((c2 - c3) >> C_SHIFT);
    dest += line_size;
    dest[0] = av_clip_uint8((c0 - c1) >> C_SHIFT);
}

#define BF(k) \
{\
    int a0, a1;\
    a0 = ptr[k];\
    a1 = ptr[8 + k];\
    ptr[k] = a0 + a1;\
    ptr[8 + k] = a0 - a1;\
}

/* only used by DV codec. The input must be interlaced. 128 is added
   to the pixels before clamping to avoid systematic error
   (1024*sqrt(2)) offset would be needed otherwise. */
/* XXX: I think a 1.0/sqrt(2) normalization should be needed to
   compensate the extra butterfly stage - I don't have the full DV
   specification */
void ff_simple_idct248_put(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    int i;
    int16_t *ptr;

    /* butterfly */
    ptr = block;
    for(i=0;i<4;i++) {
        BF(0);
        BF(1);
        BF(2);
        BF(3);
        BF(4);
        BF(5);
        BF(6);
        BF(7);
        ptr += 2 * 8;
    }

    /* IDCT8 on each line */
    for(i=0; i<8; i++) {
        idctRowCondDC_int16_8bit(block + i*8, 0);
    }

    /* IDCT4 and store */
    for(i=0;i<8;i++) {
        idct4col_put(dest + i, 2 * line_size, block + i);
        idct4col_put(dest + line_size + i, 2 * line_size, block + 8 + i);
    }
}

/* 8x4 & 4x8 WMV2 IDCT */
#undef CN_SHIFT
#undef C_SHIFT
#undef C_FIX
#undef C1
#undef C2
#define CN_SHIFT 12
#define C_FIX(x) ((int)((x) * M_SQRT2 * (1 << CN_SHIFT) + 0.5))
#define C1 C_FIX(0.6532814824)
#define C2 C_FIX(0.2705980501)
#define C3 C_FIX(0.5)
#define C_SHIFT (4+1+12)
static inline void idct4col_add(uint8_t *dest, ptrdiff_t line_size, const int16_t *col)
{
    int c0, c1, c2, c3, a0, a1, a2, a3;

    a0 = col[8*0];
    a1 = col[8*1];
    a2 = col[8*2];
    a3 = col[8*3];
    c0 = (a0 + a2)*C3 + (1 << (C_SHIFT - 1));
    c2 = (a0 - a2)*C3 + (1 << (C_SHIFT - 1));
    c1 = a1 * C1 + a3 * C2;
    c3 = a1 * C2 - a3 * C1;
    dest[0] = av_clip_uint8(dest[0] + ((c0 + c1) >> C_SHIFT));
    dest += line_size;
    dest[0] = av_clip_uint8(dest[0] + ((c2 + c3) >> C_SHIFT));
    dest += line_size;
    dest[0] = av_clip_uint8(dest[0] + ((c2 - c3) >> C_SHIFT));
    dest += line_size;
    dest[0] = av_clip_uint8(dest[0] + ((c0 - c1) >> C_SHIFT));
}

#define RN_SHIFT 15
#define R_FIX(x) ((int)((x) * M_SQRT2 * (1 << RN_SHIFT) + 0.5))
#define R1 R_FIX(0.6532814824)
#define R2 R_FIX(0.2705980501)
#define R3 R_FIX(0.5)
#define R_SHIFT 11
static inline void idct4row(int16_t *row)
{
    unsigned c0, c1, c2, c3;
    int a0, a1, a2, a3;

    a0 = row[0];
    a1 = row[1];
    a2 = row[2];
    a3 = row[3];
    c0 = (a0 + a2)*R3 + (1 << (R_SHIFT - 1));
    c2 = (a0 - a2)*R3 + (1 << (R_SHIFT - 1));
    c1 = a1 * R1 + a3 * R2;
    c3 = a1 * R2 - a3 * R1;
    row[0]= (c0 + c1) >> R_SHIFT;
    row[1]= (c2 + c3) >> R_SHIFT;
    row[2]= (c2 - c3) >> R_SHIFT;
    row[3]= (c0 - c1) >> R_SHIFT;
}

void ff_simple_idct84_add(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    int i;

    /* IDCT8 on each line */
    for(i=0; i<4; i++) {
        idctRowCondDC_int16_8bit(block + i*8, 0);
    }

    /* IDCT4 and store */
    for(i=0;i<8;i++) {
        idct4col_add(dest + i, line_size, block + i);
    }
}

void ff_simple_idct48_add(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    int i;

    /* IDCT4 on each line */
    for(i=0; i<8; i++) {
        idct4row(block + i*8);
    }

    /* IDCT8 and store */
    for(i=0; i<4; i++){
        idctSparseColAdd_int16_8bit(dest + i, line_size, block + i);
    }
}

void ff_simple_idct44_add(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    int i;

    /* IDCT4 on each line */
    for(i=0; i<4; i++) {
        idct4row(block + i*8);
    }

    /* IDCT4 and store */
    for(i=0; i<4; i++){
        idct4col_add(dest + i, line_size, block + i);
    }
}

void ff_prores_idct_10(int16_t *block, const int16_t *qmat)
{
    int i;

    for (i = 0; i < 64; i++)
        block[i] *= qmat[i];

    for (i = 0; i < 8; i++)
        idctRowCondDC_extrashift_10(block + i*8, 2);

    for (i = 0; i < 8; i++) {
        block[i] += 8192;
        idctSparseCol_extrashift_10(block + i);
    }
}

void ff_prores_idct_12(int16_t *block, const int16_t *qmat)
{
    int i;

    for (i = 0; i < 64; i++)
        block[i] *= qmat[i];

    for (i = 0; i < 8; i++)
        idctRowCondDC_int16_12bit(block + i*8, 0);

    for (i = 0; i < 8; i++) {
        block[i] += 8192;
        idctSparseCol_int16_12bit(block + i);
    }
}
//+clair add 2021-11-11
#define BIT18_PREC
#define MAX(a, b)       (((a) > (b)) ? (a) : (b))
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#define CLIP255(a)      (MIN(255, MAX(0, (a))))

static void idct1d(int out[], int in[])
{
	int i, j, tmp2, tmp3;
	int X[20], acc[8];
	int Ax[8], Bx[8], Cx[8], Dx[8], Ex[8], Fx[8], Gx[8];

	for (i = 0; i < 8; i++) {
		tmp2 = (in[i]<<2) + in[i];
		tmp3 = (in[i]<<3) + in[i];
		X[0] =  in[i]<<5;
		for (j = 1; j < 19; j++)
			X[j] = X[j - 1]>>1;
		if ((i&1) == 0) {
#ifdef BIT18_PREC
			Ax[i] = (tmp2<<1) + (tmp2>>2) + X[9] + X[15];
			Cx[i] = X[1] - (tmp2>>2) + X[10] + X[15] - X[18];
			Fx[i] = X[3] + X[4] + X[8] - X[14];
#else
			Ax[i] = (tmp2<<1) + (tmp2>>2) + X[9];
			Bx[i] = X[1] - (tmp2>>2) + X[10];
			Cx[i] = X[3] + X[4] + X[8] - X[14];
#endif
		}
		else {
#ifdef BIT18_PREC
			Bx[i] = X[1] - (tmp2>>4) + (tmp2>>10);
			Dx[i] = X[2] + tmp2 + (tmp2>>4) - (tmp3>>10) - X[17];
			Ex[i] = tmp3 - X[8] + X[11] - X[15] - X[16];
			Gx[i] = X[4] + (tmp3>>3) - X[13] + X[17];
#else
			Dx[i] = X[1] - (tmp2>>4) + X[13];
			Ex[i] = X[2] + tmp2 + (tmp2>>4) - X[12];
			Fx[i] = tmp3 - X[8] + X[11];
			Gx[i] = X[4] + (tmp3>>3) - X[13];
#endif
		}
	}

#ifdef BIT18_PREC
	acc[0] = Ax[0] + Cx[2] + Ax[4] + Fx[6];
	acc[1] = Ax[0] + Fx[2] - Ax[4] - Cx[6];
	acc[2] = Ax[0] - Fx[2] - Ax[4] + Cx[6];
	acc[3] = Ax[0] - Cx[2] + Ax[4] - Fx[6];

	acc[4] = Bx[1] + Dx[3] + Ex[5] + Gx[7];
	acc[5] = Dx[1] - Gx[3] - Bx[5] - Ex[7];
	acc[6] = Ex[1] - Bx[3] + Gx[5] + Dx[7];
	acc[7] = Gx[1] - Ex[3] + Dx[5] - Bx[7];
#else
	acc[0] = Ax[0] + Bx[2] + Ax[4] + Cx[6];
	acc[1] = Ax[0] + Cx[2] - Ax[4] - Bx[6];
	acc[2] = Ax[0] - Cx[2] - Ax[4] + Bx[6];
	acc[3] = Ax[0] - Bx[2] + Ax[4] - Cx[6];

	acc[4] = Dx[1] + Ex[3] + Fx[5] + Gx[7];
	acc[5] = Ex[1] - Gx[3] - Dx[5] - Fx[7];
	acc[6] = Fx[1] - Dx[3] + Gx[5] + Ex[7];
	acc[7] = Gx[1] - Fx[3] + Ex[5] - Dx[7];
#endif


	out[0] = acc[0] + acc[4];
	out[1] = acc[1] + acc[5];
	out[2] = acc[2] + acc[6];
	out[3] = acc[3] + acc[7];
	out[4] = acc[3] - acc[7];
	out[5] = acc[2] - acc[6];
	out[6] = acc[1] - acc[5];
	out[7] = acc[0] - acc[4];
}

static void IdctBlk(int16_t block[])
{
	int  xx[64];
	int  yy[64];
	int  temp;
	int  i;
	// IDCT ROW x 8
	for (i=0; i<64; i++)
#ifdef BIT18_PREC
        xx[i] = block[i/8 + (i&7)*8] << 6;
#else
		xx[i] = block[i/8 + (i&7)*8] << 4;
#endif

	for (i=0; i<64; i+=8)
		idct1d(yy + i, xx + i);

	for (i=0; i<64; i++) {
		temp = yy[i];
		if (temp > 0 || (temp&0xf) != 0)
			temp = (temp >> 5) + ((temp >> 4) & 1);
		else
			temp = (temp >> 5);

#ifdef BIT18_PREC
		if (temp < -131072)
			yy[i] = -131072;
		else if (temp > 131071)		// signed 2^18
			yy[i] = 131071;
#else
		if (temp < -32768)
			yy[i] = -32768;
		else if (temp > 32767)		// signed 2^16
			yy[i] = 32767;
#endif
		else
			yy[i] = temp;
	}

	// IDCT COL x 8
	for (i=0; i<64; i++)
		xx[i] = yy[i/8 + (i&7)*8];

	for (i=0; i<64; i+=8)
		idct1d(yy + i, xx + i);

	for (i=0; i<64; i++) {
		temp = yy[i];
#ifdef BIT18_PREC
        if (temp > 0 || (temp& 0x3ff) != 0)
            temp = (temp >> 11) + ((temp >> 10) & 1);
        else
            temp = (temp >> 11);
#else
		if (temp > 0 || (temp& 0xff) != 0)
			temp = (temp >> 9) + ((temp >> 8) & 1);
		else
			temp = (temp >> 9);
#endif

		if (temp < -256)
			block[i] = -256;
		else if (temp > 255)
			block[i] = 255;
		else
			block[i] = temp;
	}
}


void cnm_idct_put(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
	int  i;

    IdctBlk(block);       
    for (i = 0; i < 8; i++) {
        dest[0] = CLIP255(block[0]+128);
        dest[1] = CLIP255(block[1]+128);
        dest[2] = CLIP255(block[2]+128);
        dest[3] = CLIP255(block[3]+128);
        dest[4] = CLIP255(block[4]+128);
        dest[5] = CLIP255(block[5]+128);
        dest[6] = CLIP255(block[6]+128);
        dest[7] = CLIP255(block[7]+128);

        dest += line_size;
        block  += 8;
    }
}

void cnm_idct_add(uint8_t *dest, ptrdiff_t line_size, int16_t *block)
{
    int i;

    IdctBlk(block);
    for (i = 0; i < 8; i++) {
        dest[0] = CLIP255(dest[0]+block[0]);
        dest[1] = CLIP255(dest[1]+block[1]);
        dest[2] = CLIP255(dest[2]+block[2]);
        dest[3] = CLIP255(dest[3]+block[3]);
        dest[4] = CLIP255(dest[4]+block[4]);
        dest[5] = CLIP255(dest[5]+block[5]);
        dest[6] = CLIP255(dest[6]+block[6]);
        dest[7] = CLIP255(dest[7]+block[7]);

        dest += line_size;
        block  += 8;
    }
}
//-clair add