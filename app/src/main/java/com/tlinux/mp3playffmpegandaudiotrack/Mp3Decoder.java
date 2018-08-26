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
        nativePtr = nativeInit(path);
    }

    private native long nativeInit(String path);
    private native void nativeRelease();
    private native void nativeStart(boolean useCallback);
    private native int nativeRead(byte buffer[], int offset,int length);
    private native boolean nativePrepare();
    private native AudioInfo ngetAudioInfo();


    public AudioInfo getAudioInfo() {
        return ngetAudioInfo();
    }

    public boolean prepare() {
        return nativePrepare();
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeRelease();
    }

    public void start() {
        new Thread(){
            public void run(){
                nativeStart(true);
            }
        }.start();
    }

    public int read(byte buffer[], int offset,int length) {
        return nativeRead(buffer,offset,length);
    }

    //TODO 这个buffer会不会引起内存泄漏　会不会提前被回收　？？　猜测不会
    public void onData(byte[] buffer,int size,int count){
        Log.e("AAAA","ahhhahha");
        if (callback!=null) {
            callback.onData(buffer,size,count);
        }
    }

    private OnDataCallback callback;
    public void setOnDataCallback(OnDataCallback onDataCallback){
        callback = onDataCallback;
    }
    interface OnDataCallback{
        void onData(byte[] buffer,int size , int count);
    }
}

