
#ifndef _NIUBI_LAUNCHER_H_
#define _NIUBI_LAUNCHER_H_


/*

NIUBI_USE_OSGVIEWER  [default]  //使用 osg Viewer启动
NIUBI_USE_ANDROID               //使用NDK方式启动
NIUBI_USE_PNACL		            //使用PNACL方式启动

*/


//入口设置
#if NIUBI_USE_ANDROID
#define NIUBI_ENTRY_ANDORID_NDK_NATIVE
#else //NIUBI_USE_WINDOWS ...
#define NIUBI_SUBSYSTEM_CONSOLE
#define NIUBI_ENTRY_MAIN
#endif

#if (!NIUBI_USE_PNACL) && (!NIUBI_USE_PPAPI)
#include "entry.h"
#endif


// #if NIUBI_USE_PPAPI
    // #  if defined( NIUBI_LIBRARY )
    // #    define NIUBI_EXPORT   __declspec(dllexport)
    // #  else
    // #    define NIUBI_EXPORT   __declspec(dllimport)
    // #  endif
// #else
    #    define NIUBI_EXPORT
// #endif



//timer
#include "timer.h"


//////////////////////////////////////////////////////////////////////////
//全局对象和接口
//		任何程序必须包含一下对象和接口
//////////////////////////////////////////////////////////////////////////

//全局对象
//extern NiuBi::PluginsManager*	g_pm;
//extern NiuBi::NativeWindow*		g_window;
//extern NiuBi::GraphicsContext*	g_gc;
//extern NiuBi::Renderer*			g_render;

//全局消息
NIUBI_EXPORT void onCreate(osgViewer::Viewer* viewer, osg::Group* root, float x, float y, float w, float h);     //启动程序
//extern void onCreated();    //完成环境创建
NIUBI_EXPORT void onDestory(osgViewer::Viewer* viewer, osg::Group* root);    //准备销毁
NIUBI_EXPORT void onFrame(osgViewer::Viewer* viewer, double dt);  //动画更新
//extern void onDraw(float dt);   //绘制
NIUBI_EXPORT void onKey(osgViewer::Viewer* viewer,int keycode, int keystate, int x, int y); //key输入
NIUBI_EXPORT void onMouse(osgViewer::Viewer* viewer,int type ,int moustbutton, int moustbuttonstate, int x, int y); //鼠标输入 
NIUBI_EXPORT void onSize(osgViewer::Viewer* viewer,int w, int h); //resize
NIUBI_EXPORT void onMessage(osgViewer::Viewer* viewer,std::string str);

// type 1click 2move 3double 4WHEEL
// moustbutton 1L 2M 3R moustbuttonstate 0UP 1DOWN
// 


//class launcher_callback
//	:public NiuBi::NativeWindow::EventCallback
//{
//public:
//
//	launcher_callback(){}
//	virtual ~launcher_callback(){}
//
//	virtual void onKey(int keycode ,int keystate ,int x,int y)
//	{
//		printf("%s\n",__FUNCTION__);
//		::onKey( keycode , keystate , x, y);
//	}
//	virtual void onMouse(int moustbutton ,int moustbuttonstate ,int x,int y)
//	{
//		printf("%s\n",__FUNCTION__);
//		::onMouse( moustbutton , moustbuttonstate , x, y);
//	}
//	virtual void onSize(int w,int h)
//	{
//		//printf("%s %d %d\n",__FUNCTION__,w,h);
//		LOGW("onSize() %d %d",w,h);
//		::onSize( w, h);
//	}
//	virtual void onFrame(float dt)
//	{
//		//printf("%s\n",__FUNCTION__);
//		::onFrame( dt);
//	}
//	virtual void onDraw(float dt)
//	{
//		//printf("%s\n",__FUNCTION__);
//		::onDraw( dt);
//	}
//};


#if NIUBI_USE_ANDROID

	#include "launcher_android.inc"
	
#elif NIUBI_USE_PNACL

	#include "launcher_pnacl.inc"

#elif NIUBI_USE_PPAPI

	#include "launcher_ppapi.inc"
	
#else

	#include "launcher_osgviewer.inc"

#endif

#endif //_NIUBI_LAUNCHER_H_

