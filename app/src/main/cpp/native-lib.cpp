#include <jni.h>
#include <string>
#include "Mp3Decoder.h"

jfieldID FIELD_ID_MP3_DECODER_NATIVEPTR;


JNIEXPORT Mp3Decoder* JNICALL getNativeMp3Decoder(JNIEnv *env, jobject instance){
    long ptr = env->GetLongField(instance,FIELD_ID_MP3_DECODER_NATIVEPTR);
    return reinterpret_cast<Mp3Decoder*>(ptr);
}


JNIEXPORT jlong JNICALL
Java_com_tlinux_mp3playffmpegandaudiotrack_Mp3Decoder_nativeInit(JNIEnv *env, jobject instance,
                                                                 jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    Mp3Decoder *mp3Decoder = new Mp3Decoder(path);
    if (mp3Decoder->checkFileOk()) {
        env->ReleaseStringUTFChars(path_, path);
       //抛出io异常
//        jniThrowIOException(env, errno);  // Will throw on return
        return NULL;
    }
    env->ReleaseStringUTFChars(path_, path);
    return reinterpret_cast<long>(mp3Decoder);
}

JNIEXPORT void JNICALL
Java_com_tlinux_mp3playffmpegandaudiotrack_Mp3Decoder_nativeRelease(JNIEnv *env, jobject instance) {

}

JNIEXPORT void JNICALL
Java_com_tlinux_mp3playffmpegandaudiotrack_Mp3Decoder_nativeStart(JNIEnv *env, jobject instance) {
    Mp3Decoder *mp3Decoder = getNativeMp3Decoder(env,instance);
    mp3Decoder->start();
}

JNIEXPORT jint JNICALL
Java_com_tlinux_mp3playffmpegandaudiotrack_Mp3Decoder_nativeRead(JNIEnv *env, jobject instance,
                                                                 jbyteArray buffer_, jint offset,
                                                                 jint length) {
    jbyte *buffer = env->GetByteArrayElements(buffer_, NULL);

    // TODO

    env->ReleaseByteArrayElements(buffer_, buffer, 0);
}

JNIEXPORT void JNICALL
Java_com_tlinux_mp3playffmpegandaudiotrack_Mp3Decoder_classInit(JNIEnv *env, jclass type) {
    FIELD_ID_MP3_DECODER_NATIVEPTR = env->GetFieldID(type,"nativePtr","L");
}



extern "C"
JNIEXPORT jstring JNICALL
Java_com_tlinux_mp3playffmpegandaudiotrack_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
