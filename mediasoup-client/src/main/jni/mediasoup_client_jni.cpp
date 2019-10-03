#include <jni.h>
#include "mediasoupclient.hpp"

namespace mediasoupclient {
    extern "C"
    JNIEXPORT void JNICALL
    Java_org_mediasoup_droid_MediasoupClient_nativeInitialize(
            JNIEnv *env,
            jclass /* type */) {
        mediasoupclient::Initialize();
    }

    extern "C" JNIEXPORT void JNICALL
    Java_org_mediasoup_droid_MediasoupClient_nativeCleanup(
            JNIEnv *env,
            jclass /* type */) {
        mediasoupclient::Cleanup();
    }

    extern "C" JNIEXPORT jstring JNICALL
    Java_org_mediasoup_droid_MediasoupClient_nativeVersion(
            JNIEnv *env,
            jclass /* type */) {
        std::string version = mediasoupclient::Version();
        return env->NewStringUTF(version.c_str());
    }
}
