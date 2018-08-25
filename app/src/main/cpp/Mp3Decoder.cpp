//
// Created by tlinux on 18-8-12.
//

#include "Mp3Decoder.h"
#include <sys/stat.h>
#include "log.h"


Mp3Decoder::Mp3Decoder(const char *filePath,
                       void (*callback)(const void *, ssize_t, ssize_t, const void*) ) {
    int len = strlen(filePath);
    src_filename = new char[sizeof(char)*(len+100)];
    snprintf(src_filename,len+100,"%s",filePath);
    putBuffer = callback;
}

Mp3Decoder::Mp3Decoder(const char *filePath) {
    int len = strlen(filePath);
    src_filename = new char[sizeof(char)*(len+100)];
    snprintf(src_filename,len+100,"%s",filePath);
}

int Mp3Decoder::checkFileOk() {
    struct stat buf;
    int ret = stat(src_filename, &buf);
    if (buf.st_mode |S_IFMT) {
        return ret;
    }
    return -1;
}

Mp3Decoder::~Mp3Decoder() {
    if (src_filename) {
        delete src_filename;
        src_filename = NULL;
    }
    if (errMsg) {
        delete  errMsg;
        errMsg = NULL;
    }

    if (putBufferData) {
        delete putBufferData;
        putBufferData = NULL;
    }
}

void Mp3Decoder::setErrorMsg(const char *msg, ...) {
    if (!errMsg) {
        errMsg = new char[256];
    }
//    va_list ap;
//    int size = snprintf(errMsg,255,msg,ap);
//    errMsg[size] = '\0';
}

int Mp3Decoder::start() {
    int ret =0, got_frame;
    AVFrame *frame = NULL;
    AVPacket pkt;

    //1 打开文件获取fmt_ctx(用于解封装)
    if(avformat_open_input(&fmt_ctx,src_filename,NULL,NULL)<0){
        fprintf(stderr, "Could not open source file %s\n", src_filename);
        setErrorMsg("Could not open source file %s\n", src_filename);
        return -1;
    }

    // 找一下stream
    if (avformat_find_stream_info(fmt_ctx,NULL)<0) {
        fprintf(stderr, "Could not find stream information\n");
        setErrorMsg( "Could not find stream information\n");
        ret = -1;
        goto close_fmt;
    }

    //找到audio stream 并且获取相应的codec(解码)
    if(open_codec_context(&audio_stream_idx,&audio_dec_ctx,fmt_ctx,AVMEDIA_TYPE_AUDIO)>=0) {
        audio_stream = fmt_ctx->streams[audio_stream_idx];
    } else {
        fprintf(stderr, "Could not find stream information\n");
        setErrorMsg( "Could not find stream information\n");
        ret = -1;
        goto close_fmt;
    }

    av_dump_format(fmt_ctx,0,src_filename,0);

    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        setErrorMsg("Could not allocate frame\n");
        ret = AVERROR(ENOMEM);
        goto end;
    }

    av_init_packet(&pkt);
    pkt.data = NULL;
    pkt.size = 0;

    while (av_read_frame(fmt_ctx,&pkt)>=0) {
        AVPacket orig_pkt = pkt;
        do {
            ret = decode_packet(&pkt,frame,&got_frame,0);
            pkt.data += ret;
            pkt.size -= ret;
        } while (pkt.size>0);
        av_packet_unref(&orig_pkt);
    }

    pkt.data = NULL;
    pkt.size = 0;
    do {
        ret = decode_packet(&pkt,frame,&got_frame,0);
    } while (got_frame);

end:
    av_frame_free(&frame);
    avcodec_free_context(&audio_dec_ctx);

close_fmt:
    avformat_close_input(&fmt_ctx);
    return ret;
}

int Mp3Decoder::open_codec_context(int *stream_idx, AVCodecContext **dec_ctx,
                                   AVFormatContext *fmt_ctx, enum AVMediaType type) {
    int ret, stream_index;
    AVStream *st;
    AVCodec *dec = NULL;
    AVDictionary *ops = NULL;

    //find the stream
    ret = av_find_best_stream(fmt_ctx,type,-1,-1,NULL,0);
    if (ret<0) {
        ALOGE( "Could not find %s stream in input file '%s'\n",
                av_get_media_type_string(type), src_filename);
        setErrorMsg("Could not find %s stream in input file '%s'\n",
                    av_get_media_type_string(type), src_filename);
        return ret;
    } else{
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];

        /* find decoder for the stream */
        dec = avcodec_find_decoder(st->codecpar->codec_id);
        if (!dec) {
            ALOGE( "Failed to find %s codec\n",
                    av_get_media_type_string(type));
            setErrorMsg("Failed to find %s codec\n",
                        av_get_media_type_string(type));
            return AVERROR(EINVAL);

        }

        *dec_ctx = avcodec_alloc_context3(dec);
        if (!*dec_ctx) { // alloc falis
            ALOGE( "Failed to allocate the %s codec context\n",
                    av_get_media_type_string(type));
            setErrorMsg( "Failed to allocate the %s codec context\n",
                         av_get_media_type_string(type));
            return AVERROR(ENOMEM);
        }

        if ((ret = avcodec_parameters_to_context(*dec_ctx,st->codecpar))<0){
            ALOGE( "Failed to copy %s codec parameters to decoder context\n",
                    av_get_media_type_string(type));
            setErrorMsg("Failed to copy %s codec parameters to decoder context\n",
                        av_get_media_type_string(type));
            return ret;
        }

        /* Init the decoders, with or without reference counting */
        av_dict_set(&ops,"refcounted_frames","0",0);
        if ((ret = avcodec_open2(*dec_ctx,dec,&ops))<0){
            ALOGE( "Failed to open %s codec\n",
                    av_get_media_type_string(type));
            setErrorMsg("Failed to open %s codec\n",
                        av_get_media_type_string(type));
            return ret;
        }
        *stream_idx = stream_index;
    }

    return 0;
}


int Mp3Decoder::decode_packet(AVPacket *pPkt,AVFrame *frame,int *got_frame, int cached) {
    int ret =0;
    int decoded = pPkt->size;

    if (pPkt->stream_index!=audio_stream_idx) {
        pPkt->size = 0;
        return ret;
    }

    *got_frame = 0;

    ret = avcodec_decode_audio4(audio_dec_ctx,frame,got_frame,pPkt);
    if (ret<0){
        ALOGE("Error decoding audio frame (%s)\n", av_err2str(ret));
        setErrorMsg("Error decoding audio frame (%s)\n", av_err2str(ret));
        return ret;
    }
    /* Some audio decoders decode only part of the packet, and have to be
      * called again with the remainder of the packet data.
      * Sample: fate-suite/lossless-audio/luckynight-partial.shn
      * Also, some decoders might over-read the packet. */
    decoded = FFMIN(ret,pPkt->size);

    if (*got_frame) {
        size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(
                (AVSampleFormat) frame->format);
        ALOGD("audio_frame%s n:%d nb_samples:%d pts:%s\n",
               cached ? "(cached)" : "",
               audio_frame_count++, frame->nb_samples,
               av_ts2timestr(frame->pts, &audio_dec_ctx->time_base));

        /* Write the raw audio data samples of the first plane. This works
         * fine for packed formats (e.g. AV_SAMPLE_FMT_S16). However,
         * most audio decoders output planar audio, which uses a separate
         * plane of audio samples for each channel (e.g. AV_SAMPLE_FMT_S16P).
         * In other words, this code will write only the first audio channel
         * in these cases.
         * You should use libswresample or libavfilter to convert the frame
         * to packed data. */
//        fwrite(frame->extended_data[0], 1, unpadded_linesize, audio_dst_file);
        if (putBuffer) {
            putBuffer(frame->extended_data[0],1,unpadded_linesize,putBufferData);
        }
    }

    /* If we use frame reference counting, we own the data and need
    * to de-reference it when we don't use it anymore */
    if (*got_frame & 0)
        av_frame_unref(frame);

    return decoded;
}