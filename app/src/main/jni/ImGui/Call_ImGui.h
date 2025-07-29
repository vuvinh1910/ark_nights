#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"
#include "imgui_impl_android.h"
#include "imgui_impl_opengl3.h"
#include "include/InterBold_Font.h"
#include "include/GroupPanel.h"
#include "include/Inter_Font.h"
#include "include/Themes.h"

ImGuiWindowFlags window_flags;
float ui_dpi_scale = 0.0f;
ImFont *interbold;

#define HOOK_INPUT(ret, func, ...) \
	ret (*orig##func)(__VA_ARGS__); \
	ret my##func(__VA_ARGS__)

HOOK_INPUT(void, Input, void *thiz, void *ex_ab, void *ex_ac) { 
	origInput(thiz, ex_ab, ex_ac);
	ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
	return;
}

int32_t (*orig_ANativeWindow_getWidth)(ANativeWindow* window);
int32_t _ANativeWindow_getWidth(ANativeWindow* window) {
	screenWidth = orig_ANativeWindow_getWidth(window);
	return orig_ANativeWindow_getWidth(window);
}

int32_t (*orig_ANativeWindow_getHeight)(ANativeWindow* window);
int32_t _ANativeWindow_getHeight(ANativeWindow* window) {
	screenHeight = orig_ANativeWindow_getHeight(window);
	return orig_ANativeWindow_getHeight(window);
}

static void MetricsHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort) && ImGui::BeginTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void CircleImage(ImTextureID user_texture_id, float diameter, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
{
	ImVec2 p_min = ImGui::GetCursorScreenPos();
	ImVec2 p_max = ImVec2(p_min.x + diameter, p_min.y + diameter);
	ImGui::GetWindowDrawList()->AddImageRounded(user_texture_id, p_min, p_max, uv0, uv1, ImGui::GetColorU32(tint_col), diameter * 0.5f);
	ImGui::Dummy(ImVec2(diameter, diameter));
}

int IsClickOrHold()
{
	const float holdThreshold = 0.2f;
	const float moveThreshold = 5.0f;
	static auto clickStartTime = std::chrono::steady_clock::now();
	static bool isHolding = false;
	static float clickHoldDuration = 0.0f;
	static ImVec2 clickStartPos;
	static ImVec2 lastMousePos;
	int result = -1;

	ImGuiIO& io = ImGui::GetIO();
	ImVec2 mousePos = io.MousePos;
	
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		clickStartTime = std::chrono::steady_clock::now();
		clickStartPos = mousePos; // Lưu vị trí chuột khi nhấn
		lastMousePos = mousePos;  // Cập nhật vị trí chuột lần cuối
		isHolding = false;
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		clickHoldDuration = std::chrono::duration<float>(std::chrono::steady_clock::now() - clickStartTime).count();
		float distanceMoved = sqrtf((mousePos.x - lastMousePos.x) * (mousePos.x - lastMousePos.x) +
									 (mousePos.y - lastMousePos.y) * (mousePos.y - lastMousePos.y));

		if (clickHoldDuration >= holdThreshold || distanceMoved >= moveThreshold)
		{
			isHolding = true;
			result = 1;
		}
	}

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (!isHolding && clickHoldDuration < holdThreshold)
		{
			result = 0;
		}
	}

	lastMousePos = mousePos;

	return result;
}

ImVec4 to_vec4(float r, float g, float b, float a)
{
	return ImVec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}
