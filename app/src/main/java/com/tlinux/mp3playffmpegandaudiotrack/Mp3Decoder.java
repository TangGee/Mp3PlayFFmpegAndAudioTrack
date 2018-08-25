package com.tlinux.mp3playffmpegandaudiotrack;

import android.util.Log;

/**
 * Created by tlinux on 18-8-12.
 */

public class Mp3Decoder {

    static {
        System.loadLibrary("mp3decoder");
        classInit();
    }

    public native static void classInit();

    private long nativePtr;

    public Mp3Decoder(String path) {
        nativePtr = nativeInit(path, true);
    }

    private native long nativeInit(String path, boolean useCallBack);
    private native void nativeRelease();
    private native void nativeStart();
    private native int nativeRead(byte buffer[], int offset,int length);

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
    }

    public void start() {
        nativeStart();
    }

    public int read(byte buffer[], int offset,int length) {
        return nativeRead(buffer,offset,length);
    }

    public void onData(byte[] buffer,int size,int count){
        Log.e("AAAA","ahhhahha");
    }
}

