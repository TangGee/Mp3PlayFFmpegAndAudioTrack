//
// Created by tlinux on 18-8-12.
//

#ifndef MP3PLAYFFMPEGANDAUDIOTRACK2_MP3DECODER_H
#define MP3PLAYFFMPEGANDAUDIOTRACK2_MP3DECODER_H

//TODO some op can move to ffmpeg class
#include <libavformat/avformat.h>

class Mp3Decoder {

public:
    Mp3Decoder(const char *filePath);
    int checkFileOk();
    ~Mp3Decoder();

    int start();

private:
    char *src_filename;
    char *errMsg;
    AVFormatContext *fmt_ctx;
    int audio_stream_idx;
    AVStream *audio_stream;
    AVCodecContext *audio_dec_ctx;
    int audio_frame_count;

    int init_ffmpeg();

    void setErrorMsg(const char *msg,...);
    int open_codec_context(int *stream_idx,
                           AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type);
    int decode_packet(AVPacket *pkt,AVFrame *frame, int *got_frame,int cached);
};


#endif //MP3PLAYFFMPEGANDAUDIOTRACK2_MP3DECODER_H
