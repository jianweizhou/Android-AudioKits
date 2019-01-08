package com.jack.audiokits.library;

public class AudioUtils {

    static {
        System.loadLibrary("audiokits");
    }

    public static native String getCodecInfo();

    public static native int detachAudio(String videoPath, String audioPath);

    public static native int decodeAudioToPCM(String audioPath, String pcm);

    public static native int encodePCMToMp3(String pcmPath, String mp3Path);
}
