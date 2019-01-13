//
// Created by jack.zhou on 2019/1/9.

#define LOG_TAG "AudioDecoder"

#include <common/CommonTools.h>
#include <common/FFMpegUtils.h>
#include "AudioDecoder.h"

extern "C" {
#include <thirdparty/ffmpeg/include/libavformat/avformat.h>
}

void AudioDecoder::init(const char *audioPath, const char *pcmPath) {
    this->audioPath = audioPath;
    this->pcmPath = pcmPath;
    av_register_all();
    avcodec_register_all();
}

int AudioDecoder::decode() {

    fmtCtx = avformat_alloc_context();

    if (avformat_open_input(&fmtCtx, audioPath, NULL, NULL) != 0) {
        LOGE("decode->avformat_open_input failed!");
        return -1;
    }

    if (avformat_find_stream_info(fmtCtx, NULL) < 0) {
        LOGD("decode->avformat_find_stream_info failed!");
        return -1;
    }

    int audioStreamIndex = 0;
    AVStream *stream;
    if (FFMpegUtils::findStreamByType(&audioStreamIndex, &stream, fmtCtx, AVMEDIA_TYPE_AUDIO) < 0) {
        LOGE("can not find audio stream!");
        return -1;
    }

    codecCtx = stream->codec;
    codec = avcodec_find_decoder(codecCtx->codec_id);
    if (!codec) {
        LOGE("can not find decoder!");
        return -1;
    }

    if (avcodec_open2(codecCtx, codec, NULL) < 0) {
        LOGE("can not open decoder!");
        return -1;
    }

    //音频重采样，16bit 44100 双声道
    //通常播放器的音频渲染器，都有固定的采样率、声道、bit位宽设置，不同的音频都需要重采样保持一致
    enum AVSampleFormat inSampleFmt = codecCtx->sample_fmt;
    int inSampleRate = codecCtx->sample_rate;
    uint64_t inSampleChannelLayout = codecCtx->channel_layout;
    if (inSampleFmt != TARGET_PCM_SAMPLE_FMT || inSampleRate != TARGET_PCM_SAMPLE_RATE ||
        inSampleChannelLayout != TARGET_PCM_CHANNEL_LAYOUT) {
        swrCtx = swr_alloc();
        swr_alloc_set_opts(swrCtx, AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,
                           inSampleChannelLayout, inSampleFmt, inSampleRate, 0, NULL);
        swr_init(swrCtx);
    }

    /* alloc data container*/
    packet = av_packet_alloc();
    frame = av_frame_alloc();
    pcmFile = fopen(pcmPath, "wb");

    //buffer
    int outChannel = av_get_channel_layout_nb_channels(TARGET_PCM_CHANNEL_LAYOUT);
    int sampleSize = av_get_bytes_per_sample(TARGET_PCM_SAMPLE_FMT);
    uint8_t *outBuffer = (uint8_t *) av_malloc(sampleSize * outChannel * 44100);

    //关于处理进度计算：1.frameCount？ 2.已处理的packet的duration占总的duration?
    while (true) {
        if (decodeInternal(fmtCtx, codecCtx, packet, frame, pcmFile, outBuffer, outChannel) < 0) {
            LOGE("decodeInternal error!");
            break;
        }
    }

    fflush(pcmFile);
    return 0;
}

int AudioDecoder::decodeInternal(AVFormatContext *fmtCtx, AVCodecContext *codecCtx, AVPacket *pkt,
                                 AVFrame *frame,
                                 FILE *outfile, uint8_t *outBuffer, int outChannel) {
    int isGotFrame = 0;
    if (av_read_frame(fmtCtx, packet)) {
        LOGD("decodeInternal->reach end of failed!");
        return -1;
    }

    int len = avcodec_decode_audio4(codecCtx, frame, &isGotFrame, packet);
    if (len < 0) {
        LOGD("decodeInternal->avcodec_decode_audio4reach failed!");
        return -1;
    }
    if (isGotFrame) {
        if (swrCtx) {
            int swr_ret = swr_convert(swrCtx, &outBuffer, outChannel * frame->nb_samples,
                                      (const uint8_t **) frame->data, frame->nb_samples);
            if (swr_ret <= 0) {
                LOGE("decodeInternal->srswr_convert error!");
                return -1;
            }
            int out_buffer_size = av_samples_get_buffer_size(NULL, outChannel,
                                                             frame->nb_samples,
                                                             AV_SAMPLE_FMT_S16, 1);
            fwrite(outBuffer, 1, out_buffer_size, outfile);
        } else {
            int data_size = av_get_bytes_per_sample(codecCtx->sample_fmt);
            for (int sampleIndex = 0; sampleIndex < frame->nb_samples; sampleIndex++)
                for (int channelIndex = 0; channelIndex < codecCtx->channels; channelIndex++)
                    fwrite(frame->data[channelIndex] + data_size * sampleIndex, 1, data_size,
                           outfile);
        }
    }

    LOGD("decodeInternal loop, frameCount:%d", frameCount);
    frameCount++;
    return 0;
}

void AudioDecoder::destroy() {
    if (packet) {
        av_packet_unref(packet);
        packet = NULL;
    }
    if (pcmFile) {
        fclose(pcmFile);
        pcmFile = NULL;
    }
    if (fmtCtx) {
        avformat_close_input(&fmtCtx);
        avformat_free_context(fmtCtx);
        fmtCtx = NULL;
    }
    if (codecCtx) {
        avcodec_close(codecCtx);
        codecCtx = NULL;
    }

    if (frame) {
        av_frame_free(&frame);
        frame = NULL;
    }
    if (swrCtx) {
        swr_free(&swrCtx);
        swrCtx = NULL;
    }
}
