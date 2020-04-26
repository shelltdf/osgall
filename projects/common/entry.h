
#ifndef _NIUBI_ENTRY_H_
#define _NIUBI_ENTRY_H_


/*
    NIUBI_SUBSYSTEM_CONSOLE		[default]
    NIUBI_SUBSYSTEM_WINDOWS

    NIUBI_ENTRY_MAIN				[default]	main() winMian() etc.
    NIUBI_ENTRY_SHARE_LIBRARY					share library  etc. DLL SO JNI
    NIUBI_ENTRY_ANDORID_NDK_NATIVE				android native windows
*/


#ifdef NIUBI_ENTRY_ANDORID_NDK_NATIVE

#include <jni.h>
#include <errno.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "niubi_ndk", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "niubi_ndk", __VA_ARGS__))

#endif



struct niubi_entry_arg
{
#ifdef NIUBI_ENTRY_MAIN
    int argc;
    char** argv;
#endif
#ifdef NIUBI_ENTRY_ANDORID_NDK_NATIVE
    niubi_entry_arg():state(0){}//,native_window(0),isinit(0){}
    struct android_app* state;
    //void* native_window;
    //bool isinit;
#endif
};

//#include <nbBase/Public.h>

extern int niubi_entry(niubi_entry_arg& arg);



//ENTRY
#ifdef NIUBI_ENTRY_ANDORID_NDK_NATIVE
void android_main(struct android_app* state)
{
    //#error "----------------------------------------------"
    LOGI("android_main");
    LOGW("android_main");

    // Make sure glue isn't stripped.
    app_dummy();


    //LOGI("----------------------------------------------android_main");
    //LOGW("----------------------------------------------android_main");

    niubi_entry_arg arg;
    arg.state = state;
    niubi_entry(arg);
}
#endif //ENTRY



//ENTRY
#ifdef NIUBI_ENTRY_SHARE_LIBRARY


#endif //NIUBI_ENTRY_SHARE_LIBRARY



//ENTRY
#ifdef NIUBI_ENTRY_MAIN

//SUBSYSTEM
#ifdef NIUBI_SUBSYSTEM_WINDOWS
int winMain(int argc, char** argv)
#endif //NIUBI_SUBSYSTEM_WINDOWS
#ifdef NIUBI_SUBSYSTEM_CONSOLE
int main(int argc, char** argv)
#endif //NIUBI_SUBSYSTEM_CONSOLE

{
    niubi_entry_arg arg;
    arg.argc = argc;
    arg.argv = argv;
    return niubi_entry(arg);
}

#endif //ENTRY


#endif //_NIUBI_ENTRY_H_
