package com.tlinux.mp3playffmpegandaudiotrack;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements Mp3Decoder.OnDataCallback {

    private Mp3Decoder mMp3decoder;
    private AudioTrack audioTrack;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText("123");

        mMp3decoder = new Mp3Decoder("/sdcard/jiangzhende.mp3");
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                audioTrack.play();
                mMp3decoder.start();
            }
        });

        if(!mMp3decoder.prepare()){
            finish();
            return;
        }
        AudioInfo info = mMp3decoder.getAudioInfo();
        if (info== null) {
            finish();
            return;
        }

        int channelConfigure = AudioFormat.CHANNEL_OUT_MONO;
        if (info.channelCount == 1) {
            channelConfigure = AudioFormat.CHANNEL_OUT_MONO;
        }

        int format = AudioFormat.ENCODING_PCM_16BIT;
        if (TextUtils.equals("u8",info.encodformat)){
            format = AudioFormat.ENCODING_PCM_8BIT;
        } else if (TextUtils.equals("s16be",info.encodformat)|| TextUtils.equals("s16le",info.encodformat)){
            format = AudioFormat.ENCODING_PCM_16BIT;
        } else if (TextUtils.equals("s32be",info.encodformat)) {
//            format = AudioFormat.ENCODING_PCM_8BIT //TODO
        } else if (TextUtils.equals("f32be",info.encodformat)){
            format = AudioFormat.ENCODING_PCM_FLOAT;
        }  else if (TextUtils.equals("f64be",info.encodformat)) {
            //TODO
        }
        int minBUffer = AudioTrack.getMinBufferSize(info.sampleRate,channelConfigure,format);
        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC,info.sampleRate,channelConfigure,
                format,minBUffer,AudioTrack.MODE_STREAM);

        mMp3decoder.setOnDataCallback(this);
    }


    @Override
    public void onData(byte[] buffer, int size, int count) {
        if(!isDestroyed()){
            audioTrack.write(buffer,0,size*count);
        }
    }
}
