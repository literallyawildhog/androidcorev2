#pragma once

#include <android/log.h>

#define TAG "androidcore"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))

#define CALLLOG( ) LOGD("Calling", __FILE__, __FUNCTION__)

namespace config {
	constexpr const char* lib = "libroblox.so"; // unused, but great to use on memory manipulation
	constexpr const char* name = "Androidcore";
	constexpr const char* filename = "Android Core";
	constexpr const char* version = "2.0.0";
	constexpr const char* fprint = "Androidcore-Fingerprint";
	constexpr const char* uid = "Androidcore-User-Identifier";
	constexpr const char* roblox = "2.641.737";
}