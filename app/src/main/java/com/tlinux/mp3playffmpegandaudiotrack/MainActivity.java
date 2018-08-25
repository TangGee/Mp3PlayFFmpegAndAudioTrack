package com.tlinux.mp3playffmpegandaudiotrack;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private Mp3Decoder mMp3decoder;
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
                mMp3decoder.start();
            }
        });
    }


}
