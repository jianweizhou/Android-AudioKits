//
// Created by jack.zhou on 2019/1/9.
//

#ifndef ANDROID_AUDIOKITS_AUDIOENCODER_H
#define ANDROID_AUDIOKITS_AUDIOENCODER_H

extern "C" {
#include <thirdparty/ffmpeg/include/libavcodec/avcodec.h>
#include <thirdparty/ffmpeg/include/libavformat/avformat.h>
#include <thirdparty/ffmpeg/include/libswresample/swresample.h>
}

#define PUBLISH_BITE_RATE 128000

typedef unsigned char byte;

class AudioEncoder {
private:
    AVFormatContext *avFormatContext;
    AVCodecContext *avCodecContext;
    AVStream *audioStream;

    bool isWriteHeaderSuccess;
    double duration;

//    FILE *testFile;

    AVFrame *input_frame;
    int buffer_size;
    uint8_t *samples;
    int samplesCursor;
    SwrContext *swrContext;
    uint8_t **convert_data;
    AVFrame *swrFrame;
    uint8_t *swrBuffer;
    int swrBufferSize;

    int publishBitRate;
    int audioChannels;
    int audioSampleRate;

    int totalSWRTimeMills;
    int totalEncodeTimeMills;

    //初始化的时候，要进行的工作
    int alloc_avframe();

    int alloc_audio_stream(const char *codec_name);

    //当够了一个frame之后就要编码一个packet
    void encodePacket();

    void addADTStoPacket(uint8_t *packet, int packetLen);

    void writeAACPakcetToFile(uint8_t *data, int datalen);

public:
    AudioEncoder();

    virtual ~AudioEncoder();

    int init(int bitRate, int channels, int sampleRate, int bitsPerSample, const char *aacFilePath,
             const char *codec_name);

    int init(int bitRate, int channels, int bitsPerSample, const char *aacFilePath,
             const char *codec_name);

    void encode(byte *buffer, int size);

    void destroy();
};


#endif //ANDROID_AUDIOKITS_AUDIOENCODER_H
