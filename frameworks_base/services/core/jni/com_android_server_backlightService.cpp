/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "BacklightServiceJNI"

#include "jni.h"
#include "JNIHelp.h"
#include "android_runtime/AndroidRuntime.h"

#include <utils/misc.h>
#include <utils/Log.h>
#include <hardware/hardware.h>
#include <hardware/backlight.h>

#include <stdio.h>

namespace android
{

struct Devices {
  backlight_device_t* backlight;
};

static backlight_device_t* get_device(hw_module_t* module, char const* name)
{
  ALOGE("Entering %s\n", __func__);
  int err;
    hw_device_t* device;
    err = module->methods->open(module, name, &device);
    if (err == 0) {
      ALOGE("Line:%d call open() successfully\n", __LINE__);
        return (backlight_device_t*)device;
    } else {
        return NULL;
    }
}

static jlong init_native(JNIEnv* /* env */, jobject /* clazz */)
{
  ALOGE("Entering %s\n", __func__);
    int err;
    hw_module_t* module;
    Devices* device;
    
    device = (Devices*)malloc(sizeof(Devices));

    err = hw_get_module(BACKLIGHT_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
    if (err == 0) {
        ALOGE("Line:%d call get_device()->open()\n", __LINE__);
        device->backlight = get_device(module, BACKLIGHT_HARDWARE_MODULE_ID);
    } else {
      ALOGE("Line:%d call hw_get_module return FAIL\n", __LINE__);
        memset(device, 0, sizeof(Devices));
    }

    return (jlong)device;
}

static void setLight_native(JNIEnv* /* env */, jobject /* clazz */, jlong ptr, jint value)
{
  ALOGE("Entering %s\n", __func__);
    Devices* device = (Devices*)ptr;
    //uint32_t version = device->backlight->common.version;
    device->backlight->set_backlight(device->backlight, value);
}

static int getLight_native(JNIEnv* /* env */, jobject /* clazz */, jlong ptr)
{
  ALOGE("Entering %s\n", __func__);
    Devices* device = (Devices*)ptr;
    return device->backlight->get_backlight(device->backlight);
}

static const JNINativeMethod method_table[] = {
    { "init_native", "()J", (void*)init_native },
    { "setLight_native", "(JI)V", (void*)setLight_native },
    { "getLight_native", "(J)I", (int*)getLight_native },
};

int register_android_server_backlightService(JNIEnv *env)
{
  ALOGE("Entering %s\n", __func__);
    return jniRegisterNativeMethods(env, "com/android/server/backlight/BacklightService",
            method_table, NELEM(method_table));
}

};
