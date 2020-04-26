/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtCg.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtPlatform.h"
#include "Crt/CrtScene.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtPhysics.h"

#ifdef _WIN32 
LARGE_INTEGER update_time, render_time;  // !!!GAC temporary WINDOWS ONLY for performance timing
#endif

#ifndef NO_BULLET

CrtBool CrtRender::UsingPhysics() 
{
	return (CrtBool) (Scene->m_physics != NULL);
};

CrtBool CrtRender::SetGravity(const CrtVec3f & vec)
{
#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

	if (Scene->m_physics)
	{
		Scene->m_physics->setGravity(vec);
	}
#endif
	return CrtFalse;
}

#endif //NO_BULLET

CrtBool CrtRender::InitWindow()
{

	return CrtTrue; 
}

CrtBool  CrtRender::InitRenderSettings()
{

	return CrtTrue; 
}

CrtVoid	CrtRender::SetInitialPath( const CrtChar * path )
{

	CrtInitPaths();

	CrtChar newPath[CRT_MAX_NAME_SIZE];

	CrtCpy( newPath, path );
	CrtCat( newPath, ShaderFilePrefix );
	CrtCpy( ShaderFilePrefix, newPath ); 

	CrtCpy( newPath, path );
	CrtCat( newPath, TextureFilePrefix );
	CrtCpy( TextureFilePrefix, newPath ); 

	CrtCpy(BasePath, path );
};

static CrtUInt CurrentCamNumber = 0;

void CrtRender::SetNextCamera()
{
	// Go to the next camera, if we run out of cameras go back to the first one
	CurrentCamNumber++;
	CrtUInt count = GetScene()->GetCameraInstanceCount();
	if(CurrentCamNumber >= count)
		CurrentCamNumber = 0;

	// Get the camera instance we want and set it as the active camera
	CrtInstanceCamera *inst = GetScene()->GetCameraInstance(CurrentCamNumber);
	assert(inst);  // Should never be null
	SetActiveInstanceCamera(inst);

	CrtPrint("Active camera instance %d on node %s is based on camera %s\n", 
				CurrentCamNumber, 
				inst->Parent->GetName(),
				inst->AbstractCamera->GetName());
}

CrtVoid CrtRender::UnLoad()
{
	if (Scene)
	{
		CrtDelete(Scene);
		Scene = 0;
		Reset();
		UsePhysics		= CrtTrue;
	}
}
CrtScene * CrtRender::Load( const CrtChar * fileName, const CrtChar * basePath )
{
	UnLoad();

	// Create a new scene and name it the same as the file being read.

	CrtScene * scene = CrtNew(CrtScene); 
	scene->SetName( fileName ); 
	scene->SetDocURI( fileName ); //TODO: what should we set the URI of this scene to?

	// set the base path if there is one 
	if ( basePath )
		SetInitialPath( basePath ); 		

	// in case of multithreaded loading 
	Loading = CrtTrue; 

	if ( UseVBOs && UseRender )
		CheckForVBOs();
	
	//CrtPrint(" Loading file %s \n", fileName); 
   	
	// Setup the type of renderer and initialize Cg if necessary (we may need the context for loading)
	// !!!GAC this code used to come after the load, but now the load needs a Cg context.
	if ( UseRender )
	{
		// try and initialize cg if we can as set the default shaders 
		if ( UseCg )	
			InitCg(); 

		if ( UseShadows )
			InitShadowMap();
	}
	else
	{
		UseCg = CrtFalse; 
		UseVBOs = CrtFalse; 
		UseShadows = CrtFalse; 		
	}

	// Load the COLLADA document
	if ( basePath )
	{	
		// If we've been supplied with a basePath to go with the file name, put the 
		// filename and base path together to make the name of the file to load.
		CrtChar newPath[CRT_MAX_NAME_SIZE];
		CrtCpy( newPath, BasePath );
		CrtCat( newPath, fileName );
		if( !scene->Load( newPath ))
		{
			CrtPrint(" Failed to read scene \n" ); 
			CrtDelete( scene ); 
			Loading = CrtFalse; 
			return NULL;
		}
	}
	else 
	{
		// If there's no base path, assume fileName is a full path and load it.
		if( !scene->Load( ( char *)fileName ) )
		{
			CrtPrint(" Failed to read scene \n" ); 
			CrtDelete( scene ); 
			return NULL;
		}
	}

	CrtPrint(" Done Loading %s \n", fileName); 
	// in case of multithreaded loading 
	Loading = CrtFalse; 

	_CrtRender.range_data.ResetZoom();
	return Scene; 	
}

CrtVoid CrtRender::Destroy()
{
	if (Scene) {
		Scene->Destroy();
		Scene = NULL;
	}
	if (CgInitialized)
		DestroyCg();
}

//----------------------------------------------------------------------------------------------------
CrtBool CrtRender::Render()
{
	if (Scene==NULL) return CrtFalse;

	// !!!GAC a timebase used by some effects (needs to be linked to real time)
	if ( !Scene->IsAnimationPaused() )
	{
		extern float ticker;
		ticker += 0.01f;
#ifndef NO_BULLET
#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

		if (UsePhysics)
		{
			if (Scene->m_physics)
				Scene->m_physics->Render(Delta);
		}
#endif
#endif //NO_BULLET
	}

	//printf(" Rendering Scene \n" ); 

	// Update the scene, this traverses all the nodes and makes sure the transforms are up-to-date
	// It also sets up the hardware lights from the light instances in the scene.  (note the hardware
	// lights aren't used if you're rendering using Cg shaders.
#ifdef _WIN32  // !!!GAC temporary windows only performance timing code
	LARGE_INTEGER temp_time;
	QueryPerformanceCounter(&temp_time);
#endif
	Scene->Update(); 
#ifdef _WIN32  // !!!GAC temporary windows only performance timing code
	QueryPerformanceCounter(&update_time);
	update_time.QuadPart = update_time.QuadPart - temp_time.QuadPart;
	QueryPerformanceCounter(&temp_time);
#endif

	CrtInstanceCamera *instanceCamera = _CrtRender.GetActiveInstanceCamera();
	if (instanceCamera)
	{
		CrtMatrix4x4Mult((float *)instanceCamera->transform, (float *)instanceCamera->Parent->GetLocalToWorldMatrix());
		CrtMatrixLoadIdentity( (float *)instanceCamera->Parent->GetInverseLocalToWorldMatrix() ); 
		CrtMatrixLoadIdentity( (float *)instanceCamera->Parent->GetInverseTransposeLocalToWorldMatrix() ); 
		CrtMatrix3x4Invert( (float *)instanceCamera->Parent->GetLocalToWorldMatrix(), (float *)instanceCamera->Parent->GetInverseLocalToWorldMatrix());
		CrtMatrixTranspose( (float *)instanceCamera->Parent->GetInverseLocalToWorldMatrix(), (float *)instanceCamera->Parent->GetInverseTransposeLocalToWorldMatrix() );
	}

	// Set the camera instance we will render from.  This actually pushes the camera transform from the 
	// scene-graph into GL.

	_CrtRender.SetInstanceCamera(_CrtRender.GetActiveInstanceCamera(), _CrtRender.GetScreenWidth(), _CrtRender.GetScreenHeight());


	// reset all lights
	for(CrtUInt i=0; i < 7; i++)
		glDisable(GL_LIGHT0+i);

	for(CrtUInt i=0; i < Scene->GetLightInstanceCount(); i++)
	{
		CrtInstanceLight *instanceLight = Scene->GetLightInstances(i);
		_CrtRender.SetInstanceLight(instanceLight,i);
		instanceLight++;
	}

	// Are we using shadow maps?
	// !!!GAC Shadow map code may not be functional right now
	if ( UsingShadowMaps() )
	{
		// Render the shadow pass
		SetupRenderingToShadowMap(); 
		Scene->Render(); 	
		
		// Now render the scene 
		SetupRenderingWithShadowMap(); 
		Scene->Render();
	}
	else
	{	
		// Do a normal rendering without shadow maps
		Scene->Render();
	}
#ifdef _WIN32  // !!!GAC temporary windows only performance timing code
	QueryPerformanceCounter(&render_time);
	render_time.QuadPart = render_time.QuadPart - temp_time.QuadPart;
#endif

	// put the background here:
//	_CrtRender.InitBackground();

	return CrtTrue; 
} 
//----------------------------------------------------------------------------------------------------
CrtVoid	CrtRender::UpdateDelta()
{
	// this function should be called once per frame 	
	static CrtFloat fps = 0; 
	static CrtFloat time = 0; 
	static CrtFloat oldTime = (CrtFloat)GetTime(); 
	static CrtInt   nbrFrames = 0; 
	static CrtBool  UpdatedOnce = CrtFalse; 
	const CrtFloat	updateTime = 1.0f; 

	time = (CrtFloat )GetTime();
	
	nbrFrames++;

	if ( time > (oldTime + updateTime) )
	{
		fps = (CrtFloat) nbrFrames; 	
		Delta = 1.0f/(fps) * updateTime;
//		if (Delta > 0.04f) Delta = 0.0f; // If Delta is running less than 15 fps, stop animation and wait till fps come back to above 15 fps
		
		oldTime = time; 
		nbrFrames = 0; 

		UpdatedOnce = CrtTrue; 
#if 0	
		CrtPrint(" FPS %f Time %f \n", fps, time );
#endif
#ifdef _WIN32  // !!!GAC temporary windows only performance timing code
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		//CrtPrint("update %f, render %f, frequency %ld\n", (double)update_time.QuadPart/(double)frequency.QuadPart, (double)render_time.QuadPart/(double)frequency.QuadPart, frequency.QuadPart);
#endif
	}
	
	// until first update delta = 0;
	if ( !UpdatedOnce )
		Delta = 0; 
};

	CrtCamera * CrtRender::GetCurrentCamera(int i ){ return Scene->Cameras[i]; }
	CrtUInt	CrtRender::GetNumCameras(){return (CrtUInt) Scene->Cameras.size();}
	CrtChar * CrtRender::GetCameraName( CrtInt camNum ){return Scene->Cameras[camNum]->GetName(); }
	CrtVoid	CrtRender::SetAnimationOn( CrtBool on ){ Scene->SetAnimationOn(on); }
	CrtVoid	CrtRender::SetAnimationPaused( CrtBool paused ){ Scene->SetAnimationPaused(paused); } 
	CrtNode * CrtRender::GetSceneNode( CrtChar * nodeName ) { return Scene->Nodes[nodeName]; }
	CrtNode *CrtRender::GetSceneRoot(){ return Scene->GetSceneRoot(); }
	CrtVoid		CrtRender::UpdateNodeTransforms(){ Scene->Update(); }
	CrtVoid	CrtRender::SetTime( CrtFloat t){ Scene->SetTime( t ); }

	// !!!GAC This is a temporary fix to be removed by the refactoring, the contents of InitMembers and Init
	// !!!GAC used to be all in one Init function that was called by the constructor AND by the mainlines
	// !!!GAC of most of the samples.  This was causing Init to be called twice.  This wasn't a problem till
	// !!!GAC we wanted to add Cg initialization which shouldn't be called in the global constructor and
	// !!!GAC shouldn't be called twice.  To fix the problem I split the init function so the constructor
	// !!!GAC now only initializes members and Init initializes members and CG, this avoids the need to
	// !!!GAC change every sample that calls Init.  I will clean this up later.

	CrtRender::CrtRender()
	{
		InitMembers(); 
	}
	CrtRender::~CrtRender()
	{
		if(Scene)
		{
			Scene->Destroy(); 
			CrtDelete(Scene);
		}
	}
	CrtVoid CrtRender::InitMembers()
	{
		UseCg			= CrtFalse;
		UseVBOs			= CrtFalse; 
		UseShadows		= CrtFalse; 
		UseNormalMaps	= CrtFalse; 
		UseRender		= CrtTrue; 
		UsePhysics		= CrtTrue;

		// shadow map default with and height 
		ShadowMapWidth	= 512;
		ShadowMapHeight = 512; 
		ShadowMapId		= 0; 

		CgInitialized			= CrtFalse; 
		VBOsAvailable			= CrtFalse; 
		ShadowMapInitialized	= CrtFalse; 
		NormalMapInitialized	= CrtFalse; 

		RenderToShadowMap			= CrtFalse; 
		RenderWithShadowMap			= CrtFalse; 

		CgSkinVertexProgramId		= -1;
		CgStaticVertexProgramId		= -1;
		CgPhongFragmentProgramId	= -1;

		CgSkinShadowProgramId = -1;
		CgStaticShadowProgramId = -1; 
		CgFragmentShadowPragramId = -1; 		
		
		CgVelvetFragmentProgramId = -1;
		CgSilkFragmentProgramId = -1;

		CgStaticNormalMapId = -1;
		CgSkin4mNormalMapId = -1; 
		CgFragmentNormalMapId = -1; 


		ScreenWidth		= 640;
		ScreenHeight	= 480; 
		Delta = .03f; 

		Loading = CrtTrue; 

        RenderDebug = CrtFalse; 

		UpAxis = eCrtYUp; 

		LoadAnimation	= CrtTrue; 
		LoadImages		= CrtTrue; 
		LoadGeometry	= CrtTrue; 
		ShowHiearchy	= CrtFalse; 
	}
	CrtVoid CrtRender::Init()
	{
		InitMembers();

		// !!!GAC The new crt render path requires Cg to be initialized before anything is loaded
		CgInitialized = CrtInitCg();

		CrtPrint("------------------------------\n"); 
		CrtPrint("--- COLLADA_RT Initialized ---\n"); 
		CrtPrint("------------------------------\n"); 
	}

	CrtVoid CrtRender::Reset()
	{
		CgInitialized = CrtExitCg();
		CgInitialized = CrtInitCg();
	}

	CrtVoid	CrtRender::ZoomIn(CrtFloat zoom)
	{
		GetActiveInstanceCamera()->ZoomTransform((float) (range_data.zoom * 0.2 * zoom));
	}
	void RangeData::SetRange(CrtVec3f ** points, CrtUInt32 point_counts)
	{
		if (SetFlag==CrtTrue) return;
		CrtVec3f * Points = *points;
		for(CrtUInt32 i=0; i<point_counts; i++)
		{
			CrtFloat x = Points[i].x;
			CrtFloat y = Points[i].y;
			CrtFloat z = Points[i].z;
			SetRange(x, y, z);
		}
		CrtFloat max_hight = max(max_y, - min_y);
		CrtFloat zoom_height = max_hight * (CrtFloat) 1.376;   // 1.376 = 1/tan(36);
		CrtFloat this_zoom = 3 * max(max_z, zoom_height);
		zoom = max(zoom, this_zoom);
#if 0
		printf("max_height=%f, zoom_height=%f, this_zoom=%f, zoom=%f\n", max_hight, zoom_height, this_zoom, zoom);
#endif
	};

	void RangeData::ResetZoom()
	{
		SetFlag = CrtFalse;
		zoom = 0.0;
		min_x = min_y = min_z = 0.0;
		max_x = max_y = max_z = 0.0; 
	}
	void RangeData::SetZoom()
	{
		if (SetFlag == CrtTrue) return;
		if (zoom!=0) SetFlag = CrtTrue;
		CrtInstanceCamera * instance_camera = _CrtRender.GetActiveInstanceCamera();
		if (instance_camera)
		{
			instance_camera->ZoomTransform(zoom);
			printf("zoom=%f\n", zoom);
		}
	}
