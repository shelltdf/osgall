
#ifndef _MY_NATIVE_APP_H_
#define _MY_NATIVE_APP_H_

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))


/**
 * Our saved state data.
 */
struct saved_state 
{
    float angle;
    int32_t x;
    int32_t y;
};


class myNativeApp
{
public:

    myNativeApp()
    {
        app = 0;
        display = 0;
        surface = 0;
        context = 0;
        width  = 0;
        height = 0;
        animating   = 0;
        state.angle = 0;
        state.x     = 0;
        state.y     = 0;
    }
    
    virtual ~myNativeApp(){}
    
    //初始化程序
    void Create( struct android_app* state )
    {
        state->userData = this; //用户静态回调的参数
        state->onAppCmd     = myNativeApp::engine_handle_cmd;
        state->onInputEvent = myNativeApp::engine_handle_input;
        this->app = state;

        // Prepare to monitor accelerometer
        this->sensorManager = ASensorManager_getInstance();
        this->accelerometerSensor = ASensorManager_getDefaultSensor(
            this->sensorManager
            ,ASENSOR_TYPE_ACCELEROMETER);
        this->sensorEventQueue = ASensorManager_createEventQueue(
            this->sensorManager
            ,state->looper
            , LOOPER_ID_USER
            , NULL
            , NULL);

        if (state->savedState != NULL) 
        {
            // We are starting with a previous saved state; restore from it.
            this->state = *(struct saved_state*)state->savedState;
        }
    }
    
    //运行程序
    void run()
    {
        // loop waiting for stuff to do.
        while (1) 
        {
            // Read all pending events.
            int ident;
            int events;
            struct android_poll_source* source;

            // If not animating, we will block forever waiting for events.
            // If animating, we loop until all events are read, then continue
            // to draw the next frame of animation.
            while ((ident=ALooper_pollAll(this->animating ? 0 : -1, NULL, &events,(void**)&source)) >= 0) 
            {

                // Process this event.
                if (source != NULL) 
                {
                    source->process(this->app, source);
                }

                // If a sensor has data, process it now.
                if (ident == LOOPER_ID_USER) 
                {
                    if (this->accelerometerSensor != NULL) 
                    {
                        ASensorEvent event;
                        while (ASensorEventQueue_getEvents(this->sensorEventQueue,&event, 1) > 0) 
                        {
                            LOGI("accelerometer: x=%f y=%f z=%f",
                                    event.acceleration.x, event.acceleration.y,
                                    event.acceleration.z);
                        }
                    }
                }

                // Check if we are exiting.
                if (this->app->destroyRequested != 0) 
                {
                    //engine_term_display(&engine);
                    this->GLDistroy();
                    return;
                }
            }

            if (this->animating) 
            {
                // Done with events; draw next animation frame.
                this->state.angle += .01f;
                if (this->state.angle > 1) 
                {
                    this->state.angle = 0;
                }

                // Drawing is throttled to the screen update rate, so there
                // is no need to do timing here.
                //engine_draw_frame(&engine);
                this->GLDraw();
            }
        }
    }
    
    //初始化GL环境
    int GLCreate()
    {
        // initialize OpenGL ES and EGL
        /*
         * Here specify the attributes of the desired configuration.
         * Below, we select an EGLConfig with at least 8 bits per color
         * component compatible with on-screen windows
         */
        const EGLint attribs[] = 
        {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_NONE
        };
        EGLint w, h, dummy, format;
        EGLint numConfigs;
        EGLConfig config;
        EGLSurface surface;
        EGLContext context;

        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        eglInitialize(display, 0, 0);

        /* Here, the application chooses the configuration it desires. In this
         * sample, we have a very simplified selection process, where we pick
         * the first EGLConfig that matches our criteria */
        eglChooseConfig(display, attribs, &config, 1, &numConfigs);

        /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
         * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
         * As soon as we picked a EGLConfig, we can safely reconfigure the
         * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

        ANativeWindow_setBuffersGeometry(this->app->window, 0, 0, format);

        surface = eglCreateWindowSurface(display, config, this->app->window, NULL);
        context = eglCreateContext(display, config, NULL, NULL);

        if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) 
        {
            LOGW("Unable to eglMakeCurrent");
            return -1;
        }

        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);

        this->display = display;
        this->context = context;
        this->surface = surface;
        this->width = w;
        this->height = h;
        this->state.angle = 0;

        // Initialize GL state.
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);
        glDisable(GL_DEPTH_TEST);

        
        onGLCreate( 0,0,w,h );
        
        return 0;
    }
    
    //销毁GL环境
    void GLDistroy()
    {
        onGLDistroy();
    
        if (this->display != EGL_NO_DISPLAY) 
        {
            eglMakeCurrent(this->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (this->context != EGL_NO_CONTEXT) 
            {
                eglDestroyContext(this->display, this->context);
            }
            if (this->surface != EGL_NO_SURFACE) 
            {
                eglDestroySurface(this->display, this->surface);
            }
            eglTerminate(this->display);
        }
        this->animating = 0;
        this->display = EGL_NO_DISPLAY;
        this->context = EGL_NO_CONTEXT;
        this->surface = EGL_NO_SURFACE;
    }
    
    //绘制GL内容
    void GLDraw()
    {
        if (this->display == NULL) { return; }// No display.
        
        onGLDraw();

        eglSwapBuffers(this->display, this->surface);
    }
    
    
    //GL初始化 消息
    virtual void onGLCreate( int x,int y,int width,int height ) = 0;
    //GL销毁 消息
    virtual void onGLDistroy() = 0;
    //GL绘制 消息
    virtual void onGLDraw() = 0;
    
    
    //系统对象
    struct android_app* app;

    //传感器对象
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    //GL环境和属性
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    
    //动画属性
    int animating;
    struct saved_state state;
    
    
    //系统消息接收
    //系统输入接收
    
    /**
    * Process the next input event.
    */
    static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
    {
        myNativeApp* engine = (myNativeApp*)app->userData;
        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
        {
            engine->animating = 1;
            engine->state.x = AMotionEvent_getX(event, 0);
            engine->state.y = AMotionEvent_getY(event, 0);
            return 1;
        }
        return 0;
    }

    /**
     * Process the next main command.
     */
    static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
    {
        myNativeApp* engine = (myNativeApp*)app->userData;
        
        switch (cmd) 
        {
            case APP_CMD_SAVE_STATE:
                // The system has asked us to save our current state.  Do so.
                engine->app->savedState = malloc(sizeof(struct saved_state));
                *((struct saved_state*)engine->app->savedState) = engine->state;
                engine->app->savedStateSize = sizeof(struct saved_state);
                break;
                
            case APP_CMD_INIT_WINDOW:
                // The window is being shown, get it ready.
                if (engine->app->window != NULL) 
                {
                    //engine_init_display(engine);
                    //engine_draw_frame(engine);
                    engine->GLCreate();
                    engine->GLDraw();
                }
                break;
                
            case APP_CMD_TERM_WINDOW:
                // The window is being hidden or closed, clean it up.
                //engine_term_display(engine);
                engine->GLDistroy();
                break;
                
            case APP_CMD_GAINED_FOCUS:
                // When our app gains focus, we start monitoring the accelerometer.
                if (engine->accelerometerSensor != NULL) 
                {
                    ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                            engine->accelerometerSensor);
                    // We'd like to get 60 events per second (in us).
                    ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                            engine->accelerometerSensor, (1000L/60)*1000);
                }
                break;
                
            case APP_CMD_LOST_FOCUS:
                // When our app loses focus, we stop monitoring the accelerometer.
                // This is to avoid consuming battery while not being used.
                if (engine->accelerometerSensor != NULL) 
                {
                    ASensorEventQueue_disableSensor(engine->sensorEventQueue,engine->accelerometerSensor);
                }
                // Also stop animating.
                engine->animating = 0;
                //engine_draw_frame(engine);
                engine->GLDraw();
                break;
        }
    }
    
};

#endif //_MY_NATIVE_APP_H_

