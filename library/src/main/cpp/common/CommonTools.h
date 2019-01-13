//
// Created by jack.zhou on 2019/1/5.
//

#ifndef AUDIOKIT_COMMONTOOLS_H
#define AUDIOKIT_COMMONTOOLS_H

#include <android/log.h>
#include <time.h>

#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static inline long long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif //AUDIOKIT_COMMONTOOLS_H
