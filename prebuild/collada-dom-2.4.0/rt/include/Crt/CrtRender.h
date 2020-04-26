/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_RENDER_H_
#define _CRT_RENDER_H_  

#include "Crt/CrtCamera.h"
#include "Crt/CrtMatrix.h"
//#include "Crt/CrtPhysics.h"


class MyColladaConverter;

class CrtLight;
class CrtScene;
class CrtMaterial;
class CrtImage;
class CrtInstanceLight;
class CrtPolyGroup;

class RangeData
{

public:
	RangeData() {	zoom = 0.0;
					min_x = min_y = min_z = 0.0;
					max_x = max_y = max_z = 0.0; 
					SetFlag = CrtFalse;};
	void SetRange(CrtFloat x, CrtFloat y, CrtFloat z)
	{
		if (x > max_x) max_x = x;
		if (y > max_y) max_y = y;
		if (z > max_z) max_z = z;
		if (x < min_x) min_x = x;
		if (y < min_y) min_y = y;
		if (z < min_z) min_z = z;
	};
	void SetZoom();
	void ResetZoom();
	void SetRange(CrtVec3f ** points, CrtUInt32 point_counts);
	CrtFloat max_x;
	CrtFloat max_y;
	CrtFloat max_z;
	CrtFloat min_x;
	CrtFloat min_y;
	CrtFloat min_z;
	CrtFloat zoom;
	CrtBool SetFlag; 
};

enum CrtUpAxis
{
	eCrtYUp,
	eCrtZUp,
	eCrtXUp // Unsupported 
};

class CrtRender
{
	CrtScene *Scene; 
	CrtInt32 ScreenWidth;
	CrtInt32 ScreenHeight;
	CrtFloat Delta; 
	
	CrtBool	UseCg; 
	CrtBool	UseVBOs; 
	CrtBool	UseShadows;
	CrtBool	UseNormalMaps; 
	CrtBool	UseRender; 
	CrtBool UsePhysics;

	CrtBool	RenderToShadowMap; 
	CrtBool	RenderWithShadowMap;

	CrtBool	CgInitialized;
	CrtBool	VBOsAvailable; 
	CrtBool ShadowMapInitialized; 
	CrtBool	NormalMapInitialized; 

	CrtInt	ShadowMapWidth;
	CrtInt	ShadowMapHeight; 
	CrtUInt	ShadowMapId; 

	CrtInt	CgSkinVertexProgramId;
	CrtInt	CgPhongFragmentProgramId;
	CrtInt	CgStaticVertexProgramId;
	CrtInt	CgPhongFragmentShadowProgramId;
	
	CrtInt	CgSkinShadowProgramId;
	CrtInt	CgStaticShadowProgramId; 
	CrtInt	CgFragmentShadowPragramId; 
    
	CrtInt	CgVelvetFragmentProgramId;
	CrtInt	CgSilkFragmentProgramId;
	CrtInt	CgShadowFragmentPragramId; 
	CrtInt	CgBlendingFragmentProgramId;
	
	CrtInt	CgStaticNormalMapId;
	CrtInt	CgSkin4mNormalMapId; 
	CrtInt	CgFragmentNormalMapId; 

	CrtInt	CgBumpVertexStaticId;
	CrtInt	CgBumpFragmentId; 
			
	CrtMaterial*	CurrentMaterial; 

	CrtBool		AnimationOn;
	CrtBool		AnimationPaused; 

	CrtBool 	Loading;
	CrtBool		RenderDebug; 
	CrtUpAxis	UpAxis; 

	CrtBool		LoadAnimation; 
	CrtBool		LoadImages; 
	CrtBool		LoadGeometry; 
	CrtBool		ShowHiearchy;

	CrtMatrix   CurrentLocalToWorldMatrix; 

public:
	CrtInstanceCamera	*ActiveInstanceCamera;  /**<< Pointer to the active camera instance in the scene */
	CrtInstanceCamera	*DefaultInstanceCamera;  /**<< Pointer to the default camera instance in the scene */

	CrtRender();

	CrtVoid Init();
	CrtVoid InitMembers();

	CrtVoid Reset();
	~CrtRender();


	CrtBool	InitWindow();
	CrtBool	InitRenderSettings();
	
	CrtScene * Load( const CrtChar * fileName, const CrtChar * pathName = NULL );
	CrtVoid UnLoad();

	CrtVoid	Destroy();

	CrtBool	InitCg(); 
	CrtBool DestroyCg(); 
	CrtInt	LoadCgProgram( CrtChar * fileName, CrtInt programType );

	CrtBool	SetupCgDefaultStaticProgram();
	CrtBool	SetupCgDefaultSkinProgram();
	CrtBool	SetupCgDefaultFragmentProgram();

	CrtBool	SetupCgShadowMapStaticProgram();
	CrtBool	SetupCgShadowMapSkinProgram();
	CrtBool	SetupCgShadowMapFragmentProgram();

	CrtBool	SetupCgNormalMapStaticProgram(); 
	CrtBool	SetupCgNormalMapSkinProgram();
	CrtBool	SetupCgNormalMapFragmentProgram(); 
	
	CrtBool SetCgDefaultStaticProgram();
	CrtBool SetCgDefaultSkinProgram();
	CrtBool	SetCgDefaultFragmentProgram(); 
	
	CrtBool	SetCgShadowMapStaticProgram();
	CrtBool	SetCgShadowMapSkinProgram();
	CrtBool	SetCgShadowMapFragmentProgram();

	CrtBool	SetCgNormalMapStaticProgram();
	CrtBool	SetCgNormalMapSkinProgram(); 
	CrtBool	SetCgNormalMapFragmentProgram(); 

	CrtBool	EnableCgProfiles(); 
	CrtBool DisableCgProfiles();

	CrtBool	CheckForVBOs();
	CrtBool	InitShadowMap(); 

	CrtBool Render();

	CrtVoid SetScene(CrtScene * c) {Scene=c;}; 

	/** Set the pointer to the active camera instance (where we will render from) */
	inline CrtVoid  SetActiveInstanceCamera(CrtInstanceCamera *inst){ActiveInstanceCamera = inst;}
	/** Get a pointer to the active camera instance (where we are rendering from) */
	inline CrtInstanceCamera* GetActiveInstanceCamera(){return ActiveInstanceCamera;}

	inline CrtVoid	SetScreenWidth(CrtInt w){ScreenWidth=w;}
	inline CrtVoid	SetScreenHeight(CrtInt h){ScreenHeight=h;}
	inline CrtVoid	SetAnimDelta(CrtFloat d){Delta=d;}
	CrtVoid	SetTime( CrtFloat t);

	CrtVoid   SetInstanceLight( CrtInstanceLight *lightInstance, CrtInt lightNumber );

	inline CrtVoid	SetCurrentMaterial( CrtMaterial * mtrl ){ CurrentMaterial = mtrl; }

    inline CrtInt	GetScreenWidth(){return ScreenWidth;}
	inline CrtInt	GetScreenHeight(){return ScreenHeight;}
	inline CrtFloat	GetAnimDelta(){return Delta;}
	CrtCamera * GetCurrentCamera(int i );
	CrtVoid SetNextCamera();	

	inline CrtMaterial *	GetCurrentMaterial(){ return CurrentMaterial; }


	CrtUInt	GetNumCameras();
	CrtChar * GetCameraName( CrtInt camNum );
	CrtVoid	SetAnimationOn( CrtBool on );
	CrtVoid	SetAnimationPaused( CrtBool paused ); 

	inline CrtBool	UsingCg(){	return CgInitialized; }
	inline CrtBool	UsingVBOs(){ return VBOsAvailable; }
    inline CrtBool	UsingShadowMaps() {return ShadowMapInitialized; }
	inline CrtBool	UsingNormalMaps() {return UseNormalMaps; }

	inline CrtUInt  GetShadowMapId() {return ShadowMapId; }
	inline CrtVoid	DisableCg(){CgInitialized = CrtFalse; } 
	
	inline CrtVoid	SetUsingCg( CrtBool b ){ UseCg = b; }  
	inline CrtVoid	SetUsingVBOs( CrtBool b ){ UseVBOs = b; }
	inline CrtVoid	SetUsingShadowMaps( CrtBool b ){ UseShadows = b; }
	inline CrtVoid	SetUsingNormalMaps( CrtBool b ){ UseNormalMaps = b; } 
	inline CrtVoid	SetUsingRenderer( CrtBool b ){ UseRender = b; }
	inline CrtVoid	SetUsePhysics( CrtBool b ){ UsePhysics = b; }
	inline CrtBool	GetUsePhysics(){ return UsePhysics; }
	
	inline CrtVoid	SetRenderingToShadowMap( CrtBool b ) { RenderToShadowMap = b; }
	inline CrtVoid	SetRenderingWithShadowMap( CrtBool b ) { RenderWithShadowMap = b; }

	inline CrtBool	GetRenderingToShadowMap() { return RenderToShadowMap; }
	inline CrtBool	GetRenderingWithShadowMap() { return RenderWithShadowMap; }

	inline CrtBool 	IsLoading() { return Loading; }
	CrtNode * GetSceneNode( CrtChar * nodeName ) ;
	inline CrtScene *GetScene(){ return Scene; } 
	CrtNode *GetSceneRoot();
	inline CrtVoid	SetRenderDebug( CrtBool b ){RenderDebug = b; }
	inline CrtBool	GetRenderDebug(){ return RenderDebug ; } 
	inline CrtVoid	SetUpAxis( CrtUpAxis up ){ UpAxis = up; }
	inline CrtUpAxis	GetUpAxis(){ return UpAxis; }  

	inline CrtVoid	SetLoadAnimation( CrtBool b ){ LoadAnimation = b; }; 
	inline CrtVoid	SetLoadImages( CrtBool b ){ LoadImages = b; }
	inline CrtVoid	SetLoadGeometry( CrtBool b ){ LoadGeometry = b; }
	inline CrtVoid  SetShowHiearchy( CrtBool b ){ ShowHiearchy = b; } 

	inline CrtBool	GetLoadAnimation() { return LoadAnimation; }; 
	inline CrtBool	GetLoadImages() { return LoadImages; }; 
	inline CrtBool	GetLoadGeometry() { return LoadGeometry; }; 
	inline CrtBool  GetShowHiearchy() { return ShowHiearchy; };

	inline CrtVoid SetCurrentLMMat( CrtMatrix mat ) { CrtMatrixCopy( mat, CurrentLocalToWorldMatrix ); }
	inline CrtMatrix * GetCurrentLMMat() { return & CurrentLocalToWorldMatrix; } 

	CrtVoid 	RenderPolyGroup( CrtPolyGroup * g ); 
	CrtVoid 	SetMaterial( CrtMaterial * mat ); 
	/** Set the camera instance we will be rendering from */
	CrtVoid 	SetInstanceCamera( CrtInstanceCamera * inst, CrtInt32 sWidth, CrtInt32 sHeight );

	CrtBool	LoadTexture( CrtImage * texObj, CrtChar * name ); 
	CrtBool	CreateTexture( CrtImage * texObj );
	CrtBool	SetTexture( CrtImage * texObj );
	CrtBool DeleteTexture( CrtImage * texImageObj );

	CrtVoid	LoadMatrix( float * mat );
	CrtVoid	MultMatrix( float * mat );
	CrtVoid	PushMatrix();
	CrtVoid	PopMatrix();

	CrtVoid	UpdateDelta(); 
	CrtFloat GetTime();

	CrtInt	GenerateVBO();
	CrtBool	CopyVBOData(CrtUInt type, CrtUInt vboID, CrtVoid * data, CrtInt size );
	CrtBool	BindVBO(CrtUInt type, CrtUInt vboUID );
	CrtVoid	FreeVBO( CrtUInt vboUID ); 

	CrtVoid	SetupRenderingToShadowMap();
	CrtVoid	SetupRenderingWithShadowMap();

	CrtVoid 	PushLightViewMatrix();
	CrtVoid 	PopLightViewMatrix();
	CrtVoid		UpdateNodeTransforms();

	CrtVoid		SetInitialPath( const CrtChar * );

//	CrtVec3f *  GetFusedGeometryPoints( CrtUInt & NumPoints ); 
	CrtVec3f *  GetFusedGeometryNormals( CrtUInt & NumNormals ); 
	CrtVec2f *  GetFusedGeometryUVs( CrtUInt & NumUVs ); 

	CrtVoid ZoomIn(CrtFloat zoom);
	RangeData range_data;

#ifndef NO_BULLET
	CrtBool SetGravity(const CrtVec3f & vec);
	CrtBool UsingPhysics();
#endif
};

extern CrtRender _CrtRender; 

#endif // _CRT_RENDER_

