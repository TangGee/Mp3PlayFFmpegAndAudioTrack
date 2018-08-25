//
// Created by tlinux on 18-8-12.
//

#ifndef MP3PLAYFFMPEGANDAUDIOTRACK2_MP3DECODER_H
#define MP3PLAYFFMPEGANDAUDIOTRACK2_MP3DECODER_H


#define LOG_NDEBUG 0
#define LOG_TAG "Mp3Decoder"

//TODO some op can move to ffmpeg class
#include "FFMPEG.h"


class Mp3Decoder {

public:
    Mp3Decoder(const char *filePath,void (*callback)(const void *, ssize_t, ssize_t,const void *data));
    Mp3Decoder(const char *filePath);
    int checkFileOk();
    ~Mp3Decoder();

    int start();

    void setPutBufferImpl(void (*callback)(const void *, ssize_t, ssize_t,const void *data),void *data) {
        putBuffer = callback;
        putBufferData = data;
    }


private:
    char *src_filename;
    char *errMsg;
    AVFormatContext *fmt_ctx = NULL;
    int audio_stream_idx = 0;
    AVStream *audio_stream = NULL;
    AVCodecContext *audio_dec_ctx = NULL;
    int audio_frame_count = 0;
    void (*putBuffer)(const void* buffer,ssize_t size,ssize_t count, const void* data) = NULL;
    void* putBufferData = NULL;

    void setErrorMsg(const char *msg,...);
    int open_codec_context(int *stream_idx,
                           AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type);
    int decode_packet(AVPacket *pkt,AVFrame *frame, int *got_frame,int cached);
};


#endif //MP3PLAYFFMPEGANDAUDIOTRACK2_MP3DECODER_H
