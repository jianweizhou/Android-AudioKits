//
// Created by jack.zhou on 2019/1/10.
//

#define LOG_TAG "FFMpegUtils"

#include "FFMpegUtils.h"
#include "CommonTools.h"

extern "C" {
#include <thirdparty/ffmpeg/include/libavformat/avformat.h>
}

int FFMpegUtils::findStreamByType(int *stream_idx, AVStream **stream, AVFormatContext *fmt_ctx,
                                  enum AVMediaType type) {
    int ret, stream_index;
    AVStream *st;
    ret = av_find_best_stream(fmt_ctx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        LOGE("Could not find type:%d stream.", type);
        return ret;
    } else {
        stream_index = ret;
        st = fmt_ctx->streams[stream_index];

        *stream_idx = stream_index;
        *stream = st;
        return 0;
    }
}
