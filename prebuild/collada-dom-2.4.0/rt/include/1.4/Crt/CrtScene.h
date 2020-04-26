/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_SCENE_H_
#define _CRT_SCENE_H_  

#include "Crt/CrtAnimation.h"
#include "Crt/CrtCamera.h"
#include "Crt/CrtData.h"
#include "Crt/CrtEffect.h"
#include "Crt/CrtGeometry.h"
#include "Crt/CrtLight.h"
#include "Crt/CrtMaterial.h"
#include "Crt/CrtSkin.h"
#include "Crt/CrtTexture.h"
#include "Crt/CrtTypes.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtPhysics.h"
#include "Crt/CrtMorph.h"

// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_STREAM_DRAW                    0x88E0
#define GL_WRITE_ONLY                     0x88B9

enum CrtGeoType
{
	eCrtTriangles,
	eCrtPolygons,
	eCrtTristrips,
	eCrtTrifans,
	eCrtPolylist
};

// Forward declares for the DOM

class daeElement; 
class domLight;
class domCamera; 
class domMaterial; 
class domGeometry;
class domImage;
class domEffect; 
class domTexture; 
class domMorph; 
class domSkin; 
class domAnimation; 
class domController; 
class domScene;
class domNode; 
class domMesh;
class domTriangles;
class domTristrips;
class domTrifans;
class domLines;
class domLinestrips;
class domPolygons;
class domPolylist;
class domLibrary_lights;
class domLibrary_cameras;
class domLibrary_materials;
class domLibrary_geometries;
class domLibrary_images;
class domLibrary_animations;
class domLibrary_controllers;
class domLibrary_effects;
class domCOLLADA;
class domVisual_scene;
class domExtra;
class domProfile_COMMON;
class domCommon_newparam_type;
class domSource;
template<class T> class daeSmartRef;
typedef daeElement domElement; 
typedef class daeSmartRef<domLight> domLightRef;
typedef class daeSmartRef<domCamera> domCameraRef;
typedef class daeSmartRef<domMaterial> domMaterialRef;
typedef class daeSmartRef<domImage> domImageRef;
typedef class daeSmartRef<domEffect> domEffectRef;
typedef class daeSmartRef<domTexture> domTextureRef;
typedef class daeSmartRef<domMorph> domMorphRef;
typedef class daeSmartRef<domAnimation> domAnimationRef;
typedef class daeSmartRef<domSkin> domSkinRef;
typedef class daeSmartRef<domMorph> domMorphRef;
typedef class daeSmartRef<domController> domControllerRef;
typedef class daeSmartRef<domSource> domSourceRef;
typedef class daeSmartRef<domScene> domSceneRef;
typedef class daeSmartRef<domNode> domNodeRef;
typedef class daeSmartRef<domGeometry> domGeometryRef;
typedef class daeSmartRef<domLibrary_lights> domLibrary_lightsRef;
typedef class daeSmartRef<domLibrary_cameras> domLibrary_camerasRef;
typedef class daeSmartRef<domLibrary_effects> domLibrary_effectsRef;
typedef class daeSmartRef<domLibrary_materials> domLibrary_materialsRef;
typedef class daeSmartRef<domLibrary_geometries> domLibrary_geometriesRef;
typedef class daeSmartRef<domLibrary_images> domLibrary_imagesRef;
typedef class daeSmartRef<domLibrary_animations> domLibrary_animationsRef;
typedef class daeSmartRef<domLibrary_controllers> domLibrary_controllersRef;
typedef class daeSmartRef<domVisual_scene> domVisual_sceneRef;
typedef class daeSmartRef<domExtra> domExtraRef;
template<class T> class daeTArray; 
typedef class daeTArray<domNodeRef> domNode_Array;
typedef class daeTArray<domExtraRef> domExtra_Array;
class domInstance_geometry;
typedef daeSmartRef<domInstance_geometry> domInstance_geometryRef;
typedef daeTArray<domInstance_geometryRef> domInstance_geometry_Array;
class domInstance_light;
typedef daeSmartRef<domInstance_light> domInstance_lightRef;
typedef daeTArray<domInstance_lightRef> domInstance_light_Array;
class domInstance_material;
typedef daeSmartRef<domInstance_material> domInstance_materialRef;
typedef daeTArray<domInstance_materialRef> domInstance_material_Array;
class domInstance_camera;
typedef daeSmartRef<domInstance_camera> domInstance_cameraRef;
typedef daeTArray<domInstance_cameraRef> domInstance_camera_Array;
class domInstance_controller;
typedef daeSmartRef<domInstance_controller> domInstance_controllerRef;
typedef daeTArray<domInstance_controllerRef> domInstance_controller_Array;
class domInputLocalOffset;
typedef daeSmartRef<domInputLocalOffset> domInputLocalOffsetRef;
typedef daeTArray<domInputLocalOffsetRef> domInputLocalOffset_Array;
class domCommon_color_or_texture_type;
typedef daeSmartRef<domCommon_color_or_texture_type> domCommon_color_or_texture_typeRef;
class domCommon_float_or_param_type;
typedef daeSmartRef<domCommon_float_or_param_type> domCommon_float_or_param_typeRef;
class domSampler;
typedef daeSmartRef<domSampler> domSamplerRef;
class domChannel;
typedef daeSmartRef<domChannel> domChannelRef;
/**
 * The CrtScene class is the root of a scene that has been loaded from a collada file, all the parts
 * of the scene eventually link back here.
 */
class 	CrtScene : public CrtBase 
{
	CrtChar				FileName[CRT_MAX_NAME_SIZE];  /**< File where the scene was loaded from */
public:	
	std::map<std::string, cfxMaterial*>		cfxMaterials;			/**< Map of collada FX materials by ID */
	std::map<std::string, cfxEffect*>		cfxEffects;				/**< Map of collada FX effects by ID */
private:
	std::vector<CrtInstanceCamera *>		CameraInstances;
	std::vector<CrtInstanceLight *>			LightInstances;
	std::vector<CrtInstanceGeometry *>		GeometryInstances;
	std::vector<CrtInstanceController *>	ControllerInstances;

public:
	std::vector<CrtGeometry *>				Geometries;
	std::vector<CrtLight *>					Lights;
	std::vector<CrtCamera *>				Cameras;
	std::vector<CrtMaterial *>				Materials;
	std::vector<CrtImage *>					Images;
	std::vector<CrtTexture *>				Textures;
	std::vector<CrtShader *>				Shaders;
	std::vector<CrtController *>			Controllers;
	std::vector<CrtAnimation *>				Animations;
	std::vector<CrtEffect *>				Effects;
	std::map<std::string, CrtNode *>		Nodes;

	DAE *			m_collada;
	MyColladaConverter *	m_physics;
	CrtNode	*		SceneRoot;

	CrtInt			NumAnimatedNodes; 
	CrtCamera*		default_camera;
private:
	// for animation runtime only
	CrtFloat		LastKeyTime; 
	CrtFloat		FirstKeyTime; 
	CrtFloat		CurrentTime; 

	// stats 
	CrtInt			NumTris; 
	CrtInt			NumAnimChannelLerps;

	// Animation Controls 
	CrtBool 		AnimationOn; 
	CrtBool			AnimationPaused; 

	//TODO: we should remove these, since we should't read anything and that is references
	CrtBool	ReadMaterialLibrary( domLibrary_materialsRef lib ); 
	CrtBool	ReadImageLibrary( domLibrary_imagesRef lib );
	CrtBool	ReadEffectLibrary( domLibrary_effectsRef lib );
	CrtBool	ReadAnimationLibrary( domLibrary_animationsRef lib ); 

	// if object exist, return that object, else
	// return a pointer to that object if it read and add sucessfully, else 
	// return NULL if failed
	CrtLight		*ReadLight( domLightRef lib );
	CrtCamera		*ReadCamera( domCameraRef lib ); 
	CrtEffect		*ReadEffect( domEffectRef lib ); 
	CrtMaterial		*ReadMaterial( domMaterialRef lib ); 
	CrtVoid			ParseGeometry(CrtGeometry * newGeo, domGeometry * dom_geometry);
	CrtGeometry		*ReadGeometry( domGeometryRef lib);
	CrtImage		*ReadImage( domImageRef lib );
	CrtTexture		*ReadTexture( domTextureRef lib ); 
	CrtAnimation	*ReadAnimation( domAnimationRef lib ); 
	CrtSkin			*ReadSkin( domSkinRef lib ); 
	CrtNode			*ReadNode( domNodeRef node, CrtNode * parentNode );
	CrtMorph		*ReadMorph( domMorphRef lib );
	CrtController	*ReadController( domControllerRef lib );

	CrtAnimSrc		*ReadAnimationSource( domSourceRef source );
	CrtAnimSampler	*ReadAnimationSampler(CrtAnimation * animation, domSamplerRef sampler);
	CrtAnimChannel  *ReadAnimationChannel(CrtAnimation * animation, domChannelRef channel);

	CrtImage * GetTextureFromShader(map<string, domCommon_newparam_type*> &NewParams, domCommon_color_or_texture_type* shader);
//	void ReadPhong(CrtEffect * effect, domProfile_COMMON::domTechnique::domPhong *phong);
//	CrtFloat ReadFloatOrParamType(CrtEffect * effect, domCommon_float_or_param_typeRef float_or_param);
//	CrtColor3f ReadColorOrTextureType(CrtEffect * effect, domCommon_color_or_texture_typeRef color_or_texture);

	// TODO: add these ReadInstances later
	CrtInstanceLight *ReadInstanceLight( domInstance_lightRef lib);
	CrtInstanceCamera *ReadInstanceCamera( domInstance_cameraRef lib);
//	CrtInstanceEffect *ReadInstanceEffect( domInstance_effectRef lib);
	CrtInstanceMaterial *ReadInstanceMaterial( domInstance_materialRef lib);
	CrtInstanceGeometry *ReadInstanceGeometry( domInstance_geometryRef lib);
//	CrtInstanceImage *ReadInstanceImage( domInstance_imageRef lib);
//	CrtInstanceTexture *ReadInstanceTexture( domInstance_textureRef lib);
//	CrtInstanceAnimation *ReadInstanceAnimation( domInstance_animationRef lib);
	CrtInstanceController *ReadInstanceController( domInstance_controllerRef lib);

	CrtChar *ReadCfxBinaryFilename( domExtra_Array &extra_array );

//	CrtBool  BuildRenderData( CrtPolyGroup * group, domElement * polys, domMesh * mesh, CrtGeoType type, CrtController * ctrl );
	CrtPolygons * BuildPolygons(domPolygons * dom_polygons, CrtGeometry * geometry);
	CrtPolygons * BuildPolygons(domPolylist * dom_polylist, CrtGeometry * geometry);
	CrtTriangles * BuildTriangles(domTriangles * dom_triangles, CrtGeometry * geometry);
	CrtTriStrips * BuildTriStrips(domTristrips * dom_tristrips, CrtGeometry * geometry);
	CrtTriFans * BuildTriFans(domTrifans * dom_trifans, CrtGeometry * geometry);
	CrtLines * BuildLines(domLines * dom_lines, CrtGeometry * geometry);
	CrtLinestrips * BuildLineStrips(domLinestrips * dom_linestrips, CrtGeometry * geometry);
//	CrtInt BuildPrimitivesData(CrtPolyGroup * primitives, domInputLocalOffset_Array & inputs);


	CrtVoid	AddNode( CrtNode * ); 


	CrtBool	ReadScene( domVisual_sceneRef scene );
//	CrtBool	ReadNodes( domNode_Array array, CrtNode * parentNode );
	CrtBool ReadNodeTranforms(CrtNode * crtNode, domNodeRef node, CrtNode * parentNode );

//	CrtVoid	IndexNodes( CrtNode * node, CrtInt & idx ); 
	
public:
	CrtScene()
	{
		Init(); 
	}

	~CrtScene()
	{
		Destroy(); 
	}
	
	CrtVoid 	Update();
	CrtVoid 	Render(); 
	
	CrtVoid	Init()
	{
		FileName[0] = 0;	
		NumAnimatedNodes	= 0; 

		// for animation runtime only
		LastKeyTime = 0; 
		FirstKeyTime = 0; 
		CurrentTime = 0; 

		// stats 
		NumTris = 0; 
		NumAnimChannelLerps = 0;

		// Animation Controls 
		AnimationOn = CrtTrue; 
		AnimationPaused = CrtFalse; 
		SceneRoot = 0;
		m_collada = 0;

		m_physics = 0;
	}


	CrtVoid	Destroy();
	CrtVoid	DestroyGeos();
	
	//---------------------- External Interfaces ---------------------------------//	

	// HERE IS YOUR MAIN Entry point for getting at the collada data that was loaded 
	// and refactored to a more easily convertible data.
	// 1>First load a file that you have placed in a relative /ColladaExamples/
	// directory with just it's name like so Scene.Load("skin_anim_tri.xml") 
	// 2>Then get the Scene Root and got through the nodes pulling out what you want,
	// like the node heirarchy/geometry/cameras/ ect.
	// Look for the External Interfaces area for each type that you would like to get 
	// data from to see which members you should use and which members are completely
	// implemented yet.  There are notes like this one in each of the class you will 
	// be using to get the data.  Again this is a work in progress and not all of the 
	// collada data is ready in nice processed object, but most of the ones you need 
	// are there to get started for Node heirarcy, Camera( lookat), Lights, Geometry, 
	// and Materials.  Animation Weights and Animation Keys are currently loaded, but 
	// but the external object are not ready yet, but you will see the interface that 
	// you will use to get when they are there.  I suggest checking out CrtNode.h next and 
	// that External Interfaces section.

	inline CrtInstanceCamera *GetCameraInstance(CrtUInt num)
	{
		if(num < CameraInstances.size())
			return CameraInstances[num];
		else 
			return (NULL);
	}
	inline CrtUInt GetCameraInstanceCount(){return (CrtUInt) CameraInstances.size();}
	CrtBool	Load( CrtChar * file );
	inline  CrtInstanceLight * GetLightInstances(CrtUInt num)
	{
		if(num < LightInstances.size())
			return LightInstances[num];
		else 
			return (NULL);
	}
	inline	CrtUInt GetLightInstanceCount(){return (CrtUInt) LightInstances.size();}
	inline	CrtNode *GetSceneRoot(){ return SceneRoot; }; 
	inline  CrtFloat GetLastKeyTime() { return LastKeyTime; }

//	CrtVoid	IndexNodes();

	inline CrtCamera *GetCamera( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Cameras.size(); i++)
		{
			if ( CrtICmp(Cameras[i]->GetName(), name )  &&
				 CrtICmp(Cameras[i]->GetDocURI(), DocURI ) )
				return Cameras[i];
		}
		return NULL;
	}
	
	inline CrtGeometry *GetGeometry( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Geometries.size(); i++)
		{
			if ( CrtICmp(Geometries[i]->GetName(), name )  &&
				 CrtICmp(Geometries[i]->GetDocURI(), DocURI ) )
				return Geometries[i];
		}
		return NULL;
	}
	inline CrtLight *GetLight( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Lights.size(); i++)
		{
			if ( CrtICmp(Lights[i]->GetName(), name )  &&
				 CrtICmp(Lights[i]->GetDocURI(), DocURI ) )
				return Lights[i];
		}
		return NULL;
	}
	inline CrtEffect *GetEffect( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Effects.size(); i++)
		{
			if ( CrtICmp(Effects[i]->GetName(), name )  &&
				 CrtICmp(Effects[i]->GetDocURI(), DocURI ) )
				return Effects[i];
		}
		return NULL;
	}
	inline CrtMaterial *GetMaterial( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Materials.size(); i++)
		{
			if ( CrtICmp(Materials[i]->GetName(), name ) &&
				 CrtICmp(Materials[i]->GetDocURI(), DocURI ) )
				return Materials[i];
		}
		return NULL;
	}
	inline CrtImage *GetImage( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Images.size(); i++)
		{
			if ( CrtICmp(Images[i]->GetName(), name ) &&
				 CrtICmp(Images[i]->GetDocURI(), DocURI ) )
				return Images[i];
		}
		return NULL;
	}
	inline CrtController *GetController( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		for(CrtUInt i=0; i<Controllers.size(); i++)
		{
			if ( CrtICmp(Controllers[i]->GetName(), name ) &&
				CrtICmp(Controllers[i]->GetDocURI(), DocURI ) )
				return Controllers[i];
		}
		return NULL;
	}
	inline CrtNode *GetNode( const CrtChar * name, const CrtChar * DocURI)
	{
		if (name == NULL) return NULL;
		if (Nodes.empty()) return NULL;
		(void) DocURI;
//		for(CrtUInt i=0; i<Nodes.size(); i++)
//		{
//			if ( CrtICmp(Nodes[i]->GetName(), name ))
//				&& CrtICmp(Nodes[i]->GetDocURI(), DocURI ) )
//				return Nodes[i];
//		}
//		return NULL;
		return Nodes[name];
	}

/*	inline CrtVoid	ReduceSkinInfluences( CrtInt MaxInfluences )
	{
		for (unsigned int i=0; i<Geometries.size(); i++)
		{
			Geometries[i]->ReduceSkinInfluences(MaxInfluences);
		}
	}
*/
	inline CrtVoid	SetTime( CrtFloat t ){ CurrentTime = t; }
	inline CrtVoid	SetAnimationOn( CrtBool on ){ AnimationOn = on; }
	inline CrtVoid	SetAnimationPaused( CrtBool paused ){ AnimationPaused = paused; } 
	inline CrtBool	IsAnimationPaused(){ return AnimationPaused; } 

//	CrtNode * GetNode( CrtChar * nodeName ); 
//	CrtNode * GetNode( CrtInt nodeIdx ); 
//	CrtVoid	RemapWeights();// New Skins methods for 1.4

	//CrtBool	Save( CrtChar * file );
	//CrtBool	Load( CrtChar * file ); 

	//---------------------- End External Interfaces ---------------------------------//	
	

};	//CrtScene 


#endif // _CRT_SCENE_H_ 

