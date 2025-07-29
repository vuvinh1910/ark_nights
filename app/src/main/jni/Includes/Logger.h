#pragma once

#include <android/log.h>

enum LogType {
	debug = 3,
	error = 6,
	info = 4,
	warn = 5
};

#define TAG oxorany("TuanMeta")

#define LOGD(...) ((void)__android_log_print(debug, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(error, TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(info, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(warn, TAG, __VA_ARGS__))