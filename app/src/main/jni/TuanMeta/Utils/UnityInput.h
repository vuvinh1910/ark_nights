#pragma once

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

		static bool (*GetMouseButtonDown)(int button);
		bool is_done = false;

		static Touch (*orig_GetTouch)(int index);
		static Touch GetTouch(int index)
		{
			Touch touch = orig_GetTouch(index);

			if (index < 3) {
				ImGuiIO &io = ImGui::GetIO();
				float x = touch.m_Position.x;
				float y = round(io.DisplaySize.y) - touch.m_Position.y;

				switch (touch.m_Phase) {
					case TouchPhase::Began:
						io.MousePos.x = x;
						io.MousePos.y = y;
						io.MouseDown[index] = GetMouseButtonDown(index);
						break;
					case TouchPhase::Moved:
						io.MousePos.x = x;
						io.MousePos.y = y;
						break;
					case TouchPhase::Ended:
						io.MouseDown[0] = GetMouseButtonDown(0);
						io.MouseDown[1] = GetMouseButtonDown(1);
						io.MouseDown[2] = GetMouseButtonDown(2);
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
				GetMouseButtonDown = reinterpret_cast<decltype(GetMouseButtonDown)>(Il2Cpp::GetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetMouseButtonDown"), 1));
				Tools::Hook((void *)Il2Cpp::GetMethodOffset(OBFUSCATE("UnityEngine.InputLegacyModule.dll"), OBFUSCATE("UnityEngine"), OBFUSCATE("Input"), OBFUSCATE("GetTouch"), 1), (void *)GetTouch, (void **)&orig_GetTouch);
				is_done = true;
			}
			return orig_GetTouch != nullptr;
		}
	}
}
