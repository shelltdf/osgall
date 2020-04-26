/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtNode.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtScene.h" 

#include "dae.h"
#include "dom/domConstants.h"
#include "dom/domCOLLADA.h"
#include <stdio.h>

#include "cfxLoader.h"
#include "cfxEffect.h"
#include "cfxSurface.h"
#include "cfxPlatform.h"
#include <set>
#include <map>


int triangulate(DAE * _dae);

// !!!GAC These are globals for to make initial refactoring easier, they should be part of the renderer or scene
extern		CGcontext	cgContext;
CrtImage	*defaultImage = 0; 

// skin controller is of the most complex of 
// the data loading types and need to have the temp 
// types to properly parse and resolve their data. 
const CrtUInt MAX_ARRAYS = 10;  


extern CrtChar BasePath[];

CrtVoid CrtScene::Destroy()
{
	// I know it appears I am only deleting one of the 
	// items in each list but the lists are self deleting
	// and they properly go and first delete there next 
	// if it is there in the destructor.  That way each 
	// list cleans up after itself. 
	
	// ** HUMM this used to work before I over rode new and delete
	// I will add to the CrtDelete a distructor call as well.  Will 
	// do soon.  Til then just goin got be bad and not worry about it
	// for now.  

	while(!cfxMaterials.empty())
	{
		std::map<std::string, cfxMaterial*>::iterator iter = cfxMaterials.begin();
		CrtDelete(iter->second);
		cfxMaterials.erase(iter);
	}
	cfxMaterials.clear();

	while(!cfxEffects.empty())
	{
		std::map<std::string, cfxEffect*>::iterator iter = cfxEffects.begin();
		CrtDelete(iter->second);
		cfxEffects.erase(iter);
	}
	cfxEffects.clear();

	while(!CameraInstances.empty())
	{
		CrtDelete(CameraInstances[0]);
		CameraInstances.erase(CameraInstances.begin());
	}
	CameraInstances.clear();
	while(!LightInstances.empty())
	{
		CrtDelete(LightInstances[0]);
		LightInstances.erase(LightInstances.begin());
	}
	while(!GeometryInstances.empty())
	{
		CrtDelete(GeometryInstances[0]);
		GeometryInstances.erase(GeometryInstances.begin());
	}
	while(!ControllerInstances.empty())
	{
		CrtDelete(ControllerInstances[0]);
		ControllerInstances.erase(ControllerInstances.begin());
	}

	while(!Lights.empty())
	{
		CrtDelete( Lights[0] ); 
		Lights.erase(Lights.begin());
	}
	while(!Cameras.empty())
	{
		CrtDelete( Cameras[0] ); 
		Cameras.erase(Cameras.begin());
	}
	Cameras.clear();
	while(!Materials.empty())
	{
		CrtDelete( Materials[0] );
		Materials.erase(Materials.begin());
	}
	while(!Images.empty())
	{
		CrtDelete( Images[0] ); 
		Images.erase(Images.begin());
	}
	Images.clear();
	while(!Textures.empty())
	{
		CrtDelete( Textures[0] );
		Textures.erase(Textures.begin());
	}
	while(!Effects.empty())
	{
		CrtDelete( Effects[0] );
		Effects.erase(Effects.begin());
	}
	while(!Shaders.empty())
	{
		CrtDelete( Shaders[0] );
		Shaders.erase(Shaders.begin());
	}
	while(!Controllers.empty())
	{
		CrtDelete( Controllers[0] );
		Controllers.erase(Controllers.begin());
	}
	while(!Animations.empty())
	{
		CrtDelete( Animations[0] ); 
		Animations.erase(Animations.begin());
	}
	while(!Geometries.empty())
	{
        while (!Geometries[0]->Groups.empty())

        {

            CrtDelete(Geometries[0]->Groups[0]);

            Geometries[0]->Groups.erase(Geometries[0]->Groups.begin());

        }
		CrtDelete( Geometries[0] ); 
		Geometries.erase(Geometries.begin());
	}
	if (SceneRoot)
		CrtDelete( SceneRoot );

	// at least need to delete the goemetry and any vbos if we are 
	// using them 
	DestroyGeos(); 
	if (m_collada) {
		delete m_collada;
		m_collada = 0;
	}
}

CrtBool	CrtScene::Load( CrtChar * LFileName )
{

	//if ( LFileName == NULL )
	//	return CrtFalse; 
	CrtChar * nameOnly = CrtGetAfterPath(LFileName);
	
	// Instantiate the reference implementation
	m_collada = new DAE;

	CrtPrint("COLLADA_DOM Load Started %s\n", LFileName); 
	// load with full path 
	CrtInt res = m_collada->load(LFileName);
	if (res != DAE_OK)
	{
		CrtPrint("Error loading the COLLADA file %s make sure it is COLLADA 1.4 or greater\n", LFileName );
		delete m_collada;	
		m_collada = 0;
		return CrtFalse; 
	}

	CrtPrint("COLLADA_DOM Runtime database initialized from %s.\n\n", LFileName);
	CrtPrint("nameOnly: %p\n\n", (void*)nameOnly);
	CrtPrint("nameOnly: %s\n\n", nameOnly);

	// !!!GAC The dom (beta 10 and up) now identifies a collection by a full URI rather than the base file name.
	// !!!GAC getDOM automatically resolves relative URIs before searching, "nameOnly" should probably be replaced
	// !!!GAC with LFileName if it's contents will always be a valid relative or absolute URI...need to check this out
	// !!!GAC make the change and retest.
	domCOLLADA *dom = m_collada->getDom(nameOnly);
	if ( !dom )
		dom = m_collada->getDom(LFileName); 
	if ( !dom )
	{
		CrtPrint("COLLADA File loaded to the dom, but query for the dom assets failed \n" );
		CrtPrint("COLLADA Load Aborted! \n" );
		delete m_collada;	
		m_collada = 0;
		return CrtFalse; 
	}

	CrtPrint("Begin Conditioning\n");
//	ret = kmzcleanup(collada, true);
//	if (ret) CrtPrint("kmzcleanup complete\n");

#ifdef SN_TARGET_PS3
	CrtInt ret = 0;
	ret = triangulate(m_collada);
#endif

//	if (ret) CrtPrint("triangulate complete\n");
//	ret = deindexer(collada);
//	if (ret) CrtPrint("deindexer complete\n");

	CrtPrint("Finish Conditioning\n");

	// Need to now get the asset tag which will determine what vector x y or z is up.  Typically y or z. 
	if ( dom->getAsset()->getUp_axis() )
	{
		domAsset::domUp_axis * up = dom->getAsset()->getUp_axis();
		switch( up->getValue() )
		{
			case UPAXISTYPE_X_UP:
				CrtPrint("	X Axis is Up axis! default camera is adjusted\n" ); 
				_CrtRender.SetUpAxis(eCrtXUp); 
				break; 
			case UPAXISTYPE_Y_UP:
				CrtPrint("	Y Axis is Up axis!n" ); 
				_CrtRender.SetUpAxis(eCrtYUp); 
				break;
			case UPAXISTYPE_Z_UP:
				CrtPrint("	Z Axis is Up axis! default camera is adjusted\n" ); 
				_CrtRender.SetUpAxis(eCrtZUp); 
				break; 
			default:
			
				break; 
		}
	}

	strcpy( FileName, LFileName ); 

	// going to search for the Controller and Image libs first and go ahead and load that 
	// it makes it a lot easier to go ahead and load that then come back and 
	// load the load the geometry because there everything will be around that is
	// needed for triangulating the geometry data.  
	
#if _WIN32
	// Load the image for the default texture.
	if ( !_CrtRender.GetLoadImages() )
		return CrtFalse; 
	CrtImage * newImage = CrtNew( CrtImage );
	newImage->SetName( ""); // !!!GAC will not giving it a name cause any lookup problems?
	newImage->SetFileName( "default.tga" );
	
	// Try to load the image, don't pass a scene name and LoadImageFile will look for it in images/name
	// !!!GAC the default image is currently in a global for easy refactoring, should probably be elsewhere (the scene?)
	if(newImage->LoadImageFile( "" ))
	{
		defaultImage = newImage;
	}
	else
	{
		defaultImage = 0;
	}
	cfxSurface::setDefaultTexture(newImage->GetId());
	// Don't really need to AddImage, but this makes sure it gets cleaned up on exit. 
	Images.push_back(newImage);
#endif
	// Load all the image libraries
	for ( CrtUInt i = 0; i < dom->getLibrary_images_array().getCount(); i++)
	{
		ReadImageLibrary( dom->getLibrary_images_array()[i] );			
	}

	// Load all the effect libraries
	//Check for a binary file
	CrtChar *cfxBinFilename = ReadCfxBinaryFilename( dom->getExtra_array() );
	CrtBool success = CrtFalse;
	if ( cfxBinFilename != NULL ) 
	{
		cfxLoader::setBinaryLoadRemotePath( BasePath );
		success = (CrtBool) cfxLoader::loadMaterialsAndEffectsFromBinFile(cfxBinFilename, cfxMaterials, cfxEffects, cgContext);
		assert(success);
	}
	else
	{
		success = (CrtBool) cfxLoader::loadMaterialsAndEffects(m_collada, cfxMaterials, cfxEffects, cgContext);
		assert(success);
	}

	for ( CrtUInt i = 0; i < dom->getLibrary_effects_array().getCount(); i++)
	{
		ReadEffectLibrary( dom->getLibrary_effects_array()[i] );			
	}

	// Load all the material libraries
	for ( CrtUInt i = 0; i < dom->getLibrary_materials_array().getCount(); i++)
	{
		ReadMaterialLibrary( dom->getLibrary_materials_array()[i] );			
	}

	// Load all the animation libraries
	for ( CrtUInt i = 0; i < dom->getLibrary_animations_array().getCount(); i++)
	{
		ReadAnimationLibrary( dom->getLibrary_animations_array()[i] );			
	}

	// Find the scene we want
	domCOLLADA::domSceneRef domScene = dom->getScene();
	daeElement* defaultScene = NULL;
	if (domScene)
		if (domScene->getInstance_visual_scene())
			if (domScene->getInstance_visual_scene())
				defaultScene = domScene->getInstance_visual_scene()->getUrl().getElement();
	if(defaultScene)
		ReadScene( (domVisual_scene *)defaultScene );

	// If no lights were loaded, we need to make one so the scene won't be totally black
	if(LightInstances.empty() && SceneRoot)
	{
		// new CrtLight
		CrtPrint("CrtScene, no lights were loaded so I'm creating a default light\n");
		CrtLight * default_light	= CrtNew( CrtLight);
		default_light->SetName( "no_light_in_scene_default_light");
		default_light->SetDocURI( dom->getDocumentURI()->getURI() );
		default_light->SetType(eLIGHTTYPE_DIRECTIONAL);
		default_light->Color		= CrtColor4f( 1,1,1,1 );
		Lights.push_back(default_light );

		// new CrtNode
		CrtPrint( "CrtScene, no instance_light found creating a node with an instance\n");
		CrtNode * lightNode = CrtNew( CrtNode ); 
		lightNode->SetName( "no_light_in_scene_default_node" ); 
		lightNode->SetParent( SceneRoot );	
		CrtTransform * transform = CrtNew( CrtTransform );
		transform->SetSid( "NONE"); 
		transform->SetType( eCrtTranslate ); 
		CrtVec4f trans(-40.0, 40.0, 0.0, 0.0);
		transform->SetTranslate( trans ); 
		lightNode->Transforms.push_back(transform);
		
		Nodes["no_light_in_scene_default_node"] = lightNode;
		SceneRoot->AddChild( lightNode ); 

		// new CrtInstanceLight
		CrtInstanceLight *instanceLight = CrtNew(CrtInstanceLight);
		instanceLight->AbstractLight = default_light;
		instanceLight->Parent			= lightNode;
//		crtNode->InstanceLights.push_back(instanceLight);
		LightInstances.push_back(instanceLight);
	}

	{
		CrtPrint( "CrtScene, always create a default camera and it is the first camera to use\n");
		// new CrtCamera
		default_camera = CrtNew( CrtCamera ); 
		default_camera->SetName( "no_camera_in_scene_default_camera" );	
		default_camera->SetDocURI( dom->getDocumentURI()->getURI() );
		default_camera->SetZNear(1.0);
		default_camera->SetZFar(10000.0);
		Cameras.push_back(default_camera);

		// new CrtNode
		CrtPrint( "CrtScene, no instance_camera found creating a node with an instance\n");
		CrtNode * camNode = CrtNew( CrtNode ); 

		CrtTransform * rotate_transform = CrtNew( CrtTransform );
		rotate_transform->SetSid( "NONE"); 
		rotate_transform->SetType( eCrtRotate );
		if (_CrtRender.GetUpAxis() == eCrtZUp)
			rotate_transform->SetRotate(CrtVec4f(1.0, 0.0, 0.0, 90.0));
		else if (_CrtRender.GetUpAxis() == eCrtXUp)
			rotate_transform->SetRotate(CrtVec4f(0.0, 0.0, 1.0, 90.0));

		camNode->Transforms.push_back(rotate_transform);

		camNode->SetName( "no_camera_in_scene_default_node" ); 
		camNode->SetParent( SceneRoot );	

		Nodes["no_camera_in_scene_default_node"] = camNode;
		SceneRoot->AddChild( camNode ); 

		// new CrtInstanceCamera
		CrtInstanceCamera *instanceCamera = CrtNew(CrtInstanceCamera);
		instanceCamera->AbstractCamera = default_camera;
		instanceCamera->Parent			= camNode;

		CameraInstances.push_back(instanceCamera);
		_CrtRender.SetActiveInstanceCamera(instanceCamera);
		_CrtRender.DefaultInstanceCamera = instanceCamera;
	}


	_CrtRender.SetScene(this);

	if (domScene)
	{
#ifndef NO_BULLET
#if defined SPU_BULLET || !defined (SN_TARGET_PS3)

		size_t count_ips = domScene->getInstance_physics_scene_array().getCount();
		size_t count_lvs = dom->getLibrary_visual_scenes_array().getCount();
		size_t count_lpm = dom->getLibrary_physics_models_array().getCount();
		if (count_ips + count_lvs + count_lpm > 2)
		{
			m_physics = new MyColladaConverter();

			bool result = m_physics->SetColladaDOM(m_collada, LFileName);
			if (result)
			{
				result = m_physics->convert();
			} else
			{
				_CrtRender.SetUsePhysics(CrtFalse);
			}
		} else {
			_CrtRender.SetUsePhysics(CrtFalse);
		}
#endif
#endif // NO_BULLET
	}

	return CrtTrue; 
} 

CrtVoid 	CrtScene::Update()
{
	static CrtFloat time = FirstKeyTime; 

	// if the currentTime is set update the 
	// time to that before updating the nodes 
	if ( CurrentTime != 0 )
		time = CurrentTime; 

	// too keep animation playback stabl
	// no matter the frame rate 
	_CrtRender.UpdateDelta(); 

	//CrtPrint("Updating Scene Time %f \n", time ); 
	//time = 1.0f; 

	if (SceneRoot)
		SceneRoot->Update( time ); 

	// might need to update instancecontrollers if there are animated weight on morph targets
	for (size_t i=0; i<Controllers.size(); i++)
		Controllers[i]->Update(time);

	// update animation if not paused 
	if ( !AnimationPaused )
	{
		time += _CrtRender.GetAnimDelta() * 2; 
	//	while	( time > LastKeyTime )
	//		time -= LastKeyTime; 
		if	( time > LastKeyTime )
			time = 0; 
	}

	// set not to use the animation if 
	// animation is turned off 
	if ( !AnimationOn )
		time = -1; 
	
}

CrtVoid 	CrtScene::Render()
{

	if ( SceneRoot )
	{
		SceneRoot->Render(); 
		_CrtRender.range_data.SetZoom();
		//CrtPrint("NumTris = %d \n" , gNumTris ); 
	}
	//else
	//	CrtPrint(" No Scene Data to render \n" ); 
}
/*
CrtNode * CrtScene::GetNode( CrtChar * nodeName )
{
	return SceneRoot->GetNode( nodeName ); 		
};

CrtNode * CrtScene::GetNode( CrtInt nodeId )
{
	return SceneRoot->GetNode( nodeId ); 
}
*/
CrtVoid	CrtScene::DestroyGeos()
{
	while (!Geometries.empty())
	{
		CrtGeometry * geos = Geometries.front();
		geos->Destroy(); 
		Geometries.erase(Geometries.begin());
	}
}
/*
const CrtChar CrtVersion[100] = "COLLADA_RT Binary Version 1.0";

CrtBool	CrtScene::Save( CrtChar * file )
{
	(CrtVoid)file; 
	// open the file for writting 
	CrtFile f; 
	f.OpenFile( file ); 

	// write out the number of each type
	f.Write( CrtVersion, 50 ); 
	f.Write( &NumGeos, 4 ); 
	f.Write( &NumLights, 4 ); 
	f.Write( &NumCameras, 4 );
	f.Write( &NumImages, 4 ); 
	f.Write( &NumTextures, 4 );
	f.Write( &NumMtrls, 4 );
	f.Write( &NumShaders, 4 ); 
	f.Write( &NumCtrls, 4 );
	f.Write( &NumAnims, 4 );
	
	// write data list recursively 
	if ( Geometries )	
		Geometries->Save( &f ); 
	if ( Lights )
		Lights->Save( &f ); 
	if ( Cameras )
		Cameras->Save( &f ); 
	if ( Materials ) 
		Materials->Save( &f ); 
	if ( Images )
        Images->Save( &f );  
	if ( Textures )
		Textures->Save( &f ); 
	if ( Shaders )
		Shaders->Save( &f ); 
	if ( Controllers )
		Controllers->Save( &f ); 
	if ( Animations ) 
		Animations->Save( &f );  
	if ( SceneRoot )
		SceneRoot->Save( &f ); 	
	
	return CrtTrue; 	
}; 
*/


//Triangulation
unsigned int getMaxOffset( domInputLocalOffset_Array &input_array )
{
	unsigned int maxOffset = 0;
	for ( unsigned int i = 0; i < input_array.getCount(); i++ ) {
		if ( input_array[i]->getOffset() > maxOffset ) {
			maxOffset = (unsigned int)input_array[i]->getOffset();
		}
	}
	return maxOffset;
}

CrtVoid createTrianglesFromPolylist( domMesh *thisMesh, domPolylist *thisPolylist )
{
	// Create a new <triangles> inside the mesh that has the same material as the <polylist>
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	//thisTriangles->setCount( 0 );
	unsigned int triangles = 0;
	thisTriangles->setMaterial(thisPolylist->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <_dae> and <parameters> as the old <polylist>
	for(int i=0; i<(int)(thisPolylist->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisPolylist->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the polygons array
	int numberOfInputs = (int)getMaxOffset(thisPolylist->getInput_array()) + 1;
	int numberOfPrimitives = (int)(thisPolylist->getVcount()->getValue().getCount());

	unsigned int offset = 0;

	// Triangulate all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{	
		int triangleCount = (int)thisPolylist->getVcount()->getValue()[j] -2;
		// Write out the primitives as triangles, just fan using the first element as the base
		int idx = numberOfInputs;
		for(int k = 0; k < triangleCount; k++)
		{
			// First vertex
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + l]);
			}
			// Second vertex
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
			}
			// Third vertex
			idx += numberOfInputs;
			for(int l = 0; l < numberOfInputs; l++)
			{
				p_triangles->getValue().append(thisPolylist->getP()->getValue()[offset + idx + l]);
			}
			//thisTriangles->setCount(thisTriangles->getCount()+1);
			triangles++;
		}
		offset += (unsigned int)thisPolylist->getVcount()->getValue()[j] * numberOfInputs;
	}
	thisTriangles->setCount( triangles );
}


CrtVoid createTrianglesFromPolygons( domMesh *thisMesh, domPolygons *thisPolygons )
{
	// Create a new <triangles> inside the mesh that has the same material as the <polygons>
	domTriangles *thisTriangles = (domTriangles *)thisMesh->createAndPlace("triangles");
	thisTriangles->setCount( 0 );
	thisTriangles->setMaterial(thisPolygons->getMaterial());
	domP* p_triangles = (domP*)thisTriangles->createAndPlace("p");

	// Give the new <triangles> the same <_dae> and <parameters> as the old <polygons>
	for(int i=0; i<(int)(thisPolygons->getInput_array().getCount()); i++)
	{
		thisTriangles->placeElement( thisPolygons->getInput_array()[i]->clone() );
	}

	// Get the number of inputs and primitives for the polygons array
	int numberOfInputs = (int)getMaxOffset(thisPolygons->getInput_array()) +1;
	int numberOfPrimitives = (int)(thisPolygons->getP_array().getCount());

	// Triangulate all the primitives, this generates all the triangles in a single <p> element
	for(int j = 0; j < numberOfPrimitives; j++)
	{
		// Check the polygons for consistancy (some exported files have had the wrong number of indices)
		domP * thisPrimitive = thisPolygons->getP_array()[j];
		int elementCount = (int)(thisPrimitive->getValue().getCount());
		if((elementCount%numberOfInputs) != 0)
		{
			//cerr<<"Primitive "<<j<<" has an element count "<<elementCount<<" not divisible by the number of inputs "<<numberOfInputs<<"\n";
		}
		else
		{
			int triangleCount = (elementCount/numberOfInputs)-2;
			// Write out the primitives as triangles, just fan using the first element as the base
			int idx = numberOfInputs;
			for(int k = 0; k < triangleCount; k++)
			{
				// First vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[l]);
				}
				// Second vertex
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
				}
				// Third vertex
				idx += numberOfInputs;
				for(int l = 0; l < numberOfInputs; l++)
				{
					p_triangles->getValue().append(thisPrimitive->getValue()[idx + l]);
				}
				thisTriangles->setCount(thisTriangles->getCount()+1);
			}
		}
	}
}

int triangulate(DAE * _dae)
{
	int error = 0;

	// How many geometry elements are there?
	int geometryElementCount = (int)(_dae->getDatabase()->getElementCount(NULL, "geometry" ));
//	if(verbose)	cerr<<"There are "<<geometryElementCount<<" geometry elements in this file\n"; 

	for(int currentGeometry = 0; currentGeometry < geometryElementCount; currentGeometry++)
	{
		// Find the next geometry element
		domGeometry *thisGeometry;
//		error = _dae->getDatabase()->getElement((daeElement**)&thisGeometry,currentGeometry, NULL, "geometry");
		daeElement * element = 0;
		error = _dae->getDatabase()->getElement(&element,currentGeometry, NULL, "geometry");
		thisGeometry = (domGeometry *) element;

		// Get the mesh out of the geometry
		domMesh *thisMesh = thisGeometry->getMesh();

		if (thisMesh == NULL)
			continue;

		// Loop over all the polygon elements
		for(int currentPolygons = 0; currentPolygons < (int)(thisMesh->getPolygons_array().getCount()); currentPolygons++)
		{
			// Get the polygons out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domPolygons *thisPolygons = thisMesh->getPolygons_array()[currentPolygons];  
			createTrianglesFromPolygons( thisMesh, thisPolygons );
			// Remove the polygons from the mesh
//			thisMesh->removeChildElement(thisPolygons);
		}
		while (thisMesh->getPolygons_array().getCount() > 0)
		{
			domPolygons *thisPolygons = thisMesh->getPolygons_array().get(0);  
			// Remove the polygons from the mesh
			thisMesh->removeChildElement(thisPolygons);
		}
		int polylistElementCount = (int)(thisMesh->getPolylist_array().getCount());
		//if(verbose)	cerr<<"There are "<<polylistElementCount<<" polylist elements in this file\n"; 
		for(int currentPolylist = 0; currentPolylist < polylistElementCount; currentPolylist++)
		{
			// Get the polylist out of the mesh
			// Always get index 0 because every pass through this loop deletes the <polygons> element as it finishes with it
			domPolylist *thisPolylist = thisMesh->getPolylist_array()[currentPolylist];  
			createTrianglesFromPolylist( thisMesh, thisPolylist );
			// Remove the polylist from the mesh
//			thisMesh->removeChildElement(thisPolylist);
		}
		while (thisMesh->getPolylist_array().getCount() > 0)
		{
			domPolylist *thisPolylist = thisMesh->getPolylist_array().get(0);  
			// Remove the polylist from the mesh
			thisMesh->removeChildElement(thisPolylist);
		}
	}
	return 0;
}


