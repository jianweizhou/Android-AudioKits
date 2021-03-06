/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_jack_audiokits_library_AudioUtils */

#ifndef _Included_com_jack_audiokits_library_AudioUtils
#define _Included_com_jack_audiokits_library_AudioUtils
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jack_audiokits_library_AudioUtils
 * Method:    getCodecInfo
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_jack_audiokits_library_AudioUtils_getCodecInfo
  (JNIEnv *, jclass);

/*
 * Class:     com_jack_audiokits_library_AudioUtils
 * Method:    detachAudio
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_detachAudio
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_jack_audiokits_library_AudioUtils
 * Method:    decodeAudioToPCM
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_decodeAudioToPCM
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     com_jack_audiokits_library_AudioUtils
 * Method:    encodePCMToMp3
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_jack_audiokits_library_AudioUtils_encodePCMToMp3
  (JNIEnv *, jclass, jstring, jstring);

#ifdef __cplusplus
}
#endif
#endif
