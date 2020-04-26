/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/

#define CFX_PLATFORM_INCLUDE <cfxOSX.h>

#include <stdio.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtScene.h"
#include "Crt/CrtRender.h"
#include "dae.h"
#include "dae/daeErrorHandler.h"

#include <cfxLoader.h>
#include <cfxEffect.h>
#include <cfxMaterial.h>
#include <cfxPlatform.h>  // !!!GAC just needed for testing


CGparameter amplitudeGlobalParameter = 0;  //!!!GAC for demo of how to hookup the UI to a param

static bool    fullscreen=false;
static bool    sWireframe=false;
static bool    sHierarchy=false;
static bool    sGlLighting=true;
static int     sCulling=0;
static bool    sAnimation = true;

static bool	CreateGLWindow(int LArgC, char** LArgV, CrtChar* title, CrtInt32 width, CrtInt32 height, bool fullscreenflag);
static GLvoid	DestroyGLWindow(GLvoid);							
static GLvoid	DrawGLScene(GLvoid);
static CrtInt32	InitGL(GLvoid);									
static GLvoid	ResizeGLScreen(GLsizei width, GLsizei height);		

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
#define     WALK_SPEED              100.0f

CrtFloat    MouseRotateSpeed = 0.75f;
CrtFloat    MouseWheelSpeed = 0.02f;
CrtFloat    MouseTranslateSpeed = 0.1f;
CrtFloat    KeyboardRotateSpeed = 10.0f;
CrtFloat    KeyboardTranslateSpeed = WALK_SPEED;
CrtRender   _CrtRender;   // Global to access the extra camera transform matrix in the CRT renderer

//----------------------------------------------------------------------------------------------------
// Function to adjust keyboard and mouse responsiveness by a common multiplier
//----------------------------------------------------------------------------------------------------
void AdjustUISpeed(CrtFloat multiplier)
{
	MouseRotateSpeed		*= multiplier;
	MouseWheelSpeed			*= multiplier;
	MouseTranslateSpeed		*= multiplier;
	KeyboardRotateSpeed		*= multiplier;
	KeyboardTranslateSpeed	*= multiplier;
}

//----------------------------------------------------------------------------------------------------
// The remainder of this code is windows specific, to see how the UI is implemented look for comments
// marked "UI".  To manipulate the camera you just use CrtMatrixTranslate and CrtMatrix4x4RotateAngleAxis
// to modify the transform matrix in _CrtRender.ExtraCameraTransform.  This transform is concatenated 
// with the camera right before rendering starts.  I recommend you always reset this matrix when 
// changing cameras to prevent users from getting lost.
//----------------------------------------------------------------------------------------------------
static int Xpos, Ypos, Xsize, Ysize;
static void specialKeyboardCallback(int key, int x, int y)
{
    (void)x;
    (void)y;

    switch (key) 
    {
    case GLUT_KEY_F1:
        fullscreen = !fullscreen;
        if (fullscreen) {
            Xpos = glutGet((GLenum)GLUT_WINDOW_X);    /* Save parameters */
            Ypos = glutGet((GLenum)GLUT_WINDOW_Y);
            Xsize = glutGet((GLenum)GLUT_WINDOW_WIDTH);
            Ysize = glutGet((GLenum)GLUT_WINDOW_HEIGHT);
            glutFullScreen();                /* Go to full screen */
        } else {
            glutReshapeWindow(Xsize, Ysize);        /* Restore us */
            glutPositionWindow(Xpos,Ypos);
            glutPostRedisplay();

        }
        break;
    case GLUT_KEY_LEFT :
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed, 0.0f);
            break;
    case GLUT_KEY_RIGHT :
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, - _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed, 0.0f);
            break;
    case GLUT_KEY_UP : 
        // UI code to move the camera farther up
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, 0.0f, _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed);
            break;
    case GLUT_KEY_DOWN :
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, 0.0f, - _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed);
            break;
    default:
            printf("unused (special) key : %i\n",  key );
            break;
    }
}

static void KeyboardCallback(const unsigned char key, const int x, const int y)	
{
    (void)x;
    (void)y;

    switch (key) 
    {
    case 9 :    /* TAB key */
            _CrtRender.SetNextCamera();
            break;
    case 'k' :
    case 'K' :		
			sHierarchy = !sHierarchy;
            if (!sHierarchy) {
                _CrtRender.SetShowHiearchy(CrtFalse);    
            } else {
                _CrtRender.SetShowHiearchy(CrtTrue);
            }
            break;
    case 'l' :
    case 'L' :
            sGlLighting = !sGlLighting;
            if (sGlLighting) {
                glEnable(GL_LIGHTING);
            } else {
                glDisable(GL_LIGHTING);
            }
            break;
    case 'm' :
    case 'M' :
            AdjustUISpeed(1.25f);
            break;
    case 'n' :
    case 'N' :
            AdjustUISpeed(0.75f);
            break;
    case 'p' :
    case 'P' :
            sAnimation = !sAnimation;
            if (!sAnimation) {
                _CrtRender.SetAnimationPaused( CrtTrue );
            }
            else { 
                _CrtRender.SetAnimationPaused( CrtFalse ); 
            }
            break;
    case 'q' :
    case 'Q' :
            sWireframe = !sWireframe;
            if (sWireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            break;
    case 's' :  //zoom in
    case 'S' :
            _CrtRender.ActiveInstanceCamera->MoveTransform(_CrtRender.GetAnimDelta() * KeyboardTranslateSpeed *0.5f, 0.0f, 0.0f);
            break;
    case 'w' :  // zoom out
    case 'W' :
            _CrtRender.ActiveInstanceCamera->MoveTransform(- _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed * 0.5f, 0.0f, 0.0f);
            break;
    case 32 :   // space key, UP
				// UI code to move the camera farther up
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, 0.0f, _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed);
            break;
    case 'x' :  // down
    case 'X' :
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, 0.0f, - _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed);
            break;
    case 'd' :  // right
    case 'D' :
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, - _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed, 0.0f);
            break;
    case 'a' :  // left
    case 'A' :
            _CrtRender.ActiveInstanceCamera->MoveTransform(0.0f, _CrtRender.GetAnimDelta() * KeyboardTranslateSpeed, 0.0f);
            break;
    case 'f' :
    case 'F' :
            if(sCulling == 0)
            { // turn it front
                glEnable( GL_CULL_FACE );
                glCullFace(GL_FRONT);
                sCulling = 1;
            } else if(sCulling == 1)
            { // turn it both
                glDisable( GL_CULL_FACE );
                sCulling = 2;
            } else 
            { // turn it back
                glEnable( GL_CULL_FACE );
                glCullFace(GL_BACK);
                sCulling = 0;
            }
            break;
    default:
        printf("unused key : %i\n",  key );
        break;
    }

}
static int sLeftBtnDown = false;
static int sRightBtnDown = false;
static int sMiddleBtnDown = false;

void MouseCallback(int button, int state, int x, int y)
{
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
             if (state == GLUT_DOWN) {
                _CrtRender.SetNextCamera();
             }
            break;
    case GLUT_LEFT_BUTTON:
            if (state == GLUT_UP)
                sLeftBtnDown = false;
            else if (state == GLUT_DOWN)
                sLeftBtnDown = true;
            break;
    case GLUT_RIGHT_BUTTON:
            if (state == GLUT_UP)
                sRightBtnDown = false;
            else if (state == GLUT_DOWN)
                sRightBtnDown = true;
            break;
    default:
            break;
    }
}

//----------------------------------------------------------------------------------------------------
static int lastx = 0;
static int lasty = 0;
static int lastLeft = 0;
static int lastRight = 0;
static int lastMiddle = 0;
void MouseMotionCallback(int x,int y)
{
            if(sLeftBtnDown)
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
                }
            }
            else
            {
                lastLeft = false;
            }

            if (sMiddleBtnDown)
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
                }
            }

            if(sRightBtnDown)
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
                }
            }
            else
            {
                lastRight = false;
            }

}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
int main(int LArgC, char** LArgV)
{
	// Set the default screen size
	_CrtRender.SetScreenWidth( 640);
	_CrtRender.SetScreenHeight( 480);

	// Create an OpenGL Window
	if (!CreateGLWindow(LArgC, LArgV, "COLLADA_RT, the COLLADA_DOM Sample Viewer for PC", _CrtRender.GetScreenWidth(), _CrtRender.GetScreenHeight(), fullscreen))
	{
		return 0;									
	}


	// Initialize the renderer
	// !!!GAC for compatibility with the new COLLADA_FX code, Init now forces UsingCg and UsingVBOs to
	// !!!GAC false.  It also calls CrtInitCg, creating the CG context and calling cgGLRegisterStates.
	// !!!GAC All these things are currently required for the cfx rendering path to work, changing them
	// !!!GAC may cause problems.  This is work in progress and will be much cleaner when the refactor is done.
	
	_CrtRender.Init();

	// !!!GAC kept for reference, changing these may cause problems with the cfx include path
	//_CrtRender.SetUsingCg( CrtFalse );
	
	_CrtRender.SetUsingVBOs( CrtTrue ); 
	_CrtRender.SetUsingNormalMaps( CrtTrue ); 	

	// Load the file name provided on the command line
	if(LArgC > 1 && LArgV[1])
	{
		printf("%s() %s\n", __FUNCTION__, LArgV[1]);fflush(stdout);
		if ( !_CrtRender.Load( LArgV[1] ))
		{
			exit(0);
		}
	}
	else
	{   // No command line arg, load default COLLADA doc
		if ( !_CrtRender.Load( "dominos.dae" ))
		{
			exit(0);
		}
	}

	// This block of code shows how to enumerate all the effects, get their parameters and then
	// get their UI information.
#if 0
	if(_CrtRender.GetScene())
	{
		// Get the scene and setup to iterate over all the effects stored in the cfxLoader
		CrtScene *scene = _CrtRender.GetScene();


		std::map<std::string, cfxEffect*>::iterator effectIterator;
		effectIterator = scene->cfxEffects.begin();
		// Iterate over all the effects
		while(effectIterator != scene->cfxEffects.end())
		{
			// This is the effect name you would use in a UI
//			CrtPrint("Effect name %s\n", effectIterator->first.c_str());
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

	_CrtRender.SetAnimationOn( CrtTrue ); 

	glutMainLoop();

	_CrtRender.Destroy();

	// Shutdown
	DestroyGLWindow();								

	return(0);						
}

//----------------------------------------------------------------------------------------------------
// GL Setup 
//----------------------------------------------------------------------------------------------------
CrtInt32 InitGL(GLvoid)
{
	glEnable(GL_TEXTURE_2D);						
	glShadeModel(GL_SMOOTH);						
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glEnable(GL_LIGHT0);								
	glEnable(GL_LIGHTING);

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK ); 

	return true;										
}

//----------------------------------------------------------------------------------------------------
// Resize And Initialize The GL Window
//----------------------------------------------------------------------------------------------------
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
// Main Render 
GLvoid DrawGLScene(GLvoid)									
{
	// Clear The Screen And The Depth Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();									
	
	if(_CrtRender.GetScene()) _CrtRender.Render(); 
	glutSwapBuffers();
}


//----------------------------------------------------------------
// Create Window based on the Width and Height parameters 
//----------------------------------------------------------------
static bool CreateGLWindow(int LArgC, char** LArgV, CrtChar* title, CrtInt32 width, CrtInt32 height, bool fullscreenflag)
{
	fullscreen = fullscreenflag;			

	glutInit(&LArgC, LArgV);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow(title);

    InitGL();

    glutIdleFunc(DrawGLScene);      // Do rendering when there is no events to be processed by glut
	glutDisplayFunc(DrawGLScene);

	glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(specialKeyboardCallback);
    glutMotionFunc(MouseMotionCallback);    // Handles mouse movement while one or more mouse buttons are pressed
    glutMouseFunc(MouseCallback);           // Handles mouse clicks and its release/press state
	glutReshapeFunc(ResizeGLScreen);

	glutReshapeWindow(width, height);
	ResizeGLScreen(width, height);					

	return true;									
}

//----------------------------------------------------------------------------------------------------
GLvoid DestroyGLWindow(GLvoid)							
{
}

