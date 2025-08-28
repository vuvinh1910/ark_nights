#include <thread>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "Call_Me.h"
#include "Hook.h"
#include "Zygisk.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

class MyModule : public zygisk::ModuleBase {
public:
	void onLoad(Api *api, JNIEnv *env) override {
		this->api = api;
		this->env = env;
	}

	void preAppSpecialize(AppSpecializeArgs *args) override {
		auto package_name = env->GetStringUTFChars(args->nice_name, nullptr);
		auto app_data_dir = env->GetStringUTFChars(args->app_data_dir, nullptr);
		preSpecialize(package_name, app_data_dir);
		env->ReleaseStringUTFChars(args->nice_name, package_name);
		env->ReleaseStringUTFChars(args->app_data_dir, app_data_dir);
	}

	void postAppSpecialize(const AppSpecializeArgs *) override {
		if (enable_hack) {
			std::thread hack_thread(hack_prepare, game_data_dir, data, length);
			hack_thread.detach();
		}
	}

private:
    Api *api = nullptr;
    JNIEnv *env = nullptr;
    bool enable_hack = false;
    char *game_data_dir = nullptr;
    void *data = nullptr;
    size_t length = 0;

    void preSpecialize(const char *package_name, const char *app_data_dir) {
        if (strcmp(package_name, "com.YoStarEN.Arknights") == 0 ||
            strcmp(package_name, "com.YoStarJP.Arknights") == 0) {

            LOGI("detect game: %s", package_name);
            enable_hack = true;
            game_data_dir = strdup(app_data_dir); // dễ quản lý hơn new[]

#if defined(__i386__)
            const char *path = "zygisk/x86.so";
#elif defined(__x86_64__)
            const char *path = "zygisk/x86_64.so";
#elif defined(__arm__)
            const char *path = "zygisk/armeabi-v7a.so";
#elif defined(__aarch64__)
            const char *path = "zygisk/arm64-v8a.so";
#endif

            int dirfd = api->getModuleDir();
            int fd = openat(dirfd, path, O_RDONLY);
            if (fd != -1) {
                struct stat sb{};
                if (fstat(fd, &sb) == 0) {
                    length = sb.st_size;
                    data = mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fd, 0);
                    if (data == MAP_FAILED) {
                        LOGW("mmap failed");
                        data = nullptr;
                    }
                }
                close(fd);
            } else {
                LOGW("Unable to open so file: %s", path);
            }
        } else {
            api->setOption(zygisk::Option::DLCLOSE_MODULE_LIBRARY);
        }
    }
};

REGISTER_ZYGISK_MODULE(MyModule)

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface)
{
	eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
	eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
	
	static bool setup = false;
	static bool menu = true;
	
	if (!setup)
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		SetDarkBlueUITheme();
		
		io.Fonts->AddFontFromMemoryTTF(Inter, sizeof(Inter), 28.f, NULL, io.Fonts->GetGlyphRangesVietnamese());
		interbold = io.Fonts->AddFontFromMemoryTTF(InterBold, sizeof(InterBold), 28.f, NULL, io.Fonts->GetGlyphRangesVietnamese());
		
		io.KeyMap[ImGuiKey_UpArrow] = 19;
		io.KeyMap[ImGuiKey_DownArrow] = 20;
		io.KeyMap[ImGuiKey_LeftArrow] = 21;
		io.KeyMap[ImGuiKey_RightArrow] = 22;
		io.KeyMap[ImGuiKey_Enter] = 66;
		io.KeyMap[ImGuiKey_Backspace] = 67;
		io.KeyMap[ImGuiKey_Escape] = 111;
		io.KeyMap[ImGuiKey_Delete] = 112;
		io.KeyMap[ImGuiKey_Home] = 122;
		io.KeyMap[ImGuiKey_End] = 123;
		
		ImGui_ImplOpenGL3_Init("#version 300 es");
		ImGui::GetStyle().ScaleAllSizes(3.0f);
		loadImages();
		setup = true;
	}
	
	ImGuiIO& io = ImGui::GetIO();
	static bool WantTextInputLast = false;
	if (io.WantTextInput && !WantTextInputLast) ShowSoftKeyboardInput();
	WantTextInputLast = io.WantTextInput;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
	ImGui::NewFrame();
	
	ui_dpi_scale = ImLerp(ui_dpi_scale , menu ? 1.1f : 0.4f, 0.5f * (10.0f * ImGui::GetIO().DeltaTime));
	
	if (ui_dpi_scale <= 1.0f && ui_dpi_scale >= 0.5f) window_flags = menu ? ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar : ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	if (ui_dpi_scale < 0.5f) window_flags = menu ? ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar : ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
	
	if (ui_dpi_scale <= 1.0f && ui_dpi_scale >= 0.5f) ImGui::SetNextWindowSize(ImVec2(1280 * ui_dpi_scale, 640 * ui_dpi_scale));
	if (ui_dpi_scale < 0.5f) ImGui::SetNextWindowSize(ImVec2(700 * ui_dpi_scale - 100, 600 * ui_dpi_scale - 80));

	if (ImGui::Begin(OBFUSCATE("Menu"), nullptr, window_flags))
	{
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 170);
		
		CircleImage(Logo.textureId, 120);
		if (ImGui::IsItemHovered() && IsClickOrHold() == 0) menu = !menu;
		
		static ImVec4 active = to_vec4(0, 150, 255, 255);
		static ImVec4 inactive = to_vec4(0, 0, 0, 0);
        static int Tab = 3;

        ImGui::Spacing();
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.6f);

        ImGui::PushStyleColor(ImGuiCol_Button, Tab == 1 ? active : inactive);
        if (ImGui::Button(oxorany("A"), ImVec2(120, 50))) Tab = 1;
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Button, Tab == 2 ? active : inactive);
        if (ImGui::Button(oxorany("B"), ImVec2(120, 50))) Tab = 2;
        ImGui::PopStyleColor();

        ImGui::PushStyleColor(ImGuiCol_Button, Tab == 3 ? active : inactive);
        if (ImGui::Button(oxorany("Log"), ImVec2(120, 50))) Tab = 3;
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        ImGui::NextColumn();
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();
        ImVec2 textSize = ImGui::CalcTextSize(window_name);
        float cursorX = ImGui::GetCursorPosX();
        float textPosX = cursorX + (contentRegion.x - textSize.x) / 2.0f;
        ImGui::PushFont(interbold);
        if (ui_dpi_scale > 0.5f) ImGui::SetCursorPosX(textPosX);
        ImGui::Text(window_name);
        ImGui::PopFont();

        if (Tab == 1) {
            ImGui::Text(oxorany("Damage Multiplie"));
            ImGui::SliderInt(oxorany("##PDamage"), &dmg, 1, 1000);

            ImGui::Text(oxorany("Defense Multiplie"));
            ImGui::SliderInt(oxorany("##PDefense"), &defense, 1, 1000);

            ImGui::Text(oxorany("Attack Speed Multiplie"));
            ImGui::SliderInt(oxorany("##PSpeed"), &attacksp, 1, 20);

            ImGui::Text(oxorany("SP Recovery Multiplie"));
            ImGui::SliderInt(oxorany("##PSpRecovery"), &sp_recovery, 1, 100);

            ImGui::Checkbox(oxorany("Frozen Enemies"), &frozen);
            ImGui::Checkbox(oxorany("Auto Win"), &autowin);
        }

        if (Tab == 2) {
            ImGui::Checkbox(oxorany("OneHit Kill"), &onehit);
            ImGui::Checkbox(oxorany("GodMode"), &godMode);
            ImGui::Checkbox(oxorany("No Decrease DP"), &deploy);
            ImGui::Checkbox(oxorany("0 Card Cost"), &noCardCost);
            ImGui::Checkbox(oxorany("No Respawn Time"), &noRespawnTime);
            ImGui::Checkbox(oxorany("Free Deploy"), &freeDeploy);
            ImGui::Checkbox(oxorany("No Unit Limit"), &unlimitUnit);
            ImGui::Checkbox(oxorany("Freeze LifePoint"), &freezeLifePoint);
            ImGui::Checkbox(oxorany("999 LifePoint"), &maxLifePoint);
        }

        if (Tab == 3) {
            ImGui::Text(oxorany("Debug Logs:"));
            ImGui::BeginChild("DebugRegion", ImVec2(0, 400), true);
            for (auto& line : GetDebugLogs()) {
                ImGui::TextUnformatted(line.c_str());
            }
            ImGui::EndChild();
        }

        ImGui::End();
	}
	
	ImGui::Render();
	ImGui::EndFrame();
	
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &glWidth);
	glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &glHeight);
	
	io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_Home]] = false;
	io.KeysDown[io.KeyMap[ImGuiKey_End]] = false;
	return orig_eglSwapBuffers(dpy, surface);
}

uintptr_t il2cppBase = 0;
void *Init_thread()
{
	Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libandroid.so"), OBFUSCATE("ANativeWindow_getWidth")), (void *) _ANativeWindow_getWidth, (void **) &orig_ANativeWindow_getWidth);
	Tools::Hook((void *) DobbySymbolResolver(OBFUSCATE("/system/lib/libandroid.so"), OBFUSCATE("ANativeWindow_getHeight")), (void *) _ANativeWindow_getHeight, (void **) &orig_ANativeWindow_getHeight);
    Tools::Hook((void *) DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers"), (void *) hook_eglSwapBuffers, (void **) &orig_eglSwapBuffers);

    while (il2cppBase == 0) {
        il2cppBase = KittyMemory::findIl2cppBase();
        if (!il2cppBase) {
            AddDebugLog("Waiting for libil2cpp.so to load...");
            sleep(1);
        }
    }
    AddDebugLog("libil2cpp.so loaded at %p", (void*)il2cppBase);

	Attach();
	PollUnicodeChars();
    TouchInput::Init();

    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity") , oxorany("get_hp"), 0), (void *) get_hp , (void **) &_get_hp);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity") , oxorany("set_hp"), 1), (void *) set_hp , (void **) &_set_hp);
	Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity") , oxorany("get_atk"), 0), (void *) get_atk , (void **) &_get_atk);
	Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity") , oxorany("get_isFrozen"), 0), (void *) get_isFrozen , (void **) &_get_isFrozen);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity") , oxorany("get_attackTime"), 0), (void *) get_attackTime , (void **) &_get_attackTime);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity.SpController") , oxorany("_RecoverMp"), 2), (void *) SkillPointController , (void **) &_SkillPointController);

    sideType = GetFieldOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("BObject") , oxorany("<side>k__BackingField"));
    m_owner = GetFieldOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Entity.SpController") , oxorany("m_owner"));
    respawnState = GetFieldOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Deck.Card") , oxorany("ignoreRespawningState"));

    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("BattleController") , oxorany("ModifyCost"), 5), (void *) ModifyCost , (void **) &_ModifyCost);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("BattleController") , oxorany("ModifyLifePoint"), 4), (void *) ModifyLifePoint , (void **) &_ModifyLifePoint);
	Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("BattleController") , oxorany("OnTick"), 0), (void *) BattleController , (void **) &_BattleController);

    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Deck.Card") , oxorany("get_cost"), 0), (void *) get_cost , (void **) &_get_cost);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Deck.Card") , oxorany("get_dontOccupyDeployCnt"), 0), (void *) get_dontOccupyDeployCnt , (void **) &_get_dontOccupyDeployCnt);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Deck.Card") , oxorany("OnTick"), 1), (void *) CardController, (void **) &_CardController);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Deck") , oxorany("OnCardListChanged"), 1), (void *) OnCardListChanged , (void **) &_OnCardListChanged);
    Tools::Hook((void *) (uintptr_t) GetMethodOffset(oxorany("Assembly-CSharp.dll"), oxorany("Torappu.Battle"), oxorany("Deck.Card") , oxorany("get_readyToSpawn"), 0), (void *) get_readyToSpawn , (void **) &_get_readyToSpawn);

	return nullptr;
}
