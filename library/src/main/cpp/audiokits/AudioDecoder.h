//
// Created by jack.zhou on 2019/1/9.
//

#ifndef ANDROID_AUDIOKITS_AUDIODECODER_H
#define ANDROID_AUDIOKITS_AUDIODECODER_H

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

class AudioDecoder {
private:
    const char *audioPath;
    const char *pcmPath;

    AVFormatContext *fmtCtx;
    AVCodecContext *codecCtx;
    AVCodec *codec;
    SwrContext *swrCtx;
    AVPacket *packet;
    AVFrame *frame;

    FILE *pcmFile;

    int frameCount = 0;

    int
    decodeInternal(AVFormatContext *fmtCtx, AVCodecContext *codecCtx, AVPacket *pkt, AVFrame *frame,
                   FILE *outfile,
                   uint8_t *outBuffer, int outChannel);

public:

    const AVSampleFormat TARGET_PCM_SAMPLE_FMT = AV_SAMPLE_FMT_S16;
    const int TARGET_PCM_SAMPLE_RATE = 44100;
    const int TARGET_PCM_CHANNEL_LAYOUT = AV_CH_LAYOUT_STEREO;

    void init(const char *audioPath, const char *pcmPath);

    int decode();

    void destroy();
};


#endif //ANDROID_AUDIOKITS_AUDIODECODER_H
