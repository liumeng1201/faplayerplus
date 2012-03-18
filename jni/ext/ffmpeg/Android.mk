
LOCAL_PATH:= $(call my-dir)

FF_AVCODEC_SRC := \
    libavcodec/4xm.c \
    libavcodec/8bps.c \
    libavcodec/8svx.c \
    libavcodec/a64multienc.c \
    libavcodec/aac_ac3_parser.c \
    libavcodec/aac_adtstoasc_bsf.c \
    libavcodec/aac_parser.c \
    libavcodec/aacadtsdec.c \
    libavcodec/aaccoder.c \
    libavcodec/aacdec.c \
    libavcodec/aacenc.c \
    libavcodec/aacps.c \
    libavcodec/aacpsy.c \
    libavcodec/aacsbr.c \
    libavcodec/aactab.c \
    libavcodec/aandcttab.c \
    libavcodec/aasc.c \
    libavcodec/ac3.c \
    libavcodec/ac3_parser.c \
    libavcodec/ac3dec.c \
    libavcodec/ac3dec_data.c \
    libavcodec/ac3dsp.c \
    libavcodec/ac3enc_combined.c \
    libavcodec/ac3enc_fixed.c \
    libavcodec/ac3enc_float.c \
    libavcodec/ac3tab.c \
    libavcodec/acelp_filters.c \
    libavcodec/acelp_pitch_delay.c \
    libavcodec/acelp_vectors.c \
    libavcodec/adpcm.c \
    libavcodec/adxdec.c \
    libavcodec/adxenc.c \
    libavcodec/alac.c \
    libavcodec/alacenc.c \
    libavcodec/allcodecs.c \
    libavcodec/alsdec.c \
    libavcodec/amrnbdec.c \
    libavcodec/amrwbdec.c \
    libavcodec/anm.c \
    libavcodec/ansi.c \
    libavcodec/apedec.c \
    libavcodec/arm/ac3dsp_arm.S \
    libavcodec/arm/ac3dsp_armv6.S \
    libavcodec/arm/ac3dsp_init_arm.c \
    libavcodec/arm/dcadsp_init_arm.c \
    libavcodec/arm/dsputil_arm.S \
    libavcodec/arm/dsputil_armv6.S \
    libavcodec/arm/dsputil_init_arm.c \
    libavcodec/arm/dsputil_init_armv5te.c \
    libavcodec/arm/dsputil_init_armv6.c \
    libavcodec/arm/fft_fixed_init_arm.c \
    libavcodec/arm/fft_init_arm.c \
    libavcodec/arm/fmtconvert_init_arm.c \
    libavcodec/arm/h264dsp_init_arm.c \
    libavcodec/arm/h264pred_init_arm.c \
    libavcodec/arm/jrevdct_arm.S \
    libavcodec/arm/mpegvideo_arm.c \
    libavcodec/arm/mpegvideo_armv5te.c \
    libavcodec/arm/mpegvideo_armv5te_s.S \
    libavcodec/arm/simple_idct_arm.S \
    libavcodec/arm/simple_idct_armv5te.S \
    libavcodec/arm/simple_idct_armv6.S \
    libavcodec/arm/vp56dsp_init_arm.c \
    libavcodec/arm/vp8_armv6.S \
    libavcodec/arm/vp8dsp_init_arm.c \
    libavcodec/ass.c \
    libavcodec/ass_split.c \
    libavcodec/assdec.c \
    libavcodec/assenc.c \
    libavcodec/asv1.c \
    libavcodec/atrac.c \
    libavcodec/atrac1.c \
    libavcodec/atrac3.c \
    libavcodec/audioconvert.c \
    libavcodec/aura.c \
    libavcodec/avfft.c \
    libavcodec/avpacket.c \
    libavcodec/avs.c \
    libavcodec/bethsoftvideo.c \
    libavcodec/bfi.c \
    libavcodec/bgmc.c \
    libavcodec/bink.c \
    libavcodec/binkaudio.c \
    libavcodec/binkidct.c \
    libavcodec/bitstream.c \
    libavcodec/bitstream_filter.c \
    libavcodec/bmp.c \
    libavcodec/bmpenc.c \
    libavcodec/c93.c \
    libavcodec/cabac.c \
    libavcodec/cavs.c \
    libavcodec/cavs_parser.c \
    libavcodec/cavsdec.c \
    libavcodec/cavsdsp.c \
    libavcodec/cdgraphics.c \
    libavcodec/celp_filters.c \
    libavcodec/celp_math.c \
    libavcodec/cga_data.c \
    libavcodec/chomp_bsf.c \
    libavcodec/cinepak.c \
    libavcodec/cljr.c \
    libavcodec/cook.c \
    libavcodec/cscd.c \
    libavcodec/cyuv.c \
    libavcodec/dca.c \
    libavcodec/dca_parser.c \
    libavcodec/dcadsp.c \
    libavcodec/dcaenc.c \
    libavcodec/dct.c \
    libavcodec/dfa.c \
    libavcodec/dirac.c \
    libavcodec/dirac_parser.c \
    libavcodec/dnxhd_parser.c \
    libavcodec/dnxhddata.c \
    libavcodec/dnxhddec.c \
    libavcodec/dnxhdenc.c \
    libavcodec/dpcm.c \
    libavcodec/dpx.c \
    libavcodec/dpxenc.c \
    libavcodec/dsicinav.c \
    libavcodec/dsputil.c \
    libavcodec/dump_extradata_bsf.c \
    libavcodec/dv.c \
    libavcodec/dvbsub.c \
    libavcodec/dvbsub_parser.c \
    libavcodec/dvbsubdec.c \
    libavcodec/dvdata.c \
    libavcodec/dvdsub_parser.c \
    libavcodec/dvdsubdec.c \
    libavcodec/dvdsubenc.c \
    libavcodec/dwt.c \
    libavcodec/dxa.c \
    libavcodec/eac3dec.c \
    libavcodec/eac3dec_data.c \
    libavcodec/eacmv.c \
    libavcodec/eaidct.c \
    libavcodec/eamad.c \
    libavcodec/eatgq.c \
    libavcodec/eatgv.c \
    libavcodec/eatqi.c \
    libavcodec/elbg.c \
    libavcodec/error_resilience.c \
    libavcodec/escape124.c \
    libavcodec/faandct.c \
    libavcodec/faanidct.c \
    libavcodec/faxcompr.c \
    libavcodec/fft_fixed.c \
    libavcodec/fft_float.c \
    libavcodec/ffv1.c \
    libavcodec/flac.c \
    libavcodec/flac_parser.c \
    libavcodec/flacdata.c \
    libavcodec/flacdec.c \
    libavcodec/flacenc.c \
    libavcodec/flashsv.c \
    libavcodec/flashsv2enc.c \
    libavcodec/flashsvenc.c \
    libavcodec/flicvideo.c \
    libavcodec/flvdec.c \
    libavcodec/flvenc.c \
    libavcodec/fmtconvert.c \
    libavcodec/fraps.c \
    libavcodec/frwu.c \
    libavcodec/g722.c \
    libavcodec/g726.c \
    libavcodec/gif.c \
    libavcodec/gifdec.c \
    libavcodec/golomb.c \
    libavcodec/gsmdec.c \
    libavcodec/gsmdec_data.c \
    libavcodec/h261.c \
    libavcodec/h261_parser.c \
    libavcodec/h261dec.c \
    libavcodec/h261enc.c \
    libavcodec/h263.c \
    libavcodec/h263_parser.c \
    libavcodec/h263dec.c \
    libavcodec/h264.c \
    libavcodec/h264_cabac.c \
    libavcodec/h264_cavlc.c \
    libavcodec/h264_direct.c \
    libavcodec/h264_hl_motion.c \
    libavcodec/h264_loopfilter.c \
    libavcodec/h264_mp4toannexb_bsf.c \
    libavcodec/h264_parser.c \
    libavcodec/h264_ps.c \
    libavcodec/h264_refs.c \
    libavcodec/h264_sei.c \
    libavcodec/h264dsp.c \
    libavcodec/h264idct.c \
    libavcodec/h264pred.c \
    libavcodec/huffman.c \
    libavcodec/huffyuv.c \
    libavcodec/idcinvideo.c \
    libavcodec/iff.c \
    libavcodec/iirfilter.c \
    libavcodec/imc.c \
    libavcodec/imgconvert.c \
    libavcodec/imx_dump_header_bsf.c \
    libavcodec/indeo2.c \
    libavcodec/indeo3.c \
    libavcodec/indeo5.c \
    libavcodec/intelh263dec.c \
    libavcodec/interplayvideo.c \
    libavcodec/intrax8.c \
    libavcodec/intrax8dsp.c \
    libavcodec/ituh263dec.c \
    libavcodec/ituh263enc.c \
    libavcodec/ivi_common.c \
    libavcodec/ivi_dsp.c \
    libavcodec/jfdctfst.c \
    libavcodec/jfdctint.c \
    libavcodec/jpegls.c \
    libavcodec/jpeglsdec.c \
    libavcodec/jpeglsenc.c \
    libavcodec/jrevdct.c \
    libavcodec/jvdec.c \
    libavcodec/kbdwin.c \
    libavcodec/kgv1dec.c \
    libavcodec/kmvc.c \
    libavcodec/lagarith.c \
    libavcodec/lagarithrac.c \
    libavcodec/latm_parser.c \
    libavcodec/lcldec.c \
    libavcodec/lclenc.c \
    libavcodec/ljpegenc.c \
    libavcodec/loco.c \
    libavcodec/lpc.c \
    libavcodec/lsp.c \
    libavcodec/lzw.c \
    libavcodec/lzwenc.c \
    libavcodec/mace.c \
    libavcodec/mdct_fixed.c \
    libavcodec/mdct_float.c \
    libavcodec/mdec.c \
    libavcodec/mimic.c \
    libavcodec/mjpeg.c \
    libavcodec/mjpeg2jpeg_bsf.c \
    libavcodec/mjpeg_parser.c \
    libavcodec/mjpega_dump_header_bsf.c \
    libavcodec/mjpegbdec.c \
    libavcodec/mjpegdec.c \
    libavcodec/mjpegenc.c \
    libavcodec/mlp.c \
    libavcodec/mlp_parser.c \
    libavcodec/mlpdec.c \
    libavcodec/mlpdsp.c \
    libavcodec/mmvideo.c \
    libavcodec/motion_est.c \
    libavcodec/motionpixels.c \
    libavcodec/movsub_bsf.c \
    libavcodec/mp3_header_compress_bsf.c \
    libavcodec/mp3_header_decompress_bsf.c \
    libavcodec/mpc.c \
    libavcodec/mpc7.c \
    libavcodec/mpc8.c \
    libavcodec/mpeg12.c \
    libavcodec/mpeg12data.c \
    libavcodec/mpeg12enc.c \
    libavcodec/mpeg4audio.c \
    libavcodec/mpeg4video.c \
    libavcodec/mpeg4video_parser.c \
    libavcodec/mpeg4videodec.c \
    libavcodec/mpeg4videoenc.c \
    libavcodec/mpegaudio.c \
    libavcodec/mpegaudio_parser.c \
    libavcodec/mpegaudiodata.c \
    libavcodec/mpegaudiodec.c \
    libavcodec/mpegaudiodec_float.c \
    libavcodec/mpegaudiodecheader.c \
    libavcodec/mpegaudioenc.c \
    libavcodec/mpegvideo.c \
    libavcodec/mpegvideo_enc.c \
    libavcodec/mpegvideo_parser.c \
    libavcodec/msgsmdec.c \
    libavcodec/msmpeg4.c \
    libavcodec/msmpeg4data.c \
    libavcodec/msrle.c \
    libavcodec/msrledec.c \
    libavcodec/msvideo1.c \
    libavcodec/msvideo1enc.c \
    libavcodec/mxpegdec.c \
    libavcodec/nellymoser.c \
    libavcodec/nellymoserdec.c \
    libavcodec/nellymoserenc.c \
    libavcodec/noise_bsf.c \
    libavcodec/nuv.c \
    libavcodec/options.c \
    libavcodec/pamenc.c \
    libavcodec/parser.c \
    libavcodec/pcm-mpeg.c \
    libavcodec/pcm.c \
    libavcodec/pcx.c \
    libavcodec/pcxenc.c \
    libavcodec/pgssubdec.c \
    libavcodec/pictordec.c \
    libavcodec/png.c \
    libavcodec/pngdec.c \
    libavcodec/pngenc.c \
    libavcodec/pnm.c \
    libavcodec/pnm_parser.c \
    libavcodec/pnmdec.c \
    libavcodec/pnmenc.c \
    libavcodec/psymodel.c \
    libavcodec/pthread.c \
    libavcodec/ptx.c \
    libavcodec/qcelpdec.c \
    libavcodec/qdm2.c \
    libavcodec/qdrw.c \
    libavcodec/qpeg.c \
    libavcodec/qtrle.c \
    libavcodec/qtrleenc.c \
    libavcodec/r210dec.c \
    libavcodec/ra144.c \
    libavcodec/ra144dec.c \
    libavcodec/ra144enc.c \
    libavcodec/ra288.c \
    libavcodec/rangecoder.c \
    libavcodec/ratecontrol.c \
    libavcodec/raw.c \
    libavcodec/rawdec.c \
    libavcodec/rawenc.c \
    libavcodec/rdft.c \
    libavcodec/remove_extradata_bsf.c \
    libavcodec/resample.c \
    libavcodec/resample2.c \
    libavcodec/rl2.c \
    libavcodec/rle.c \
    libavcodec/roqaudioenc.c \
    libavcodec/roqvideo.c \
    libavcodec/roqvideodec.c \
    libavcodec/roqvideoenc.c \
    libavcodec/rpza.c \
    libavcodec/rtjpeg.c \
    libavcodec/rv10.c \
    libavcodec/rv10enc.c \
    libavcodec/rv20enc.c \
    libavcodec/rv30.c \
    libavcodec/rv30dsp.c \
    libavcodec/rv34.c \
    libavcodec/rv40.c \
    libavcodec/rv40dsp.c \
    libavcodec/s3tc.c \
    libavcodec/sgidec.c \
    libavcodec/sgienc.c \
    libavcodec/shorten.c \
    libavcodec/simple_idct.c \
    libavcodec/sinewin.c \
    libavcodec/sipr.c \
    libavcodec/sipr16k.c \
    libavcodec/smacker.c \
    libavcodec/smc.c \
    libavcodec/snow.c \
    libavcodec/sonic.c \
    libavcodec/sp5xdec.c \
    libavcodec/srtdec.c \
    libavcodec/srtenc.c \
    libavcodec/sunrast.c \
    libavcodec/svq1.c \
    libavcodec/svq1dec.c \
    libavcodec/svq1enc.c \
    libavcodec/svq3.c \
    libavcodec/synth_filter.c \
    libavcodec/targa.c \
    libavcodec/targaenc.c \
    libavcodec/tiertexseqv.c \
    libavcodec/tiff.c \
    libavcodec/tiffenc.c \
    libavcodec/tmv.c \
    libavcodec/truemotion1.c \
    libavcodec/truemotion2.c \
    libavcodec/truespeech.c \
    libavcodec/tscc.c \
    libavcodec/tta.c \
    libavcodec/twinvq.c \
    libavcodec/txd.c \
    libavcodec/ulti.c \
    libavcodec/utils.c \
    libavcodec/v210dec.c \
    libavcodec/v210enc.c \
    libavcodec/v210x.c \
    libavcodec/vb.c \
    libavcodec/vc1.c \
    libavcodec/vc1_parser.c \
    libavcodec/vc1data.c \
    libavcodec/vc1dec.c \
    libavcodec/vc1dsp.c \
    libavcodec/vcr1.c \
    libavcodec/vmdav.c \
    libavcodec/vmnc.c \
    libavcodec/vorbis.c \
    libavcodec/vorbis_data.c \
    libavcodec/vorbisdec.c \
    libavcodec/vorbisenc.c \
    libavcodec/vp3.c \
    libavcodec/vp3_parser.c \
    libavcodec/vp3dsp.c \
    libavcodec/vp5.c \
    libavcodec/vp56.c \
    libavcodec/vp56data.c \
    libavcodec/vp56dsp.c \
    libavcodec/vp56rac.c \
    libavcodec/vp6.c \
    libavcodec/vp6dsp.c \
    libavcodec/vp8.c \
    libavcodec/vp8_parser.c \
    libavcodec/vp8dsp.c \
    libavcodec/vqavideo.c \
    libavcodec/wavpack.c \
    libavcodec/wma.c \
    libavcodec/wmadec.c \
    libavcodec/wmaenc.c \
    libavcodec/wmaprodec.c \
    libavcodec/wmavoice.c \
    libavcodec/wmv2.c \
    libavcodec/wmv2dec.c \
    libavcodec/wmv2enc.c \
    libavcodec/wnv1.c \
    libavcodec/ws-snd1.c \
    libavcodec/xan.c \
    libavcodec/xiph.c \
    libavcodec/xl.c \
    libavcodec/xsubdec.c \
    libavcodec/xsubenc.c \
    libavcodec/xxan.c \
    libavcodec/yop.c \
    libavcodec/zmbv.c \
    libavcodec/zmbvenc.c

FF_AVCODEC_NEON_SRC := \
    libavcodec/arm/dsputil_init_neon.c \
    libavcodec/arm/dsputil_neon.S \
    libavcodec/arm/fmtconvert_neon.S \
    libavcodec/arm/int_neon.S \
    libavcodec/arm/mpegvideo_neon.S \
    libavcodec/arm/simple_idct_neon.S \
    libavcodec/arm/fft_neon.S \
    libavcodec/arm/fft_fixed_neon.S \
    libavcodec/arm/mdct_neon.S \
    libavcodec/arm/mdct_fixed_neon.S \
    libavcodec/arm/rdft_neon.S \
    libavcodec/arm/h264dsp_neon.S \
    libavcodec/arm/h264idct_neon.S \
    libavcodec/arm/h264pred_neon.S \
    libavcodec/arm/ac3dsp_neon.S \
    libavcodec/arm/dcadsp_neon.S \
    libavcodec/arm/synth_filter_neon.S \
    libavcodec/arm/vp3dsp_neon.S \
    libavcodec/arm/vp56dsp_neon.S \
    libavcodec/arm/vp8dsp_neon.S

FF_AVDEVICE_SRC := \
    libavdevice/alldevices.c \
    libavdevice/avdevice.c

FF_AVFILTER_SRC := \
    libavfilter/af_anull.c \
    libavfilter/allfilters.c \
    libavfilter/asink_anullsink.c \
    libavfilter/asrc_anullsrc.c \
    libavfilter/avfilter.c \
    libavfilter/avfiltergraph.c \
    libavfilter/defaults.c \
    libavfilter/drawutils.c \
    libavfilter/formats.c \
    libavfilter/graphparser.c \
    libavfilter/vf_aspect.c \
    libavfilter/vf_copy.c \
    libavfilter/vf_crop.c \
    libavfilter/vf_drawbox.c \
    libavfilter/vf_fade.c \
    libavfilter/vf_fieldorder.c \
    libavfilter/vf_fifo.c \
    libavfilter/vf_format.c \
    libavfilter/vf_gradfun.c \
    libavfilter/vf_hflip.c \
    libavfilter/vf_null.c \
    libavfilter/vf_overlay.c \
    libavfilter/vf_pad.c \
    libavfilter/vf_pixdesctest.c \
    libavfilter/vf_scale.c \
    libavfilter/vf_setpts.c \
    libavfilter/vf_settb.c \
    libavfilter/vf_showinfo.c \
    libavfilter/vf_slicify.c \
    libavfilter/vf_transpose.c \
    libavfilter/vf_unsharp.c \
    libavfilter/vf_vflip.c \
    libavfilter/vsink_nullsink.c \
    libavfilter/vsrc_buffer.c \
    libavfilter/vsrc_color.c \
    libavfilter/vsrc_movie.c \
    libavfilter/vsrc_nullsrc.c

FF_AVFORMAT_SRC := \
    libavformat/4xm.c \
    libavformat/a64.c \
    libavformat/aacdec.c \
    libavformat/ac3dec.c \
    libavformat/adtsenc.c \
    libavformat/aea.c \
    libavformat/aiffdec.c \
    libavformat/aiffenc.c \
    libavformat/allformats.c \
    libavformat/amr.c \
    libavformat/anm.c \
    libavformat/apc.c \
    libavformat/ape.c \
    libavformat/apetag.c \
    libavformat/applehttp.c \
    libavformat/applehttpproto.c \
    libavformat/asf.c \
    libavformat/asfcrypt.c \
    libavformat/asfdec.c \
    libavformat/asfenc.c \
    libavformat/assdec.c \
    libavformat/assenc.c \
    libavformat/au.c \
    libavformat/audiointerleave.c \
    libavformat/avc.c \
    libavformat/avi.c \
    libavformat/avidec.c \
    libavformat/avienc.c \
    libavformat/avio.c \
    libavformat/aviobuf.c \
    libavformat/avlanguage.c \
    libavformat/avs.c \
    libavformat/bethsoftvid.c \
    libavformat/bfi.c \
    libavformat/bink.c \
    libavformat/c93.c \
    libavformat/caf.c \
    libavformat/cafdec.c \
    libavformat/cavsvideodec.c \
    libavformat/cdg.c \
    libavformat/concat.c \
    libavformat/crcenc.c \
    libavformat/crypto.c \
    libavformat/cutils.c \
    libavformat/daud.c \
    libavformat/dfa.c \
    libavformat/diracdec.c \
    libavformat/dnxhddec.c \
    libavformat/dsicin.c \
    libavformat/dtsdec.c \
    libavformat/dv.c \
    libavformat/dvenc.c \
    libavformat/dxa.c \
    libavformat/eacdata.c \
    libavformat/electronicarts.c \
    libavformat/ffmdec.c \
    libavformat/ffmenc.c \
    libavformat/ffmetadec.c \
    libavformat/ffmetaenc.c \
    libavformat/file.c \
    libavformat/filmstripdec.c \
    libavformat/filmstripenc.c \
    libavformat/flacdec.c \
    libavformat/flacenc.c \
    libavformat/flacenc_header.c \
    libavformat/flic.c \
    libavformat/flvdec.c \
    libavformat/flvenc.c \
    libavformat/framecrcenc.c \
    libavformat/gif.c \
    libavformat/gopher.c \
    libavformat/gxf.c \
    libavformat/gxfenc.c \
    libavformat/h261dec.c \
    libavformat/h263dec.c \
    libavformat/h264dec.c \
    libavformat/http.c \
    libavformat/httpauth.c \
    libavformat/id3v1.c \
    libavformat/id3v2.c \
    libavformat/idcin.c \
    libavformat/idroqdec.c \
    libavformat/idroqenc.c \
    libavformat/iff.c \
    libavformat/img2.c \
    libavformat/ingenientdec.c \
    libavformat/ipmovie.c \
    libavformat/isom.c \
    libavformat/iss.c \
    libavformat/iv8.c \
    libavformat/ivfdec.c \
    libavformat/ivfenc.c \
    libavformat/jvdec.c \
    libavformat/lmlm4.c \
    libavformat/lxfdec.c \
    libavformat/m4vdec.c \
    libavformat/matroska.c \
    libavformat/matroskadec.c \
    libavformat/matroskaenc.c \
    libavformat/md5enc.c \
    libavformat/md5proto.c \
    libavformat/metadata.c \
    libavformat/microdvddec.c \
    libavformat/microdvdenc.c \
    libavformat/mm.c \
    libavformat/mmf.c \
    libavformat/mms.c \
    libavformat/mmsh.c \
    libavformat/mmst.c \
    libavformat/mov.c \
    libavformat/movenc.c \
    libavformat/movenchint.c \
    libavformat/mp3dec.c \
    libavformat/mp3enc.c \
    libavformat/mpc.c \
    libavformat/mpc8.c \
    libavformat/mpeg.c \
    libavformat/mpegenc.c \
    libavformat/mpegts.c \
    libavformat/mpegtsenc.c \
    libavformat/mpegvideodec.c \
    libavformat/mpjpeg.c \
    libavformat/msnwc_tcp.c \
    libavformat/mtv.c \
    libavformat/mvi.c \
    libavformat/mxf.c \
    libavformat/mxfdec.c \
    libavformat/mxfenc.c \
    libavformat/mxg.c \
    libavformat/ncdec.c \
    libavformat/nsvdec.c \
    libavformat/nullenc.c \
    libavformat/nut.c \
    libavformat/nutdec.c \
    libavformat/nutenc.c \
    libavformat/nuv.c \
    libavformat/oggdec.c \
    libavformat/oggenc.c \
    libavformat/oggparsecelt.c \
    libavformat/oggparsedirac.c \
    libavformat/oggparseflac.c \
    libavformat/oggparseogm.c \
    libavformat/oggparseskeleton.c \
    libavformat/oggparsespeex.c \
    libavformat/oggparsetheora.c \
    libavformat/oggparsevorbis.c \
    libavformat/oma.c \
    libavformat/options.c \
    libavformat/os_support.c \
    libavformat/pcm.c \
    libavformat/pcmdec.c \
    libavformat/pcmenc.c \
    libavformat/pmpdec.c \
    libavformat/psxstr.c \
    libavformat/pva.c \
    libavformat/qcp.c \
    libavformat/r3d.c \
    libavformat/rawdec.c \
    libavformat/rawenc.c \
    libavformat/rawvideodec.c \
    libavformat/rdt.c \
    libavformat/riff.c \
    libavformat/rl2.c \
    libavformat/rm.c \
    libavformat/rmdec.c \
    libavformat/rmenc.c \
    libavformat/rpl.c \
    libavformat/rso.c \
    libavformat/rsodec.c \
    libavformat/rsoenc.c \
    libavformat/rtmppkt.c \
    libavformat/rtmpproto.c \
    libavformat/rtp.c \
    libavformat/rtpdec.c \
    libavformat/rtpdec_amr.c \
    libavformat/rtpdec_asf.c \
    libavformat/rtpdec_h263.c \
    libavformat/rtpdec_h264.c \
    libavformat/rtpdec_latm.c \
    libavformat/rtpdec_mpeg4.c \
    libavformat/rtpdec_qcelp.c \
    libavformat/rtpdec_qdm2.c \
    libavformat/rtpdec_qt.c \
    libavformat/rtpdec_svq3.c \
    libavformat/rtpdec_vp8.c \
    libavformat/rtpdec_xiph.c \
    libavformat/rtpenc.c \
    libavformat/rtpenc_aac.c \
    libavformat/rtpenc_amr.c \
    libavformat/rtpenc_chain.c \
    libavformat/rtpenc_h263.c \
    libavformat/rtpenc_h264.c \
    libavformat/rtpenc_mpv.c \
    libavformat/rtpenc_vp8.c \
    libavformat/rtpenc_xiph.c \
    libavformat/rtpproto.c \
    libavformat/rtsp.c \
    libavformat/rtspdec.c \
    libavformat/rtspenc.c \
    libavformat/sapdec.c \
    libavformat/sapenc.c \
    libavformat/sauce.c \
    libavformat/sdp.c \
    libavformat/seek.c \
    libavformat/segafilm.c \
    libavformat/sierravmd.c \
    libavformat/siff.c \
    libavformat/smacker.c \
    libavformat/sol.c \
    libavformat/soxdec.c \
    libavformat/soxenc.c \
    libavformat/spdif.c \
    libavformat/spdifdec.c \
    libavformat/spdifenc.c \
    libavformat/srtdec.c \
    libavformat/swfdec.c \
    libavformat/swfenc.c \
    libavformat/tcp.c \
    libavformat/thp.c \
    libavformat/tiertexseq.c \
    libavformat/tmv.c \
    libavformat/tta.c \
    libavformat/tty.c \
    libavformat/txd.c \
    libavformat/udp.c \
    libavformat/utils.c \
    libavformat/vc1test.c \
    libavformat/vc1testenc.c \
    libavformat/voc.c \
    libavformat/vocdec.c \
    libavformat/vocenc.c \
    libavformat/vorbiscomment.c \
    libavformat/vqf.c \
    libavformat/wav.c \
    libavformat/wc3movie.c \
    libavformat/westwood.c \
    libavformat/wtv.c \
    libavformat/wtvdec.c \
    libavformat/wv.c \
    libavformat/xa.c \
    libavformat/xwma.c \
    libavformat/yop.c \
    libavformat/yuv4mpeg.c

FF_AVUTIL_SRC := \
    libavutil/adler32.c \
    libavutil/aes.c \
    libavutil/arm/cpu.c \
    libavutil/audioconvert.c \
    libavutil/avstring.c \
    libavutil/base64.c \
    libavutil/cpu.c \
    libavutil/crc.c \
    libavutil/des.c \
    libavutil/error.c \
    libavutil/eval.c \
    libavutil/fifo.c \
    libavutil/file.c \
    libavutil/imgutils.c \
    libavutil/intfloat_readwrite.c \
    libavutil/inverse.c \
    libavutil/lfg.c \
    libavutil/lls.c \
    libavutil/log.c \
    libavutil/lzo.c \
    libavutil/mathematics.c \
    libavutil/md5.c \
    libavutil/mem.c \
    libavutil/opt.c \
    libavutil/parseutils.c \
    libavutil/pixdesc.c \
    libavutil/random_seed.c \
    libavutil/rational.c \
    libavutil/rc4.c \
    libavutil/samplefmt.c \
    libavutil/sha.c \
    libavutil/tree.c \
    libavutil/utils.c

FF_SWSCALE_SRC := \
    libswscale/options.c \
    libswscale/rgb2rgb.c \
    libswscale/swscale.c \
    libswscale/utils.c \
    libswscale/yuv2rgb.c

FF_CFLAGS := -std=c99
FF_CFLAGS += -DHAVE_AV_CONFIG_H -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libavcodec

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/avcodec

LOCAL_CFLAGS += $(FF_CFLAGS)

LOCAL_SRC_FILES := \
    $(FF_AVCODEC_SRC)

ifeq ($(BUILD_WITH_NEON),1)
LOCAL_CFLAGS += -DHAVE_NEON=1
LOCAL_SRC_FILES += $(FF_AVCODEC_NEON_SRC)
else
LOCAL_CFLAGS += -DHAVE_NEON=0
endif

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libavdevice

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/avdevice

LOCAL_CFLAGS += $(FF_CFLAGS)

LOCAL_SRC_FILES := \
    $(FF_AVDEVICE_SRC)

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libavfilter

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/avfilter

LOCAL_CFLAGS += $(FF_CFLAGS)

LOCAL_SRC_FILES := \
    $(FF_AVFILTER_SRC)

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libavformat

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/avformat

LOCAL_CFLAGS += $(FF_CFLAGS)

LOCAL_SRC_FILES := \
    $(FF_AVFORMAT_SRC)

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libavutil

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/avutil

LOCAL_CFLAGS += $(FF_CFLAGS)

LOCAL_SRC_FILES := \
    $(FF_AVUTIL_SRC)

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
ifeq ($(BUILD_WITH_NEON),1)
LOCAL_ARM_NEON := true
endif

LOCAL_MODULE := libswscale

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/swscale

LOCAL_CFLAGS += $(FF_CFLAGS)

LOCAL_SRC_FILES := \
    $(FF_SWSCALE_SRC)

LOCAL_CFLAGS += -DHAVE_NEON=$(BUILD_WITH_NEON)

include $(BUILD_STATIC_LIBRARY)

include $(LOCAL_PATH)/Binary.mk

