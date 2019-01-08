#include <jni.h>
#include "com_jack_audiokits_library_AudioUtils.h"
#include "../common/CommonTools.h"

extern "C" {
#include "../thirdparty/ffmpeg/include/libavcodec/avcodec.h"
}

#define LOG_TAG "AudioUtils"

JNIEXPORT jstring JNICALL Java_com_jack_audiokits_library_AudioUtils_getCodecInfo
        (JNIEnv *env, jclass c) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_getCodecInfo->");
    return env->NewStringUTF(avcodec_configuration());
}

JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_detachAudio
        (JNIEnv *env, jclass c, jstring videoPath, jstring audioPath) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_detachAudio->");
    return 0;
}

JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_decodeAudioToPCM
        (JNIEnv *env, jclass c, jstring audioPath, jstring pcmPath) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_decodeAudioToPCM->");
    return 0;
}

JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_encodePCMToMp3
        (JNIEnv *env, jclass c, jstring pcmPath, jstring mp3Path) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_encodePCMToMp3->");
    return 0;
}




