#pragma once

typedef jint(*jni_getvms_t)(JavaVM**, jsize, jsize*);
static jni_getvms_t JNI_GetCreatedJavaVMs_p;

jni_getvms_t getVMFunction_fake() {
	void* library = xdl_open("libart.so", 0);
	if(library == nullptr) {
		//LOGE("Google trolling failed. Giving up.");
		return nullptr;
	}
	void* sym = xdl_sym(library, "JNI_GetCreatedJavaVMs", 0);
	xdl_close(library);
	return (jni_getvms_t)sym;
}

jni_getvms_t getVMFunction() {
	void* library = xdl_open("libnativehelper.so", RTLD_LAZY);
	if(library == nullptr) {
		//LOGE("Time to troll Google!");
		return getVMFunction_fake();
	}
	void* sym = xdl_sym(library, "JNI_GetCreatedJavaVMs", 0);
	xdl_close(library);
	return sym == nullptr ? getVMFunction_fake() : (jni_getvms_t) sym;
}

JavaVM *jvm;

int ShowSoftKeyboardInput() {
	JNIEnv *env;
	jint result;
	jint flags = 0;
	
	jvm->AttachCurrentThread(&env, NULL);
	
	jclass looperClass = env->FindClass(OBFUSCATE("android/os/Looper"));
	auto prepareMethod = env->GetStaticMethodID(looperClass, OBFUSCATE("prepare"), OBFUSCATE("()V"));
	env->CallStaticVoidMethod(looperClass, prepareMethod);
	
	jclass activityThreadClass = env->FindClass(OBFUSCATE("android/app/ActivityThread"));
	jfieldID sCurrentActivityThreadField = env->GetStaticFieldID(activityThreadClass, OBFUSCATE("sCurrentActivityThread"), OBFUSCATE("Landroid/app/ActivityThread;"));
	jobject sCurrentActivityThread = env->GetStaticObjectField(activityThreadClass, sCurrentActivityThreadField);
	
	jfieldID mInitialApplicationField = env->GetFieldID(activityThreadClass, OBFUSCATE("mInitialApplication"), OBFUSCATE("Landroid/app/Application;"));
	jobject mInitialApplication = env->GetObjectField(sCurrentActivityThread, mInitialApplicationField);
	
	jclass contextClass = env->FindClass(OBFUSCATE("android/content/Context"));
	jfieldID fieldINPUT_METHOD_SERVICE = env->GetStaticFieldID(contextClass, OBFUSCATE("INPUT_METHOD_SERVICE"), OBFUSCATE("Ljava/lang/String;"));
	jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(contextClass, fieldINPUT_METHOD_SERVICE);
	jmethodID getSystemServiceMethod = env->GetMethodID(contextClass, OBFUSCATE("getSystemService"), OBFUSCATE("(Ljava/lang/String;)Ljava/lang/Object;"));
	jobject callObjectMethod = env->CallObjectMethod(mInitialApplication, getSystemServiceMethod, INPUT_METHOD_SERVICE);
	
	jclass classInputMethodManager = env->FindClass(OBFUSCATE("android/view/inputmethod/InputMethodManager"));
	jmethodID toggleSoftInputId = env->GetMethodID(classInputMethodManager, OBFUSCATE("toggleSoftInput"), OBFUSCATE("(II)V"));
	
	if (result) {
		env->CallVoidMethod(callObjectMethod, toggleSoftInputId, 2, flags);
	} else {
		env->CallVoidMethod(callObjectMethod, toggleSoftInputId, flags, flags);
	}
	
	env->DeleteLocalRef(classInputMethodManager);
	env->DeleteLocalRef(callObjectMethod);
	env->DeleteLocalRef(contextClass);
	env->DeleteLocalRef(mInitialApplication);
	env->DeleteLocalRef(activityThreadClass);
	jvm->DetachCurrentThread();
	
	return result;
}

int (*o_inject_event)(JNIEnv *env, jobject thiz, jobject inputEvent);
int hook_input(JNIEnv *env, jobject __this, jobject input_event) {
    ImGuiIO &io = ImGui::GetIO();
    
    jclass motionEventClass = env->FindClass(OBFUSCATE("android/view/MotionEvent"));
    
    if (env->IsInstanceOf(input_event, motionEventClass)) {
        jmethodID getActionMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getAction"), OBFUSCATE("()I"));
        jint getAction = env->CallIntMethod(input_event, getActionMethod);
        
        jmethodID getXMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getX"), OBFUSCATE("()F"));
        jfloat getX = env->CallFloatMethod(input_event, getXMethod);
        
        jmethodID getYMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getY"), OBFUSCATE("()F"));
        jfloat getY = env->CallFloatMethod(input_event, getYMethod);
        
        jmethodID getPointerCountMethod = env->GetMethodID(motionEventClass, OBFUSCATE("getPointerCount"), OBFUSCATE("()I"));
        jint getPointerCount = env->CallIntMethod(input_event, getPointerCountMethod);
        
        switch(getAction) {
            
        case 0:
            io.MouseDown[0] = true;
            break;
        case 1:
            io.MouseDown[0] = false;
            break;
        case 2:
            if (getPointerCount > 1) {
                io.MouseDown[0] = false;
            } else {
                io.MouseWheel = 0;
            }
            break;
        }
        io.MousePos = ImVec2(getX, getY);
    }
    
    jclass KeyEventClass = env->FindClass(OBFUSCATE("android/view/KeyEvent"));
    if (env->IsInstanceOf(input_event, KeyEventClass)) {
        jmethodID getActionMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getAction"), OBFUSCATE("()I"));
        if (env->CallIntMethod(input_event, getActionMethod) == 0) {
            jmethodID getKeyCodeMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getKeyCode"), OBFUSCATE("()I"));
            jmethodID getUnicodeCharMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getUnicodeChar"), OBFUSCATE("(I)I"));
            jmethodID getMetaStateMethod = env->GetMethodID(KeyEventClass, OBFUSCATE("getMetaState"), OBFUSCATE("()I"));
            
            jint keyCode = env->CallIntMethod(input_event, getKeyCodeMethod);
            switch (keyCode)
            {
                case 19:
                    io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = true;
                    break;
                case 20:
                    io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = true;
                    break;
                case 21:
                    io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = true;
                    break;
                case 22:
                    io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = true;
                    break;
                case 66:
                    io.KeysDown[io.KeyMap[ImGuiKey_Enter]] = true;
                    break;
                case 67:
                    io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = true;;
                    break;
                case 111:
                    io.KeysDown[io.KeyMap[ImGuiKey_Escape]] = true;
                    break;
                case 112:
                    io.KeysDown[io.KeyMap[ImGuiKey_Delete]] = true;
                    break;
                case 122:
                    io.KeysDown[io.KeyMap[ImGuiKey_Home]] = true;
                    break;
                case 123:
                    io.KeysDown[io.KeyMap[ImGuiKey_End]] = true;
                    break;
                default:
                    io.AddInputCharacter(env->CallIntMethod(input_event, getUnicodeCharMethod, env->CallIntMethod(input_event, getMetaStateMethod)));
                    break;
            }
        }
    }
    return o_inject_event(env, __this, input_event);
}

bool PollUnicodeChars()
{
	ProcMap unityMap = KittyMemory::getLibraryBaseMap("libunity.so");
	
	if (!unityMap.isValid())
		return false;
	
	std::vector<ProcMap> unityMaps = KittyMemory::getMapsByName("libunity.so");
	KittyScanner::RegisterNativeFn nativeInjectEvent = KittyScanner::findRegisterNativeFn(unityMaps, "nativeInjectEvent");
	if(nativeInjectEvent.isValid())
	{
		Tools::Hook((void *) nativeInjectEvent.fnPtr, (void *)hook_input, (void **)&o_inject_event);
		return true;
	}
	
	return false;
}
