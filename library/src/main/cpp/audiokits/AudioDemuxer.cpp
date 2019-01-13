//
// Created by jack.zhou on 2019/1/9.
//

#define LOG_TAG "AudioDemuxer"

#include <common/CommonTools.h>
#include <common/FFMpegUtils.h>
#include "AudioDemuxer.h"

extern "C" {
#include <thirdparty/ffmpeg/include/libavcodec/avcodec.h>
#include <thirdparty/ffmpeg/include/libavutil/mathematics.h>
}

int AudioDemuxer::init(const char *videoPath, const char *audioPath) {
    av_register_all();
    avcodec_register_all();
    this->videoPath = videoPath;
    this->audioPath = audioPath;
    return 0;
}

int AudioDemuxer::demux() {

    /* open input file, and allocate format context */
    if (avformat_open_input(&fmtCtx, videoPath, NULL, NULL) < 0) {
        LOGE("Could not open source file %s", videoPath);
        return -1;
    }

    /* retrieve stream information */
    if (avformat_find_stream_info(fmtCtx, NULL) < 0) {
        LOGE("Could not find stream information");
        return -1;
    }

    /* locate audio stream*/
    if (FFMpegUtils::findStreamByType(&audioStreamIndex, &audioStream, fmtCtx, AVMEDIA_TYPE_AUDIO) <
        0) {
        LOGE("Could not find stream AVMEDIA_TYPE_AUDIO failed!");
        return -1;
    }

    /* alloc output context*/
    if (avformat_alloc_output_context2(&outFmtCtx, NULL, NULL, audioPath) < 0) {
        LOGE("Could not allocate out_fmt_context for %s", videoPath);
        return -1;
    }
    outFmt = outFmtCtx->oformat;
    outStream = avformat_new_stream(outFmtCtx, audioStream->codec->codec);

    //copy codec context
    if (avcodec_copy_context(outStream->codec, audioStream->codec) < 0) {
        LOGE("Could not copy codec context to output");
        return -1;
    }
    outStream->codec->codec_tag = 0;
    if (outFmt->flags & AVFMT_GLOBALHEADER) {
        outStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    /* open out file*/
    if (!(outFmt->flags & AVFMT_NOFILE)) {
        if (avio_open(&outFmtCtx->pb, audioPath, AVIO_FLAG_WRITE) < 0) {
            LOGE("Couldn't open output file '%s'", audioPath);
            return -1;
        }
    }

    /* packet for holding encoded output */
    packet = av_packet_alloc();
    if (!packet) {
        LOGE("av_packet_alloc failed!");
        return -1;
    }

    if (avformat_write_header(outFmtCtx, NULL) < 0) {
        LOGE("Error occurred when opening video output file");
        return -1;
    }

    int curPts = 0;

    while (true) {

        //read packet(raw data
        if (av_read_frame(fmtCtx, packet) < 0) {
            LOGD("Error occurred when read frame, maybe is end of file.");
            break;
        }

        //未知原因存在空packet，导致44帧时写入出错，先暂时跳过
        if (*(packet->data) == '\0') {
            continue;
        }

        // 转换PTS/DTS
        packet->pts = av_rescale_q_rnd(packet->pts, audioStream->time_base, outStream->time_base,
                                       (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet->dts = av_rescale_q_rnd(packet->dts, audioStream->time_base, outStream->time_base,
                                       (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
        packet->duration = av_rescale_q(packet->duration, audioStream->time_base,
                                        outStream->time_base);
        packet->pos = -1;
        packet->stream_index = 0;

        //write packet to file
        if (av_write_frame(outFmtCtx, packet) < 0) {
            LOGE("Error write %d packet", audioFrameCount);
            break;
        }

        av_packet_unref(packet);
        audioFrameCount++;
    }

    av_write_frame(outFmtCtx, NULL);
    av_write_trailer(outFmtCtx);
    resetPointer();
    return 0;
}

void AudioDemuxer::destroy() {
    resetPointer();
}

void AudioDemuxer::resetPointer() {
    audioFrameCount = 0;
    if (!packet) {
        av_packet_free(&packet);
        packet = NULL;
    }
    if (!fmtCtx) {
        avformat_close_input(&fmtCtx);
        avformat_free_context(fmtCtx);
        fmtCtx = NULL;
    }
    if (outFmtCtx && !(outFmt->flags & AVFMT_NOFILE)) {
        avio_close(outFmtCtx->pb);
    }
    if (!outFmtCtx) {
        if (outFmt && outFmt->flags & AVFMT_NOFILE) {
            avio_close(outFmtCtx->pb);
        }
        avformat_free_context(outFmtCtx);
        outFmtCtx = NULL;
    }
}

