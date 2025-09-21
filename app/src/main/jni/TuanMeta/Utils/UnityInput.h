#pragma once
#include <android/input.h>
#include "imgui.h"
#include "imgui_impl_android.h"
#include "DebugLog.h"

namespace TouchInput {

    static void* origConsume = nullptr;
    static void* origInitializeMotion = nullptr;

    // Hook cho Android 14+ (mới)
    static int32_t HookConsume(void* consumer, void* factory, bool isRaw,
                               long sequenceId, uint32_t* outPolicyFlags, void** outEventPtr) {
        int32_t result = ((int32_t(*)(void*, void*, bool, long, uint32_t*, void**))origConsume)(
                consumer, factory, isRaw, sequenceId, outPolicyFlags, outEventPtr);

        if (outEventPtr && *outEventPtr) {
            AInputEvent* event = reinterpret_cast<AInputEvent*>(*outEventPtr);
            ImGui_ImplAndroid_HandleInputEvent(event);
        }
        return result;
    }

    // Hook cho Android cũ (trước Android 14)
    static void HookOldInitializeMotion(void* thiz, void* motionEvent, void* inputMessage) {
        ((void(*)(void*, void*, void*))origInitializeMotion)(thiz, motionEvent, inputMessage);

        if (motionEvent) {
            AInputEvent* event = reinterpret_cast<AInputEvent*>(motionEvent);
            ImGui_ImplAndroid_HandleInputEvent(event);
        }
    }

    // Hàm khởi tạo hook
    static void Init() {
        AddDebugLog("[TouchInput] Initializing hook...");

        void* handle = xdl_open("libinput.so", RTLD_NOW);
        if (!handle) {
            AddDebugLog("[TouchInput] Failed to open libinput.so");
            return;
        }

        // 1. Thử hook Android cũ (initializeMotionEvent)
        void* oldAddr = xdl_sym(handle,
                                "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE",
                                nullptr);
        if (oldAddr) {
            if (DobbyHook(oldAddr, (void*)HookOldInitializeMotion, &origInitializeMotion) == 0) {
                AddDebugLog("[TouchInput] HookOldInitializeMotion installed successfully!");
                return; // Ưu tiên hook cũ nếu có
            } else {
                AddDebugLog("[TouchInput] HookOldInitializeMotion failed!");
            }
        }

        // 2. Nếu không có hàm cũ, hook Android 14/15
#ifdef __aarch64__
        const char* consume_symbol =
                "_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEblPjPPNS_10InputEventE";
#else
        const char* consume_symbol =
            "_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEbxPjPPNS_10InputEventE";
#endif

        void* addr = xdl_sym(handle, consume_symbol, nullptr);
        if (addr) {
            if (DobbyHook(addr, (void*)HookConsume, &origConsume) == 0) {
                AddDebugLog("[TouchInput] HookConsume installed successfully!");
            } else {
                AddDebugLog("[TouchInput] HookConsume failed!");
            }
        } else {
            AddDebugLog("[TouchInput] Symbol not found for new Android version!");
        }
    }

} // namespace TouchInput
