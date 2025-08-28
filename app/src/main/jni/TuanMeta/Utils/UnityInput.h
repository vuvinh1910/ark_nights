#pragma once

/*
	ImGui Touch For Arena Of Valor
	Original: Tis Nguyen
	Modified: @TuanMeta
*/

namespace UnityEngine {
    namespace Input {
        enum class TouchPhase {
            Began,
            Moved,
            Stationary,
            Ended,
            Canceled
        };

        enum class TouchType {
            Direct,
            Indirect,
            Stylus
        };

        struct Touch {
            int m_FingerId{};
            Vector2 m_Position;
            Vector2 m_RawPosition;
            Vector2 m_PositionDelta;
            float m_TimeDelta{};
            int m_TapCount{};
            TouchPhase m_Phase;
            TouchType m_Type;
            float m_Pressure{};
            float m_maximumPossiblePressure{};
            float m_Radius{};
            float m_RadiusVariance{};
            float m_AltitudeAngle{};
            float m_AzimuthAngle{};
        };

        bool is_done = false;

        Touch (*orig_GetTouch)(void *instance, int index);
        Touch GetTouch(void *instance, int index) {
            Touch touch = orig_GetTouch(instance, index);

            if (instance != nullptr && index < 3) {
                ImGuiIO &io = ImGui::GetIO();
                float x = touch.m_Position.x;
                float y = round(io.DisplaySize.y) - touch.m_Position.y;

                switch (touch.m_Phase) {
                    case TouchPhase::Began:
                        io.MousePos = ImVec2(x, y);
                        io.MouseDown[0] = true;
                        break;
                    case TouchPhase::Moved:
                        io.MousePos = ImVec2(x, y);
                        break;
                    case TouchPhase::Ended:
                    case TouchPhase::Canceled:
                        io.MouseDown[0] = false;
                        break;
                    default: break;
                }

                if (io.WantCaptureMouse) {
                    touch.m_Position = {};
                    touch.m_RawPosition = {};
                    touch.m_TapCount = {};
                }
            }

            return touch;
        }

        bool Setup() {
            if (!is_done) {
                Tools::Hook(GetMethodOffset("UnityEngine.UI.dll", "UnityEngine.EventSystems", "BaseInput", "GetTouch", 1), (void *)GetTouch, (void **)&orig_GetTouch);
                is_done = true;
            }
            return orig_GetTouch != nullptr;
        }
    }
}
