#define MSC_CLASS "peerConnection_jni"

#include <jni.h>
#include "Logger.hpp"
#include "PeerConnection.hpp"
#include "sdk/android/src/jni/jni_helpers.h"
#include "sdk/android/src/jni/pc/peer_connection.h"
#include "sdk/android/src/jni/pc/peer_connection_factory.h"
#include "sdk/android/native_api/jni/scoped_java_ref.h"

namespace mediasoupclient {

class PrivateListenerProxy : public PeerConnection::PrivateListener {

private:
    webrtc::jni::PeerConnectionObserverJni observerJni;

public:
    PrivateListenerProxy(JNIEnv *jni, const webrtc::JavaRef<jobject> &j_observer)
            : observerJni(jni, j_observer) {
    }

    void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState newState) override {
        PrivateListener::OnSignalingChange(newState);
        observerJni.OnSignalingChange(newState);
    }

    void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
        PrivateListener::OnAddStream(stream);
        observerJni.OnAddStream(stream);
    }

    void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override {
        PrivateListener::OnRemoveStream(stream);
        observerJni.OnRemoveStream(stream);
    }

    void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannel) override {
        PrivateListener::OnDataChannel(dataChannel);
        observerJni.OnDataChannel(dataChannel);
    }

    void OnRenegotiationNeeded() override {
        PrivateListener::OnRenegotiationNeeded();
        observerJni.OnRenegotiationNeeded();
    }

    void
    OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState newState) override {
        PrivateListener::OnIceConnectionChange(newState);
        observerJni.OnIceConnectionChange(newState);
    }

    void
    OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState newState) override {
        PrivateListener::OnIceGatheringChange(newState);
        observerJni.OnIceGatheringChange(newState);
    }

    void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override {
        PrivateListener::OnIceCandidate(candidate);
        observerJni.OnIceCandidate(candidate);
    }

    void OnIceCandidatesRemoved(const std::vector<cricket::Candidate> &candidates) override {
        PrivateListener::OnIceCandidatesRemoved(candidates);
        observerJni.OnIceCandidatesRemoved(candidates);
    }

    void OnIceConnectionReceivingChange(bool receiving) override {
        PrivateListener::OnIceConnectionReceivingChange(receiving);
        observerJni.OnIceConnectionReceivingChange(receiving);
    }

    void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
                    const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>> &streams) override {
        PrivateListener::OnAddTrack(receiver, streams);
        observerJni.OnAddTrack(receiver, streams);
    }

    void OnTrack(rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver) override {
        PrivateListener::OnTrack(transceiver);
        observerJni.OnTrack(transceiver);
    }

    void OnRemoveTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver) override {
        PrivateListener::OnRemoveTrack(receiver);
        observerJni.OnRemoveTrack(receiver);
    }

    void OnInterestingUsage(int usagePattern) override {
        PrivateListener::OnInterestingUsage(usagePattern);
        observerJni.OnInterestingUsage(usagePattern);
    }
};

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeNewListener(
        JNIEnv *env,
        jclass /* j_type */,
        jobject j_listener) {
    MSC_TRACE();

    auto listener = new PrivateListenerProxy(env, webrtc::JavaParamRef<jobject>(j_listener));
    return webrtc::jni::jlongFromPointer(listener);
}

extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeFreeListener(
        JNIEnv */* env */,
        jclass /* j_type */,
        jlong j_native_listener) {
    MSC_TRACE();

    auto *listener = reinterpret_cast<PrivateListenerProxy *>(j_native_listener);
    MSC_ASSERT(listener != nullptr, "native peerConnection::PrivateListener pointer null");
    delete listener;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeNewPeerConnection(
        JNIEnv *env,
        jclass /* j_type */,
        jlong j_native_Listener,
        jobject j_rtc_config,
        jlong j_native_peerConnection_factory) {
    MSC_TRACE();

    auto privateListener = reinterpret_cast<PeerConnection::PrivateListener *>(j_native_Listener);
    webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
            webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
    webrtc::jni::JavaToNativeRTCConfiguration(env, webrtc::JavaParamRef<jobject>(j_rtc_config),
                                              &rtc_config);
    PeerConnection::Options options;
    options.config = rtc_config;
    options.factory = reinterpret_cast<webrtc::PeerConnectionFactoryInterface *>(j_native_peerConnection_factory);

    auto *pc = new PeerConnection(privateListener, &options);
    return webrtc::jni::jlongFromPointer(pc);
}


extern "C"
JNIEXPORT void JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeFreePeerConnection(
        JNIEnv */* env */,
        jclass /* j_type */,
        jlong j_peerConnection) {
    MSC_TRACE();

    auto *pc = reinterpret_cast<PeerConnection *>(j_peerConnection);
    MSC_ASSERT(pc != nullptr, "native peerConnection pointer null");
    delete pc;
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_org_mediasoup_droid_PeerConnection_nativeSetConfiguration(
        JNIEnv *env,
        jobject /* j_object */,
        jlong j_peerConnection,
        jobject j_rtc_config) {
    MSC_TRACE();

    auto *pc = reinterpret_cast<PeerConnection *>(j_peerConnection);
    MSC_ASSERT(pc != nullptr, "native peerConnection pointer null");

    webrtc::PeerConnectionInterface::RTCConfiguration rtc_config(
            webrtc::PeerConnectionInterface::RTCConfigurationType::kAggressive);
    webrtc::jni::JavaToNativeRTCConfiguration(env, webrtc::JavaParamRef<jobject>(j_rtc_config),
                                              &rtc_config);
    bool result = pc->SetConfiguration(rtc_config);
    return static_cast<jboolean>(result);
}

}

