package com.tlinux.mp3playffmpegandaudiotrack;

/**
 * Created by tlinux on 18-8-12.
 */

public class Mp3Decoder {


    public native static void classInit();

    private long nativePtr;

    public Mp3Decoder(String path) {
        nativePtr = nativeInit(path);
    }

    private native long nativeInit(String path);
    private native void nativeRelease();
    private native void nativeStart();
    private native int nativeRead(byte buffer[], int offset,int length);

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        nativeRelease();
    }

    public void start() {
        nativeStart();
    }

    public int read(byte buffer[], int offset,int length) {
        return nativeRead(buffer,offset,length);
    }
}

