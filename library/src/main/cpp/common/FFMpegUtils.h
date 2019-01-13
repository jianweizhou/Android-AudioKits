//
// Created by jack.zhou on 2019/1/10.
//

#ifndef ANDROID_AUDIOKITS_FFMPEGUTILS_H
#define ANDROID_AUDIOKITS_FFMPEGUTILS_H

extern "C" {
#include <thirdparty/ffmpeg/include/libavformat/avformat.h>
#include <thirdparty/ffmpeg/include/libavutil/avutil.h>
}

class FFMpegUtils {
public:
    static int findStreamByType(int *stream_idx, AVStream **stream, AVFormatContext *fmt_ctx,
                                enum AVMediaType type);
};


#endif //ANDROID_AUDIOKITS_FFMPEGUTILS_H
