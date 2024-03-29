#include <cstring>
#include <jni.h>
#include <pthread.h>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <dlfcn.h>

#include <list>
#include <vector>
#include <cstring>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <linux/ptrace.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <dlfcn.h>

#include "modmenu.h"
#include "zygisk.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

class MyModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        jni_env = env;
        JavaVM *vm;
        jni_env->GetJavaVM(&vm);
        java_vm = vm;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) {
            LOGE(OBFUSCATE("Skip unknown process"));
            return;
        }
        enable_hack = isGame(jni_env, args->app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (enable_hack) {
            int ret;
            pthread_t ntid;
            if ((ret = pthread_create(&ntid, nullptr, hack_thread, nullptr))) {
                LOGE(OBFUSCATE("can't create thread: %s"), strerror(ret));
            }
        }
    }
};

REGISTER_ZYGISK_MODULE(MyModule)