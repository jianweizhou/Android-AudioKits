#include <jni.h>
#include "com_jack_audiokits_library_AudioUtils.h"
#include "../common/CommonTools.h"
#include "../audiokits/AudioDemuxer.h"
#include "../audiokits/AudioDecoder.h"
#include "AudioEncoder.h"

extern "C" {
#include "../thirdparty/ffmpeg/include/libavcodec/avcodec.h"
}

#define LOG_TAG "AudioUtils"

void encodeAACUsingFFmpeg(const char *pcmPath, const char *aacPath, int channels, int bitRate,
                          int sampleRate);

JNIEXPORT jstring JNICALL Java_com_jack_audiokits_library_AudioUtils_getCodecInfo
        (JNIEnv *env, jclass c) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_getCodecInfo->");
    return env->NewStringUTF(avcodec_configuration());
}

JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_detachAudio
        (JNIEnv *env, jclass c, jstring videoPath, jstring audioPath) {
    const char *video = env->GetStringUTFChars(videoPath, NULL);
    const char *audio = env->GetStringUTFChars(audioPath, NULL);
    LOGD(LOG_TAG, "detachAudio->videoPath:%s \n audioPath:%s", *video, *audio);
    AudioDemuxer demuxer = AudioDemuxer();
    demuxer.init(video, audio);
    int ret = demuxer.demux();
    if (ret < 0) {
        LOGE("detachAudio failed!");
    }
    demuxer.destroy();
    env->ReleaseStringUTFChars(videoPath, video);
    env->ReleaseStringUTFChars(audioPath, audio);
    return ret;
}

JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_decodeAudioToPCM
        (JNIEnv *env, jclass c, jstring audioPath, jstring pcmPath) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_decodeAudioToPCM->");
    const char *audio = env->GetStringUTFChars(audioPath, NULL);
    const char *pcm = env->GetStringUTFChars(pcmPath, NULL);
    LOGD(LOG_TAG, "decodeAudioToPCM->audio:%s \n pcm:%s", *audio, *pcm);
    AudioDecoder *decoder = new AudioDecoder();
    decoder->init(audio, pcm);
    int ret = decoder->decode();
    if (ret < 0) {
        LOGE("decodeAudio failed!");
    }
    decoder->destroy();
    env->ReleaseStringUTFChars(audioPath, audio);
    env->ReleaseStringUTFChars(pcmPath, pcm);
    delete decoder;
    return 0;
}

JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_encodePCMToMp3
        (JNIEnv *env, jclass c, jstring pcmPath, jstring mp3Path) {
    LOGD(LOG_TAG, "Java_com_jack_audiokits_library_AudioUtils_encodePCMToMp3->");
    const char *pcm = env->GetStringUTFChars(pcmPath, NULL);
    const char *mp3 = env->GetStringUTFChars(mp3Path, NULL);
    encodeAACUsingFFmpeg(pcm, mp3, 2, PUBLISH_BITE_RATE, 44100);
    env->ReleaseStringUTFChars(pcmPath, pcm);
    env->ReleaseStringUTFChars(mp3Path, mp3);
    return 0;
}

void encodeAACUsingFFmpeg(const char *pcmPath, const char *aacPath, int channels, int bitRate,
                          int sampleRate) {
    AudioEncoder *audioEncoder = new AudioEncoder();
    int bitsPerSample = 16;
    char *codec_name = "libmp3lame";//"libmp3lame libfdk_aac";
    LOGD("before audioEncoder init...");
    audioEncoder->init(bitRate, channels, sampleRate, bitsPerSample, aacPath, codec_name);
    int bufferSize = 1024 * 256;
    byte *buffer = new byte[bufferSize];
    FILE *pcmFileHandle = fopen(pcmPath, "rb+");
    int readBufferSize = 0;
    while ((readBufferSize = fread(buffer, 1, bufferSize, pcmFileHandle)) > 0) {
        audioEncoder->encode(buffer, readBufferSize);
    }
    LOGD("After Encode");
    delete[] buffer;
    fclose(pcmFileHandle);
    audioEncoder->destroy();
    delete audioEncoder;
}







