/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

/** 
 * The code in this file is mostly concerned with loading the various libraries from a COLLADA file into
 * RT's structures.  If you find parameters in COLLADA files that aren't compatible with your rendering
 * system, you should check and override those parameters as they are read in here so all the overrides
 * are in one place and easy to find.  See the camera library code for an example where zNear is overridden.
 */
#include "Crt/CrtScene.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtGeometry.h"
#include "Crt/CrtController.h"

#include <cfxEffect.h>
#include <cfxMaterial.h>
#include <cfxSurface.h>

// COLLADA_DOM includes 

#include "dae.h"
#include "dae/daeSIDResolver.h"
#include "dom/domCOLLADA.h"
#include "dom/domProfile_COMMON.h"
#include "dae/domAny.h"
#include "dom/domElements.h"

#include <stdio.h>
#include <map>
#include <string>
using namespace std;

// !!!GAC should not be a global, temporarily made it one during refactoring
extern CrtImage	*defaultImage; 

CrtLight *CrtScene::ReadLight( domLightRef lib )
{
	if (lib->getId()==NULL) return NULL;
	CrtLight * light = GetLight(lib->getID(), lib->getDocumentURI()->getURI());
	if (light)	// light is found
		return light;
	CrtPrint( "Add new light %s \n", lib->getId() );  

	domLight* LightElement = (domLight*)(domLight*)lib;

	if ( LightElement )
	{
		// Create the CrtLight
		CrtLight * newLight = CrtNew(CrtLight);
		CrtAssert("No memory\n", newLight != NULL);
		// Set its name to the light's id
		newLight->SetName( LightElement->getId() ); 
		newLight->SetDocURI( LightElement->getDocumentURI()->getURI() ); 
		// Get the common technique for this light
		domLight::domTechnique_common * commonTechnique	= LightElement->getTechnique_common();
		if (commonTechnique == NULL) 
		{
			CrtDelete(newLight);
			return NULL;
		}
		// Only one of these light types can be present
		domLight::domTechnique_common::domAmbient		*ambientElement		= commonTechnique->getAmbient();
		domLight::domTechnique_common::domDirectional	*directionalElement	= commonTechnique->getDirectional();
		domLight::domTechnique_common::domPoint			*pointElement		= commonTechnique->getPoint();
		domLight::domTechnique_common::domSpot			*spotElement		= commonTechnique->getSpot();
		if(ambientElement)
		{
			newLight->SetType(eLIGHTTYPE_AMBIENT);
			domFloat3 &color	= ambientElement->getColor()->getValue();
			newLight->Color		= CrtColor4f((float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0f);
		}
		else if(directionalElement)
		{
			newLight->SetType(eLIGHTTYPE_DIRECTIONAL);
			domFloat3 &color = directionalElement->getColor()->getValue();
			newLight->Color		= CrtColor4f((float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0f);
		}
		else if(pointElement)
		{
			newLight->SetType(eLIGHTTYPE_POINT);
			domFloat3 &color = pointElement->getColor()->getValue();
			newLight->Color		= CrtColor4f((float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0f);
			if ( pointElement->getConstant_attenuation()) 
				newLight->ConstantAttenuation	= (float)(pointElement->getConstant_attenuation()->getValue());
			if ( pointElement->getLinear_attenuation()) 
				newLight->LinearAttenuation		= (float)(pointElement->getLinear_attenuation()->getValue());
			if ( pointElement->getQuadratic_attenuation()) 
				newLight->QuadraticAttenuation	= (float)(pointElement->getQuadratic_attenuation()->getValue());
		}
		else if(spotElement)
		{
			newLight->SetType(eLIGHTTYPE_SPOT);	
			domFloat3 &color = spotElement->getColor()->getValue();
			newLight->Color		= CrtColor4f((float)(color[0]), (float)(color[1]), (float)(color[2]), 1.0f);
			if (spotElement->getConstant_attenuation())
				newLight->ConstantAttenuation	= (float)(spotElement->getConstant_attenuation()->getValue());
			if (spotElement->getLinear_attenuation())
				newLight->LinearAttenuation		= (float)(spotElement->getLinear_attenuation()->getValue());
			if (spotElement->getQuadratic_attenuation())
				newLight->QuadraticAttenuation	= (float)(spotElement->getQuadratic_attenuation()->getValue());
			if (spotElement->getFalloff_angle())
				newLight->FalloffAngle			= (float)(spotElement->getFalloff_angle()->getValue());
			if (spotElement->getFalloff_exponent())
				newLight->FalloffExponent		= (float)(spotElement->getFalloff_exponent()->getValue());
		}

		Lights.push_back(newLight);
		return newLight; 	
	}
	return NULL; 	
};

CrtCamera *CrtScene::ReadCamera( domCameraRef lib )
{
	if (lib->getId()==NULL) return NULL;
	CrtCamera * camera = GetCamera(lib->getID(), lib->getDocumentURI()->getURI());
	if (camera)	// camera is found
		return camera;
	CrtPrint( "Add new Camera %s \n", lib->getId() );  

	domCamera * CameraElement = (domCamera*)(domElement*)lib; 
	if ( CameraElement )
	{	
		// Make the camera
		CrtCamera * newCam = CrtNew( CrtCamera ); 
		CrtAssert("No memory\n", newCam!=NULL);

		// Set its name to the id of the collada element
		newCam->SetName( CameraElement->getId() );
		newCam->SetDocURI( CameraElement->getDocumentURI()->getURI() ); 
		
		// Get the optics
		domCamera::domOptics *cameraOptics = CameraElement->getOptics();
		
		// Get the optics common profile technique
		domCamera::domOptics::domTechnique_common *cameraCommonTechnique = cameraOptics->getTechnique_common();
		
		// Get the get the perspective and orthographic common profiles (if they exist)
		domCamera::domOptics::domTechnique_common::domPerspective *cameraPerspective = cameraCommonTechnique->getPerspective();
		domCamera::domOptics::domTechnique_common::domOrthographic *cameraOrthographic = cameraCommonTechnique->getOrthographic();
		
		// Setup camera parameters, note we have to check if a parameter is there before doing getValue
		// Parameters not in the COLLADA data will be left at the CrtCamera defaults
		if(cameraPerspective)
		{
			if(cameraPerspective->getXfov())
				newCam->SetXFov((float)(cameraPerspective->getXfov()->getValue()));
			if(cameraPerspective->getYfov())
				newCam->SetYFov((float)(cameraPerspective->getYfov()->getValue()));
			if(cameraPerspective->getAspect_ratio())
				newCam->SetAspect((float)(cameraPerspective->getAspect_ratio()->getValue()));
			if(cameraPerspective->getZnear())
				newCam->SetZNear((float)(cameraPerspective->getZnear()->getValue()));
			if(cameraPerspective->getZfar())
				newCam->SetZFar((float)(cameraPerspective->getZfar()->getValue()));
			// !!!GAC force Znear to be >= 1.0 to avoid rendering problems
			if( newCam->GetZNear() < 1.0f)
				newCam->SetZNear(1.0f);
		}
		if(cameraOrthographic)
		{
			// Setup orthographic camera
			CrtPrint("Support for orthograph cameras incomplete, this camera will probably look odd\n");
			if(cameraOrthographic->getXmag())
				newCam->SetXMag((float)(cameraOrthographic->getXmag()->getValue()));
			if(cameraOrthographic->getYmag())
				newCam->SetYMag((float)(cameraOrthographic->getYmag()->getValue()));
			if(cameraOrthographic->getAspect_ratio())
				newCam->SetAspect((float)(cameraOrthographic->getAspect_ratio()->getValue()));
			if(cameraOrthographic->getZnear())
				newCam->SetZNear((float)(cameraOrthographic->getZnear()->getValue()));
			if(cameraOrthographic->getZfar())
				newCam->SetZFar((float)(cameraOrthographic->getZfar()->getValue()));
			// !!!GAC force Znear to be >= 1.0 to avoid rendering problems
			if( newCam->GetZNear() < 1.0f)
				newCam->SetZNear(1.0f);
		}
		Cameras.push_back(newCam);
		return newCam; 	
	}
	return NULL;
};

// ------------ Read Effect Libraray ---------------------
// !!!GAC the ReadEffectLibrary code will be replaced by the COLLADA_FX loader
CrtBool	CrtScene::ReadEffectLibrary( domLibrary_effectsRef lib )
{
	CrtPrint(" CrtScene::Reading Effect Library \n" );	
	
	for ( CrtUInt i = 0; i < lib->getEffect_array().getCount(); i++)
	{
		ReadEffect( lib->getEffect_array()[i] ); 
	}

	return CrtTrue; 	
};
CrtFloat ReadFloatOrParamType(CrtEffect * effect, domCommon_float_or_param_typeRef float_or_param)
{
	(void) effect;
	if (float_or_param->getFloat())
	{
		return (CrtFloat)  float_or_param->getFloat()->getValue();
	}
//	if (color_or_texture->getParam())
//		color_or_texture->getParam();
	return (CrtFloat) 0;
}
CrtColor4f ReadColorOrTextureType(CrtEffect * effect, domCommon_color_or_texture_type_complexType* color_or_texture)
{
	(void) effect;
	if (color_or_texture->getColor())
	{
		domFx_color_common & color = color_or_texture->getColor()->getValue();

		return CrtColor4f((float) color[0], (float) color[1], (float) color[2], (float) color[3]);
	}
//	if (color_or_texture->getParam())
//		color_or_texture->getParam();
//	if (color_or_texture->getTexture())
//		color_or_texture->getTexture()->getTexture();
//
	return CrtColor4f();
}
class ShaderElements
{
public:
	ShaderElements() : emission(NULL), ambient(NULL), diffuse(NULL), specular(NULL), shininess(NULL),
		               reflective(NULL), reflectivity(NULL), transparent(NULL), transarency(NULL), index_of_refaction(NULL){};
	~ShaderElements() {};
	domCommon_color_or_texture_type *emission;
	domCommon_color_or_texture_type *ambient;
	domCommon_color_or_texture_type *diffuse;
	domCommon_color_or_texture_type *specular;
	domCommon_float_or_param_type   *shininess;

	domCommon_color_or_texture_type *reflective;
	domCommon_float_or_param_type   *reflectivity;
	domCommon_color_or_texture_type_complexType *transparent;
	domCommon_float_or_param_type   *transarency;
	domCommon_float_or_param_type   *index_of_refaction;
}; 
void ReadConstant(CrtEffect * effect, ShaderElements * shader, domProfile_COMMON::domTechnique::domConstant *constant)
{
	shader->emission = constant->getEmission();
	if (shader->emission ) {
		effect->Emission = ReadColorOrTextureType(effect, shader->emission);
	}
	shader->reflective = constant->getReflective();
	if (shader->reflective) {
		effect->Reflective = ReadColorOrTextureType(effect, shader->reflective);
	}
	shader->reflectivity = constant->getReflectivity();
	if (shader->reflectivity) {
		effect->Reflectivity = ReadFloatOrParamType(effect, shader->reflectivity);
	}
	shader->transparent = constant->getTransparent();
	if (shader->transparent) {
		effect->Transparent = ReadColorOrTextureType(effect, shader->transparent);
	}
	shader->transarency = constant->getTransparency();
	if (shader->transarency) {
		effect->Transparency = ReadFloatOrParamType(effect, shader->transarency);
	}
	shader->index_of_refaction = constant->getIndex_of_refraction();
	if (shader->index_of_refaction) {
		effect->RefractiveIndex = ReadFloatOrParamType(effect, shader->index_of_refaction);
	}
}
void ReadLambert(CrtEffect * effect, ShaderElements * shader, domProfile_COMMON::domTechnique::domLambert *lambert)
{
	shader->emission = lambert->getEmission();
	if (shader->emission ) {
		effect->Emission = ReadColorOrTextureType(effect, shader->emission);
	}
	shader->ambient = lambert->getAmbient();
	if (shader->ambient ) {
		effect->Ambient = ReadColorOrTextureType(effect, shader->ambient);
	}
	shader->diffuse = lambert->getDiffuse();
	if (shader->diffuse) {
		effect->Diffuse = ReadColorOrTextureType(effect, shader->diffuse);
	}
	shader->reflective = lambert->getReflective();
	if (shader->reflective) {
		effect->Reflective = ReadColorOrTextureType(effect, shader->reflective);
	}
	shader->reflectivity = lambert->getReflectivity();
	if (shader->reflectivity) {
		effect->Reflectivity = ReadFloatOrParamType(effect, shader->reflectivity);
	}
	shader->transparent = lambert->getTransparent();
	if (shader->transparent) {
		effect->Transparent = ReadColorOrTextureType(effect, shader->transparent);
	}
	shader->transarency = lambert->getTransparency();
	if (shader->transarency) {
		effect->Transparency = ReadFloatOrParamType(effect, shader->transarency);
	}
	shader->index_of_refaction = lambert->getIndex_of_refraction();
	if (shader->index_of_refaction) {
		effect->RefractiveIndex = ReadFloatOrParamType(effect, shader->index_of_refaction);
	}
}
void ReadPhong(CrtEffect * effect, ShaderElements * shader, domProfile_COMMON::domTechnique::domPhong *phong)
{
	shader->emission = phong->getEmission();
	if (shader->emission ) {
		effect->Emission = ReadColorOrTextureType(effect, shader->emission);
	}
	shader->ambient = phong->getAmbient();
	if (shader->ambient ) {
		effect->Ambient = ReadColorOrTextureType(effect, shader->ambient);
	}
	shader->diffuse = phong->getDiffuse();
	if (shader->diffuse) {
		effect->Diffuse = ReadColorOrTextureType(effect, shader->diffuse);
	}
	shader->specular = phong->getSpecular();
	if (shader->specular) {
		effect->Specular = ReadColorOrTextureType(effect, shader->specular);
	}
	shader->shininess = phong->getShininess();
	if (shader->shininess) {
		effect->Shininess = ReadFloatOrParamType(effect, shader->shininess);
	}
	shader->reflective = phong->getReflective();
	if (shader->reflective) {
		effect->Reflective = ReadColorOrTextureType(effect, shader->reflective);
	}
	shader->reflectivity = phong->getReflectivity();
	if (shader->reflectivity) {
		effect->Reflectivity = ReadFloatOrParamType(effect, shader->reflectivity);
	}
	shader->transparent = phong->getTransparent();
	if (shader->transparent) {
		effect->Transparent = ReadColorOrTextureType(effect, shader->transparent);
	}
	shader->transarency = phong->getTransparency();
	if (shader->transarency) {
		effect->Transparency = ReadFloatOrParamType(effect, shader->transarency);
	}
	shader->index_of_refaction = phong->getIndex_of_refraction();
	if (shader->index_of_refaction) {
		effect->RefractiveIndex = ReadFloatOrParamType(effect, shader->index_of_refaction);
	}
}
void ReadBlinn(CrtEffect * effect, ShaderElements * shader, domProfile_COMMON::domTechnique::domBlinn *blinn)
{
	shader->emission = blinn->getEmission();
	if (shader->emission ) {
		effect->Emission = ReadColorOrTextureType(effect, shader->emission);
	}
	shader->ambient = blinn->getAmbient();
	if (shader->ambient ) {
		effect->Ambient = ReadColorOrTextureType(effect, shader->ambient);
	}
	shader->diffuse = blinn->getDiffuse();
	if (shader->diffuse) {
		effect->Diffuse = ReadColorOrTextureType(effect, shader->diffuse);
	}
	shader->specular = blinn->getSpecular();
	if (shader->specular) {
		effect->Specular = ReadColorOrTextureType(effect, shader->specular);
	}
	shader->shininess = blinn->getShininess();
	if (shader->shininess) {
		effect->Shininess = ReadFloatOrParamType(effect, shader->shininess);
	}
	shader->reflective = blinn->getReflective();
	if (shader->reflective) {
		effect->Reflective = ReadColorOrTextureType(effect, shader->reflective);
	}
	shader->reflectivity = blinn->getReflectivity();
	if (shader->reflectivity) {
		effect->Reflectivity = ReadFloatOrParamType(effect, shader->reflectivity);
	}
	shader->transparent = blinn->getTransparent();
	if (shader->transparent) {
		effect->Transparent = ReadColorOrTextureType(effect, shader->transparent);
	}
	shader->transarency = blinn->getTransparency();
	if (shader->transarency) {
		effect->Transparency = ReadFloatOrParamType(effect, shader->transarency);
	}
	shader->index_of_refaction = blinn->getIndex_of_refraction();
	if (shader->index_of_refaction) {
		effect->RefractiveIndex = ReadFloatOrParamType(effect, shader->index_of_refaction);
	}
}
// Get diffuse texture name
CrtImage * CrtScene::GetTextureFromShader(map<string, domCommon_newparam_type*> &NewParams, domCommon_color_or_texture_type* shader)
{
	if (shader==NULL) return NULL;
	domCommon_color_or_texture_type::domTexture *textureElement = shader->getTexture();
	if (textureElement==NULL) return NULL;

	string sampler2D_SID = textureElement->getTexture();
	if (NewParams[sampler2D_SID]==0)
	{
		xsIDREF idref(sampler2D_SID.c_str());
		idref.setContainer(shader);
		idref.resolveElement();
		domImage* image_element = (domImage*)(domElement*)idref.getElement();
		return ReadImage(image_element);
	}
	string surface_SID = NewParams[sampler2D_SID]->getSampler2D()->getSource()->getValue();

	if (!NewParams[surface_SID]->getSurface()->getFx_surface_init_common())
		return NULL;

	xsIDREF& idRef = NewParams[surface_SID]->getSurface()->getFx_surface_init_common()->getInit_from_array()[0]->getValue();
	idRef.resolveElement();
	domImage* image_element = (domImage*)(domElement*) idRef.getElement();;
	return ReadImage(image_element);
}
CrtEffect *CrtScene::ReadEffect( domEffectRef lib )
{
	if (lib->getId()==NULL) return NULL;
	CrtEffect * effect = GetEffect(lib->getID(), lib->getDocumentURI()->getURI());
	if (effect)	// effect is found
		return effect;
	CrtPrint( "Add new Effect %s \n", lib->getId() );  
	
	// Get a pointer to the effect element
	domEffect * EffectElement = (domEffect*)(domElement*)lib; 

	if ( EffectElement )
	{
		for (CrtUInt i = 0; i < EffectElement->getImage_array().getCount(); i++ )
		{
			ReadImage( EffectElement->getImage_array()[i] );
		}
		// Make a CrtEffect to put this in and set the name
		CrtEffect * newEffect = CrtNew(CrtEffect);
		CrtAssert("No memory\n", newEffect!=NULL);

		newEffect->SetName( EffectElement->getId());
		newEffect->SetDocURI( EffectElement->getDocumentURI()->getURI() ); 
		
		// How many profiles are there
		CrtUInt numProfiles = (CrtUInt) EffectElement->getFx_profile_abstract_array().getCount(); 

		// Scan the profiles to find the profile_COMMON
		for ( CrtUInt p = 0; p < numProfiles;  p ++)
		{
			CrtChar * typeName = (CrtChar *) EffectElement->getFx_profile_abstract_array()[p]->getTypeName(); 
			
			if ( CrtCmp("profile_COMMON", typeName ) )
			{
				// Found the common profile, get the technique from it as well
				domProfile_COMMON * common = (domProfile_COMMON *)(domFx_profile_abstract*)EffectElement->getFx_profile_abstract_array()[p]; 

				// Get all images in profile_COMMON
				for (CrtUInt i = 0; i < common->getImage_array().getCount(); i++ )
				{
					ReadImage( common->getImage_array()[i] );
				}

				// Get all images in profile_COMMON
				domProfile_COMMON::domTechnique *technique = common->getTechnique(); 
				if ( technique == NULL )
					break; 

				for (CrtUInt i = 0; i < technique->getImage_array().getCount(); i++ )
				{
					ReadImage( technique->getImage_array()[i] );
				}				

				// This support is really basic, since the shader models don't descend from a common type
				// we have to handle each one individually.  There can only be one in the technique.
				// All of them assume the texture is in the diffuse component for now.

				ShaderElements shader;
				domProfile_COMMON::domTechnique::domConstant *constant = technique->getConstant();
				if (constant)
					ReadConstant(newEffect, &shader, constant);
				domProfile_COMMON::domTechnique::domLambert *lambert = technique->getLambert();
				if (lambert)
					ReadLambert(newEffect, &shader, lambert);
				domProfile_COMMON::domTechnique::domPhong *phong = technique->getPhong();
				if (phong)
					ReadPhong(newEffect, &shader, phong);
				domProfile_COMMON::domTechnique::domBlinn *blinn = technique->getBlinn();
				if(blinn)
					ReadBlinn(newEffect, &shader, blinn);

				domCommon_newparam_type_Array newparam_array = common->getNewparam_array();
				std::map<string, domCommon_newparam_type*> NewParams;
				for (CrtUInt i=0; i<newparam_array.getCount(); i++) {
					NewParams[newparam_array[i]->getSid()] = newparam_array[i];
				}
				
				// TODO: take only the texture from diffuse for now
				CrtImage * image;
				image = GetTextureFromShader(NewParams, shader.diffuse);
				if (image)
					newEffect->Textures.push_back(image);
/*					image = GetTextureFromShader(NewParams, shader.emission);
				if (image)
					newEffect->Textures.push_back(image);
				image = GetTextureFromShader(NewParams, shader.ambient);
				if (image)
					newEffect->Textures.push_back(image);
				image = GetTextureFromShader(NewParams, shader.specular);
				if (image)
					newEffect->Textures.push_back(image);
				image = GetTextureFromShader(NewParams, shader.reflective);
				if (image)
					newEffect->Textures.push_back(image);
				image = GetTextureFromShader(NewParams, shader.transparent);
				if (image)
					newEffect->Textures.push_back(image);
*/				
				// Handle an effect with no texture
				Effects.push_back(newEffect);
				return newEffect; 	
			} else {
				CrtPrint("%s is not supported\n", typeName);
			}
		}
		delete newEffect;
	}
	return NULL; 	
};

// ------------ Read Material Libraray ---------------------
// !!!GAC the ReadMaterialLibrary code will be replaced by the COLLADA_FX loader
CrtBool	CrtScene::ReadMaterialLibrary( domLibrary_materialsRef lib )
{
	CrtPrint(" CrtScene::Reading Material Library \n" );	
	
	for ( CrtUInt i = 0; i < lib->getMaterial_array().getCount(); i++)
	{
		ReadMaterial( lib->getMaterial_array()[i] ); 
	}

	return CrtTrue; 	
};

CrtMaterial *CrtScene::ReadMaterial( domMaterialRef lib )
{
	if (lib->getId()==NULL) return NULL;
	CrtMaterial * material = GetMaterial(lib->getID(), lib->getDocumentURI()->getURI());
	if (material)	// material is found
		return material;
	CrtPrint( "Add new Material %s \n", lib->getId() );  
	
	domMaterial * MaterialElement = (domMaterial*)(domElement*)lib; 

	if ( MaterialElement ) 
	{
		if ( !MaterialElement->getInstance_effect() )
			return NULL; 

		domElement * element = (domElement *) MaterialElement->getInstance_effect()->getUrl().getElement(); 
		if (element==NULL)
			return NULL;
		
		// find the effect that the material is refering too 
		CrtEffect * effect = ReadEffect((domEffect *) element);
		if (effect) 
		{
			CrtMaterial * newMat = CrtNew(CrtMaterial); 
			CrtAssert("No memory\n", newMat!=NULL);
			newMat->SetName( MaterialElement->getId() ); 
			newMat->SetDocURI( MaterialElement->getDocumentURI()->getURI() ); 
			newMat->SetEffect( effect ); 
			CrtPrint("Attaching effect %s to Material %s \n", effect->GetName(), newMat->GetName() ); 
			Materials.push_back(newMat);
			return newMat; 	
		} else
		{
			CrtPrint("*** No effect %s for old-style matrial %s (no problem if there is a cfx version of this material)\n", element->getID(), lib->getId()); 
			return NULL;
		}
	}
	return NULL; 	
};

CrtInt	CrtGetTriIndexList( CrtUInt * Indices, domTriangles * tris, CrtUInt triNum )
{
	// All the triangles are in a single P now
	// The number of indices in a triangle is the number of inputs times 3
	CrtUInt idxCount = 3 * ((CrtUInt)tris->getInput_array().getCount());
	// The first index of this triangle is triNum * the number of indices per triangle
	CrtUInt triStart = triNum * idxCount;
	// Since this is a triangle, we just do a straight copy of the indices into the output array
	for ( CrtUInt i= 0 ;i < idxCount ;i++)
	{
		Indices[i] = (CrtUInt) tris->getP()->getValue()[triStart+i]; 	
	}
	return idxCount; 
}

CrtUInt 	CrtCountPotentialTris( domPolygons * polygons )
{
	CrtUInt numTris = 0; 
	CrtUInt numInputs = (CrtUInt)polygons->getInput_array().getCount(); 
	CrtUInt count = (CrtUInt)polygons->getP_array().getCount();
	for ( CrtUInt i = 0; i < count; i++)
	{
		// number of indexes / number of inputs - 2
		numTris += (CrtUInt)(polygons->getP_array()[i]->getValue().getCount()/numInputs) - 2; 
	}
	return numTris; 
}

CrtUInt 	CrtCountPotentialTris( domPolylist * polylist )
{
	CrtUInt numTris = 0; 
	CrtUInt count = (CrtUInt)polylist->getVcount()->getValue().getCount();
	for ( CrtUInt i = 0; i < count; i++)
	{
		// number of indexes / number of inputs - 2
		numTris += (CrtUInt)polylist->getVcount()->getValue()[i] - 2; 
	}
	return numTris; 
}

CrtUInt	GetMaxOffsetFromInputs(domInputLocalOffset_Array & inputs)
{
	CrtUInt maxoffset = 0;
	CrtUInt count = (CrtUInt) inputs.getCount();
	for(CrtUInt i=0; i< count ;i++)
	{
		CrtUInt thisoffset  = (CrtUInt) inputs[i]->getOffset();
		if (maxoffset < thisoffset) maxoffset++;
	}
	return maxoffset + 1;
}

class CrtOffsets
{
public:
	CrtOffsets(domInputLocalOffset_Array & inputs)
	{
		max_offset = 0;
		position_offset = -1;
		normal_offset = -1;
		texture1_offset = -1;
		position_floats = NULL;
		normal_floats = NULL;
		texture1_floats = NULL;
		SetInputs(inputs);
	};
	~CrtOffsets(){};
	CrtInt max_offset;

	CrtInt position_offset;
	CrtInt normal_offset;
	CrtInt texture1_offset;

	domListOfFloats * position_floats;
	domListOfFloats * normal_floats;
	domListOfFloats * texture1_floats;
private:
	void SetInputs(domInputLocalOffset_Array & inputs)
	{
		// inputs with offsets
		for(CrtUInt i=0; i<inputs.getCount(); i++)
		{
			CrtInt thisoffset  = (CrtUInt) inputs[i]->getOffset();
			if (max_offset < thisoffset) max_offset++;

			domSource * source = (domSource*) (domElement*) inputs[i]->getSource().getElement();
			if(CrtCmp("VERTEX", inputs[i]->getSemantic())) {
				position_offset = thisoffset;
			} else if(CrtCmp("NORMAL", inputs[i]->getSemantic())) {
				normal_offset = thisoffset;
				normal_floats = & source->getFloat_array()->getValue();
			} else if(CrtCmp("TEXCOORD", inputs[i]->getSemantic())) {
				texture1_offset = thisoffset;
				texture1_floats = & source->getFloat_array()->getValue();
			}
		}
		max_offset++;

		// inputs without offsets in vertices
		domMesh * mesh = (domMesh*) inputs[0]->getParentElement()->getParentElement();
		domVertices * vertices = mesh->getVertices();
		domInputLocal_Array& vertices_inputs = vertices->getInput_array();
		for(CrtUInt i=0; i<vertices_inputs.getCount(); i++)
		{
			domSource * source = (domSource*) (domElement*) vertices_inputs[i]->getSource().getElement();
			if(CrtCmp("POSITION", vertices_inputs[i]->getSemantic())) {
				position_floats = & source->getFloat_array()->getValue();
			} else if(CrtCmp("NORMAL", vertices_inputs[i]->getSemantic())) {
				normal_floats = & source->getFloat_array()->getValue();
				normal_offset = position_offset;
			} else if(CrtCmp("TEXCOORD", vertices_inputs[i]->getSemantic())) {
				texture1_floats = & source->getFloat_array()->getValue();
				texture1_offset = position_offset;
			}
		}

	};
};

template <class T>
void GrowData(T **old_data, CrtUInt count, CrtUInt max)
{
	T * new_data = new T[max];
//	for (CrtUInt i=0; i<count; i++)
//	{
//		new_data[i] = old_data[i];
//	}
	memcpy(new_data, *old_data, sizeof(T) * count);
	delete [] *old_data; //TODO: WARNING, MEMORY LEAK
	*old_data = new_data;
}
CrtInt GrowVertexData(CrtGeometry * geometry, CrtUInt size)
{
	CrtUInt memory_need = geometry->vertexcount + size;
	if (memory_need < geometry->vertexcountmax) return 0;
	geometry->vertexcountmax = geometry->vertexcountmax * ((memory_need / geometry->vertexcountmax) + 1);
	GrowData(&(geometry->Points), geometry->vertexcount, geometry->vertexcountmax);
	GrowData(&(geometry->Normals), geometry->vertexcount, geometry->vertexcountmax);
	GrowData(&(geometry->TexCoords[0]), geometry->vertexcount, geometry->vertexcountmax);
	GrowData(&(geometry->SkinIndex), geometry->vertexcount, geometry->vertexcountmax);
	return 0;
}

CrtInt SetVertexData(CrtOffsets& offset, CrtGeometry * geometry, domListOfUInts &values, CrtUInt i)
{
	domUint index;
	index = values[i*offset.max_offset + offset.position_offset];
	geometry->SkinIndex[geometry->vertexcount] = (CrtUInt) index;

	geometry->Points[geometry->vertexcount].x = (CrtFloat) (*offset.position_floats)[(size_t)index*3+0];
	geometry->Points[geometry->vertexcount].y = (CrtFloat) (*offset.position_floats)[(size_t)index*3+1];
	geometry->Points[geometry->vertexcount].z = (CrtFloat) (*offset.position_floats)[(size_t)index*3+2];

	if (offset.normal_offset != -1)
	{
		index = values[i*offset.max_offset + offset.normal_offset];
		geometry->Normals[geometry->vertexcount].x = (CrtFloat) (*offset.normal_floats)[(size_t)index*3+0];
		geometry->Normals[geometry->vertexcount].y = (CrtFloat) (*offset.normal_floats)[(size_t)index*3+1];
		geometry->Normals[geometry->vertexcount].z = (CrtFloat) (*offset.normal_floats)[(size_t)index*3+2];
	}

	if (offset.texture1_offset != -1)
	{
		index = values[i*offset.max_offset + offset.texture1_offset];
		geometry->TexCoords[0][geometry->vertexcount].x = (CrtFloat) (*offset.texture1_floats)[(size_t)index*2+0];
		geometry->TexCoords[0][geometry->vertexcount].y = (CrtFloat) (*offset.texture1_floats)[(size_t)index*2+1];
	}
	geometry->vertexcount++;
	return geometry->vertexcount-1;
}

CrtLines * CrtScene::BuildLines(domLines * dom_lines, CrtGeometry * geometry)
{
	CrtLines * lines = CrtNew(CrtLines);
	lines->count = (CrtUInt) dom_lines->getCount();
	GrowVertexData(geometry, lines->count * 2);
	// resolve Material	
	daeString str_material = dom_lines->getMaterial();
	if (str_material)
		CrtCpy(lines->MaterialName, str_material);
	// prepare data
	lines->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_lines->getInput_array();
	CrtOffsets offsets(inputs);

		// set index, they all have the same index since we process deindexer conditioner
	domListOfUInts P = dom_lines->getP()->getValue();
	lines->indexes = CrtNewData(CrtUInt, lines->count * 2);
	for (CrtUInt ivertex=0; ivertex< lines->count * 2; ivertex++)
	{
		lines->indexes[ivertex] = SetVertexData(offsets, geometry, P, ivertex);
//		lines->indexes[ivertex] = (CrtUInt) P[ivertex*maxoffset ];
	}

	return lines;
}
CrtLinestrips * CrtScene::BuildLineStrips(domLinestrips * dom_linestrips, CrtGeometry * geometry)
{
	CrtLinestrips * linestrips = CrtNew(CrtLinestrips);
	linestrips->count = (CrtUInt) dom_linestrips->getCount();
	// resolve Material	
	daeString str_material = dom_linestrips->getMaterial();
	if (str_material)
		CrtCpy(linestrips->MaterialName, str_material);
	// prepare data
	linestrips->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_linestrips->getInput_array();
	CrtOffsets offsets(inputs);

	domP_Array &P = dom_linestrips->getP_array();
	for (CrtUInt i=0; i< P.getCount(); i++)
	{		
		CrtUInt vcount = (CrtUInt) P[i]->getValue().getCount() / offsets.max_offset;
		CrtUInt * strips = CrtNewData(CrtUInt, vcount );
		GrowVertexData(geometry, vcount * 2);

		for (CrtUInt j=0; j< vcount ; j++)
		{
			strips[j] = SetVertexData(offsets, geometry, P[i]->getValue(), j);
//			strips[j] = (CrtUInt) P[i]->getValue()[j*maxoffset];
		}
		linestrips->countvector.push_back( vcount );
		linestrips->indexvector.push_back( strips );
	}

	return linestrips;
}
CrtTriStrips * CrtScene::BuildTriStrips(domTristrips * dom_tristrips, CrtGeometry * geometry)
{
	CrtTriStrips * tristrips = CrtNew(CrtTriStrips);
	// triangles count
	tristrips->count = (CrtUInt) dom_tristrips->getCount();
	// resolve Material	
	daeString str_material = dom_tristrips->getMaterial();
	if (str_material)
		CrtCpy(tristrips->MaterialName, str_material);
	
	// prepare data
	tristrips->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_tristrips->getInput_array();
	CrtOffsets offsets(inputs);

	// set index, they all have the same index since we process deindexer conditioner
	domP_Array &P = dom_tristrips->getP_array();
	for (CrtUInt i=0; i< P.getCount(); i++)
	{		
		CrtUInt vcount = (CrtUInt) P[i]->getValue().getCount() / offsets.max_offset;
		CrtUInt * strips = CrtNewData(CrtUInt, vcount );
		GrowVertexData(geometry, vcount * 3 );

		for (CrtUInt j=0; j< vcount ; j++)
		{
			strips[j] = SetVertexData(offsets, geometry, P[i]->getValue(), j);
//			strips[j] = (CrtUInt) P[i]->getValue()[j*maxoffset];
		}
		tristrips->countvector.push_back( vcount );
		tristrips->indexvector.push_back( strips );
	}

	return tristrips;
}
CrtTriFans * CrtScene::BuildTriFans(domTrifans * dom_trifans, CrtGeometry * geometry)
{
	CrtTriFans * trifans = CrtNew(CrtTriFans);
	// triangles count
	trifans->count = (CrtUInt) dom_trifans->getCount();
	// resolve Material	
	daeString str_material = dom_trifans->getMaterial();
	if (str_material)
		CrtCpy(trifans->MaterialName, str_material);
	
	// prepare data
	trifans->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_trifans->getInput_array();
	CrtOffsets offsets(inputs);

	// set index, they all have the same index since we process deindexer conditioner
	domP_Array &P = dom_trifans->getP_array();
	for (CrtUInt i=0; i< P.getCount(); i++)
	{		
		CrtUInt vcount = (CrtUInt) P[i]->getValue().getCount() / offsets.max_offset;
		CrtUInt * strips = CrtNewData(CrtUInt, vcount );
		GrowVertexData(geometry, vcount * 3 );

		for (CrtUInt j=0; j< vcount ; j++)
		{
			strips[j] = SetVertexData(offsets, geometry, P[i]->getValue(), j);
//			strips[j] = (CrtUInt) P[i]->getValue()[j*maxoffset];
		}
		trifans->countvector.push_back( vcount );
		trifans->indexvector.push_back( strips );
	}

	return trifans;
}
CrtPolygons * CrtScene::BuildPolygons(domPolylist * dom_polylist, CrtGeometry * geometry)
{
	CrtPolygons * polygons = CrtNew(CrtPolygons);
	polygons->count = (CrtUInt) dom_polylist->getCount();

	daeString str_material = dom_polylist->getMaterial();
	if (str_material)
		CrtCpy(polygons->MaterialName, str_material);
	polygons->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_polylist->getInput_array();
	CrtOffsets offsets(inputs);

	domPolylist::domVcountRef Vcount = dom_polylist->getVcount();
	domPRef P = dom_polylist->getP();

	CrtUInt vertex_start_index = 0;
	for (CrtUInt i=0; i< Vcount->getValue().getCount(); i++)
	{		
		CrtUInt vcount = (CrtUInt) Vcount->getValue()[i];
		CrtUInt * vertices = CrtNewData(CrtUInt, vcount );
		GrowVertexData(geometry, vcount);

		for (CrtUInt j=0; j< vcount ; j++)
		{
			vertices[j] = SetVertexData(offsets, geometry, P->getValue(), vertex_start_index+j);
//			vertices[j] = (CrtUInt) P->getValue()[(j+vertex_start_index)*maxoffset];
		}
		polygons->countvector.push_back( vcount );
		polygons->indexvector.push_back( vertices );
		vertex_start_index += vcount;
	}

	return polygons;
}	

//CrtInt CrtScene::BuildVertexData(CrtPolyGroup * primitives, domInputLocalOffset_Array & inputs)
//{
//
//}

CrtPolygons * CrtScene::BuildPolygons(domPolygons * dom_polygons, CrtGeometry * geometry)
{
	CrtPolygons * polygons = CrtNew(CrtPolygons);
	// triangles count
	polygons->count = (CrtUInt) dom_polygons->getCount();

	daeString str_material = dom_polygons->getMaterial();
	if (str_material)
		CrtCpy(polygons->MaterialName, str_material);
	polygons->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_polygons->getInput_array();
	CrtOffsets offsets(inputs);
	
	CrtUInt vertex_count = 0;
	domP_Array &P = dom_polygons->getP_array();
	for (CrtUInt i=0; i< P.getCount(); i++)
	{		
		CrtUInt vcount = (CrtUInt) P[i]->getValue().getCount() / offsets.max_offset;
		vertex_count += vcount;
		GrowVertexData(geometry, vertex_count);

		CrtUInt * vertices = CrtNewData(CrtUInt, vcount );
		for (CrtUInt j=0; j< vcount ; j++)
		{
			vertices[j] = SetVertexData(offsets, geometry, P[i]->getValue(), j);
//			vertices[j] = (CrtUInt) P[i]->getValue()[j*maxoffset];
		}
		polygons->countvector.push_back( vcount );
		polygons->indexvector.push_back( vertices );
	}

	return polygons;
}

CrtTriangles * CrtScene::BuildTriangles(domTriangles * dom_triangles, CrtGeometry * geometry)
{
	CrtTriangles * triangles = CrtNew(CrtTriangles);
	// triangles count
	triangles->count = (CrtUInt) dom_triangles->getCount();
	GrowVertexData(geometry, triangles->count * 3);
	// resolve Material	
	daeString str_material = dom_triangles->getMaterial();
	if (str_material)
		CrtCpy(triangles->MaterialName, str_material);
	
	// prepare data
	triangles->geometry = geometry;
	domInputLocalOffset_Array & inputs = dom_triangles->getInput_array();
	CrtOffsets offsets(inputs);

	// set index, they all have the same index since we process deindexer conditioner
	domListOfUInts P = dom_triangles->getP()->getValue();
	triangles->indexes = CrtNewData(CrtUInt, triangles->count * 3);
	for (CrtUInt ivertex=0; ivertex< triangles->count * 3; ivertex++)
	{
		triangles->indexes[ivertex] = SetVertexData(offsets, geometry, P, ivertex);
//		triangles->indexes[ivertex] = (CrtUInt) P[ivertex*maxoffset ];
	}

	return triangles;
}

CrtVoid CrtScene::ParseGeometry(CrtGeometry * newGeo, domGeometry * dom_geometry)
{
	domMesh			*meshElement		= dom_geometry->getMesh();
	newGeo->SetName( dom_geometry->getId() );
	newGeo->SetDocURI( dom_geometry->getDocumentURI()->getURI() ); 

	//not sure if we should get primitives by groups or by whatever comes first, I think it shouldn't matter, let's confirm later.
	CrtUInt numPolylistGroups = (CrtUInt)meshElement->getPolylist_array().getCount();
	for (CrtUInt i=0; i< numPolylistGroups; i++)
	{
		CrtPolyGroup *newprimitives = BuildPolygons(meshElement->getPolylist_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	CrtUInt numPolygonGroups = (CrtUInt)meshElement->getPolygons_array().getCount();
	for (CrtUInt i=0; i< numPolygonGroups; i++)
	{
		CrtPolyGroup *newprimitives = BuildPolygons(meshElement->getPolygons_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	CrtUInt numTriangleGroups = (CrtUInt)meshElement->getTriangles_array().getCount();
	for (CrtUInt i=0; i< numTriangleGroups; i++)
	{
		CrtPolyGroup *newprimitives = BuildTriangles(meshElement->getTriangles_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	CrtUInt numTriStripsGroups = (CrtUInt)meshElement->getTristrips_array().getCount();
	for (CrtUInt i=0; i< numTriStripsGroups ; i++)
	{
		CrtPolyGroup *newprimitives = BuildTriStrips(meshElement->getTristrips_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	CrtUInt numTriFansGroups = (CrtUInt)meshElement->getTrifans_array().getCount();
	for (CrtUInt i=0; i< numTriFansGroups ; i++)
	{
		CrtPolyGroup *newprimitives = BuildTriFans(meshElement->getTrifans_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	CrtUInt numLinesGroups = (CrtUInt)meshElement->getLines_array().getCount();
	for (CrtUInt i=0; i< numLinesGroups ; i++)
	{
		CrtPolyGroup *newprimitives = BuildLines(meshElement->getLines_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	CrtUInt numLineStripsGroups = (CrtUInt)meshElement->getLinestrips_array().getCount();
	for (CrtUInt i=0; i< numLineStripsGroups ; i++)
	{
		CrtPolyGroup *newprimitives = BuildLineStrips(meshElement->getLinestrips_array()[i], newGeo);
		newGeo->Groups.push_back(newprimitives);
	}

	// set controller, we shouldn't need to set controller from geometry, only geometry to controller
	// it is setting it two ways now, we will fix this later.
/*	if (skin) {
		newGeo->SetSkin(skin);
		skin->base_geometry = newGeo;
//		newGeo->Weights = CrtNewData( CrtWeight, newGeo->vertexcount );
//		for (CrtUInt p=0; p< newGeo->vertexcount; p++)
//			newGeo->Weights[p].Copy( &skin->Weights[newGeo->SkinIndex[p]] );
	}
*/
	if (_CrtRender.UsingVBOs())
	{
		for (CrtUInt i=0; i<newGeo->Groups.size() ; i++)
			newGeo->Groups[i]->SetVBOs();

		newGeo->VBOIDs[eGeoPoints] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ARRAY_BUFFER, newGeo->VBOIDs[eGeoPoints],newGeo->Points, newGeo->vertexcount*3*sizeof(CrtFloat));

		newGeo->VBOIDs[eGeoNormals] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ARRAY_BUFFER, newGeo->VBOIDs[eGeoNormals],newGeo->Normals, newGeo->vertexcount*3*sizeof(CrtFloat));

		newGeo->VBOIDs[eGeoTexCoord0] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ARRAY_BUFFER, newGeo->VBOIDs[eGeoTexCoord0],newGeo->TexCoords[0], newGeo->vertexcount*2*sizeof(CrtFloat));
	}

}
CrtGeometry *CrtScene::ReadGeometry( domGeometryRef lib)
{
	if (lib->getId()==NULL) return NULL;
	if ( !_CrtRender.GetLoadGeometry() )
		return NULL; 

	CrtGeometry * geometry = GetGeometry(lib->getID(), lib->getDocumentURI()->getURI());
	if (geometry)	// geometry is found
		return geometry;

	//CrtPrint( "Add new Geometry %s \n", lib->getId() );

	// See what kind of data is in the geometry (there can be only one)
	domAsset		*assetElement		= lib->getAsset();
	domMesh			*meshElement		= lib->getMesh();
	domConvex_mesh	*convexMeshElement	= lib->getConvex_mesh();
	domExtra_Array	&extraElement		= lib->getExtra_array();
	domSpline		*splineElement		= lib->getSpline();

	if(assetElement)
	{
		CrtPrint( "	CrtScene::Item %s has an <asset> which RT doesn't support reading, skipped it.\n", lib->getId() );
	}
	else if (convexMeshElement)
	{
		CrtPrint( "	CrtScene::Item %s has an <convex_mesh> which RT doesn't support reading, skipped it.\n", lib->getId() );
	}
	else if (extraElement.getCount())
	{
		CrtPrint( "	CrtScene::Item %s has an <extra> which RT doesn't support reading, skipped it.\n", lib->getId() );
	}
	else if (splineElement)
	{
		CrtPrint( "	CrtScene::Item %s has an <spline> which RT doesn't support reading, skipped it.\n", lib->getId() );
	}

	if (meshElement == NULL)
		return NULL;

	// Allocate space for the new geometry object 
	CrtGeometry * newGeo     = CrtNew(CrtGeometry);
	CrtAssert("No memory\n", newGeo!=NULL);

	ParseGeometry(newGeo, lib);

	Geometries.push_back(newGeo);
	return newGeo; 	
};

// ------------ Read Image Libraray ---------------------
CrtBool	CrtScene::ReadImageLibrary( domLibrary_imagesRef lib )
{
	CrtPrint(" CrtScene::Reading Image Library \n" );	
	for ( CrtUInt i = 0; i < lib->getImage_array().getCount(); i++)
	{
		ReadImage( lib->getImage_array()[i] ); 
	}

	return CrtTrue; 	 
};

CrtImage *CrtScene::ReadImage( domImageRef lib )
{
	if (lib==NULL) return NULL;
	if (lib->getId()==NULL) return NULL;
	if ( !_CrtRender.GetLoadImages() )
		return NULL; 

	CrtImage * image = GetImage(lib->getID(), lib->getDocumentURI()->getURI());
	if (image)	// image is found
		return image;


	CrtPrint( "Add new image %s \n", lib->getId() );

	domImage* imageElement = (domImage*)(domElement*)lib;
	
	if ( imageElement )
	{
		CrtImage * newImage = CrtNew( CrtImage );
		CrtAssert("No memory\n", newImage!=NULL);
		newImage->SetName( imageElement->getId() );
		newImage->SetDocURI( imageElement->getDocumentURI()->getURI() );
		newImage->SetFileName(cdom::uriToNativePath(
			imageElement->getInit_from()->getValue().str()).c_str());
		
		// load the actual image passing in the current file name 
		// to first look relative to the current <file>_Textures
		// for the textures 
		if ( newImage->LoadImageFile( (CrtChar *)FileName ) )
		{
			if ((CrtInt32) newImage->GetId() == -1) 
			{
				CrtDelete(newImage ); 
				return NULL;
			}
			cfxSurface::addImage(newImage->GetName(), newImage->GetId());
			// add the newImage to the runtime 
			Images.push_back(newImage);
			return newImage;
		}
		else
		{
			CrtDelete(newImage ); 
			return NULL; 
		}
	}
	return NULL; 	
};

// ------------ Read Animation Libraray ---------------------
 
CrtBool	CrtScene::ReadAnimationLibrary( domLibrary_animationsRef lib )
{
	// See if we want to read animations or not
	if ( !_CrtRender.GetLoadAnimation() )
		return CrtFalse; 

	CrtPrint(" CrtScene::Reading Animation Library \n" );	

	// Read in each animation in the library

	for ( CrtUInt i = 0; i < lib->getAnimation_array().getCount(); i++)
	{
		ReadAnimation( lib->getAnimation_array()[i] ); 
	}

	return CrtTrue; 	
};


CrtAnimSrc *CrtScene::ReadAnimationSource( domSourceRef source )
{
	if (source==NULL) return NULL;
	CrtAnimSrc * new_source = CrtNew( CrtAnimSrc );

	// Set the source name
	CrtCpy( new_source->ID,  source->getId() );
	new_source->SetName( new_source->ID ); 
	new_source->SetDocURI( source->getDocumentURI()->getURI()); 
		
	// Copy over the float array data if any 
	if (source->getFloat_array()) // for float array
	{
		new_source->Count = (CrtUInt) source->getFloat_array()->getCount();
		new_source->Array = CrtNewData( CrtFloat, new_source->Count);

		daeDoubleArray& floatArray = source->getFloat_array()->getValue();
		
		// copy the array data 
		for ( CrtUInt a = 0; a < floatArray.getCount(); a++)
		{
			new_source->Array[a] = (CrtFloat)floatArray[a];
		}	
	} else if (source->getName_array())
	{
		CrtPrint( "Animation source %s is not support\n", new_source->ID );  
	} else {
		CrtPrint( "Animation source %s is not support\n", new_source->ID );  
	}
	return new_source;
}
CrtAnimSampler *CrtScene::ReadAnimationSampler(CrtAnimation * animation, domSamplerRef sampler)
{
	if (sampler==NULL) return NULL;
	CrtAnimSampler * new_sampler = CrtNew( CrtAnimSampler );
	domInputLocal_Array & input_array = sampler->getInput_array();
	for (CrtUInt i=0; i<input_array.getCount(); i++)
	{
		domSource * source = (domSource*) (domElement*) input_array[i]->getSource().getElement();
		new_sampler->inputs[input_array[i]->getSemantic()] = animation->Sources[source->getID()];
	}
	return new_sampler;
}
CrtAnimChannel *CrtScene::ReadAnimationChannel(CrtAnimation * animation, domChannelRef channel)
{
	if (channel==NULL) return NULL;
	CrtAnimChannel * new_channel = CrtNew( CrtAnimChannel );
	domSampler * sampler = (domSampler*) (domElement*) channel->getSource().getElement();
	const CrtChar * target = channel->getTarget();
	new_channel->SetSampler(animation->Samplers[sampler->getID()]);
//	new_channel->SetTarget(target.c_str());
	CrtAnimSrc * input_source = animation->Sources[new_channel->Sampler->inputs["INPUT"]->ID];
	CrtAnimSrc * output_source = animation->Sources[new_channel->Sampler->inputs["OUTPUT"]->ID];
	new_channel->SetInputSrc(input_source);
	new_channel->SetOutputSrc(output_source);

	// parse target
//	CrtCpy(new_channel->TargetIDStr, target, '/' ); 
	// get a pointer to the target element 
	const CrtChar * id_sep = CrtFindStr( target, "/");
	if (id_sep == NULL)	id_sep = CrtFindStr( target, "(");
	if (id_sep == NULL)	id_sep = CrtFindStr( target, ".");

	// get a pointer to the final element 
	const CrtChar * sid_sep = CrtFindStr( target, "." );	// search for '.'
	if (sid_sep == NULL) sid_sep = CrtFindStr( target, "(" );		// search for '('
	if (sid_sep == NULL) sid_sep = target + CrtLen(target);			// nothing else matches, then there is no member
	
	if (id_sep != NULL) CrtNCpy(new_channel->TargetIDStr, target, (CrtInt) (id_sep - target)); 
	if (id_sep != NULL && id_sep !=NULL && sid_sep>id_sep) CrtNCpy(new_channel->TargetSIDStr, id_sep+1, (CrtInt) (sid_sep - id_sep - 1)); 
	if (sid_sep!=NULL && (*sid_sep)!='\0') CrtCpy(new_channel->TargetMemberStr, sid_sep+1); 

	// resolve target
	domElement * element = 0;
	domElement * rootnode = channel->getDocument()->getDomRoot();
	daeSIDResolver sidresolver(rootnode, target);
	element = sidresolver.getElement();
	if (element==NULL)
	{
		CrtPrint("Animation target target=%s can not be solved\n", target);
		CrtDelete( new_channel );
		return NULL;
	}

	// set channel info
	COLLADA_TYPE::TypeEnum type = element->getElementType();
	switch(type)
	{
	case COLLADA_TYPE::TRANSLATE:
		animation->HasTranslation = CrtTrue;
		new_channel->SetTarget( eTranslate );
		new_channel->NumElementTargets = 3; 
		break;
	case COLLADA_TYPE::ROTATE:
		animation->HasRotation = CrtTrue;
		new_channel->SetTarget( eRotate );
		new_channel->NumElementTargets = 4; 
		break;
	case COLLADA_TYPE::SCALE:
		animation->HasScale = CrtTrue;
		new_channel->SetTarget( eScale );
		new_channel->NumElementTargets = 3; 
		break;
	case COLLADA_TYPE::SOURCE:
		animation->HasSource = CrtTrue;
		new_channel->SetTarget( eSource);
		new_channel->NumElementTargets = 1; 
		break;
	case COLLADA_TYPE::MATRIX:
		animation->HasMatrix = CrtTrue;
		new_channel->SetTarget( eMatrix);
		new_channel->NumElementTargets = 16; 
		break;
	default:
		CrtPrint("animation target not supported target=%s\n", target);
		CrtDelete( new_channel );
		return NULL;
	}

	// parse member
	if (new_channel->TargetMemberStr[0] != '\0')
	{
		const CrtChar * target_member = new_channel->TargetMemberStr;
		if ( CrtICmp( target_member, "AXIS" ) ||
			 CrtICmp( target_member, "ANGLE" ) )
		{
			domRotate * rotate = (domRotate *) element;
			if (rotate->getValue()[0] == 1)
			{	// rotate x axis
				animation->HasRotation = CrtTrue;
				new_channel->SetTarget(eRotXAxis);
				new_channel->NumElementTargets = 1; 
			} else if (rotate->getValue()[1] == 1) 
			{	// rotate y axis
				animation->HasRotation = CrtTrue;
				new_channel->SetTarget(eRotYAxis);
				new_channel->NumElementTargets = 1; 
			} else if (rotate->getValue()[2] == 1) 
			{	// rotate y axis
				animation->HasRotation = CrtTrue;
				new_channel->SetTarget(eRotZAxis);
				new_channel->NumElementTargets = 1; 
			}
		}
		else
		if ( CrtICmp( target_member, "X" )) 		 
		{
			new_channel->SetTarget(eAnimTargetX); 
			new_channel->NumElementTargets = 1; 
		}
		else 
		if ( CrtICmp( target_member, "Y" ) )		 
		{
			new_channel->SetTarget(eAnimTargetY); 
			new_channel->NumElementTargets = 1; 
		}
		else
		if ( CrtICmp( target_member, "Z" ) )		 
		{
			new_channel->SetTarget(eAnimTargetZ); 
			new_channel->NumElementTargets = 1; 
		}
		else
		if ( (*target_member)>='0' && (*target_member)<='9')		 
		{
			new_channel->SetTarget(eSource); 
			new_channel->NumElementTargets = 1; 
		}
		else
		{
			new_channel->SetTarget(eAnimTargetXYZ);
			new_channel->NumElementTargets = 3; 
		}
	}
	animation->NumAnimChannels = new_channel->NumElementTargets;
	return new_channel;
}
CrtAnimation *CrtScene::ReadAnimation( domAnimationRef animation )
{
	if (animation==NULL) return NULL;

	CrtPrint( "Add new Animation %s \n", animation->getId() );  

	if ( animation )
	{
		CrtAnimation * newAnim = CrtNew(CrtAnimation ); 
		CrtAssert("No memory\n", newAnim!=NULL);
		// get the animation name - target - keys and time 
		newAnim->SetName( animation->getId() );
		newAnim->SetDocURI( animation->getDocumentURI()->getURI() ); 

		// get the number of sources 
		domSource_Array & source_array = animation->getSource_array();
		for (CrtUInt i=0; i<source_array.getCount(); i++)
		{
			CrtAnimSrc * source = ReadAnimationSource(source_array[i]);
			if (source) newAnim->Sources[source->ID] = source;
		}

		domSampler_Array & sampler_array = animation->getSampler_array();
		for (CrtUInt s=0; s<sampler_array.getCount(); s++)
		{
			CrtAnimSampler * anim_sampler = ReadAnimationSampler(newAnim, sampler_array[s]);
			if (anim_sampler) newAnim->Samplers[sampler_array[s]->getID()] = anim_sampler;
		}

		domChannel_Array & channel_array = animation->getChannel_array();
		for (CrtUInt i=0; i<channel_array.getCount(); i++)
		{
			CrtAnimChannel * channel = ReadAnimationChannel(newAnim, channel_array[i]);
			if (channel) newAnim->Channels.push_back(channel);
		}

		if (newAnim->Channels.size() > 0)
		{
			newAnim->GenerateKeys(); 
			Animations.push_back(newAnim);
			// also get it's last key time and first key time 
			if( newAnim->GetEndTime() > LastKeyTime )
				LastKeyTime = newAnim->GetEndTime(); 
		} else
		{
			CrtPrint("No Channel found in this animation\n");
			CrtDelete(newAnim);
			return NULL;
		}
		return newAnim;
	}
	return NULL;
};

CrtMorph *CrtScene::ReadMorph( domMorphRef lib )
{
	const CrtChar * morph_weight_id = NULL;
	domGeometry * base_dom_geometry = (domGeometry*) (domElement*) lib->getSource().getElement();
	if (base_dom_geometry == NULL)		// base geometry no found
		return NULL;

	// base geometry
	CrtGeometry * base_geometry =  ReadGeometry(base_dom_geometry);
	if (base_geometry == NULL)
		return NULL;

	CrtMorph * new_morph = CrtNew(CrtMorph);
	new_morph->SetBaseGeometry(base_geometry);

	new_morph->type = CrtController_Morph_Types;

	// morph method
	domMorphMethodType method = lib->getMethod();
	if (method == MORPHMETHODTYPE_NORMALIZED)
		new_morph->SetMorphType(MORPH_NORMAL_TYPE);
	else if (method == MORPHMETHODTYPE_RELATIVE)
		new_morph->SetMorphType(MORPH_RELATIVE_TYPE);

	// parse targets
	domMorph::domTargetsRef targets = lib->getTargets();
	domInputLocal_Array & inputs = targets->getInput_array();
	for (size_t i=0; i<inputs.getCount(); i++)
	{
		const CrtChar * semantic = inputs[i]->getSemantic();
		domSource * source = (domSource*) (domElement*) inputs[i]->getSource().getElement();
		if (source && semantic)
		{
			if (CrtICmp(semantic, "MORPH_TARGET"))
			{
				domIDREF_array * idref_array = source->getIDREF_array();
				domUint count = idref_array->getCount();
				xsIDREFS & idrefs = source->getIDREF_array()->getValue();
				for (size_t j=0; j<count; j++)
				{
					domGeometry * target_geometry_element = (domGeometry*) (domElement*) idrefs[j].getElement();
					if (target_geometry_element)
					{
						CrtGeometry * base_geometry = ReadGeometry(target_geometry_element);
						new_morph->targets.push_back(base_geometry);
					}
				}
			} else if (CrtICmp(semantic, "MORPH_WEIGHT"))
			{
				morph_weight_id = source->getID();
				domFloat_array * float_array = source->getFloat_array();
				domListOfFloats & floats = float_array->getValue();
				for (size_t j=0; j<float_array->getCount(); j++)
				{
					new_morph->weights.push_back((CrtFloat) floats[j]);
				}
			} else {
				CrtPrint("ReadMorph semantic=%s no understood\n", semantic);
			}
		}
	}

	new_morph->morphed_geometry = CrtNew(CrtGeometry);
	CrtAssert("No memory\n", new_morph->morphed_geometry!=NULL);
	ParseGeometry(new_morph->morphed_geometry, base_dom_geometry);
	Geometries.push_back(new_morph->morphed_geometry);

	new_morph->Update(-1.0f);

	// check if there are animation on morph weight
	for (size_t i=0; i<Animations.size(); i++)
	{
		CrtAnimation * animation = Animations[i];
		for (size_t j=0; j<animation->Channels.size(); j++)
		{
			CrtAnimChannel * channel = animation->Channels[j];
			if (CrtICmp(morph_weight_id, channel->GetTargetID()))
			{
				new_morph->Animations.push_back(animation);
			}
		}
	}

	return new_morph;
}

CrtController *CrtScene::ReadController( domControllerRef lib )
{
	if (lib->getId()==NULL) return NULL;
	CrtPrint( "Add new controller %s \n", lib->getId() );
	CrtController * new_crtcontroller = 0;
	domMorph * morphElement = lib->getMorph();
	if (morphElement) 
		new_crtcontroller = ReadMorph(morphElement);

	domSkin *skinElement	= lib->getSkin();
	if (skinElement) // no skin in this controller DO LATER, we will support non-skin controller later
		new_crtcontroller = ReadSkin(skinElement);

	if (new_crtcontroller)
		Controllers.push_back(new_crtcontroller);

	return new_crtcontroller;
}

CrtSkin *CrtScene::ReadSkin( domSkinRef skinElement )
{
	if (skinElement==NULL) // no skin in this controller DO LATER, we will support non-skin controller later
		return NULL;

	domController *lib = (domController *) skinElement->getParentElement();

	domElement * source_element = skinElement->getSource().getElement();
	if (source_element==NULL) // skin source not found.
		return NULL;
	if (source_element->getElementType() != COLLADA_TYPE::GEOMETRY){ // skin source is not geometry, DO LATER
		CrtPrint("Skin=%s source is not geometry, not supported\n", lib->getId());
		return NULL;
	}

	// Make a CrtSkin to hold this info
	CrtSkin *newSkin = CrtNew( CrtSkin );
	CrtAssert("No memory\n", newSkin!=NULL);

	// set the geometry
	newSkin->base_geometry = ReadGeometry((domGeometry *) source_element);
	if (newSkin->base_geometry==NULL)	{		// target skin is not found
		CrtDelete(newSkin);
		return NULL;
	}

	// Allocate space for the new geometry object 
	newSkin->skinned_geometry     = CrtNew(CrtGeometry);
	CrtAssert("No memory\n", newSkin->skinned_geometry!=NULL);
	ParseGeometry(newSkin->skinned_geometry, (domGeometry*) source_element);

	newSkin->type = CrtController_Skin_Types;

	// Get the targetid from the <skin> and set it in newController
//	char *targetID = (char *)skinElement->getSource().getID(); 
//	newSkin->SetTargetId( targetID ); 
	newSkin->SetName( lib->getId() ); 
	newSkin->SetDocURI( lib->getDocumentURI()->getURI() ); 

	if (skinElement->getBind_shape_matrix())
	{
		for (CrtUInt m=0; m<16; m++)
		{
			newSkin->BindShapeMats[m] = (CrtFloat)skinElement->getBind_shape_matrix()->getValue()[m];
		}
	}

	// Find the <joints> and <vertex_weights> elements inside <skin>
	domSkin::domJoints			*jointsElement			= skinElement->getJoints();
	domSource					*jointNamesSource		= NULL;
	domSource					*invBindMatsSource		= NULL;

	// Scan the <joint> looking for specific <input> semantics and remember where they are
	for(CrtUInt input = 0; input < jointsElement->getInput_array().getCount(); input++)
	{
		if(CrtICmp(jointsElement->getInput_array()[input]->getSemantic(),"JOINT"))
		{
			// Found the JOINT input semantic
			jointNamesSource = (domSource *)((daeElement *)jointsElement->getInput_array()[input]->getSource().getElement());
		}
		else if(CrtICmp(jointsElement->getInput_array()[input]->getSemantic(),"INV_BIND_MATRIX"))
		{
			// Found the INV_BIND_MATRIX semantic
			invBindMatsSource = (domSource *)((daeElement *)jointsElement->getInput_array()[input]->getSource().getElement());
		}
	}

	// Find the inputs we want from <vertex_weights>
	domSkin::domVertex_weights	*vertexWeightsElement	= skinElement->getVertex_weights();
	domSource	*weightsSource	= NULL;

	// Scan the <vertex_weights> looking for specific <input> semantics and remember them
	for(CrtUInt input = 0; input < vertexWeightsElement->getInput_array().getCount(); input++)
	{
		if(CrtICmp(vertexWeightsElement->getInput_array()[input]->getSemantic(),"WEIGHT"))
		{
			// Found the WEIGHT semantic
			weightsSource = (domSource *)((daeElement *)vertexWeightsElement->getInput_array()[input]->getSource().getElement());
		}
	}

	// Allocate space for the joint names and copy them
	CrtJoint	*jointNames = 0;
	CrtUInt 	jointCount=0;
	if (jointNamesSource->getName_array()) {
		jointCount = (CrtUInt) jointNamesSource->getName_array()->getCount();
		jointNames = CrtNewData(CrtJoint, jointCount);
		CrtAssert("No memory\n", jointNames!=NULL);
		for (CrtUInt n = 0; n< jointCount; n++)
		{
			CrtCpy( jointNames[n].Name, jointNamesSource->getName_array()->getValue()[n] );
		}
		newSkin->JointsType = CrtJoint::CrtJointsType_SID;
	} else if (jointNamesSource->getIDREF_array()) {
		jointCount	= (CrtUInt) jointNamesSource->getIDREF_array()->getCount();
		jointNames  = CrtNewData(CrtJoint, jointCount);
		CrtAssert("No memory\n", jointNames!=NULL);
		for (CrtUInt n = 0; n< jointCount; n++)
		{
			CrtCpy( jointNames[n].Name, jointNamesSource->getIDREF_array()->getValue()[n].getID() );
		}
		newSkin->JointsType = CrtJoint::CrtJointsType_ID;;
	} else {
		CrtAssert("Should never assert here\n", false);
	}
	// set the JointNames 
	newSkin->Joints		= jointNames; 
	newSkin->JointCount	= jointCount; 

	// Allocate space for the inverse bind matrices and copy them
	CrtUInt 	invBindMatsCount	= (CrtUInt) invBindMatsSource->getFloat_array()->getCount();
	CrtMatrix	*inverseBindMats	= CrtNewData( CrtMatrix, invBindMatsCount/16 );
	CrtAssert("No memory\n", inverseBindMats!=NULL);
	CrtFloat	*iBMfloat			= (CrtFloat *)inverseBindMats; 
    for (CrtUInt m = 0; m< invBindMatsCount; m++)
	{
		iBMfloat[m] = (CrtFloat)invBindMatsSource->getFloat_array()->getValue()[m];
	}
	// Transpose all of the matrices 
	for (CrtUInt m = 0; m < invBindMatsCount/16; m++)
	{
		CrtMatrixTranspose(inverseBindMats[m],inverseBindMats[m]); 
	}
	// set the InverseBindMats
	newSkin->AddIBindMats( inverseBindMats, invBindMatsCount/16 );
	newSkin->IBindMatCount = invBindMatsCount/16; 
	CrtDeleteData(inverseBindMats);

	// Allocate space for the joint weights
	CrtUInt 		vertexWeightsCount	= (CrtUInt) vertexWeightsElement->getCount(); 
	CrtSkinWeight	*weights			= CrtNewData( CrtSkinWeight, vertexWeightsCount );
	CrtAssert("No memory\n", weights!=NULL);
	// <vcount> tells how many bones are associated with each vertex, this indicates how many
	// pairs of joint/weight indices to process out of the <v> array for this vertex.
	// get pointers to the vcount and v arrays
	domSkin::domVertex_weights::domVcount *vcountElement	= vertexWeightsElement->getVcount();
	domSkin::domVertex_weights::domV	  *vElement			= vertexWeightsElement->getV();
	CrtUInt vPos = 0;
	// For each vertex in <vcount>
	for(CrtUInt vertex = 0; vertex < vertexWeightsCount; vertex++)
	{
		// Find number of bones (joints/weights) this vertex influences and allocate space to store them
		CrtUInt numInfluences		= (CrtUInt) vcountElement->getValue()[vertex];
		weights[vertex].NumWeights	= numInfluences; 

		weights[vertex].Weights		= CrtNewData( CrtFloat, numInfluences );
		CrtAssert("No memory\n", weights[vertex].Weights!=NULL);

		weights[vertex].Joints		= CrtNewData( CrtUInt, numInfluences ); 
		CrtAssert("No memory\n", weights[vertex].Joints!=NULL);
		// For each bone, copy in the joint number and the actual float value in the weights (indexed by the 
		// second value in the <v> array
		for(CrtUInt inf = 0; inf < numInfluences; inf++)
		{
			weights[vertex].Joints[inf]		= (CrtUInt) vElement->getValue()[vPos++];
			weights[vertex].Weights[inf]	= (CrtFloat)weightsSource->getFloat_array()->getValue()[(size_t)(vElement->getValue()[vPos++])];
		}
		weights[vertex].SortWeights(); 
	}
	// set the Weight
	newSkin->Weights = weights;
	newSkin->WeightCount = vertexWeightsCount;
	
//	newGeo->Weights = CrtNewData( CrtWeight, newGeo->vertexcount );
//		for (CrtUInt p=0; p< newGeo->vertexcount; p++)
//			newGeo->Weights[p].Copy( &skin->Weights[newGeo->SkinIndex[p]] );


	return newSkin;
};

// ------------ Read Scene ---------------------
CrtBool CrtScene::ReadScene( domVisual_sceneRef scene )
{
	// create the scene root 
	SceneRoot = CrtNew(CrtNode);
	CrtAssert("No memory\n", SceneRoot!=NULL);
	// get the scene name 
	if ( scene->getName() ) 
		SceneRoot->SetName( scene->getName() ); 
	if ( scene->getID() ) 
		SceneRoot->SetId( scene->getID() ); 

	CrtPrint(" CrtScene::Reading Collada Scene %s\n", scene->getName()); 	

	// recurse through the scene, read and add nodes 
	for ( CrtUInt i = 0; i < scene->getNode_array().getCount(); i++)
	{
		CrtNode * node = ReadNode( scene->getNode_array()[i], SceneRoot );
		if (node) 
		{
			Nodes[node->GetId()] = node;
		}
	}
	Nodes[SceneRoot->GetId()] = SceneRoot;

	// for each skin controller find the nodes needed for skinning 
	for (CrtUInt i=0; i<Controllers.size(); i++)
	{	// for each controller
		CrtController * controller = Controllers[i];
		if (controller->type==CrtController_Skin_Types)
		{
			CrtSkin * skin = (CrtSkin *) controller;
			if (skin->JointsType == CrtJoint::CrtJointsType_SID)
			{	// bind joints using SID
				for(size_t j=0; j<Controllers[i]->skeletons.size(); j++)
				{	// for each skeletons
					string skeleton_id = Controllers[i]->skeletons[j];
					CrtNode * node = Nodes[skeleton_id];
					if (node)
						skin->FindJointNodes( node );
				}
			} else if (skin->JointsType == CrtJoint::CrtJointsType_ID)
			{	// bind joints using ID
				skin->FindJointNodes( SceneRoot );
			}
		}
	}
	return CrtTrue; 
}

CrtBool CrtScene::ReadNodeTranforms( CrtNode * crtNode, domNodeRef node, CrtNode * parentNode )
{
	// just to get ride of the warning 
	parentNode=parentNode;

	// load the node transformations as they are to be able to 
	// handle any matrix stack configurations independant of the tools 
	for ( CrtUInt i = 0; i < node->getContents().getCount(); i++)
	{
		// get the component type string
		CrtTransform * transform = NULL;
		CrtChar * sid = NULL;
		CrtChar * typeName = (CrtChar *)node->getContents()[i]->getTypeName(); 
		CrtTFormType type = CrtTransform::GetType( typeName ); 
		// set the type of the transform and switch on it 
		switch( type )
		{
		case eCrtRotate:
		{
			// need to create the crttransforms to store the node transform stack 
			transform = CrtNew( CrtTransform );
			CrtAssert("No Free memory\n", transform!=NULL);
			transform->SetSid( ""); 
			transform->SetType( type ); 
			// load rotation
			domRotateRef rotateArray = (domRotate * )(domElement * )node->getContents()[i];
	
			assert( rotateArray->getValue().getCount() == 4 ); 
			// get the rotation data 
			CrtVec4f rot;
			rot.x = (CrtFloat)rotateArray->getValue()[0];
			rot.y = (CrtFloat)rotateArray->getValue()[1];
			rot.z = (CrtFloat)rotateArray->getValue()[2];
			rot.w = (CrtFloat)rotateArray->getValue()[3];
			
			transform->SetRotate( rot );

			// this will be used to bind to an animation later 
			sid = (CrtChar *)rotateArray->getSid();
			if ( sid )
				transform->SetSid( sid );
			
			crtNode->Transforms.push_back(transform);
		}
			break;
		case eCrtTranslate:
		{
			// need to create the crttransforms to store the node transform stack 
			transform = CrtNew( CrtTransform );
			CrtAssert("No Free memory\n", transform!=NULL);
			transform->SetSid( ""); 
			transform->SetType( type ); 
			// load rotation
			domTranslateRef translateArray = (domTranslate * )(domElement * )node->getContents()[i];
			assert( translateArray->getValue().getCount() == 3 ); 
			
			// get the transation data 
			CrtVec4f trans;
			trans.x = (CrtFloat)translateArray->getValue()[0];
			trans.y = (CrtFloat)translateArray->getValue()[1];
			trans.z = (CrtFloat)translateArray->getValue()[2];			

			transform->SetTranslate( trans ); 
			
			// this will be used to bind to an animation later 
			sid = (CrtChar *)translateArray->getSid();
			if ( sid )
				transform->SetSid( sid ); 
			
			crtNode->Transforms.push_back(transform);
		}
			break;
		case eCrtScale:
		{
			// need to create the crttransforms to store the node transform stack 
			transform = CrtNew( CrtTransform );
			CrtAssert("No Free memory\n", transform!=NULL);
			transform->SetSid( ""); 
			transform->SetType( type ); 
			// load scale 
			domScaleRef scaleArray = (domScale * )(domElement * )node->getContents()[i];
			assert( scaleArray->getValue().getCount() == 3 ); 
			
			// get the rotation data 
			CrtVec4f scale;
			scale.x = (CrtFloat)scaleArray->getValue()[0];
			scale.y = (CrtFloat)scaleArray->getValue()[1];
			scale.z = (CrtFloat)scaleArray->getValue()[2];
				
			transform->SetScale( scale ); 
			
			// this will be used to bind to an animation later 
			sid = (CrtChar *)scaleArray->getSid();
			if ( sid )
				transform->SetSid( sid );
			
			crtNode->Transforms.push_back(transform);
		}
			break;

		case eCrtLookAt:
		{
			// need to create the crttransforms to store the node transform stack 
			transform = CrtNew( CrtTransform );
			CrtAssert("No Free memory\n", transform!=NULL);
			transform->SetSid( ""); 
			transform->SetType( type ); 
			// load rotation
			domLookatRef lookatArray = (domLookat * )(domElement * )node->getContents()[i];
	
			assert( lookatArray->getValue().getCount() == 9); 
			// get the lkatation data 
			CrtVec3f lkat[3];
			lkat[0].x = (CrtFloat)lookatArray->getValue()[0];
			lkat[0].y = (CrtFloat)lookatArray->getValue()[1];
			lkat[0].z = (CrtFloat)lookatArray->getValue()[2];

			lkat[1].x = (CrtFloat)lookatArray->getValue()[3];
			lkat[1].y = (CrtFloat)lookatArray->getValue()[4];
			lkat[1].z = (CrtFloat)lookatArray->getValue()[5];

			lkat[2].x = (CrtFloat)lookatArray->getValue()[6];
			lkat[2].y = (CrtFloat)lookatArray->getValue()[7];
			lkat[2].z = (CrtFloat)lookatArray->getValue()[8];
			transform->SetLookAt(lkat); 
						
			sid = (CrtChar *)lookatArray->getSid();
			if ( sid )
				transform->SetSid( sid ); 

			crtNode->Transforms.push_back(transform);
		}
			break; 
		case eCrtMatrix:
		{
			// need to create the crttransforms to store the node transform stack 
			transform = CrtNew( CrtTransform );
			CrtAssert("No Free memory\n", transform!=NULL);
			transform->SetSid( ""); 
			transform->SetType( type ); 
			// beware, RT is row major, collada spec is column major
			domMatrixRef matrixArray = (domMatrix * )(domElement * )node->getContents()[i];
			CrtMatrix mat;
			for (int m=0; m<16; m++)
				mat[m] = (CrtFloat) matrixArray->getValue()[m];
			CrtMatrixTranspose(mat, mat);
			transform->SetMatrix(mat);
			sid = (CrtChar *)matrixArray->getSid();
			if ( sid )
				transform->SetSid( sid ); 

			crtNode->Transforms.push_back(transform);
		}
			break;
		case eCrtSkew:
			CrtPrint("Skew transforms not supported yet\n");
			continue;
		case eCrtTFormUnknown:
			// If it's not a transform, it's an instance or something else that will be handled later.
			continue; 
		}

		for (CrtUInt i=0; i<Animations.size(); i++)
		{
			CrtAnimation * anim = Animations[i];
			if (anim->HasFoundTarget())
				continue;
			CrtBool found_target = CrtFalse;
			for (CrtUInt j=0; j<anim->Channels.size(); j++)
			{
				CrtChar * target_id = anim->Channels[j]->GetTargetID();
				CrtChar * target_sid = anim->Channels[j]->GetTargetSID();
				CrtChar * id = crtNode->GetId();
				if (target_id && target_sid && id && sid)
				{
					if (CrtICmp(target_id, id) &&
						CrtICmp(target_sid, sid))
					{
						anim->SetFoundTarget(); 
						transform->SetAnimation(anim); 
						transform->AddChannelId(j);
						found_target = CrtTrue;
						
						crtNode->SetAnimation( anim ); 
						// don't want to break, though there maybe be multiple anim targets 
						NumAnimatedNodes++;

						break;
					}
				}
			}
			if (found_target)
				break;
		}
	}	
/*
	// see if this node is the target for any of the animations we have loaded 
	//CrtPrint(" Checking for Animation Associations to %s \n", Name ); 
	for (CrtUInt i=0; i<Animations.size(); i++)
	{
		CrtAnimation * anim = Animations[i];
		if (CrtCmp( crtNode->GetId(), anim->GetTarget() ))
		{
			anim->SetFoundTarget(); 
            for ( CrtUInt t = 0; t < crtNode->Transforms.size(); t++)
			{
				// there could be multipule channels for one animation 
				for ( CrtUInt c = 0; c < anim->Channels.size(); c++)
				{
					if ( CrtICmp( crtNode->Transforms[t]->GetSid(), anim->Channels[c]->GetTargetElementString() ) )
					{
						crtNode->Transforms[t]->SetAnimation(anim); 
						crtNode->Transforms[t]->AddChannelId(c); 
					}
				}
			}
		}
	}
*/	return CrtTrue; 
}

CrtInstanceLight *CrtScene::ReadInstanceLight( domInstance_lightRef lib)
{
	xsAnyURI & urltype  = lib->getUrl();
	domElement * element = (domElement* ) urltype.getElement();
	if (element==NULL) // this instance light is not found skip to the next one
	{
		CrtPrint("*** Can't find this light: %s\n", urltype.getURI());
		return NULL;
	}

	CrtLight * light = ReadLight((domLight *) element);
	if (light)
	{
		CrtInstanceLight *instanceLight = CrtNew(CrtInstanceLight);
		CrtAssert("No memory\n", instanceLight!=NULL);
		instanceLight->AbstractLight = light;
		return instanceLight;
	}
	return NULL;
}

CrtInstanceCamera *CrtScene::ReadInstanceCamera( domInstance_cameraRef lib)
{
	xsAnyURI & urltype  = lib->getUrl();
	domElement * element = (domElement* ) urltype.getElement();
	if (element==NULL) // this instance light is not found skip to the next one
	{
		CrtPrint("*** Can't find this camera: %s\n", urltype.getURI());
		return NULL;
	}

	CrtCamera * camera = ReadCamera((domCamera *) element);
	if (camera)
	{
		CrtInstanceCamera *instanceCamera = CrtNew(CrtInstanceCamera);
		CrtAssert("No memory\n", instanceCamera!=NULL);
		instanceCamera->AbstractCamera = camera;
		return instanceCamera;
	}
	return NULL;
}
//	CrtInstanceEffect *ReadInstanceEffect( domInstance_effectRef lib){}
CrtInstanceMaterial *CrtScene::ReadInstanceMaterial( domInstance_materialRef lib)
{
	// Make a new CrtInstanceMaterial and add it to the list in newCrtInstanceGeometry
	CrtInstanceMaterial *newCrtInstanceMaterial = CrtNew(CrtInstanceMaterial);
	CrtAssert("No memory\n", newCrtInstanceMaterial!=NULL);
	newCrtInstanceMaterial->symbol = lib->getSymbol();
	newCrtInstanceMaterial->target = lib->getTarget().getID();

	// resolve bind target for fx materials
	std::map<std::string, cfxMaterial*>::iterator iter;
	for(iter = cfxMaterials.begin(); iter != cfxMaterials.end(); iter++)
	{
		std::string id = (*iter).first;
		if (id==newCrtInstanceMaterial->target)
		{
			newCrtInstanceMaterial->targetcfxMaterial = (*iter).second;
			break;
		}
	}

	if (newCrtInstanceMaterial->targetcfxMaterial==NULL)
	{   // can't resolve fx materials, try common materials
		domElement * element = lib->getTarget().getElement();
		if (element)
		{
			CrtMaterial * material = ReadMaterial((domMaterial *) element);
			if (material) // material added or found
			{
				newCrtInstanceMaterial->targetMaterial = material;				
			} else
			{
				CrtPrint("*** Couldn't find instance Materials for binding: %s\n",newCrtInstanceMaterial->target.c_str());
			}
		}
	}

	return newCrtInstanceMaterial;
}
CrtInstanceGeometry *CrtScene::ReadInstanceGeometry( domInstance_geometryRef lib)
{
	
	// Find the <geometry> the <instance_geometry> is pointing at (there can be only one)
	// by searching the Geometries list in the scene.
//	domInstance_geometry *instanceGeometry = node->getInstance_geometry_array()[i];
	xsAnyURI & urltype  = lib->getUrl();
//	const char * url	= urltype.getID(); //TODO: We might not need this
	urltype.resolveElement();
	domElement * element = (domElement* ) urltype.getElement();
	if (element==NULL) // this instance geometry is not found skip to the next one
		return NULL;

	CrtGeometry * geo = ReadGeometry((domGeometry* ) element);
	if (geo==NULL)			// couldn't find from existing pool of geometries
		return NULL;

	CrtInstanceGeometry *newCrtInstanceGeometry = CrtNew(CrtInstanceGeometry);
	CrtAssert("No memory\n", newCrtInstanceGeometry!=NULL);
	newCrtInstanceGeometry->AbstractGeometry = geo;

	domBind_material *bindMaterial =  lib->getBind_material();
	if(bindMaterial)
	{
		// Get the <technique_common>
		domBind_material::domTechnique_common *techniqueCommon = bindMaterial->getTechnique_common();
		if(techniqueCommon)
		{
			// Get the <instance_material>s
			domInstance_material_Array &instanceMaterialArray = techniqueCommon->getInstance_material_array();
			for(CrtUInt j = 0; j < instanceMaterialArray.getCount(); j++)
			{
				CrtInstanceMaterial * newiMaterial = ReadInstanceMaterial(instanceMaterialArray[j]);
				newCrtInstanceGeometry->MaterialInstances.push_back(newiMaterial);
			}
		}
	}

	return newCrtInstanceGeometry;
}

CrtInstanceController *CrtScene::ReadInstanceController( domInstance_controllerRef lib)
{
	xsAnyURI & urltype  = lib->getUrl();
	domElement * element = urltype.getElement();
	domController * controller = (domController *) element;
	CrtController * ctrl = ReadController(controller);
	if (ctrl==NULL)		// controller not found
		return NULL;

	CrtInstanceController *newiController = CrtNew(CrtInstanceController);
	CrtAssert("No memory\n", newiController!=NULL);
	newiController->AbstractController = ctrl;
//	newiController->AbstractGeometry = ctrl->Geometry;

	domBind_materialRef bind_material = lib->getBind_material();
	if(bind_material)
	{
		// Get the <technique_common>
		domBind_material::domTechnique_common *techniqueCommon = bind_material->getTechnique_common();
		if(techniqueCommon)
		{
			// Get the <instance_material>s
			domInstance_material_Array &instance_material_array = techniqueCommon->getInstance_material_array();
			for(CrtUInt j = 0; j < instance_material_array.getCount(); j++)
			{
				CrtInstanceMaterial * newiMaterial = ReadInstanceMaterial(instance_material_array[j]);
				newiController->MaterialInstances.push_back(newiMaterial);
			}
		}
	}

	domInstance_controller::domSkeleton_Array & skeleton_array = lib->getSkeleton_array();
	if (skeleton_array.getCount() > 1)
		CrtPrint("There are more than one skeleton, RT is not supported yet\n");

	if (skeleton_array.getCount() > 0)
	{
		domNode * node = (domNode*) (domElement*) skeleton_array[0]->getValue().getElement();
		if (node) ctrl->skeletons.push_back(node->getID());
	}
	return newiController;
}

CrtNode * CrtScene::ReadNode( domNodeRef node, CrtNode * parentNode )
{
	CrtNode * findnode = GetNode(node->getId(), NULL);
	if (findnode) return findnode;
	CrtPrint(" CrtScene::Reading Scene Node %s \n", node->getId() );

	CrtNode * crtNode = CrtNew( CrtNode );
	// Create a new node and initialize it with name and parent pointer
	CrtAssert("No memory\n", crtNode!=NULL);
	if ( node->getName() ) crtNode->SetName( node->getName() ); 
	if ( node->getId() ) crtNode->SetId( node->getId() ); 
	if ( node->getSid() ) crtNode->SetSid( node->getSid() ); 

//	crtNode->SetDocURI( node->getDocumentURI()->getURI() ); 
	crtNode->SetParent( parentNode ); 

	// future support method that will support any rot/trans/scale matrix combination 
	ReadNodeTranforms( crtNode, node, parentNode);
  	// Process Instance Geometries
	for (CrtUInt i = 0; i < node->getInstance_geometry_array().getCount(); i++)
	{
		CrtInstanceGeometry * instanceGeometry = ReadInstanceGeometry(node->getInstance_geometry_array()[i]);
		if ( instanceGeometry == NULL ) continue;
		instanceGeometry->Parent = crtNode;
		crtNode->InstanceGeometries.push_back(instanceGeometry);
		GeometryInstances.push_back(instanceGeometry);
	}

	// Process Instance Controllers 
	for (CrtUInt i = 0; i < node->getInstance_controller_array().getCount(); i++)
	{
		domInstance_controller *icontroller  = node->getInstance_controller_array()[i];
		CrtInstanceController * instanceController = ReadInstanceController(icontroller);
		if (instanceController==NULL) //if instance Controller can not be created, skip to the next one
			continue;

		instanceController->Parent = crtNode;
		crtNode->InstanceControllers.push_back(instanceController);
		ControllerInstances.push_back(instanceController);
	}

	// Process Instance Lights 
	for (CrtUInt i = 0; i < node->getInstance_light_array().getCount(); i++)
	{
		CrtInstanceLight * instanceLight = ReadInstanceLight(node->getInstance_light_array()[i]);
		if (instanceLight) {
			instanceLight->Parent = crtNode;
			crtNode->InstanceLights.push_back(instanceLight);
			LightInstances.push_back(instanceLight);
		}
	}

	// Process Instance Cameras 
	for (CrtUInt i = 0; i < node->getInstance_camera_array().getCount(); i++)
	{	
		CrtInstanceCamera *instanceCamera = ReadInstanceCamera(node->getInstance_camera_array()[i]);
		if (instanceCamera) {
			instanceCamera->Parent = crtNode;
	//		crtNode->InstanceCameras.push_back(instanceCamera);
			CameraInstances.push_back(instanceCamera);
		}
	}

	// see if this node is the target for any of the animations we have loaded 
/*	for(CrtUInt i=0; i<Animations.size(); i++)
	{
		CrtAnimation * anim = Animations[i];
		if ( CrtCmp( crtNode->GetId(), anim->GetTarget() ) )
//		if ( CrtCmp( crtNode->GetName(), anim->GetTarget() ) )
		{
			anim->SetFoundTarget(); 
			crtNode->SetAnimation( anim ); 
			// don't want to break, though there maybe be multiple anim targets 
			NumAnimatedNodes++;
		}
	}
*/
	// add to parent 
	parentNode->AddChild( crtNode ); 

	// read children 
	for (CrtUInt i = 0; i < node->getNode_array().getCount(); i++)
	{
		// read in each child an recursively it's children 
		CrtNode * readnode  = ReadNode( node->getNode_array()[i], crtNode );
		if (readnode) Nodes[readnode->GetId()] = readnode;
	}

		// read children <instance_nodes>, can be 0 or more 
	for (CrtUInt i = 0; i < node->getInstance_node_array().getCount(); i++)
	{
		// read in each child an recursively it's children 
		domInstance_node * instance_node = node->getInstance_node_array()[i];
		domNode * urlnode = (domNode*) (domElement*) instance_node->getUrl().getElement();
		if (urlnode) {
			CrtNode * readnode = ReadNode( urlnode, crtNode );
			if (readnode) Nodes[readnode->GetId()] = readnode;
		}
	}

	return crtNode; 
};
/*
CrtVoid		CrtScene::IndexNodes()
{	
	CrtInt idx = 0; 
	IndexNodes( SceneRoot, idx );    
}

CrtVoid		CrtScene::IndexNodes( CrtNode * node, CrtInt & idx )
{
	CrtNode * cnodes = node->GetChild();
	CrtNode * snodes = node->GetNext(); 

	
	node->SetIdx( idx ); 
	CrtPrint("CrtScene::Setting Node %s Idx %d \n", node->GetName(), idx );
	idx++; 

	// index child
	if ( cnodes )
		IndexNodes( cnodes, idx );

	// index all the siblings 
	if ( snodes )
		IndexNodes( snodes, idx );	

	node->SetDepth(); 
	NumNodes = idx; 
}
*/
/* // New Skins methods for 1.4
CrtVoid		CrtScene::RemapWeights()
{
	// first set indexs for each node int he scene 
	IndexNodes();

	// now go threw the geometries 
	// for the geometries with a skin controller 
	// we need to update their weights ids to the new joint ids 
	// to globalize the weight mappings to the scene 
	// heiarchy and not just it's relavent joint list  

	CrtGeometry * geo = Geometries;

	while( geo )
	{
		if( geo->HasSkinController() )
			geo->RemapWeights(); 

		geo = (CrtGeometry *)geo->GetNext(); 
	}
}	
*/
CrtChar *CrtScene::ReadCfxBinaryFilename( domExtra_Array &extra_array )
{
	for( CrtUInt i = 0; i < extra_array.getCount(); i++ ) 
	{
		if ( extra_array[i]->getType() && strcmp( extra_array[i]->getType(), "cfxBinary" ) == 0 )
		{
			domExtra *extra = extra_array[i];
			for ( CrtUInt c = 0; c < extra->getTechnique_array().getCount(); c++ ) 
			{
				if ( strcmp( extra->getTechnique_array()[c]->getProfile(), "COLLADA_RT" ) == 0 )
				{
					daeElementRefArray &children = extra->getTechnique_array()[c]->getContents();
					for ( CrtUInt j = 0; j < children.getCount(); j++ )
					{
						if ( strcmp( children[j]->getTypeName(), "any" ) == 0 && 
							 strcmp( children[j]->getElementName(), "cfxBinary" ) == 0 )
						{
							return (CrtChar*)(((domAny*)(daeElement*)children[j])->getAttributeValue(0));
						}
					}
				}
			}
		}
	}
	return NULL;
}
