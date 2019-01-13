package com.jack.audiokits;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.jack.audiokits.library.AudioUtils;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "MainActivity";

    private TextView tvInfo;

    private String sdcradPath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvInfo = findViewById(R.id.sample_text);

        sdcradPath = Environment.getExternalStorageDirectory().getAbsolutePath();
    }

    @Override
    public void onClick(View view) {
        int id = view.getId();
        switch (id) {
            case R.id.btn_codec_info: {
                tvInfo.setText(AudioUtils.getCodecInfo());
                break;
            }
            case R.id.btn_audio_demux: {
                final String videoPath = sdcradPath + "/1.mp4";
                final String audioPath = sdcradPath + "/audiokits_detached.m4a";
                new Thread() {
                    @Override
                    public void run() {
                        int ret = AudioUtils.detachAudio(videoPath, audioPath);
                        Log.d(TAG, "detachAudio->ret:" + ret);
                    }
                }.start();
                break;
            }
            case R.id.btn_decode_audio: {
                final String audioPath = sdcradPath + "/audiokits_detached.m4a";
                final String pcmPath = sdcradPath + "/audiokits_decode.pcm";
                new Thread() {
                    @Override
                    public void run() {
                        int ret = AudioUtils.decodeAudioToPCM(audioPath, pcmPath);
                        Log.d(TAG, "decodeAudioToPCM->ret:" + ret);
                    }
                }.start();
                break;
            }
            case R.id.btn_encode_audio: {
                final String pcmPath = sdcradPath + "/audiokits_decode.pcm";
                final String mp3Path = sdcradPath + "/audiokits_detached.mp3";
                new Thread() {
                    @Override
                    public void run() {
                        int ret = AudioUtils.encodePCMToMp3(pcmPath, mp3Path);
                        Log.d(TAG, "encodePCMToMp3->ret:" + ret);
                    }
                }.start();
                break;
            }
        }
    }
}
