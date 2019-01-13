//
// Created by jack.zhou on 2019/1/9.
//

#ifndef ANDROID_AUDIOKITS_AUDIODEMUXER_H
#define ANDROID_AUDIOKITS_AUDIODEMUXER_H

#include <string>

extern "C" {
#include <thirdparty/ffmpeg/include/libavcodec/avcodec.h>
#include <thirdparty/ffmpeg/include/libavutil/imgutils.h>
#include <thirdparty/ffmpeg/include/libavutil/samplefmt.h>
#include <thirdparty/ffmpeg/include/libavutil/timestamp.h>
#include <thirdparty/ffmpeg/include/libavformat/avformat.h>
}
using namespace std;

class AudioDemuxer {

private:
    const char *videoPath;
    const char *audioPath;

    AVFormatContext *fmtCtx = NULL;
    AVStream *audioStream = NULL;

    AVFormatContext *outFmtCtx = NULL;
    AVOutputFormat *outFmt = NULL;
    AVStream *outStream = NULL;
    AVPacket *packet;

    int audioStreamIndex = 0;
    int audioFrameCount = 0;

//    AudioDemuxer(const AudioDemuxer &audioDemuxer);

    void resetPointer();

public:
    AudioDemuxer() {}

    ~AudioDemuxer() {}

    int init(const char *videoPath, const char *audioPath);

    int demux();

    void destroy();
};


#endif //ANDROID_AUDIOKITS_AUDIODEMUXER_H
