./configure --disable-x86asm
make




##ta-ubuntu@taubuntu-B560M-D3H:~/Users/gregory/ffmpeg$ ./configure --disable-x86asm
##install prefix            /usr/local
##source path               .
##C compiler                gcc
##C library                 glibc
##ARCH                      x86 (generic)
##big-endian                no
##runtime cpu detection     yes
##standalone assembly       no
##x86 assembler             nasm
##MMX enabled               yes
##MMXEXT enabled            yes
##3DNow! enabled            yes
##3DNow! extended enabled   yes
##SSE enabled               yes
##SSSE3 enabled             yes
##AESNI enabled             yes
##AVX enabled               yes
##AVX2 enabled              yes
##AVX-512 enabled           yes
##XOP enabled               yes
##FMA3 enabled              yes
##FMA4 enabled              yes
##i686 features enabled     yes
##CMOV is fast              yes
##EBX available             yes
##EBP available             yes
##debug symbols             yes
##strip symbols             yes
##optimize for size         no
##optimizations             yes
##static                    yes
##shared                    no
##postprocessing support    no
##network support           yes
##threading support         pthreads
##safe bitstream reader     yes
##texi2html enabled         no
##perl enabled              yes
##pod2man enabled           yes
##makeinfo enabled          no
##makeinfo supports HTML    no

##External libraries:
##iconv                   libxcb                  zlib

##External libraries providing hardware acceleration:
##v4l2_m2m                vaapi

##Libraries:
##avcodec                 avdevice                avfilter                avformat                avutil                  swresample              swscale

##Programs:
##ffmpeg                  ffprobe

##Enabled decoders:
##aac                     alac                    dds                     h263_v4l2m2m            mp3adu                  pcm_s24le               sbc                     vc1
##aac_fixed               alias_pix               derf_dpcm               h263i                   mp3adufloat             pcm_s24le_planar        scpr                    vc1_v4l2m2m
##aac_latm                als                     dfa                     h263p                   mp3float                pcm_s32be               screenpresso            vc1image
##aasc                    amrnb                   dirac                   h264                    mp3on4                  pcm_s32le               sdx2_dpcm               vcr1
##ac3                     amrwb                   dnxhd                   h264_v4l2m2m            mp3on4float             pcm_s32le_planar        sga                     vmdaudio
##ac3_fixed               amv                     dolby_e                 hap                     mpc7                    pcm_s64be               sgi                     vmdvideo
##acelp_kelvin            anm                     dpx                     hca                     mpc8                    pcm_s64le               sgirle                  vmnc
##adpcm_4xm               ansi                    dsd_lsbf                hcom                    mpeg1_v4l2m2m           pcm_s8                  sheervideo              vorbis
##adpcm_adx               ape                     dsd_lsbf_planar         hevc                    mpeg1video              pcm_s8_planar           shorten                 vp3
##adpcm_afc               apng                    dsd_msbf                hevc_v4l2m2m            mpeg2_v4l2m2m           pcm_sga                 simbiosis_imx           vp4
##adpcm_agm               aptx                    dsd_msbf_planar         hnm4_video              mpeg2video              pcm_u16be               sipr                    vp5
##adpcm_aica              aptx_hd                 dsicinaudio             hq_hqa                  mpeg4                   pcm_u16le               siren                   vp6
##adpcm_argo              arbc                    dsicinvideo             hqx                     mpeg4_v4l2m2m           pcm_u24be               smackaud                vp6a
##adpcm_ct                argo                    dss_sp                  huffyuv                 mpegvideo               pcm_u24le               smacker                 vp6f
##adpcm_dtk               ass                     dst                     hymt                    mpl2                    pcm_u32be               smc                     vp7
##adpcm_ea                asv1                    dvaudio                 iac                     msa1                    pcm_u32le               smvjpeg                 vp8
##adpcm_ea_maxis_xa       asv2                    dvbsub                  idcin                   mscc                    pcm_u8                  snow                    vp8_v4l2m2m
##adpcm_ea_r1             atrac1                  dvdsub                  idf                     msmpeg4v1               pcm_vidc                sol_dpcm                vp9
##adpcm_ea_r2             atrac3                  dvvideo                 iff_ilbm                msmpeg4v2               pcx                     sonic                   vp9_v4l2m2m
##adpcm_ea_r3             atrac3al                dxa                     ilbc                    msmpeg4v3               pfm                     sp5x                    vplayer
##adpcm_ea_xas            atrac3p                 dxtory                  imc                     msp2                    pgm                     speedhq                 vqa
##adpcm_g722              atrac3pal               dxv                     imm4                    msrle                   pgmyuv                  srgc                    wavpack
##adpcm_g726              atrac9                  eac3                    imm5                    mss1                    pgssub                  srt                     wcmv
##adpcm_g726le            aura                    eacmv                   indeo2                  mss2                    pgx                     ssa                     webp
##adpcm_ima_acorn         aura2                   eamad                   indeo3                  msvideo1                photocd                 stl                     webvtt
##adpcm_ima_alp           av1                     eatgq                   indeo4                  mszh                    pictor                  subrip                  wmalossless
##adpcm_ima_amv           avrn                    eatgv                   indeo5                  mts2                    pixlet                  subviewer               wmapro
##adpcm_ima_apc           avrp                    eatqi                   interplay_acm           mv30                    pjs                     subviewer1              wmav1
##adpcm_ima_apm           avs                     eightbps                interplay_dpcm          mvc1                    png                     sunrast                 wmav2
##adpcm_ima_cunning       avui                    eightsvx_exp            interplay_video         mvc2                    ppm                     svq1                    wmavoice
##adpcm_ima_dat4          ayuv                    eightsvx_fib            ipu                     mvdv                    prores                  svq3                    wmv1
##adpcm_ima_dk3           bethsoftvid             escape124               jacosub                 mvha                    prosumer                tak                     wmv2
##adpcm_ima_dk4           bfi                     escape130               jpeg2000                mwsc                    psd                     targa                   wmv3
##adpcm_ima_ea_eacs       bink                    evrc                    jpegls                  mxpeg                   ptx                     targa_y216              wmv3image
##adpcm_ima_ea_sead       binkaudio_dct           exr                     jv                      nellymoser              qcelp                   tdsc                    wnv1
##adpcm_ima_iss           binkaudio_rdft          fastaudio               kgv1                    notchlc                 qdm2                    text                    wrapped_avframe
##adpcm_ima_moflex        bintext                 ffv1                    kmvc                    nuv                     qdmc                    theora                  ws_snd1
##adpcm_ima_mtf           bitpacked               ffvhuff                 lagarith                on2avc                  qdraw                   thp                     xan_dpcm
##adpcm_ima_oki           bmp                     ffwavesynth             loco                    opus                    qpeg                    tiertexseqvideo         xan_wc3
##adpcm_ima_qt            bmv_audio               fic                     lscr                    paf_audio               qtrle                   tiff                    xan_wc4
##adpcm_ima_rad           bmv_video               fits                    m101                    paf_video               r10k                    tmv                     xbin
##adpcm_ima_smjpeg        brender_pix             flac                    mace3                   pam                     r210                    truehd                  xbm
##adpcm_ima_ssi           c93                     flashsv                 mace6                   pbm                     ra_144                  truemotion1             xface
##adpcm_ima_wav           cavs                    flashsv2                magicyuv                pcm_alaw                ra_288                  truemotion2             xl
##adpcm_ima_ws            ccaption                flic                    mdec                    pcm_bluray              ralf                    truemotion2rt           xma1
##adpcm_ms                cdgraphics              flv                     metasound               pcm_dvd                 rasc                    truespeech              xma2
##adpcm_mtaf              cdtoons                 fmvc                    microdvd                pcm_f16le               rawvideo                tscc                    xpm
##adpcm_psx               cdxl                    fourxm                  mimic                   pcm_f24le               realtext                tscc2                   xsub
##adpcm_sbpro_2           cfhd                    fraps                   mjpeg                   pcm_f32be               rl2                     tta                     xwd
##adpcm_sbpro_3           cinepak                 frwu                    mjpegb                  pcm_f32le               roq                     twinvq                  y41p
##adpcm_sbpro_4           clearvideo              g2m                     mlp                     pcm_f64be               roq_dpcm                txd                     ylc
##adpcm_swf               cljr                    g723_1                  mmvideo                 pcm_f64le               rpza                    ulti                    yop
##adpcm_thp               cllc                    g729                    mobiclip                pcm_lxf                 rscc                    utvideo                 yuv4
##adpcm_thp_le            comfortnoise            gdv                     motionpixels            pcm_mulaw               rv10                    v210                    zero12v
##adpcm_vima              cook                    gif                     movtext                 pcm_s16be               rv20                    v210x                   zerocodec
##adpcm_xa                cpia                    gremlin_dpcm            mp1                     pcm_s16be_planar        rv30                    v308                    zlib
##adpcm_yamaha            cri                     gsm                     mp1float                pcm_s16le               rv40                    v408                    zmbv
##adpcm_zork              cscd                    gsm_ms                  mp2                     pcm_s16le_planar        s302m                   v410
##agm                     cyuv                    h261                    mp2float                pcm_s24be               sami                    vb
##aic                     dca                     h263                    mp3                     pcm_s24daud             sanm                    vble

##Enabled encoders:
##a64multi                apng                    ffvhuff                 mlp                     pcm_s16be               pcx                     sonic                   vp9_vaapi
##a64multi5               aptx                    fits                    movtext                 pcm_s16be_planar        pfm                     sonic_ls                wavpack
##aac                     aptx_hd                 flac                    mp2                     pcm_s16le               pgm                     speedhq                 webvtt
##ac3                     ass                     flashsv                 mp2fixed                pcm_s16le_planar        pgmyuv                  srt                     wmav1
##ac3_fixed               asv1                    flashsv2                mpeg1video              pcm_s24be               png                     ssa                     wmav2
##adpcm_adx               asv2                    flv                     mpeg2_vaapi             pcm_s24daud             ppm                     subrip                  wmv1
##adpcm_argo              avrp                    g723_1                  mpeg2video              pcm_s24le               prores                  sunrast                 wmv2
##adpcm_g722              avui                    gif                     mpeg4                   pcm_s24le_planar        prores_aw               svq1                    wrapped_avframe
##adpcm_g726              ayuv                    h261                    mpeg4_v4l2m2m           pcm_s32be               prores_ks               targa                   xbm
##adpcm_g726le            bmp                     h263                    msmpeg4v2               pcm_s32le               qtrle                   text                    xface
##adpcm_ima_alp           cfhd                    h263_v4l2m2m            msmpeg4v3               pcm_s32le_planar        r10k                    tiff                    xsub
##adpcm_ima_amv           cinepak                 h263p                   msvideo1                pcm_s64be               r210                    truehd                  xwd
##adpcm_ima_apm           cljr                    h264_v4l2m2m            nellymoser              pcm_s64le               ra_144                  tta                     y41p
##adpcm_ima_qt            comfortnoise            h264_vaapi              opus                    pcm_s8                  rawvideo                ttml                    yuv4
##adpcm_ima_ssi           dca                     hevc_v4l2m2m            pam                     pcm_s8_planar           roq                     utvideo                 zlib
##adpcm_ima_wav           dnxhd                   hevc_vaapi              pbm                     pcm_u16be               roq_dpcm                v210                    zmbv
##adpcm_ima_ws            dpx                     huffyuv                 pcm_alaw                pcm_u16le               rpza                    v308
##adpcm_ms                dvbsub                  jpeg2000                pcm_dvd                 pcm_u24be               rv10                    v408
##adpcm_swf               dvdsub                  jpegls                  pcm_f32be               pcm_u24le               rv20                    v410
##adpcm_yamaha            dvvideo                 ljpeg                   pcm_f32le               pcm_u32be               s302m                   vc2
##alac                    eac3                    magicyuv                pcm_f64be               pcm_u32le               sbc                     vorbis
##alias_pix               exr                     mjpeg                   pcm_f64le               pcm_u8                  sgi                     vp8_v4l2m2m
##amv                     ffv1                    mjpeg_vaapi             pcm_mulaw               pcm_vidc                snow                    vp8_vaapi

##Enabled hwaccels:
##av1_vaapi               h264_vaapi              mjpeg_vaapi             mpeg4_vaapi             vp8_vaapi               wmv3_vaapi
##h263_vaapi              hevc_vaapi              mpeg2_vaapi             vc1_vaapi               vp9_vaapi

##Enabled parsers:
##aac                     bmp                     dolby_e                 g723_1                  hevc                    mpegvideo               sipr                    webp
##aac_latm                cavsvideo               dpx                     g729                    ipu                     opus                    tak                     xbm
##ac3                     cook                    dvaudio                 gif                     jpeg2000                png                     vc1                     xma
##adx                     cri                     dvbsub                  gsm                     mjpeg                   pnm                     vorbis
##av1                     dca                     dvd_nav                 h261                    mlp                     rv30                    vp3
##avs2                    dirac                   dvdsub                  h263                    mpeg4video              rv40                    vp8
##avs3                    dnxhd                   flac                    h264                    mpegaudio               sbc                     vp9

##Enabled demuxers:
##aa                      bethsoftvid             filmstrip               image_exr_pipe          luodat                  nsv                     rpl                     tiertexseq
##aac                     bfi                     fits                    image_gif_pipe          lvf                     nut                     rsd                     tmv
##aax                     bfstm                   flac                    image_j2k_pipe          lxf                     nuv                     rso                     truehd
##ac3                     bink                    flic                    image_jpeg_pipe         m4v                     obu                     rtp                     tta
##ace                     binka                   flv                     image_jpegls_pipe       matroska                ogg                     rtsp                    tty
##acm                     bintext                 fourxm                  image_pam_pipe          mca                     oma                     s337m                   txd
##act                     bit                     frm                     image_pbm_pipe          mcc                     paf                     sami                    ty
##adf                     bmv                     fsb                     image_pcx_pipe          mgsts                   pcm_alaw                sap                     v210
##adp                     boa                     fwse                    image_pgm_pipe          microdvd                pcm_f32be               sbc                     v210x
##ads                     brstm                   g722                    image_pgmyuv_pipe       mjpeg                   pcm_f32le               sbg                     vag
##adx                     c93                     g723_1                  image_pgx_pipe          mjpeg_2000              pcm_f64be               scc                     vc1
##aea                     caf                     g726                    image_photocd_pipe      mlp                     pcm_f64le               sdp                     vc1t
##afc                     cavsvideo               g726le                  image_pictor_pipe       mlv                     pcm_mulaw               sdr2                    vividas
##aiff                    cdg                     g729                    image_png_pipe          mm                      pcm_s16be               sds                     vivo
##aix                     cdxl                    gdv                     image_ppm_pipe          mmf                     pcm_s16le               sdx                     vmd
##alp                     cine                    genh                    image_psd_pipe          mods                    pcm_s24be               segafilm                vobsub
##amr                     codec2                  gif                     image_qdraw_pipe        moflex                  pcm_s24le               ser                     voc
##amrnb                   codec2raw               gsm                     image_sgi_pipe          mov                     pcm_s32be               sga                     vpk
##amrwb                   concat                  gxf                     image_sunrast_pipe      mp3                     pcm_s32le               shorten                 vplayer
##anm                     data                    h261                    image_svg_pipe          mpc                     pcm_s8                  siff                    vqf
##apc                     daud                    h263                    image_tiff_pipe         mpc8                    pcm_u16be               simbiosis_imx           w64
##ape                     dcstr                   h264                    image_webp_pipe         mpegps                  pcm_u16le               sln                     wav
##apm                     derf                    hca                     image_xbm_pipe          mpegts                  pcm_u24be               smacker                 wc3
##apng                    dfa                     hcom                    image_xpm_pipe          mpegtsraw               pcm_u24le               smjpeg                  webm_dash_manifest
##aptx                    dhav                    hevc                    image_xwd_pipe          mpegvideo               pcm_u32be               smush                   webvtt
##aptx_hd                 dirac                   hls                     ingenient               mpjpeg                  pcm_u32le               sol                     wsaud
##aqtitle                 dnxhd                   hnm                     ipmovie                 mpl2                    pcm_u8                  sox                     wsd
##argo_asf                dsf                     ico                     ipu                     mpsub                   pcm_vidc                spdif                   wsvqa
##argo_brp                dsicin                  idcin                   ircam                   msf                     pjs                     srt                     wtv
##argo_cvg                dss                     idf                     iss                     msnwc_tcp               pmp                     stl                     wv
##asf                     dts                     iff                     iv8                     msp                     pp_bnk                  str                     wve
##asf_o                   dtshd                   ifv                     ivf                     mtaf                    pva                     subviewer               xa
##ass                     dv                      ilbc                    ivr                     mtv                     pvf                     subviewer1              xbin
##ast                     dvbsub                  image2                  jacosub                 musx                    qcp                     sup                     xmv
##au                      dvbtxt                  image2_alias_pix        jv                      mv                      r3d                     svag                    xvag
##av1                     dxa                     image2_brender_pix      kux                     mvi                     rawvideo                svs                     xwma
##avi                     ea                      image2pipe              kvag                    mxf                     realtext                swf                     yop
##avr                     ea_cdata                image_bmp_pipe          live_flv                mxg                     redspark                tak                     yuv4mpegpipe
##avs                     eac3                    image_cri_pipe          lmlm4                   nc                      rl2                     tedcaptions
##avs2                    epaf                    image_dds_pipe          loas                    nistsphere              rm                      thp
##avs3                    ffmetadata              image_dpx_pipe          lrc                     nsp                     roq                     threedostr

##Enabled muxers:
##a64                     bit                     framecrc                ircam                   mpeg1video              pcm_mulaw               rtp_mpegts              ttml
##ac3                     caf                     framehash               ismv                    mpeg2dvd                pcm_s16be               rtsp                    uncodedframecrc
##adts                    cavsvideo               framemd5                ivf                     mpeg2svcd               pcm_s16le               sap                     vc1
##adx                     codec2                  g722                    jacosub                 mpeg2video              pcm_s24be               sbc                     vc1t
##aiff                    codec2raw               g723_1                  kvag                    mpeg2vob                pcm_s24le               scc                     voc
##alp                     crc                     g726                    latm                    mpegts                  pcm_s32be               segafilm                w64
##amr                     dash                    g726le                  lrc                     mpjpeg                  pcm_s32le               segment                 wav
##amv                     data                    gif                     m4v                     mxf                     pcm_s8                  smjpeg                  webm
##apm                     daud                    gsm                     matroska                mxf_d10                 pcm_u16be               smoothstreaming         webm_chunk
##apng                    dirac                   gxf                     matroska_audio          mxf_opatom              pcm_u16le               sox                     webm_dash_manifest
##aptx                    dnxhd                   h261                    md5                     null                    pcm_u24be               spdif                   webp
##aptx_hd                 dts                     h263                    microdvd                nut                     pcm_u24le               spx                     webvtt
##argo_asf                dv                      h264                    mjpeg                   oga                     pcm_u32be               srt                     wsaud
##argo_cvg                eac3                    hash                    mkvtimestamp_v2         ogg                     pcm_u32le               stream_segment          wtv
##asf                     f4v                     hds                     mlp                     ogv                     pcm_u8                  streamhash              wv
##asf_stream              ffmetadata              hevc                    mmf                     oma                     pcm_vidc                sup                     yuv4mpegpipe
##ass                     fifo                    hls                     mov                     opus                    psp                     swf
##ast                     fifo_test               ico                     mp2                     pcm_alaw                rawvideo                tee
##au                      filmstrip               ilbc                    mp3                     pcm_f32be               rm                      tg2
##avi                     fits                    image2                  mp4                     pcm_f32le               roq                     tgp
##avm2                    flac                    image2pipe              mpeg1system             pcm_f64be               rso                     truehd
##avs2                    flv                     ipod                    mpeg1vcd                pcm_f64le               rtp                     tta

##Enabled protocols:
##async                   data                    gopher                  icecast                 pipe                    rtp                     tee
##cache                   ffrtmphttp              hls                     md5                     prompeg                 srtp                    udp
##concat                  file                    http                    mmsh                    rtmp                    subfile                 udplite
##crypto                  ftp                     httpproxy               mmst                    rtmpt                   tcp                     unix

##Enabled filters:
##abench                  anlms                   cas                     despill                 guided                  mestimate               scdet                   swaprect
##abitscope               anoisesrc               cellauto                detelecine              haas                    metadata                scroll                  swapuv
##acompressor             anull                   channelmap              dilation                haldclut                midequalizer            select                  tblend
##acontrast               anullsink               channelsplit            displace                haldclutsrc             minterpolate            selectivecolor          telecine
##acopy                   anullsrc                chorus                  dnn_classify            hdcd                    mix                     sendcmd                 testsrc
##acrossfade              apad                    chromahold              dnn_detect              headphone               monochrome              separatefields          testsrc2
##acrossover              aperms                  chromakey               dnn_processing          hflip                   movie                   setdar                  thistogram
##acrusher                aphasemeter             chromanr                doubleweave             highpass                msad                    setfield                threshold
##acue                    aphaser                 chromashift             drawbox                 highshelf               negate                  setparams               thumbnail
##addroi                  aphaseshift             ciescope                drawgraph               hilbert                 nlmeans                 setpts                  tile
##adeclick                apulsator               codecview               drawgrid                histogram               noformat                setrange                tlut2
##adeclip                 arealtime               color                   drmeter                 hqx                     noise                   setsar                  tmedian
##adelay                  aresample               colorbalance            dynaudnorm              hstack                  normalize               settb                   tmidequalizer
##adenorm                 areverse                colorchannelmixer       earwax                  hue                     null                    sharpness_vaapi         tmix
##aderivative             arnndn                  colorcontrast           ebur128                 hwdownload              nullsink                shear                   tonemap
##adrawgraph              aselect                 colorcorrect            edgedetect              hwmap                   nullsrc                 showcqt                 tonemap_vaapi
##aecho                   asendcmd                colorhold               elbg                    hwupload                oscilloscope            showfreqs               tpad
##aemphasis               asetnsamples            colorize                entropy                 hysteresis              overlay                 showinfo                transpose
##aeval                   asetpts                 colorkey                epx                     identity                pad                     showpalette             transpose_vaapi
##aevalsrc                asetrate                colorlevels             equalizer               idet                    pal100bars              showspatial             treble
##aexciter                asettb                  colorspace              erosion                 il                      pal75bars               showspectrum            tremolo
##afade                   ashowinfo               colortemperature        estdif                  inflate                 palettegen              showspectrumpic         trim
##afftdn                  asidedata               compand                 exposure                interleave              paletteuse              showvolume              unpremultiply
##afftfilt                asoftclip               compensationdelay       extractplanes           join                    pan                     showwaves               unsharp
##afifo                   asplit                  concat                  extrastereo             kirsch                  perms                   showwavespic            untile
##afir                    astats                  convolution             fade                    lagfun                  photosensitivity        shuffleframes           v360
##afirsrc                 astreamselect           convolve                fftdnoiz                lenscorrection          pixdesctest             shufflepixels           vectorscope
##aformat                 asubboost               copy                    fftfilt                 life                    pixscope                shuffleplanes           vflip
##afreqshift              asubcut                 crop                    field                   limiter                 premultiply             sidechaincompress       vfrdet
##agate                   asupercut               crossfeed               fieldhint               loop                    prewitt                 sidechaingate           vibrance
##agraphmonitor           asuperpass              crystalizer             fieldmatch              loudnorm                procamp_vaapi           sidedata                vibrato
##ahistogram              asuperstop              cue                     fieldorder              lowpass                 pseudocolor             sierpinski              vif
##aiir                    atadenoise              curves                  fifo                    lowshelf                psnr                    signalstats             vignette
##aintegral               atempo                  datascope               fillborders             lumakey                 qp                      silencedetect           vmafmotion
##ainterleave             atrim                   dblur                   firequalizer            lut                     random                  silenceremove           volume
##alimiter                avectorscope            dcshift                 flanger                 lut1d                   readeia608              sinc                    volumedetect
##allpass                 avgblur                 dctdnoiz                floodfill               lut2                    readvitc                sine                    vstack
##allrgb                  axcorrelate             deband                  format                  lut3d                   realtime                smptebars               w3fdif
##allyuv                  bandpass                deblock                 fps                     lutrgb                  remap                   smptehdbars             waveform
##aloop                   bandreject              decimate                framepack               lutyuv                  removegrain             sobel                   weave
##alphaextract            bass                    deconvolve              framerate               mandelbrot              removelogo              spectrumsynth           xbr
##alphamerge              bbox                    dedot                   framestep               maskedclamp             replaygain              speechnorm              xfade
##amerge                  bench                   deesser                 freezedetect            maskedmax               reverse                 split                   xmedian
##ametadata               bilateral               deflate                 freezeframes            maskedmerge             rgbashift               sr                      xstack
##amix                    biquad                  deflicker               gblur                   maskedmin               rgbtestsrc              ssim                    yadif
##amovie                  bitplanenoise           deinterlace_vaapi       geq                     maskedthreshold         roberts                 stereotools             yaepblur
##amplify                 blackdetect             dejudder                gradfun                 maskfun                 rotate                  stereowiden             yuvtestsrc
##amultiply               blend                   denoise_vaapi           gradients               mcompand                scale                   streamselect            zoompan
##anequalizer             bm3d                    derain                  graphmonitor            median                  scale2ref               superequalizer
##anlmdn                  bwdif                   deshake                 greyedge                mergeplanes             scale_vaapi             surround

##Enabled bsfs:
##aac_adtstoasc           dca_core                h264_metadata           hevc_mp4toannexb        mp3_header_decompress   opus_metadata           text2movsub             vp9_superframe
##av1_frame_merge         dump_extradata          h264_mp4toannexb        imx_dump_header         mpeg2_metadata          pcm_rechunk             trace_headers           vp9_superframe_split
##av1_frame_split         eac3_core               h264_redundant_pps      mjpeg2jpeg              mpeg4_unpack_bframes    prores_metadata         truehd_core
##av1_metadata            extract_extradata       hapqa_extract           mjpega_dump_header      noise                   remove_extradata        vp9_metadata
##chomp                   filter_units            hevc_metadata           mov2textsub             null                    setts                   vp9_raw_reorder

##Enabled indevs:
##fbdev                   lavfi                   oss                     v4l2                    xcbgrab

##Enabled outdevs:
##fbdev                   oss                     v4l2

##License: LGPL version 2.1 or later
##ta-ubuntu@taubuntu-B560M-D3H:~/Users/gregory/ffmpeg$ ./configure --disable-x86asm