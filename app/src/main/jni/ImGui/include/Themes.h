#pragma once

static void SetDarkBlueUITheme()
{
	// Thiết lập màu chủ đề
	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text]                   = ImVec4(0.86f, 0.86f, 0.86f, 1.00f); // Màu chữ
	colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f); // Màu chữ vô hiệu hóa
	colors[ImGuiCol_WindowBg]               = ImVec4(0.16f, 0.16f, 0.16f, 0.95f); // Màu nền cửa sổ
	colors[ImGuiCol_ChildBg]                = ImVec4(0.1333f, 0.1333f, 0.1333f, 0.8f); // Màu nền bên trong Child window
	colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f); // Màu nền của Popup window
	colors[ImGuiCol_Border]                 = ImVec4(0.13f, 0.13f, 0.13f, 0.40f); // Màu đường viền
	colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); // Màu bóng đổ đường viền
	colors[ImGuiCol_FrameBg]                = ImVec4(0.27f, 0.27f, 0.27f, 0.54f); // Màu nền Frame
	colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.47f, 0.47f, 0.47f, 0.40f); // Màu nền Frame khi hover
	colors[ImGuiCol_FrameBgActive]          = ImVec4(0.40f, 0.40f, 0.40f, 0.67f); // Màu nền Frame khi active
	colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f); // Màu nền tiêu đề cửa sổ
	colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.16f, 0.16f, 1.00f); // Màu nền tiêu đề cửa sổ khi active
	colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f); // Màu nền tiêu đề cửa sổ khi bị thu gọn
	colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f); // Màu nền của MenuBar
	colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f); // Màu nền của thanh cuộn
	colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f); // Màu của cần cẩu thanh cuộn
	colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f); // Màu của cần cẩu thanh cuộn khi hover
	colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f); // Màu của cần cẩu thanh cuộn khi active
	colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Màu của dấu check
	colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f); // Màu của cần cẩu thanh trượt
	colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Màu của cần cẩu thanh trượt khi active
	colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f); // Màu của nút
	colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Màu của nút khi hover
	colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f); // Màu của nút khi active
	colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f); // Màu của tiêu đề
	colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f); // Màu của tiêu đề khi hover
	colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Màu của tiêu đề khi active
	colors[ImGuiCol_Separator]              = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Màu của dấu phân cách
	colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Màu của dấu phân cách khi hover
	colors[ImGuiCol_SeparatorActive]        = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Màu của dấu phân cách khi active
	colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.25f); // Màu của khớp co giãn
	colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f); // Màu của khớp co giãn khi hover
	colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f); // Màu của khớp co giãn khi active
	colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.18f, 0.18f, 0.86f); // Màu của tab
	colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f); // Màu của tab khi hover
	colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Màu của tab khi active
	colors[ImGuiCol_TabUnfocused]           = ImVec4(0.06f, 0.06f, 0.06f, 0.97f); // Màu của tab khi không focus
	colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.13f, 0.13f, 0.13f, 1.00f); // Màu của tab khi không focus nhưng active
	colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f); // Màu của đồ thị dạng line
	colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.61f, 0.61f, 0.61f, 1.00f); // Màu của đồ thị dạng line khi hover
	colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f); // Màu của đồ thị dạng histogram
	colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f); // Màu của đồ thị dạng histogram khi hover
	colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f); // Màu nền khi chọn văn bản
	colors[ImGuiCol_DragDropTarget]         = ImVec4(0.41f, 0.41f, 0.41f, 1.00f); // Màu của Drag and Drop target
	colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f); // Màu highlight trong navigation
	colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f); // Màu highlight trong navigation khi windowing
	colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f); // Màu nền khi windowing
	colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f); // Màu nền khi modal window
}
