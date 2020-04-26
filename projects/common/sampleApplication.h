
#ifndef OSGXI_SAMPLE_APPLICATION_H
#define OSGXI_SAMPLE_APPLICATION_H


#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgText/String>
#include <osgText/Text>
#include <osgGA/TrackballManipulator>
#include <osg/Version>

#include "timer.h"

#define USE_NIUBI_WINDOW 0
#if USE_NIUBI_WINDOW
#include "common/NativeWindow.h"
#include "common/GraphicsContext.h"
#endif


class sampleApplication
{
public:
    sampleApplication():is_done(false){}
    virtual ~sampleApplication(){}

    //最开始的消息 基本上只有溅射画面可以用这个消息 
    virtual void onStart(){}

    //建立场景
    virtual void onCreateScene(osgViewer::Viewer* viewer,osg::Group* root,float x,float y,float w,float h){}

    //启动主循环之前最后一个消息
    virtual void onStartLoop(osgViewer::Viewer* viewer,osg::Group* root){}

    //循环
    virtual void onPreFrame(osgViewer::Viewer* viewer,double dt){}
    virtual void onPostFrame(osgViewer::Viewer* viewer,double dt){}

    //销毁
    virtual void onDestroyScene(osgViewer::Viewer* viewer,osg::Group* root){}

    void setDone(bool b){is_done = b;}
    bool isDone(){return is_done;}

private:
    bool is_done;
};


#if USE_NIUBI_WINDOW

osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
NiuBi::NativeWindow* g_window = 0;
NiuBi::GraphicsContext* g_gc = 0;

class launcher_callback
    :public NiuBi::NativeWindow::EventCallback
{
public:

    launcher_callback(){}
    virtual ~launcher_callback(){}

    virtual void onKey(int keycode ,int keystate ,int x,int y)
    {
        //printf("%s\n",__FUNCTION__);
        //::onKey( keycode , keystate , x, y);

        if( keystate == 0 )
        {
            _gw->getEventQueue()->keyPress((osgGA::GUIEventAdapter::KeySymbol)keycode);
        }
        if( keystate == 1 )
        {
            _gw->getEventQueue()->keyRelease((osgGA::GUIEventAdapter::KeySymbol)keycode);
        }

    }
    virtual void onMouse(int moustbutton ,int moustbuttonstate ,int x,int y)
    {
        //printf("mouse %d %d %d %d\n", moustbutton , moustbuttonstate , x , y );
        //printf("%s\n",__FUNCTION__);
        //::onMouse( moustbutton , moustbuttonstate , x, y);

        if( moustbuttonstate == 4 ) //move
        {
            _gw->getEventQueue()->mouseMotion(x,y);
        }
        else
        {
            int btn = moustbutton;
            if( btn == 2 ) btn = 3;
            else if( btn == 3 ) btn = 2;
            if( moustbuttonstate == 0 )
            {
                _gw->getEventQueue()->mouseButtonRelease(x,y,btn);
            }
            if( moustbuttonstate == 1 )
            {
                _gw->getEventQueue()->mouseButtonPress(x,y,btn);
            }
        }
    }
    virtual void onSize(int w,int h)
    {
        //printf("%s %d %d\n",__FUNCTION__,w,h);
        if(_gw.valid())
        {
            POINT origin;
            origin.x = 0;
            origin.y = 0;
            ::ClientToScreen( (HWND)g_window->m_winID , &origin);
            _gw->resized(origin.x,origin.y,w,h);
            _gw->getEventQueue()->windowResize(origin.x,origin.y, w, h );
            //printf("%s %d %d %d %d\n",__FUNCTION__,origin.x,origin.y, w, h );
        }
    }
    virtual void onFrame(float dt)
    {
        //printf("%s\n",__FUNCTION__);
        //::onFrame( dt);
    }
    virtual void onDraw(float dt)
    {
        //printf("%s\n",__FUNCTION__);
        //::onDraw( dt);
    }
};


static void initWin32Window( osgViewer::Viewer* viewer ,int x,int y,int w,int h,bool full)
{
    //建立win32窗口 连接glrc
    g_window = new NiuBi::NativeWindow();
    g_gc  = new NiuBi::GraphicsContext();
    NiuBi::NativeWindow::EventCallback* mycb = new launcher_callback();
    g_window->setEventCallback(mycb);
    g_window->setGraphicsContext(g_gc);
    g_window->createWindow("NiuBi Test Window",w,h,32,full);

    //查询当前窗口坐标
    POINT origin;
    origin.x = 0;
    origin.y = 0;
    ::ClientToScreen( (HWND)g_window->m_winID , &origin);

    //初始化osgviewer
    _gw = new osgViewer::GraphicsWindowEmbedded(origin.x,origin.y,w,h);
    viewer->getCamera()->setGraphicsContext( _gw );

    //其他属性
    viewer->getCamera()->setViewport(new osg::Viewport(0,0,w,h));
    viewer->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(w)/static_cast<double>(h), 1.0f, 10000.0f); //near far no use
    viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
}

#endif


static int doApp_osgViewer(sampleApplication* app 
    , bool modify_light = true 
    , bool enable_camera_manipulator = true 
    , bool enable_event_handler = true )
{
    app->onStart();

    std::cout << "doApp_osgViewer()\n";

    //setup viewer
    osgViewer::Viewer viewer;
#if (!USE_NIUBI_WINDOW)  //是否启动默认OSG窗口
#if OSG_VERSION_LESS_THAN(3,1,0)
    viewer.setUpViewInWindow(100,100,1024,768); //这里在320版本时导致启动的RESIZE无法接收
#else
    viewer.realize();
#endif
#else
    int w = 1280;
    int h = 720;
    bool f = true;
    initWin32Window( &viewer ,-1,-1,w,h,f);
    g_window->m_event_callback->onSize(w,h);
#endif

    //create scene
    osg::Group* root = new osg::Group();
    viewer.setSceneData(root);
    //viewer.setLightingMode(osg::View::NO_LIGHT);
    viewer.getCamera()->setClearColor(osg::Vec4(0.2,0.2,0.2,1.0));
    //viewer.getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT);

#if 1
    viewer.getCamera()->setComputeNearFarMode(osg::Camera::DO_NOT_COMPUTE_NEAR_FAR);
    double fovy, aspectRatio, zNear, zFar;
    viewer.getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
    viewer.getCamera()->setProjectionMatrixAsPerspective(45.0, aspectRatio,0.1,2000.0);   //这里限定广角方式 增强场景的纵深感
#endif

    if(modify_light)
    {
        viewer.getLight()->setAmbient(osg::Vec4(1.0,1.0,1.0,1.0));
        viewer.getLight()->setDiffuse(osg::Vec4(1.0,1.0,1.0,1.0));
        viewer.getLight()->setSpecular(osg::Vec4(1.0,1.0,1.0,1.0));
    }

    osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>( viewer.getCamera()->getGraphicsContext() );
    if ( gw )
    {
        int x, y, w, h; 
        gw->getWindowRectangle( x, y, w, h );
        app->onCreateScene(&viewer,root,x, y, w, h);
    }


    // add the state manipulator
    if( enable_event_handler )
    {
        viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
        viewer.addEventHandler( new osgViewer::StatsHandler);
        viewer.addEventHandler( new osgViewer::WindowSizeHandler);
        viewer.addEventHandler( new osgViewer::ThreadingHandler);
    }
    else
    {
        viewer.setKeyEventSetsDone(0);//禁止esc
    }

    // set camera maniplator
    if( (!viewer.getCameraManipulator()) && enable_camera_manipulator )
    {
        osgGA::TrackballManipulator* tm = new osgGA::TrackballManipulator();
        viewer.setCameraManipulator(tm);
        tm->home(0);
    }

    // Send window size event for initializing
    //osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>( viewer.getCamera()->getGraphicsContext() );
    if ( gw )
    {
        int x, y, w, h; 
        gw->getWindowRectangle( x, y, w, h );
        viewer.getEventQueue()->windowResize( x, y, w, h );
    }

    //
    app->onStartLoop(&viewer,root);

    //run
    Timer t;
#if USE_NIUBI_WINDOW
    while( (!g_window->m_done) && (!viewer.done()) && (!app->isDone()) )
#else
    while( (!viewer.done()) && (!app->isDone()) )
#endif
    {
        //计算更新时间 ms
        double dt = t.GetCounter();
        t.StartCounter();

#if USE_NIUBI_WINDOW
        g_window->frame( float(dt)/1000.0f );//s
        //g_window->sleep(0);
#endif

        app->onPreFrame( &viewer, dt/1000.0 );
        viewer.frame();
        app->onPostFrame(&viewer, dt/1000.0 );

#if USE_NIUBI_WINDOW
        g_gc->swapBuffers();
#endif
    }

    //cleanup
#if USE_NIUBI_WINDOW
    _gw = 0;
#endif
    app->onDestroyScene(&viewer,root);
    return 0;
}

#endif //OSGXI_SAMPLE_APPLICATION_H

