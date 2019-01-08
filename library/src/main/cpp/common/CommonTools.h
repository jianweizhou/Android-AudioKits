//
// Created by jack.zhou on 2019/1/5.
//

#ifndef AUDIOKIT_COMMONTOOLS_H
#define AUDIOKIT_COMMONTOOLS_H

#include <android/log.h>

#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //AUDIOKIT_COMMONTOOLS_H
