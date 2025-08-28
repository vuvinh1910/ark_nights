#pragma once
#include <android/input.h>
#include "imgui.h"
#include "imgui_impl_android.h"
#include "DebugLog.h"

namespace TouchInput {

    static void* origConsume = nullptr;

// Hook libinput consume để đưa sự kiện vào ImGui
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

// Gọi trong Init_thread() để hook
    static void Init() {
        AddDebugLog("[TouchInput] Initializing hook...");

        void* handle = xdl_open("libinput.so", RTLD_NOW);
        if (!handle) {
            AddDebugLog("[TouchInput] Failed to open libinput.so");
            return;
        }

        const char* symbol =
                "_ZN7android13InputConsumer7consumeEPNS_26InputEventFactoryInterfaceEblPjPPNS_10InputEventE";

        void* addr = xdl_sym(handle, symbol, nullptr);
        if (addr) {
            if (DobbyHook(addr, (void*)HookConsume, &origConsume) == 0) {
                AddDebugLog("[TouchInput] HookConsume installed successfully!");
            } else {
                AddDebugLog("[TouchInput] HookConsume failed!");
            }
        } else {
            AddDebugLog("[TouchInput] Symbol not found!");
        }
    }

} // namespace TouchInput
