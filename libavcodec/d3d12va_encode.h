/*
 * Direct3D 12 HW acceleration video encoder
 *
 * Copyright (c) 2024 Intel Corporation
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

#ifndef AVCODEC_D3D12VA_ENCODE_H
#define AVCODEC_D3D12VA_ENCODE_H

#include <stdio.h>

#include "libavutil/fifo.h"
#include "libavutil/hwcontext.h"
#include "libavutil/hwcontext_d3d12va_internal.h"
#include "libavutil/hwcontext_d3d12va.h"
#include "avcodec.h"
#include "internal.h"
#include "hwconfig.h"
#include "hw_base_encode.h"

struct D3D12VAEncodeType;

extern const AVCodecHWConfigInternal *const ff_d3d12va_encode_hw_configs[];

#define MAX_PARAM_BUFFER_SIZE 4096
#define D3D12VA_VIDEO_ENC_ASYNC_DEPTH 8

typedef struct D3D12VAEncodePicture {
    int             header_size;
    int             aligned_header_size;

    AVD3D12VAFrame *input_surface;
    AVD3D12VAFrame *recon_surface;

    AVBufferRef    *output_buffer_ref;
    ID3D12Resource *output_buffer;

    ID3D12Resource *encoded_metadata;
    ID3D12Resource *resolved_metadata;

    int            subresource_index;

    D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA pic_ctl;

    int             fence_value;

    // ROI delta QP map (void* to support both INT8 for H.264/HEVC and INT16 for AV1)
    void           *qp_map;
    int             qp_map_size;
} D3D12VAEncodePicture;

typedef struct D3D12VAEncodeProfile {
    /**
     * lavc profile value (AV_PROFILE_*).
     */
    int       av_profile;

    /**
     * Supported bit depth.
     */
    int       depth;

    /**
     * Number of components.
     */
    int       nb_components;

    /**
     * Chroma subsampling in width dimension.
     */
    int       log2_chroma_w;

    /**
     * Chroma subsampling in height dimension.
     */
    int       log2_chroma_h;

    /**
     * D3D12 profile value.
     */
    D3D12_VIDEO_ENCODER_PROFILE_DESC d3d12_profile;
} D3D12VAEncodeProfile;

enum {
    RC_MODE_AUTO,
    RC_MODE_CQP,
    RC_MODE_CBR,
    RC_MODE_VBR,
    RC_MODE_QVBR,
    RC_MODE_MAX = RC_MODE_QVBR,
};

/**
 * DX Bitstream file format structures.
 * IVF-based container for capturing D3D12 encoder parameters.
 */
#define DX_IVF_SIGNATURE    MKTAG('D', 'X', 'I', 'F')
#define DX_FRAME_SIGNATURE  MKTAG('D', 'X', 'F', 'H')
#define DX_BUFFER_SIGNATURE MKTAG('D', 'X', 'B', 'H')

typedef struct DXIvfHeader {
    uint32_t signature;       // DXIF
    uint16_t version;
    uint16_t length;          // header length (32)
    uint32_t fourcc;          // codec fourcc
    uint16_t width;
    uint16_t height;
    uint32_t framerate;
    uint32_t level_profile_tier;  // D3D12 encoder level value at low 16bit, profile value at 16-24bit, tier value at 24-32bit
    uint32_t frame_count;
    uint32_t input_format;    // DXGI_FORMAT
} DXIvfHeader;

#pragma pack(push, 1)
typedef struct DXIvfFrameHeader {
    uint32_t size;            // IVF frame data size (not including this 12-byte header)
    int64_t  timestamp;
} DXIvfFrameHeader;
#pragma pack(pop)

typedef struct DXFrameHeader {
    uint32_t signature;       // DXFH
    uint32_t frame_index;
    uint32_t num_buffers;
} DXFrameHeader;

typedef struct DXBufferHeader {
    uint32_t signature;       // DXBH
    uint32_t dx_buffer_type;
    int32_t  dx_buffer_size;
} DXBufferHeader;

enum DXBufferType {
    DX_BUFFER_TYPE_ENCODEFRAME_INPUT       = 0,
    DX_BUFFER_TYPE_ENCODEFRAME_OUTPUT      = 1,
    DX_BUFFER_TYPE_RESOLVE_METADATA_INPUT  = 2,
    DX_BUFFER_TYPE_RESOLVE_METADATA_OUTPUT = 3,
    DX_BUFFER_TYPE_QP_MAP                  = 4,
    DX_BUFFER_TYPE_INPUT_FRAME_DATA        = 5,
};

typedef struct D3D12_VIDEO_ENCODER_PROFILE_DESC_STATIC
    {
    UINT DataSize;
    union 
        {
        // D3D12_VIDEO_ENCODER_PROFILE_H264 h264Profile;
        // D3D12_VIDEO_ENCODER_PROFILE_HEVC HEVCProfile;
        D3D12_VIDEO_ENCODER_AV1_PROFILE AV1Profile;
        } 	;
    } 	D3D12_VIDEO_ENCODER_PROFILE_DESC_STATIC;

typedef struct D3D12_VIDEO_ENCODER_CODEC_CONFIGURATION_STATIC
    {
    UINT DataSize;
    union 
        {
        // D3D12_VIDEO_ENCODER_CODEC_CONFIGURATION_H264 H264Config;
        // D3D12_VIDEO_ENCODER_CODEC_CONFIGURATION_HEVC HEVCConfig;
        D3D12_VIDEO_ENCODER_AV1_CODEC_CONFIGURATION AV1Config;
        } 	;
    } 	D3D12_VIDEO_ENCODER_CODEC_CONFIGURATION_STATIC;
typedef struct D3D12_VIDEO_ENCODER_DESC_STATIC
    {
    UINT NodeMask;
    D3D12_VIDEO_ENCODER_FLAGS Flags;
    D3D12_VIDEO_ENCODER_CODEC EncodeCodec;
    D3D12_VIDEO_ENCODER_PROFILE_DESC_STATIC EncodeProfile;
    DXGI_FORMAT InputFormat;
    D3D12_VIDEO_ENCODER_CODEC_CONFIGURATION_STATIC CodecConfiguration;
    D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE MaxMotionEstimationPrecision;
    } 	D3D12_VIDEO_ENCODER_DESC_STATIC;

typedef struct D3D12_VIDEO_ENCODER_LEVEL_SETTING_STATIC
    {
    UINT DataSize;
    union 
        {
        // D3D12_VIDEO_ENCODER_LEVELS_H264 H264LevelSetting;
        // D3D12_VIDEO_ENCODER_LEVEL_TIER_CONSTRAINTS_HEVC HEVCLevelSetting;
        D3D12_VIDEO_ENCODER_AV1_LEVEL_TIER_CONSTRAINTS AV1LevelSetting;
        } 	;
    } 	D3D12_VIDEO_ENCODER_LEVEL_SETTING_STATIC;

typedef struct D3D12_VIDEO_ENCODER_HEAP_DESC_STATIC
    {
    UINT NodeMask;
    D3D12_VIDEO_ENCODER_HEAP_FLAGS Flags;
    D3D12_VIDEO_ENCODER_CODEC EncodeCodec;
    D3D12_VIDEO_ENCODER_PROFILE_DESC_STATIC EncodeProfile;
    D3D12_VIDEO_ENCODER_LEVEL_SETTING_STATIC EncodeLevel;
    UINT ResolutionsListCount;
    D3D12_VIDEO_ENCODER_PICTURE_RESOLUTION_DESC pResolutionList[1];
    } 	D3D12_VIDEO_ENCODER_HEAP_DESC_STATIC;


typedef struct D3D12_VIDEO_ENCODER_RATE_CONTROL_CONFIGURATION_PARAMS_STATIC
    {
    UINT DataSize;
    union 
        {
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_CQP Configuration_CQP;
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_CBR Configuration_CBR;
        D3D12_VIDEO_ENCODER_RATE_CONTROL_VBR Configuration_VBR; // size of this struct is larger than CQP and CBR, so put it first in the union to save some memory when using CQP or CBR 
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_QVBR Configuration_QVBR;
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_CQP1 Configuration_CQP1;
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_CBR1 Configuration_CBR1;
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_VBR1 Configuration_VBR1;
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_QVBR1 Configuration_QVBR1;
        // const D3D12_VIDEO_ENCODER_RATE_CONTROL_ABSOLUTE_QP_MAP Configuration_AbsoluteQPMap;
        } 	;
    } 	D3D12_VIDEO_ENCODER_RATE_CONTROL_CONFIGURATION_PARAMS_STATIC;

typedef struct D3D12_VIDEO_ENCODER_RATE_CONTROL_STATIC
    {
    D3D12_VIDEO_ENCODER_RATE_CONTROL_MODE Mode;
    D3D12_VIDEO_ENCODER_RATE_CONTROL_FLAGS Flags;
    D3D12_VIDEO_ENCODER_RATE_CONTROL_CONFIGURATION_PARAMS_STATIC ConfigParams;
    DXGI_RATIONAL TargetFrameRate;
    } 	D3D12_VIDEO_ENCODER_RATE_CONTROL_STATIC;

typedef struct D3D12_VIDEO_ENCODER_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA_STATIC
    {
    UINT DataSize;
    union 
        {
        // const D3D12_VIDEO_ENCODER_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA_SLICES SlicesPartition_H264;
        // const D3D12_VIDEO_ENCODER_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA_SLICES SlicesPartition_HEVC;
        D3D12_VIDEO_ENCODER_AV1_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA_TILES TilesPartition_AV1;
        } 	;
    } 	D3D12_VIDEO_ENCODER_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA_STATIC;

typedef struct D3D12_VIDEO_ENCODER_SEQUENCE_GOP_STRUCTURE_STATIC
    {
    UINT DataSize;
    union 
        {
        // D3D12_VIDEO_ENCODER_SEQUENCE_GOP_STRUCTURE_H264 H264GroupOfPictures;
        // D3D12_VIDEO_ENCODER_SEQUENCE_GOP_STRUCTURE_HEVC HEVCGroupOfPictures;
        D3D12_VIDEO_ENCODER_AV1_SEQUENCE_STRUCTURE AV1SequenceStructure;
        } 	;
    } 	D3D12_VIDEO_ENCODER_SEQUENCE_GOP_STRUCTURE_STATIC;

typedef struct D3D12_VIDEO_ENCODER_SEQUENCE_CONTROL_DESC_STATIC
    {
    D3D12_VIDEO_ENCODER_SEQUENCE_CONTROL_FLAGS Flags;
    D3D12_VIDEO_ENCODER_INTRA_REFRESH IntraRefreshConfig;
    D3D12_VIDEO_ENCODER_RATE_CONTROL_STATIC RateControl;
    D3D12_VIDEO_ENCODER_PICTURE_RESOLUTION_DESC PictureTargetResolution;
    D3D12_VIDEO_ENCODER_FRAME_SUBREGION_LAYOUT_MODE SelectedLayoutMode;
    D3D12_VIDEO_ENCODER_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA_STATIC FrameSubregionsLayoutData;
    D3D12_VIDEO_ENCODER_SEQUENCE_GOP_STRUCTURE_STATIC CodecGopSequence;
    } 	D3D12_VIDEO_ENCODER_SEQUENCE_CONTROL_DESC_STATIC;

typedef struct D3D12_VIDEO_ENCODER_AV1_PICTURE_CONTROL_CODEC_DATA_STATIC
    {
    D3D12_VIDEO_ENCODER_AV1_PICTURE_CONTROL_FLAGS Flags;
    D3D12_VIDEO_ENCODER_AV1_FRAME_TYPE FrameType;
    D3D12_VIDEO_ENCODER_AV1_COMP_PREDICTION_TYPE CompoundPredictionType;
    D3D12_VIDEO_ENCODER_AV1_INTERPOLATION_FILTERS InterpolationFilter;
    D3D12_VIDEO_ENCODER_AV1_RESTORATION_CONFIG FrameRestorationConfig;
    D3D12_VIDEO_ENCODER_AV1_TX_MODE TxMode;
    UINT SuperResDenominator;
    UINT OrderHint;
    UINT PictureIndex;
    UINT TemporalLayerIndexPlus1;
    UINT SpatialLayerIndexPlus1;
    D3D12_VIDEO_ENCODER_AV1_REFERENCE_PICTURE_DESCRIPTOR ReferenceFramesReconPictureDescriptors[ 8 ];
    UINT ReferenceIndices[ 7 ];
    UINT PrimaryRefFrame;
    UINT RefreshFrameFlags;
    D3D12_VIDEO_ENCODER_CODEC_AV1_LOOP_FILTER_CONFIG LoopFilter;
    D3D12_VIDEO_ENCODER_CODEC_AV1_LOOP_FILTER_DELTA_CONFIG LoopFilterDelta;
    D3D12_VIDEO_ENCODER_CODEC_AV1_QUANTIZATION_CONFIG Quantization;
    D3D12_VIDEO_ENCODER_CODEC_AV1_QUANTIZATION_DELTA_CONFIG QuantizationDelta;
    D3D12_VIDEO_ENCODER_AV1_CDEF_CONFIG CDEF;
    UINT QPMapValuesCount;
    _Field_size_full_(QPMapValuesCount)  INT16 *pRateControlQPMap; // this pointer can be ignored. this data will be stored in another DXBuffer data of the dx bistream file. 
    D3D12_VIDEO_ENCODER_AV1_SEGMENTATION_CONFIG CustomSegmentation;
    D3D12_VIDEO_ENCODER_AV1_SEGMENTATION_MAP CustomSegmentsMap;
    } 	D3D12_VIDEO_ENCODER_AV1_PICTURE_CONTROL_CODEC_DATA_STATIC;

typedef struct D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA_STATIC
    {
    UINT DataSize;
    union 
        {
        // D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA_H264 *pH264PicData;
        // D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA_HEVC *pHEVCPicData;
        // D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA_HEVC1 *pHEVCPicData1;
        D3D12_VIDEO_ENCODER_AV1_PICTURE_CONTROL_CODEC_DATA_STATIC AV1PicData;
        } 	;
    } 	D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA_STATIC;
    

typedef struct D3D12_VIDEO_ENCODE_REFERENCE_FRAMES_STATIC
    {
    UINT NumTexture2Ds;
    // _Field_size_full_(NumTexture2Ds)  ID3D12Resource **ppTexture2Ds;
    ID3D12Resource *pTexture2Ds[8]; // assume that the maximim value of NumTexture2Ds is 8.
    // _Field_size_full_(NumTexture2Ds)  UINT *pSubresources;
    UINT pSubresources[8]; // assume that the maximim value of NumTexture2Ds is 8.
    } 	D3D12_VIDEO_ENCODE_REFERENCE_FRAMES_STATIC;

typedef struct D3D12_VIDEO_ENCODER_PICTURE_CONTROL_DESC_STATIC
    {
    UINT IntraRefreshFrameIndex;
    D3D12_VIDEO_ENCODER_PICTURE_CONTROL_FLAGS Flags;
    D3D12_VIDEO_ENCODER_PICTURE_CONTROL_CODEC_DATA_STATIC PictureControlCodecData;
    D3D12_VIDEO_ENCODE_REFERENCE_FRAMES_STATIC ReferenceFrames;
    } 	D3D12_VIDEO_ENCODER_PICTURE_CONTROL_DESC_STATIC;

typedef struct D3D12_VIDEO_ENCODER_ENCODEFRAME_INPUT_ARGUMENTS_STATIC
    {
    D3D12_VIDEO_ENCODER_SEQUENCE_CONTROL_DESC_STATIC SequenceControlDesc;
    D3D12_VIDEO_ENCODER_PICTURE_CONTROL_DESC_STATIC PictureControlDesc;
    ID3D12Resource *pInputFrame;
    UINT InputFrameSubresource;
    UINT CurrentFrameBitstreamMetadataSize;
    } 	D3D12_VIDEO_ENCODER_ENCODEFRAME_INPUT_ARGUMENTS_STATIC;

typedef struct D3D12_VIDEO_ENCODER_RESOLVE_METADATA_INPUT_ARGUMENTS_STATIC
    {
    D3D12_VIDEO_ENCODER_CODEC EncoderCodec;
    D3D12_VIDEO_ENCODER_PROFILE_DESC_STATIC EncoderProfile;
    DXGI_FORMAT EncoderInputFormat;
    D3D12_VIDEO_ENCODER_PICTURE_RESOLUTION_DESC EncodedPictureEffectiveResolution;
    D3D12_VIDEO_ENCODER_ENCODE_OPERATION_METADATA_BUFFER HWLayoutMetadata;
    } 	D3D12_VIDEO_ENCODER_RESOLVE_METADATA_INPUT_ARGUMENTS_STATIC;

typedef struct D3D12VAEncodeRCMode {
    /**
     * Mode from above enum (RC_MODE_*).
     */
    int mode;

    /**
     * Name.
     *
     */
    const char *name;

    /**
     * Uses bitrate parameters.
     *
     */
    int bitrate;

    /**
     * Supports maxrate distinct from bitrate.
     *
     */
    int maxrate;

    /**
     * Uses quality value.
     *
     */
    int quality;

    /**
     * Supports HRD/VBV parameters.
     *
     */
    int hrd;

    /**
     * D3D12 mode value.
     */
    D3D12_VIDEO_ENCODER_RATE_CONTROL_MODE d3d12_mode;
} D3D12VAEncodeRCMode;

typedef struct D3D12VAEncodeContext {
    FFHWBaseEncodeContext base;

    /**
     * Codec-specific hooks.
     */
    const struct D3D12VAEncodeType *codec;

    /**
     * Max frame size
     */
    int max_frame_size;

    /**
     * Explicitly set RC mode (otherwise attempt to pick from
     * available modes).
     */
    int explicit_rc_mode;

    /**
     * Explicitly-set QP, for use with the "qp" options.
     * (Forces CQP mode when set, overriding everything else.)
     */
    int explicit_qp;

    /**
     * RC quality level - meaning depends on codec and RC mode.
     * In CQP mode this sets the fixed quantiser value.
     */
    int rc_quality;

    /**
     * Chosen encoding profile details.
     */
    const D3D12VAEncodeProfile *profile;

    AVD3D12VADeviceContext *hwctx;

    /**
     * ID3D12Device3 interface.
     */
    ID3D12Device3 *device3;

    /**
     * ID3D12VideoDevice3 interface.
     */
    ID3D12VideoDevice3 *video_device3;

    /**
     * Pool of (reusable) bitstream output buffers.
     */
    AVBufferPool *output_buffer_pool;

    /**
     * Flag indicates if the HW is texture array mode.
     */
    int is_texture_array;

    /**
     * The number of planes in the input DXGI FORMAT.
     */
    int plane_count;

    /**
     * D3D12 video encoder.
     */
    AVBufferRef *encoder_ref;

    ID3D12VideoEncoder *encoder;

    /**
     * D3D12 video encoder heap.
     */
    ID3D12VideoEncoderHeap *encoder_heap;

    /**
     * A cached queue for reusing the D3D12 command allocators.
     *
     * @see https://learn.microsoft.com/en-us/windows/win32/direct3d12/recording-command-lists-and-bundles#id3d12commandallocator
     */
    AVFifo *allocator_queue;

    /**
     * D3D12 command queue.
     */
    ID3D12CommandQueue *command_queue;

    /**
     * D3D12 video encode command list.
     */
    ID3D12VideoEncodeCommandList2 *command_list;

    /**
     * The sync context used to sync command queue.
     */
    AVD3D12VASyncContext sync_ctx;

    /**
     * The bi_not_empty feature.
     */
    int bi_not_empty;

    /**
     * D3D12_FEATURE structures.
     */
    D3D12_FEATURE_DATA_VIDEO_ENCODER_RESOURCE_REQUIREMENTS req;

    D3D12_FEATURE_DATA_VIDEO_ENCODER_RESOLUTION_SUPPORT_LIMITS res_limits;

    /**
     * D3D12_VIDEO_ENCODER structures.
     */
    D3D12_VIDEO_ENCODER_PICTURE_RESOLUTION_DESC resolution;

    D3D12_VIDEO_ENCODER_CODEC_CONFIGURATION codec_conf;

    D3D12_VIDEO_ENCODER_RATE_CONTROL rc;

    D3D12_VIDEO_ENCODER_SEQUENCE_GOP_STRUCTURE gop;

    D3D12_VIDEO_ENCODER_LEVEL_SETTING level;

    D3D12_VIDEO_ENCODER_PICTURE_CONTROL_SUBREGIONS_LAYOUT_DATA subregions_layout;

    /**
     * Intra refresh configuration
     */
    D3D12_VIDEO_ENCODER_INTRA_REFRESH intra_refresh;

    /**
     * Current frame index within intra refresh cycle
     */
    UINT intra_refresh_frame_index;

    /**
     * Motion estimation precision mode
     */
    D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE me_precision;

    /**
     * QP map region pixel size (block size for QP map)
     */
    int qp_map_region_size;

    /**
     * DX bitstream file output path (user option).
     */
    char *dx_bitstream_path;

    /**
     * DX bitstream file handle.
     */
    FILE *dx_bitstream_file;

    /**
     * DX bitstream frame counter.
     */
    uint32_t dx_frame_count;
} D3D12VAEncodeContext;

typedef struct D3D12VAEncodeType {
    /**
     * List of supported profiles.
     */
   const D3D12VAEncodeProfile *profiles;

    /**
     * D3D12 codec name.
     */
    D3D12_VIDEO_ENCODER_CODEC d3d12_codec;

    /**
     * Codec feature flags.
     */
    int flags;

    /**
     * Default quality for this codec - used as quantiser or RC quality
     * factor depending on RC mode.
     */
    int default_quality;

    /**
     * Query codec configuration and determine encode parameters like
     * block sizes for surface alignment and slices. If not set, assume
     * that all blocks are 16x16 and that surfaces should be aligned to match
     * this.
     */
    int (*get_encoder_caps)(AVCodecContext *avctx);

    /**
     * Perform any extra codec-specific configuration.
     */
    int (*configure)(AVCodecContext *avctx);

    /**
     * Set codec-specific level setting.
     */
    int (*set_level)(AVCodecContext *avctx);

    /**
     * Set codec-specific tile setting.
     */
    int (*set_tile)(AVCodecContext *avctx);

    /**
     * The size of any private data structure associated with each
     * picture (can be zero if not required).
     */
    size_t picture_priv_data_size;

    /**
     * Fill the corresponding parameters.
     */
    int (*init_sequence_params)(AVCodecContext *avctx);

    int (*init_picture_params)(AVCodecContext *avctx,
                               FFHWBaseEncodePicture *base_pic);

    void (*free_picture_params)(D3D12VAEncodePicture *pic);

    /**
     * Write the packed header data to the provided buffer.
     */
    int (*write_sequence_header)(AVCodecContext *avctx,
                                 char *data, size_t *data_len);

    /**
     * Fill the coded data into AVPacket
     */
    int (*get_coded_data)(AVCodecContext *avctx,
                          D3D12VAEncodePicture *pic, AVPacket *pkt);
} D3D12VAEncodeType;

int ff_d3d12va_encode_receive_packet(AVCodecContext *avctx, AVPacket *pkt);

int ff_d3d12va_encode_init(AVCodecContext *avctx);
int ff_d3d12va_encode_close(AVCodecContext *avctx);

void ff_d3d12va_encode_check_encoder_feature_flags(void *log_ctx,
                                                   D3D12_VIDEO_ENCODER_VALIDATION_FLAGS flags);

#define D3D12VA_ENCODE_INTRA_REFRESH_MODE(name, mode, desc) \
    { #name, desc, 0, AV_OPT_TYPE_CONST, { .i64 = D3D12_VIDEO_ENCODER_INTRA_REFRESH_MODE_ ## mode }, \
      0, 0, FLAGS, .unit = "intra_refresh_mode" }

#if CONFIG_D3D12VA_ME_PRECISION_EIGHTH_PIXEL
#define D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_MAX_VALUE D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_EIGHTH_PIXEL
#else
#define D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_MAX_VALUE D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_QUARTER_PIXEL
#endif

#define D3D12VA_ENCODE_ME_PRECISION_MODE(name, mode, desc) \
    { #name, #desc " pixel precision", 0, AV_OPT_TYPE_CONST, \
      { .i64 = D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_ ## mode }, \
      0, 0, FLAGS, .unit = "me_precision" }

#if CONFIG_D3D12VA_ME_PRECISION_EIGHTH_PIXEL
#define FFPP_D3D12VA_ME_PRECISION_EIGHTH_PIXEL \
    , D3D12VA_ENCODE_ME_PRECISION_MODE(eighth_pixel, EIGHTH_PIXEL, Eighth)
#else
#define FFPP_D3D12VA_ME_PRECISION_EIGHTH_PIXEL
#endif

#define D3D12VA_ENCODE_COMMON_OPTIONS \
    { "max_frame_size", \
      "Maximum frame size (in bytes)",\
      OFFSET(common.max_frame_size), AV_OPT_TYPE_INT, \
      { .i64 = 0 }, 0, INT_MAX / 8, FLAGS }, \
    { "intra_refresh_mode", \
      "Set intra refresh mode", \
      OFFSET(common.intra_refresh.Mode), AV_OPT_TYPE_INT, \
      { .i64 = D3D12_VIDEO_ENCODER_INTRA_REFRESH_MODE_NONE }, \
      D3D12_VIDEO_ENCODER_INTRA_REFRESH_MODE_NONE, \
      D3D12_VIDEO_ENCODER_INTRA_REFRESH_MODE_ROW_BASED, FLAGS, .unit = "intra_refresh_mode" }, \
    D3D12VA_ENCODE_INTRA_REFRESH_MODE(none, NONE, "Disable intra refresh"), \
    D3D12VA_ENCODE_INTRA_REFRESH_MODE(row_based, ROW_BASED, "Row-based intra refresh"), \
    { "intra_refresh_duration", \
      "Number of frames over which to spread intra refresh (0 = GOP size)", \
      OFFSET(common.intra_refresh.IntraRefreshDuration), AV_OPT_TYPE_INT, \
      { .i64 = 0 }, 0, INT_MAX, FLAGS }, \
    { "me_precision", "Motion estimation precision mode", \
      OFFSET(common.me_precision), AV_OPT_TYPE_INT, \
      { .i64 = D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_MAXIMUM }, \
      D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_MAXIMUM, \
      D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_MAX_VALUE, \
      FLAGS, .unit = "me_precision" }, \
    { "maximum", "Maximum (best quality, slowest)", 0, AV_OPT_TYPE_CONST, \
      { .i64 = D3D12_VIDEO_ENCODER_MOTION_ESTIMATION_PRECISION_MODE_MAXIMUM }, \
      0, 0, FLAGS, .unit = "me_precision" }, \
    D3D12VA_ENCODE_ME_PRECISION_MODE(full_pixel, FULL_PIXEL, Full), \
    D3D12VA_ENCODE_ME_PRECISION_MODE(half_pixel, HALF_PIXEL, Half), \
    D3D12VA_ENCODE_ME_PRECISION_MODE(quarter_pixel, QUARTER_PIXEL, Quarter) \
    FFPP_D3D12VA_ME_PRECISION_EIGHTH_PIXEL, \
    { "dx_bitstream", \
      "Path to output DX bitstream file (.ivf)", \
      OFFSET(common.dx_bitstream_path), AV_OPT_TYPE_STRING, \
      { .str = NULL }, 0, 0, FLAGS }

#define D3D12VA_ENCODE_RC_MODE(name, desc) \
    { #name, desc, 0, AV_OPT_TYPE_CONST, { .i64 = RC_MODE_ ## name }, \
      0, 0, FLAGS, .unit = "rc_mode" }
#define D3D12VA_ENCODE_RC_OPTIONS \
    { "rc_mode",\
      "Set rate control mode", \
      OFFSET(common.explicit_rc_mode), AV_OPT_TYPE_INT, \
      { .i64 = RC_MODE_AUTO }, RC_MODE_AUTO, RC_MODE_MAX, FLAGS, .unit = "rc_mode" }, \
    { "auto", "Choose mode automatically based on other parameters", \
      0, AV_OPT_TYPE_CONST, { .i64 = RC_MODE_AUTO }, 0, 0, FLAGS, .unit = "rc_mode" }, \
    D3D12VA_ENCODE_RC_MODE(CQP,  "Constant-quality"), \
    D3D12VA_ENCODE_RC_MODE(CBR,  "Constant-bitrate"), \
    D3D12VA_ENCODE_RC_MODE(VBR,  "Variable-bitrate"), \
    D3D12VA_ENCODE_RC_MODE(QVBR, "Quality-defined variable-bitrate")

#endif /* AVCODEC_D3D12VA_ENCODE_H */
