/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/paths.h>

#include "Crt/CrtRender.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtPlatform.h"
#include "Crt/CrtMatrix.h"
#include "1.4/Crt/CrtScene.h"

#include <sys/process.h>
#include <sys/paths.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include "FWGLCamControlApplication.h"
#include "FWDisplayInfo.h"
#include <cell/sysmodule.h>
#include <cell/dbgfont.h>

#include "BrowserUI.h"


CrtRender _CrtRender; 

class COLLADA_Viewer : public FWGLCamControlApplication
{
public:
	COLLADA_Viewer();
	~COLLADA_Viewer();
	virtual bool	onInit(int argc, char **ppArgv);
	virtual void  onShutdown();
	virtual void	onRender();
	virtual bool  onUpdate();
	virtual void  ParseDir(char *path);
	virtual void  InitFS();
	virtual void  checkFS();
private:
	std::string resourceName;
	static void loadFunction();

#if defined SPU_BULLET || !defined (SN_TARGET_PS3)
	void SixAxis(float flX, float flY, float flZ, float flG);
#endif
	//pad data 
	FWInputDevice	*mpPad;
	FWInputFilter	*mpInputX0;
	FWInputFilter	*mpInputY0;
	FWInputFilter	*mpInputX1;
	FWInputFilter	*mpInputY1;

	// FPS REPORTING
	FWTimeVal		mLastTime;
	const static float REPORT_TIME;  
	int frames;
	int framesLastReport;
	double timeElapsed;
	double timeLastReport;
	double timeReport;

	CellDbgFontConsoleId mDbgFontID;
	bool			mRunning;
	GLuint m_vertex_buffers, m_color_buffers;

	const char * mCurrentFile;
	BrowserUI		Browser;
};

// instantiate the class
COLLADA_Viewer app;
bool togglewireframe = false;
bool togglelighting = true;
bool load_ok = false;
// FPS REPORTING
const float COLLADA_Viewer::REPORT_TIME = 5.0; 

COLLADA_Viewer::COLLADA_Viewer() 
{
	strcpy(mStartupInfo.mWindowTitle, "COLLADA Viewer");
	strcpy(mStartupInfo.mArgumentsFilename, "fwArgs.txt");

	mStartupInfo.mUseDebugConsole = false;

	// FPS REPORTING
	frames=0;
	framesLastReport=0;
	timeElapsed=0.0;
	timeLastReport=0.0;
	timeReport=REPORT_TIME;
	mRunning=true;
	mCurrentFile=0;
} 
COLLADA_Viewer::~COLLADA_Viewer() 
{

}

void  COLLADA_Viewer::ParseDir(char *path)
{
    DIR *  dir;
    struct dirent * dent;

	dir = opendir(path);
	if (dir == 0) {
	    printf("can't open %s\n", path);
		return;
	}

	char pathname[MAX_PATH];
	char t_pathname[MAX_PATH];
    while (1) {
        dent = readdir(dir);
		if (dent == 0)
			break;

		if (dent->d_type == DT_DIR)
		{
			if (strcmp(dent->d_name, ".")==0 || strcmp(dent->d_name, "..")==0)
				continue;
			sprintf(pathname, "%s/%s", path, dent->d_name);
			ParseDir(pathname);
		} else if (dent->d_type == DT_REG)
		{
			size_t filelen = strlen(dent->d_name);
			if ( ((dent->d_name[filelen-4] == '.')) &&
				 ((dent->d_name[filelen-3] == 'd') || (dent->d_name[filelen-3] == 'D')) &&
				 ((dent->d_name[filelen-2] == 'a') || (dent->d_name[filelen-2] == 'A')) &&
				 ((dent->d_name[filelen-1] == 'e') || (dent->d_name[filelen-1] == 'E'))
				 )
			{
				sprintf(pathname, "%s/%s", path, dent->d_name);
				printf("%s is a collada document\n", pathname);
				sprintf(t_pathname, "%s/%s.raw", path, dent->d_name);
				Browser.AddThumbnail(pathname, t_pathname);
			}
		}
    } 

    closedir(dir);
}

void COLLADA_Viewer::InitFS()
{
}

void COLLADA_Viewer::checkFS()
{

}
bool COLLADA_Viewer::onInit(int argc, char **ppArgv)
{
	FWGLApplication::onInit(argc, ppArgv);

    glClearColor(0.3f,0.3f,0.7f, 0.0f);
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);

	FWDebugFont::setColor(1.f, 1.f, 1.f, 1.f);  

	InitFS();
	Browser.Init();

	psglLoadShaderLibrary("/app_home/shaders.bin");

	// Initialize the renderer
	_CrtRender.Init();
	_CrtRender.SetUsingVBOs(CrtTrue);
	_CrtRender.SetShowHiearchy(CrtTrue);
	glEnable(GL_TEXTURE_2D);							
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 1.0f, 0.5f);				
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK ); 

    cgRTCgcInit();
	ParseDir("/app_home/");

  // Load the target file
	Browser.onUpdate();
	if (argc > 0)
	{
		char path_name[1024];
		mCurrentFile = ppArgv[0];
		sprintf(path_name, "%s%s", "/app_home/", ppArgv[0]);
		printf(path_name);
		load_ok  = _CrtRender.Load(path_name, NULL);
		if ( !load_ok )
   			return false;  
	}
	else if (Browser.GetSize() > 0)
	{
		mCurrentFile = Browser.GetThumbnail(0)->GetDocument();
		load_ok  = _CrtRender.Load(mCurrentFile, NULL);
		if ( !load_ok )
   			return false;  
	}
	// Get the gamepad so we can read it later 
	mpPad = FWInput::getDevice(FWInput::DeviceType_Pad,0);
	if(mpPad==NULL)
	{
	  printf("Error, couldn't get a pad\n");
	  exit(0);
	}

	mpInputX0 = mpPad->bindFilter();
	mpInputX0->setChannel(FWInput::Channel_XAxis_0);
	mpInputX0->setGain( 1.0f );
	mpInputX0->setDeadzone( 0.3f );

	mpInputY0 = mpPad->bindFilter();
	mpInputY0->setChannel(FWInput::Channel_YAxis_0);
	mpInputY0->setGain( 1.0f );
	mpInputY0->setDeadzone( 0.3f );

	mpInputX1 = mpPad->bindFilter();
	mpInputX1->setChannel(FWInput::Channel_XAxis_1);
	mpInputX1->setGain( 1.0f );
	mpInputX1->setDeadzone( 0.3f );

	mpInputY1 = mpPad->bindFilter();
	mpInputY1->setChannel(FWInput::Channel_YAxis_1);
	mpInputY1->setGain( 1.0f );
	mpInputY1->setDeadzone( 0.3f );


  	// initialize debug font library, then open console.
	int ret;
	CellDbgFontConfig cfg;
	memset(&cfg, 0, sizeof(CellDbgFontConfig));
	cfg.bufSize      = 512;
	cfg.screenWidth  = mDispInfo.mWidth;
	cfg.screenHeight = mDispInfo.mHeight;
	ret = cellDbgFontInit(&cfg);
	if (ret != CELL_OK) {
		printf("cellDbgFontInit() failed %x\n", ret);
		return true;
	}

	CellDbgFontConsoleConfig ccfg;
	memset(&ccfg, 0, sizeof(CellDbgFontConsoleConfig));
	ccfg.posLeft     = 0.1f;
	ccfg.posTop      = 0.6f;
	ccfg.cnsWidth    = 16;
	ccfg.cnsHeight   = 4;
	ccfg.scale       = 1.5f;
	ccfg.color       = 0xffffffff;
	mDbgFontID       = cellDbgFontConsoleOpen(&ccfg);
	if (mDbgFontID < 0) {
		printf("cellDbgFontConsoleOpen() failed %x\n", mDbgFontID);
		return true;
	}

	return true;
}


bool COLLADA_Viewer::onUpdate()
{
	float tolerrance = 0.15;
	static float buttonTime = 0;
	static float cameraTime = 0;
	bool result = FWGLApplication::onUpdate();

	if (mRunning==false)
	   return result;

	FWTimeVal curTime = FWTime::getCurrentTime();

	if ((float)curTime - buttonTime > 0.15)	// execute these control base on time elapse, not frame rate
	{
		buttonTime = curTime;
		if(mpPad->getRawBool(FWInput::Channel_Button_Select))
		{
		  _CrtRender.SetNextCamera();
		  return result;
		} else if(mpPad->getRawBool(FWInput::Channel_Button_L3))
		{
		   if (Browser.IsVisible())
			   Browser.SetVisible(false);
		   else
			   Browser.SetVisible(true);
		} else if(mpPad->getRawBool(FWInput::Channel_Button_R3))
		{
			if (togglewireframe) {
				togglewireframe = false;
			} else {
				togglewireframe = true;
			}
		} else if(mpPad->getRawBool(FWInput::Channel_Button_Square)) 
		{
			if (togglelighting) {
				togglelighting = false;
			} else {
				togglelighting = true;
			}
		} else if(mpPad->getRawBool(FWInput::Channel_Button_Cross)) 
		{
			mCurrentFile = Browser.GetThumbnail(Browser.GetSelection())->GetDocument();
			load_ok = _CrtRender.Load(mCurrentFile, NULL);
			return result;
		} else if(mpPad->getRawBool(FWInput::Channel_Button_Up))
		{
		   Browser.SelectPrev();
		} else if(mpPad->getRawBool(FWInput::Channel_Button_Down))
		{
		   Browser.SelectNext();
		}
	}

	if ((float)curTime - cameraTime > 0.05)	// execute these control base on time elapse, not frame rate
	{
		cameraTime = curTime;
			   // Get the values from the analog sticks
	   float conditioned_X_0 = mpPad->getRawFloat(FWInput::Channel_XAxis_0);
	   float conditioned_Y_0 = mpPad->getRawFloat(FWInput::Channel_YAxis_0);
	   float conditioned_X_1 = mpPad->getRawFloat(FWInput::Channel_XAxis_1);
	   float conditioned_Y_1 = mpPad->getRawFloat(FWInput::Channel_YAxis_1);

	   if (-tolerrance < conditioned_X_0 && conditioned_X_0 < tolerrance) conditioned_X_0 = 0.0f;
	   if (-tolerrance < conditioned_Y_0 && conditioned_Y_0 < tolerrance) conditioned_Y_0 = 0.0f;
	   if (-tolerrance < conditioned_X_1 && conditioned_X_1 < tolerrance) conditioned_X_1 = 0.0f;
	   if (-tolerrance < conditioned_Y_1 && conditioned_Y_1 < tolerrance) conditioned_Y_1 = 0.0f;

	   conditioned_X_0 = mpInputX0 ? -mpInputX0->getFloatValue() : 0.f;
	   conditioned_Y_0 = mpInputY0 ? -mpInputY0->getFloatValue() : 0.f;
	   conditioned_X_1 = mpInputX1 ? -mpInputX1->getFloatValue() : 0.f;
	   conditioned_Y_1 = mpInputY1 ? -mpInputY1->getFloatValue() : 0.f;

	   float multiplier = 10.0f;
	   if (conditioned_X_0 != 0.0f || conditioned_Y_0 != 0.0f)
	   {
		  _CrtRender.ActiveInstanceCamera->MoveOrbit(conditioned_X_0 * multiplier, conditioned_Y_0 * multiplier);
	   }
	   if (conditioned_X_1 != 0.0f || conditioned_Y_1 != 0.0f)
	   {
		  _CrtRender.ActiveInstanceCamera->SetPanAndTilt(conditioned_X_1 * multiplier, conditioned_Y_1 * multiplier);
	   }
	}
	
	if(mpPad->getRawBool(FWInput::Channel_Button_L2))
	{  // zoom in
	  _CrtRender.ZoomIn(-0.005f);
	} else if(mpPad->getRawBool(FWInput::Channel_Button_L1))
	{  // zoom out
	  _CrtRender.ZoomIn(0.005f);
   	}
	
	if (togglewireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (togglelighting) {
		glEnable(GL_LIGHTING);
	} else {
		glDisable(GL_LIGHTING);
	}


#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

   if (mpPad->getRawBool(FWInput::Channel_Button_Triangle))
   {

	   float fl_x, fl_y, fl_z, fl_g;
	   fl_x = mpPad->getRawFloat(FWInput::Channel_XAxis_2);
	   fl_y = mpPad->getRawFloat(FWInput::Channel_YAxis_2);
	   fl_z = mpPad->getRawFloat(FWInput::Channel_ZAxis_2);
	   fl_g = mpPad->getRawFloat(FWInput::Channel_Gyro);

	   SixAxis(fl_x, fl_y, fl_z, fl_g);
   } else {
	    _CrtRender.SetGravity(CrtVec3f(0.0f, -10.0f, 0.0f));
   }
#endif
	static const int CONS_PUTS_INTERVAL = 50;

	if (frames % CONS_PUTS_INTERVAL == 0){
	}

	char title[1024];
	const char * selectedfile = Browser.GetThumbnail(Browser.GetSelection())->GetDocument();
	sprintf(title, "%s %s", "COLLADA Viewer", mCurrentFile);
	cellDbgFontPuts(0.1f, 0.1f, 1.0f, 0xffffffff, title);
	cellDbgFontPuts(0.1f, 0.9f, 1.0f, 0xffffffff, selectedfile);

	Browser.onUpdate();
	return result; 
}

void COLLADA_Viewer::onRender()
{
	// base implementation clears screen and sets up camera

	FWGLApplication::onRender();

	if (!load_ok)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		Browser.onRender();
		cellDbgFontPuts(0.5f, 0.5f, 1.0f, 0xff00ffff, "Load Error!");
		cellDbgFontDraw();
		return;
	}

	if (mRunning==false)
		return;

	glPushMatrix();
	glEnable(GL_DEPTH_TEST);
	_CrtRender.Render();
	glPopMatrix();
    

	// FPS REPORTING
	glDisable(GL_VSYNC_SCE); 
	// get current timing info
	FWTimeVal	timeNow = FWTime::getCurrentTime();
	float fElapsedInFrame = (float)(timeNow - mLastTime);
	mLastTime = timeNow;
	++frames;
	timeElapsed+=fElapsedInFrame;
	// report fps at appropriate interval
	if (timeElapsed>=timeReport)
	{
		//printf("FPS: %.2f\n",(frames-framesLastReport)*1.f/(float)(timeElapsed-timeLastReport));
		timeReport+=REPORT_TIME;
		timeLastReport=timeElapsed;
		framesLastReport=frames;
	}


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	Browser.onRender();

	cellDbgFontDraw();
}

void COLLADA_Viewer::onShutdown()
{
	FWGLApplication::onShutdown();
	_CrtRender.Destroy();

	cellDbgFontConsoleClose(mDbgFontID);
	cellDbgFontExit();
}

#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

void COLLADA_Viewer::SixAxis(float flX, float flY, float flZ, float flG)
{
	(void) flG;
	if (_CrtRender.UsingPhysics())
	{
		static float dampedX = 0.0f;
		static float dampedY = 0.0f;
		static float dampedZ = 0.0f;

		dampedX=(flX + 10.f*dampedX)/11.f;
		dampedY=(flY + 10.f*dampedY)/11.f;
		dampedZ=(flZ + 10.f*dampedZ)/11.f;
		
		btTransform floorTrans;
		floorTrans.setIdentity();
		
		btVector3 xAxis(1,0,0);
		btVector3 zAxis(0,0,1);
		btQuaternion rotX(zAxis,dampedX*10.f);
		btQuaternion rotZ(xAxis,-dampedZ*10.f);
		
		btQuaternion combinedRot = rotZ*rotX;
		
		btMatrix3x3 orn;
		orn.setRotation(combinedRot);
		btVector3 up = orn.getColumn(1);

		CrtVec3f newG = CrtVec3f(up.getX(),up.getY(),up.getZ());
		_CrtRender.SetGravity(newG);
//	    printf("new gravity (%f, %f, %f)\n", newG.getX(), newG.getY(), newG.getZ());
	}
}
#endif