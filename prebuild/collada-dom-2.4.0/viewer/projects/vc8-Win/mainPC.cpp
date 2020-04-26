/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// #define HEAP_DEBUG   // Define this to turn on windows heap debugging

#include <windows.h>
#include <zmouse.h>
#include <stdio.h>			
#include <gl\gl.h>			
#include <gl\glu.h>			

#include "stdafx.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtScene.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtWin.h"
#include "dae.h"
#include "dae/daeErrorHandler.h"
#include <stdio.h>
#include <time.h>
#include <cfxLoader.h>
#include <cfxEffect.h>
#include <cfxMaterial.h>
#include <cfxPlatform.h>  // !!!GAC just needed for testing

#include <IL/il.h>

CGparameter amplitudeGlobalParameter = 0;  //!!!GAC for demo of how to hookup the UI to a param

#if HEAP_DEBUG
#include <crtdbg.h>  // Only for heap debugging
#endif

HDC			hDC=NULL;	// GDI Device Context	
HGLRC		hRC=NULL;	// Rendering Context
HWND		hWnd=NULL;	// Holds Our Window Handle	
HINSTANCE	hInstance;	// Application Instance 	

// Window Active Flag Set To TRUE By Default
bool	active=TRUE;		
bool	fullscreen=TRUE;
bool    togglewireframe=TRUE;
bool    togglehiearchy=TRUE;
bool	togglelighting=TRUE;
int		togglecullingface=0;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	
BOOL CreateGLWindow(CrtChar* title, CrtInt32 width, CrtInt32 height, CrtInt32 bits, bool fullscreenflag);
GLvoid DestroyGLWindow(GLvoid);							
CrtInt32 DrawGLScene(GLvoid);									
CrtInt32 InitGL(GLvoid);									
GLvoid ResizeGLScreen(GLsizei width, GLsizei height);		

#ifndef CRTLIB_BUILD

class cfxDaeErrorHandler : public daeErrorHandler
{
public:
	cfxDaeErrorHandler()
	{
		setErrorHandler(this);
	}

	void handleError(daeString msg)
	{
		CrtPrint( "DOM Error: %s\n", msg );
	}

	void handleWarning(daeString msg)
	{
		CrtPrint( "DOM Warning: %s\n", msg );
	}
} gErrorHandler;



//----------------------------------------------------------------------------------------------------
// Between here and the next double line is non-windows specific UI code, you can cut/paste this into
// your own code to enable a similar keyboard/mouse based UI.
// NOTE: the original version of this code is in COLLADA_RT_VIEWER mainPC.cpp, if you make changes
// or fix bugs, please fix them there too.
//----------------------------------------------------------------------------------------------------
// Multipliers we use to scale the speed of the UI
CrtFloat	MouseRotateSpeed = 0.75f;
CrtFloat	MouseWheelSpeed = 0.02f;
CrtFloat	MouseTranslateSpeed = 0.1f;
CrtFloat	KeyboardRotateSpeed = 10.0f;
#define     RUN_SPEED				500.0f	
#define     WALK_SPEED				100.0f
CrtFloat	KeyboardTranslateSpeed = WALK_SPEED;
CrtRender   _CrtRender;   // Global to access the extra camera transform matrix in the CRT renderer
char * cleaned_file_name;
// Function to adjust all the UI speeds (keyboard and mouse) by a common multiplier
void AdjustUISpeed(CrtFloat multiplier)
{
	MouseRotateSpeed		*= multiplier;
	MouseWheelSpeed			*= multiplier;
	MouseTranslateSpeed		*= multiplier;
	KeyboardRotateSpeed		*= multiplier;
	KeyboardTranslateSpeed	*= multiplier;
}
// The camera selection mechanism is application defined so I put it here rather than in CrtRender
//----------------------------------------------------------------------------------------------------
// End of non-windows specific UI code
// The code below is somewhat windows specific but only because it uses windows keycodes in "keys"
//----------------------------------------------------------------------------------------------------
bool	keys[256];   // Used to track which keys are held down, the index is the windows
					 // keycode returned by wndProc in wndParm, true means the key is down
bool	sAnimationEnable = true;

// Call ProcessInput once per frame to process input keys
void ProcessInput( bool	keys[] )
{
	// These keys we don't want to auto-repeat, so we clear them in "keys" after handling them once
	if (keys['E'] && amplitudeGlobalParameter)
	{
		float value;
		cgGetParameterValuefc(amplitudeGlobalParameter, 1, &value);
		value += 0.1f;
		cgSetParameter1f(amplitudeGlobalParameter, value);
		keys['E'] = false;
	}
	if (keys['R'] && amplitudeGlobalParameter)
	{
		float value;
		cgGetParameterValuefc(amplitudeGlobalParameter,1, &value);
		value -= 0.1f;
		cgSetParameter1f(amplitudeGlobalParameter, value);
		keys['R'] = false;
	}
	if (keys[VK_TAB] )
	{
		// When 'C' is pressed, change cameras
		_CrtRender.SetNextCamera();
		keys[VK_TAB] = false;
	}

	if ( keys['M'] )
	{
		// Speed up UI by 25%
		AdjustUISpeed(1.25f);  
		keys['M'] = false;
	}
	if ( keys['N'] )
	{
		// Slow down UI by 25%
		AdjustUISpeed(0.75f);  // Go 25% slower
		keys['N'] = false;
	}
	if (keys['Q'])
	{
		if (togglewireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			togglewireframe = FALSE;
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			togglewireframe = TRUE;
		}
		keys['Q'] = false;
	}

	if (keys['K'])
	{
		if (togglehiearchy) {
			_CrtRender.SetShowHiearchy(CrtTrue);	
			togglehiearchy = FALSE;
		} else {
			_CrtRender.SetShowHiearchy(CrtFalse);
			togglehiearchy = TRUE;
		}
		keys['K'] = false;
	}
	if (keys['L'])
	{
		if (togglelighting) {
			glDisable(GL_LIGHTING);
			togglelighting = FALSE;
		} else {
			glEnable(GL_LIGHTING);
			togglelighting = TRUE;
		}
		keys['L'] = false;
	}

	if (keys['P'] )
	{
		if (sAnimationEnable) {
			_CrtRender.SetAnimationPaused( CrtTrue );
			sAnimationEnable = false;
		}
		else { 
			_CrtRender.SetAnimationPaused( CrtFalse ); 
			sAnimationEnable = true;
		}
		keys['P'] = false;
	}
	if (keys[VK_F1])		
	{
		keys[VK_F1]=FALSE;		
		_CrtRender.Destroy();
		DestroyGLWindow();			
		fullscreen=!fullscreen;		
		// Recreate Our OpenGL Window
		if (!CreateGLWindow("Collada Viewer for PC", _CrtRender.GetScreenWidth(), _CrtRender.GetScreenHeight(),32,fullscreen))
		{
			exit(1);
		}
		if ( !_CrtRender.Load( cleaned_file_name ))
		{
			exit(0);
		}

		keys[VK_F1] = false;
	}
	
	// These keys that do a function as long as they are held down, so we don't clear "keys".
	// Remember to scale these functions by time!

	if (keys['S'])
	{
		// UI code to move the camera closer
		_CrtRender.ActiveInstanceCamera->MoveTransform(_CrtRender.GetAnimDelta() * KeyboardTranslateSpeed *0.5f, 0.0f, 0.0f);
	}

	if (keys['W'])
	{
		// UI code to move the camera farther away
		_CrtRender.ActiveInstanceCamera->MoveTransform(- _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed * 0.5f, 0.0f, 0.0f);
	}

	if (keys[VK_SPACE])
	{
		// UI code to move the camera farther up
		_CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, 0.0f, _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed);
	}

	if (keys['X'])
	{
		// UI code to move the camera farther down
		_CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, 0.0f, - _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed);
	}

	if (keys['D'])
	{
		// UI code to move the camera farther right
		_CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, - _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed, 0.0f);
	}

	if (keys['A'])
	{
		// UI code to move the camera farther left
		_CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed, 0.0f);
	}

	if (keys['F'])
	{
		if(togglecullingface == 0)
		{ // turn it front
			glEnable( GL_CULL_FACE );
			glCullFace(GL_FRONT);
			togglecullingface = 1;
		} else if(togglecullingface == 1)
		{ // turn it both
			glDisable( GL_CULL_FACE );
			togglecullingface = 2;
		} else 
		{ // turn it back
			glEnable( GL_CULL_FACE );
			glCullFace(GL_BACK);
			togglecullingface = 0;
		}
		keys['F'] = false;
	}
}
//----------------------------------------------------------------------------------------------------
// The remainder of this code is windows specific, to see how the UI is implemented look for comments
// marked "UI".
//----------------------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(	HWND	hWnd,			
							UINT	uMsg,			
							WPARAM	wParam,			
							LPARAM	lParam)			
{	

	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
			{
				active=TRUE;						
			}
			else
			{
				active=FALSE;						
			}

			return 0;								
		}

		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}

		case WM_KEYDOWN:						
		{
			// We only want to know which keys are down, so if this was an auto-repeat, ignore it
			if(!(HIWORD(lParam) & KF_REPEAT))
			{
				// Remember which keys are being held down
				keys[wParam] = TRUE;
			}
			return 0;							
		}

		case WM_KEYUP:							
		{
			keys[wParam] = FALSE;				
			return 0;							
		}

		case WM_SIZE:							
		{
			ResizeGLScreen(LOWORD(lParam),HIWORD(lParam));
			return 0;								
		}
		case WM_MOUSEWHEEL:
		{
			if (_CrtRender.ActiveInstanceCamera)
			{
				float gcWheelDelta = (short) HIWORD(wParam);
				_CrtRender.ZoomIn((CrtFloat) (-gcWheelDelta * MouseWheelSpeed));

				return 0;
			}
		}
		case WM_MBUTTONDOWN:
		{
			// Change camera
			_CrtRender.SetNextCamera();
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			// UI code to move camera in response to mouse movement.
			static float lastx = 0, lasty = 0;
			static int	 lastLeft = 0, lastRight = 0, lastMiddle = 0;
			// Retrieve mouse screen position and button state
			float x=(float) (short)LOWORD(lParam);
			float y=(float) (short)HIWORD(lParam);
//			bool controlButtonDown=((wParam & MK_CONTROL)  !=0);
			bool leftButtonDown=((wParam & MK_LBUTTON)  !=0);
			bool middleButtonDown=((wParam & MK_MBUTTON)  !=0);			
			bool rightButtonDown=((wParam & MK_RBUTTON) !=0);
			// Handle rotations if left button was pressed
			if(leftButtonDown)
			{
				if(lastLeft && _CrtRender.ActiveInstanceCamera)
				{
					_CrtRender.ActiveInstanceCamera->SetPanAndTilt((lastx - x) * MouseRotateSpeed, (lasty - y) * MouseRotateSpeed);
					lastx = x;
					lasty = y;
				}
				else
				{
					// Remember where the mouse was when it first went down.
					lastLeft = true;
					lastx = x;
					lasty = y;
					return 0;
				}
			}
			else
			{
				lastLeft = false;
			}
			if (middleButtonDown)
			{
				if(lastMiddle && _CrtRender.ActiveInstanceCamera)
				{
					_CrtRender.ActiveInstanceCamera->MoveOrbit((lastx - x) * MouseTranslateSpeed, - (lasty - y) * MouseTranslateSpeed);
					lastx = x;
					lasty = y;
				}
				else
				{
					// Remember where the mouse was when it first went down.
					lastMiddle = true;
					lastx = x;
					lasty = y;
					return 0;
				}
			}
			if(rightButtonDown)
			{
				// Was the mouse previously down?
				if(lastRight && _CrtRender.ActiveInstanceCamera)
				{
					_CrtRender.ActiveInstanceCamera->MoveOrbit((lastx - x) * MouseTranslateSpeed, - (lasty - y) * MouseTranslateSpeed);
					lastx = x;
					lasty = y;
				}
				else
				{
					// Remember that the button was down, and where it went down
					lastRight = true;
					lastx = x;
					lasty = y;
					return 0;
				}
			}
			else
			{
				lastRight = false;
			}
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}



//----------------------------------------------------------------------------------------------------
// Standard windows mainline, this is the program entry point
//
CrtInt32 WINAPI WinMain(	HINSTANCE	hInstance,		
					HINSTANCE	hPrevInstance,	
					LPSTR		lpCmdLine,			
					CrtInt32			nCmdShow)	
{
	(void)hPrevInstance; // Avoid warnings
	(void)nCmdShow; // Avoid warnings
	(void)hInstance; // Avoid warnings

#ifndef NO_DEVIL
	ilInit();
#endif

	MSG		msg;									
	BOOL	done=FALSE;								
	
	// Avoid warnings later
	msg.wParam = 0;

	// Turns on windows heap debugging
#if HEAP_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF /*| _CRTDBG_DELAY_FREE_MEM_DF*/);
#endif

	// Ask The User Which Screen Mode They Prefer
	//	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							
	}

	// Set the default screen size
	_CrtRender.SetScreenWidth( 640);
	_CrtRender.SetScreenHeight( 480);

	// Create an OpenGL Window
	if (!CreateGLWindow("Collada Viewer for PC", _CrtRender.GetScreenWidth(), _CrtRender.GetScreenHeight(),32,fullscreen))
	{
		return 0;									
	}
	
	// Turn data dumping (debug) off
	//CrtBool dumpData = CrtFalse; 

	// Initialize the renderer
	// !!!GAC for compatibility with the new COLLADA_FX code, Init now forces UsingCg and UsingVBOs to
	// !!!GAC false.  It also calls CrtInitCg, creating the CG context and calling cgGLRegisterStates.
	// !!!GAC All these things are currently required for the cfx rendering path to work, changing them
	// !!!GAC may cause problems.  This is work in progress and will be much cleaner when the refactor is done.
	
	_CrtRender.Init();
	//_CrtRender.SetRenderDebug( CrtTrue ); 

	// !!!GAC kept for reference, changing these may cause problems with the cfx include path
	//_CrtRender.SetUsingCg( CrtFalse );
	// Turn off VBOs (the GL skinning path doesn't work with VBOs yet)
	_CrtRender.SetUsingVBOs( CrtTrue ); 
	_CrtRender.SetUsingNormalMaps( CrtTrue ); 	
	//_CrtRender.SetRenderDebug( CrtTrue ); 
	//_CrtRender.SetUsingShadowMaps(CrtTrue);

	// We might get a windows-style path on the command line, this can mess up the DOM which expects
	// all paths to be URI's.  This block of code does some conversion to try and make the input
	// compliant without breaking the ability to accept a properly formatted URI.  Right now this only
	// displays the first filename
	char
		file[512],
		*in = lpCmdLine,
		*out = file;
	*out = NULL;
	// If the first character is a ", skip it (filenames with spaces in them are quoted)
	if(*in == '\"')
	{
		in++;
	}
	if(*(in+1) == ':')
	{
		// Second character is a :, assume we have a path with a drive letter and add a slash at the beginning
		*(out++) = '/';
	}
	int i;
	for(i =0; i<512; i++)
	{
		// If we hit a null or a quote, stop copying.  This will get just the first filename.
		if(*in == NULL || *in == '\"')
			break;
		// Copy while swapping backslashes for forward ones
		if(*in == '\\')
		{
			*out = '/';
		}
		else
		{
			*out = *in;
		}
		in++;
		out++;
	}
	
	// Should throw an error if i>= 512, but we don't have error dialongs in the code yet so just let it try to load and fail
	if(i < 511)
		*out = NULL;

	time_t seconds =  time (NULL);
	clock_t clocka = clock ();

	cleaned_file_name = file;
	// Load the file name provided on the command line
	if ( !_CrtRender.Load( cleaned_file_name ))
	{
		exit(0);
	}
	time_t loadtime = time (NULL) - seconds;
	int clockload = (int) clock () - clocka;

	CrtPrint("\nLOAD TIME OF %s\n", file);
	CrtPrint("IS %d SECONDS\n", loadtime);
	CrtPrint("IS %d CLOCK TICKS\n\n", clockload);


	// This block of code shows how to enumerate all the effects, get their parameters and then
	// get their UI information.
#if 1
	{
		// Get the scene and setup to iterate over all the effects stored in the cfxLoader
		CrtScene *scene = _CrtRender.GetScene();
		std::map<std::string, cfxEffect*>::iterator effectIterator;
		effectIterator = scene->cfxEffects.begin();
		// Iterate over all the effects
		while(effectIterator != scene->cfxEffects.end())
		{
			// This is the effect name you would use in a UI
			CrtPrint("Effect name %s\n", effectIterator->first.c_str());
			cfxEffect *thiscfxEffect = effectIterator->second;
			CGeffect thisCGEffect = thiscfxEffect->getEffect();
			CGparameter thisCGParameter = cgGetFirstEffectParameter(thisCGEffect);
			while(thisCGParameter != NULL)
			{
				// This is the parameter name you would use in the UI
				const char *parameterName = cgGetParameterName(thisCGParameter);
				// This is for the example of how to tweek a parameter (doesn't work yet)
				if(CrtCmp(parameterName, "Amplitude"))
				{
					// Capture the parameter and save it in a global, in a GUI you would
					// save this handle in the widget so it would know what to tweek.
					amplitudeGlobalParameter = thisCGParameter;
				}
#if 0
				// This is here for debugging, it iterates over all the annotations and prints them out
				// so you can see what's in them.  Normally this code will be turned off.
				CrtPrint("  Parameter name %s\n",parameterName);
				CGannotation dbgCGAnnotation = cgGetFirstParameterAnnotation(thisCGParameter);
				while(dbgCGAnnotation != NULL)
				{
					const char *annotationName = cgGetAnnotationName(dbgCGAnnotation);
					CrtPrint("      Annotation: %s",annotationName);
					if(cgGetAnnotationType(dbgCGAnnotation) == CG_STRING)
					{
						const char *annotationString = cgGetStringAnnotationValue(dbgCGAnnotation);
						CrtPrint(" value: %s\n",annotationString);
					}
					else if(cgGetAnnotationType(dbgCGAnnotation) == CG_FLOAT)
					{
						int nvalues; 
						const float *value = cgGetFloatAnnotationValues(dbgCGAnnotation, &nvalues);
						CrtPrint(" value: %f\n",*value);  // Assume there is one value
					}
					else
					{
						CrtPrint("\n");
					}
					dbgCGAnnotation = cgGetNextAnnotation(dbgCGAnnotation);
				}
#endif
				// This code looks at the parameter annotations to see if they specify some kind of UI
				// cgGetNamedParameterAnnotation isn't used for this because it is case sensitive and at
				// least some of the annotations FXcomposer uses for UI appear to NOT be case sensitive.
				// This method should collect the parameter values regardless of case, but it has to scan
				// ALL the parameters and do case-blind compares on each one, which is slower.
				// This code currently only collects the annotation values for defining sliders and color pickers.
				const char *UIName		= "unknown";
				const char *UIWidget	= "unknown";
				float UIMin				= -99999.0f;
				float UIMax				= 99999.0f;
				float UIStep			= 0.0f;
				int   nvalues;
				CGannotation thisCGAnnotation = cgGetFirstParameterAnnotation(thisCGParameter);
				// Iterate over all the annotations
				while(thisCGAnnotation != NULL)
				{
					// Get the name of this annotation
					const char *annotationName = cgGetAnnotationName(thisCGAnnotation);
					// Do case-blind compares to see if the annotation is one of the ones used to make UI
					// and save the value if it is.
					if(CrtICmp("UIWidget",annotationName))
					{
						// This is the widget type
						UIWidget = cgGetStringAnnotationValue(thisCGAnnotation);
					}
					if(CrtICmp("UIName",annotationName))
					{
						// This is the name to attach to the widget
						UIName = cgGetStringAnnotationValue(thisCGAnnotation);
					}
					if(CrtICmp("UIMin",annotationName))
					{
						// This is the minimum value for a slider widget
						const float *value = cgGetFloatAnnotationValues(thisCGAnnotation, &nvalues);
						if(nvalues == 1)
							UIMin = *value;
					}
					if(CrtICmp("UIMax",annotationName))
					{
						// This is the maximum value for a slider widget
						const float *value = cgGetFloatAnnotationValues(thisCGAnnotation, &nvalues);
						if(nvalues == 1)
							UIMax = *value;
					}
					if(CrtICmp("UIStep",annotationName))
					{
						// This is the step (minimum change) for a slider widget
						const float *value = cgGetFloatAnnotationValues(thisCGAnnotation, &nvalues);
						if(nvalues == 1)
							UIStep = *value;
					}
					// Get the next annotation
					thisCGAnnotation = cgGetNextAnnotation(thisCGAnnotation);
				}
				// Is the UIWidget a type that we recognize? (just slider and color picker for now)
				// Replace the CrtPrint with the code that generates the UI, remember the UI needs to
				// store thisCGParameter someplace so it can use it to change the parameter later. 
				if(CrtICmp("slider", UIWidget))
				{
					CrtPrint("Parameter %s needs a slider named %s going from %f to %f with step %f\n",parameterName,UIName,UIMin,UIMax, UIStep );
				}
				if(CrtICmp("color", UIWidget))
				{
					CrtPrint("Parameter %s needs a color picker named %s\n",parameterName,UIName);
				}
				// Move on to the next parameter
				thisCGParameter = cgGetNextParameter(thisCGParameter);
			}
			// Move on to the next effect
			effectIterator++;
		}
	}
#endif
	while(!done)									
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							
			}
			else									
			{
				SwapBuffers(hDC);					
				ProcessInput( keys ); 				
			}
		}
	}

	_CrtRender.Destroy();

	// Shutdown
#ifndef NO_DEVIL
	ilShutDown();
#endif
	DestroyGLWindow();								
	return (int)(msg.wParam);						
}
#endif //CRTLIB_BUILD

//----------------------------------------------------------------------------------------------------
// Resize And Initialize The GL Window
GLvoid ResizeGLScreen(GLsizei width, GLsizei height)		
{
	// Prevent A Divide By Zero By
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();
	
	// Reset the renderer's screen size to the new size
	_CrtRender.SetScreenWidth( width);
	_CrtRender.SetScreenHeight( height);
}

//----------------------------------------------------------------------------------------------------
// GL Setup 
CrtInt32 InitGL(GLvoid)									
{
	glEnable(GL_TEXTURE_2D);							
	glShadeModel(GL_SMOOTH);							
	glClearColor(.9f, 0.9f, .9f, 1.f);				
	glClearDepth(1.0f);									
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glEnable(GL_LIGHT0);								
	glEnable(GL_LIGHTING);

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK ); 

	return TRUE;										
}

//----------------------------------------------------------------------------------------------------
// Main Render 
CrtInt32 DrawGLScene(GLvoid)									
{

	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
	
	CrtMaterial mat; 

	mat.Ambient = CrtColor3f( 1,1,1 );
	mat.Diffuse = CrtColor3f( 1,1,1 ); 

	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,  (GLfloat *)&mat.Diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&mat.Ambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&mat.Specular );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat )mat.Shininess ); 

	_CrtRender.Render(); 
	return TRUE;										
}

//----------------------------------------------------------------------------------------------------
GLvoid DestroyGLWindow(GLvoid)							
{
	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);								
	}

	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}

	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										
	}

	if (!UnregisterClass("OpenGL",hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
}

//----------------------------------------------------------------------------------------------------
// Hear Creating the Window based on the Width and Height parameters 
 
BOOL CreateGLWindow(CrtChar* title, CrtInt32 width, CrtInt32 height, CrtInt32 bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);				
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;			
	wc.lpfnWndProc		= (WNDPROC) WndProc;
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								

	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	
	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","COLLADA_RT GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									
			}
		}
	}

	if (fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		

	// Create The Window
	
	hWnd=CreateWindowEx(	dwExStyle,							
							"OpenGL",							
							title,								
							dwStyle |							
							WS_CLIPSIBLINGS |					
							WS_CLIPCHILDREN,					
							0, 0,								
							WindowRect.right-WindowRect.left,	
							WindowRect.bottom-WindowRect.top,	
							NULL,								
							NULL,								
							hInstance,							
							NULL);
	if (!hWnd)								
	{
		DestroyGLWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,											
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |						
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,								
		(BYTE)bits,										
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};
	hDC=GetDC(hWnd);
	if (!hDC)							
	{
		DestroyGLWindow();							
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	
	PixelFormat=ChoosePixelFormat(hDC,&pfd);
	if (!PixelFormat)	
	{
		DestroyGLWindow();							
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		DestroyGLWindow();							
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	hRC=wglCreateContext(hDC);
	if (!hRC)				
	{
		DestroyGLWindow();							
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					
	{
		DestroyGLWindow();							
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ResizeGLScreen(width, height);					

	if (!InitGL())									
	{
		DestroyGLWindow();							
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;									
}
