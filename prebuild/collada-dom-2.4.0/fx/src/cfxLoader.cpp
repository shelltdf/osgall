/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes

#include <stdio.h>
#include <cstdlib>
#include <iostream>

//#include <Cg/cg.h>
#include <cfxPlatform.h>

// User includes

#include <cfxLoader.h>
#include <cfxAnnotatable.h>
#include <cfxAnnotate.h>
#include <cfxBindParam.h>
#include <cfxCode.h>
#include <cfxConnectParam.h>
#include <cfxDataMaker.h>
#include <cfxEffect.h>
#include <cfxGlPipelineSetting.h>
#include <cfxGlPipelineSettingMaker.h>
#include <cfxMaterial.h>
#include <cfxNewParam.h>
#include <cfxParamable.h>
#include <cfxPass.h>
#include <cfxSetParam.h>
#include <cfxShader.h>
#include <cfxSurface.h>
#include <cfxTechnique.h>
#include <cfxError.h>
#include <cfxBinaryUtil.h>
#include <cfxTypes.h>

#include <dae.h>

#include <dom/domEffect.h>
#include <dom/domMaterial.h>
#include <dom/domTypes.h>
#include <dom/domProfile_CG.h>
#include <dom/domCg_surface_type.h>
#include <dom/domConstants.h>

// the next section used to be in the header as private members of the class
// moved internal so external libraries don't need to deal with the dom stuff

#include <dom/domFx_newparam_common.h>
#include <dom/domFx_annotate_common.h>

#include <dom/domInstance_effect.h>


  // for effects
//  static void loadCodeArray(domFx_code_common_Array& codeArrayElement, cfxEffect* effect);
  static void loadNewParamArray(domFx_newparam_common_Array& newParamArrayElement, cfxParamable* paramable, cfxEffect* effect);
  static void loadNewParamArray(domCg_newparam_Array& newParamArrayElement, cfxParamable* paramable, cfxEffect* effect);
  static void loadProfileArray(domFx_profile_abstract_Array& profileArrayElement, cfxEffect* effect);
  static void loadAnnotateArray(domFx_annotate_common_Array& annotateArrayElement, cfxAnnotatable* annotatable, cfxEffect* effect);

  static bool hasCGProfile( domEffect *effect );

  // for materials
  static void loadSetParamArray(domInstance_effect::domSetparam_Array& setParamArrayElement, cfxParamable* paramable, cfxEffect* effect, cfxMaterial* material = NULL);

// the previous section used to be in the header as private members of the class
// moved internal so external libraries don't need to deal with the dom stuff


std::string cfxLoader::platformString( DEFAULT_CG_PLATFORM );

//This function loads the binary file format for the material and effects libraries.
cfxBool cfxLoader::loadMaterialsAndEffectsFromBinFile(const std::string& file, std::map<std::string, cfxMaterial*>& materials, 
					      std::map<std::string, cfxEffect*>& effects, CGcontext _context )
{
	return cfxBinaryUtil::load( file, materials, effects, _context );
}
cfxBool cfxLoader::saveBinFile(const std::string& file, const std::map<std::string, cfxMaterial*>& materials, 
					      const std::map<std::string, cfxEffect*>& effects, cfxBool replace )
{
	return cfxBinaryUtil::save( file, materials, effects, replace );
}

void cfxLoader::setBinaryLoadRemotePath( const std::string &path )
{
	cfxBinaryUtil::setRemotePath( path );
}

void cfxLoader::setPlatformString( const std::string &platform ) 
{
	platformString = platform;
}

std::string &cfxLoader::getPlatformString() 
{
	return platformString;
}

  // this routine will load the materials and effects from a COLLADA file and populate the maps 
  // it will resolve the references from materials to effects
  // passing a NULL context will create a new context for this effect 
cfxBool cfxLoader::loadMaterialsAndEffectsFromFile(const std::string& filename, std::map<std::string, cfxMaterial*>& materials, 
						std::map<std::string, cfxEffect*>& effects, CGcontext _context)
{
    // Instantiate the DAE object for the input file
  
    DAE *input = new DAE;
    
    // Load the COLLADA file
	
    cfxPrint("loading %s\n", filename.c_str());

    cfxError error = input->load(filename.c_str());
    if (error != DAE_OK)
	{
	    cfxPrint("load failed error:  %s\n", daeErrorString(error));
	    return false;
	}

    cfxBool result = loadMaterialsAndEffects(input, materials, effects, _context);

    // delete input;
    // DAE::cleanup();

    return result;
}
  
  // this routine will load the materials and effects and populate the maps 
  // it will resolve the references from materials to effects
  // passing a NULL context will create a new context for this effect 
cfxBool cfxLoader::loadMaterialsAndEffects(DAE *colladaAsset, std::map<std::string, cfxMaterial*>& materials, 
					std::map<std::string, cfxEffect*>& effects, CGcontext _context)
{
    cfxBool resultEffects, resultMaterials;
    resultEffects = loadEffects(colladaAsset, effects, _context);

    if (resultEffects)
	{
	    resultMaterials = loadMaterials(colladaAsset, materials, &effects);
	}
    else
	{
	    resultMaterials = loadMaterials(colladaAsset, materials, NULL);
	}

    return (resultEffects && resultMaterials);
}
  
  // this routine will load the effects and populate the map
  // passing a NULL context will create a new context for this effect 
cfxBool cfxLoader::loadEffects(DAE *colladaAsset, std::map<std::string, cfxEffect*>& effects, CGcontext _context)
{
    // How many effect elements are there?
    cfxUint effectElementCount = (cfxUint) colladaAsset->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_EFFECT);
    cfxPrint("There are %d effect elements in this file\n", effectElementCount); 

    if (effectElementCount > 0)
	{
	    cgSetErrorCallback(cfxCgErrorCallback);

	    if (_context == NULL)
		{
		    _context = cgCreateContext(); 
		    cgGLRegisterStates(_context);
		}
	}

    // Loop over all the effect elements
    for(cfxUint currentEffect = 0; currentEffect < effectElementCount; currentEffect++)
	{
	    // Get the next effect element
	    domEffect *effectElement;
	    // is the cast to daeElement** needed? tsl
//	    cfxError error = colladaAsset->getDatabase()->getElement((daeElement**)&effectElement,currentEffect, NULL, COLLADA_ELEMENT_EFFECT);
		daeElement * element = 0;
		cfxError error = colladaAsset->getDatabase()->getElement( &element,currentEffect, NULL, COLLADA_ELEMENT_EFFECT);
		effectElement = (domEffect *) element;
	    // ERROR CHECK! !!!tsl i'm not sure this is the correct error check for this statement.
	    if (error != DAE_OK)
		{
		    return false;
		}

	    // currently using managed textures to ensure that the texture objects are 
	    // on the correct texture unit where the program expects them to be.
#ifndef _LIB
	    cgGLSetManageTextureParameters(_context, true);
#endif
	    
		if( hasCGProfile( effectElement ) )
		{

		    cfxPrint("Loading effect %d\n", currentEffect);

		    cfxEffect* effect = new cfxEffect(effectElement->getId(), _context);

		    // load all the parts at the effect scope
		    // <asset>
		    // <annotate>
		    // <import>
		    // <image>
		    // <newparam>
		    // <profile_*>
		    loadAnnotateArray(effectElement->getAnnotate_array(), effect, effect);
		    loadNewParamArray(effectElement->getNewparam_array(), effect, effect);
		    loadProfileArray(effectElement->getFx_profile_abstract_array(), effect);
#ifndef _LIB
		    effect->apply();
#endif

		    effects.insert(std::make_pair(effect->getName(), effect));

		}
		else
		{
			// An effect with no profiles creates an effect with a name but a zero effect pointers
			// this can cause crashes later.
			cfxPrint("*** Effect %d with id %s has NO CG profile, this may cause crashes if the effect is used\n",currentEffect, effectElement->getId());
		}
	}

    return true;
}
  
  // this routine will load the materials and populate the material map 
  // if an effects map is provided it will resolve the references from materials to effects
cfxBool cfxLoader::loadMaterials(DAE *colladaAsset, std::map<std::string, cfxMaterial*>& materials, 
			      const std::map<std::string, cfxEffect*>* effects)
{
    // How many material elements are there?
    cfxUint materialElementCount = (cfxUint) colladaAsset->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_MATERIAL);

    cfxPrint("There are %d material elements in this file\n", materialElementCount); 

    // Loop over all the material elements
    for(cfxUint currentMaterial = 0; currentMaterial < materialElementCount; currentMaterial++)
	{
	    // Get the next material element
	    domMaterial *materialElement;
	    // is the cast to daeElement** needed? tsl
//	    cfxError error = colladaAsset->getDatabase()->getElement((daeElement**)&materialElement,currentMaterial, NULL, COLLADA_ELEMENT_MATERIAL);
		daeElement * element = 0;
	    cfxError error = colladaAsset->getDatabase()->getElement(&element,currentMaterial, NULL, COLLADA_ELEMENT_MATERIAL);
		materialElement = (domMaterial*) element;
	    // ERROR CHECK! !!!tsl i'm not sure this is the correct error check for this statement.
	    if (error != DAE_OK)
		{
		    return false;
		}

	    domInstance_effect* instanceEffectElement = materialElement->getInstance_effect();

		if ( !instanceEffectElement )
			continue; 

	    std::string effectId = instanceEffectElement->getUrl().getID();

	    cfxEffect* effect = NULL;

	    if (effects)
		{
		    std::map<std::string, cfxEffect*>::const_iterator effectFinder = effects->find(effectId);
		    if (effectFinder != effects->end())
			{
			    effect = effectFinder->second;
			}
		}

		// skip this one if empty
		if ( !effect ) 
			continue; 

	    cfxMaterial* material = new cfxMaterial(materialElement->getId(), effectId, effect);
	    
	    cfxPrint("Created material %p name %s effect %s %p\n", material,  materialElement->getName(), effectId.c_str(), effect);

	    // now extract the setparams
	    loadSetParamArray(instanceEffectElement->getSetparam_array(), material, effect, material);

	    materials.insert(std::make_pair(material->getName(), material));

	}

    return true;
}



cfxBool cfxLoader::loadFile(const std::string& file, std::vector<cfxEffect*>& effects, CGcontext _context)
{

    // Instantiate the DAE object for the input file
  
    DAE *input = new DAE;
    
    int error; 

    // Load the COLLADA file
	
    cfxPrint("loading %s\n", file.c_str());

    error = input->load(file.c_str());
    if (error != DAE_OK)
	{
	    //cerr<<"load failed "<<daeErrorString(error)<<"\n";
	    return false;
	}

    // How many effect elements are there?

    unsigned int effectElementCount = input->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_EFFECT);

    cfxPrint("There are %d effect elements in this file\n", effectElementCount); 

	if (effectElementCount > 0)
		{
			cgSetErrorCallback(cfxCgErrorCallback);

			if (_context == NULL)
			{
				_context = cgCreateContext(); 
			    cgGLRegisterStates(_context);
			}
		}
    // Loop over all the effect elements

    for(unsigned int currentEffect = 0; currentEffect < effectElementCount; currentEffect++)
	{
	    // Get the next effect element
	    domEffect *effectElement;
	    // is the cast to daeElement** needed? tsl
//	    error = input->getDatabase()->getElement((daeElement**)&effectElement,currentEffect, NULL, COLLADA_ELEMENT_EFFECT);
		daeElement * element = 0;
	    error = input->getDatabase()->getElement(&element,currentEffect, NULL, COLLADA_ELEMENT_EFFECT);
		effectElement = (domEffect*) element;
	    // ERROR CHECK! !!!tsl i'm not sure this is the correct error check for this statement.
	    if (error != DAE_OK)
		{
		    return false;
		}

	    // currently using managed textures to ensure that the texture objects are 
	    // on the correct texture unit where the program expects them to be.
	    cgGLSetManageTextureParameters(_context, true);
	    
	    unsigned int profileCount = (unsigned int) effectElement->getFx_profile_abstract_array().getCount();
	    cfxPrint("effect %d with id %s has %d profiles\n", currentEffect, effectElement->getId(), profileCount);
	    if (profileCount > 0)
		{

	    cfxPrint("Loading effect %d\n", currentEffect);

	    cfxEffect* effect = new cfxEffect(effectElement->getId(), _context);

	    // load all the parts at the effect scope
	    // <asset>
	    // <annotate>
	    // <import>
	    // <image>
	    // <newparam>
	    // <profile_*>
	    loadAnnotateArray(effectElement->getAnnotate_array(), effect, effect);
	    loadNewParamArray(effectElement->getNewparam_array(), effect, effect);
	    loadProfileArray(effectElement->getFx_profile_abstract_array(), effect);
      
		cfxPrint("Done creating effect\n");

		effects.push_back(effect);

#ifndef _LIB
	    effect->apply();
#endif
		}
	}

	cfxPrint("Done resolving effect\n");

    // delete input;
    // DAE::cleanup();

    return true;
}



void loadAnnotateArray(domFx_annotate_common_Array& annotateArrayElement, cfxAnnotatable* annotatable, cfxEffect* effect)
{
    // utilize annotatable class to create and push annotate into list
   cfxUint annotateCount = (cfxUint) annotateArrayElement.getCount();
  
    // loop over the annotate blocks
    for (cfxUint currentAnnotate = 0; currentAnnotate < annotateCount; currentAnnotate++)
	{
	    // this element has a name and the data as child elements
	    domFx_annotate_common* annotateElement = annotateArrayElement.get(currentAnnotate);

	    // then use the element which holds the actual values to make the data
	    cfxData* annotateData = cfxDataMaker::makeDataForAnnotate(annotateElement->getFx_annotate_type_common(), effect);

	    // make the annotate and push it into the array where it belongs
	    annotatable->pushAnnotate(new cfxAnnotate(annotateData, std::string(annotateElement->getName())));
	}
      
}


/*
  void cfxLoader::loadCodeArray(domFx_code_common_Array& codeArrayElement, cfxEffect* effect)
  {
  // code in the effect elements may be used by one or more techniques
  int codeCount = codeArrayElement.getCount();
  // loop over the code blocks
  for (int currentCode = 0; currentCode < codeCount; currentCode++)
  {
  domFx_code_common* codeElement = codeArrayElement.get(currentCode); 
  // since we can't compile, perhaps value is not the right field...  I think we want a file pointer to the fpo or vbo.
  cfxCode* code = new cfxCode(effect, codeElement->getSid(), codeElement->getProfile(), codeElement->getValue());	    
  effect->pushCode(code);
  }
      
  }
*/


namespace {
	domFx_surface_init_from_common_Array* getInitFromArray(domFx_surface_common_complexType* surface) {
		if (surface && surface->getFx_surface_init_common())
			return &surface->getFx_surface_init_common()->getInit_from_array();
		return NULL;
	}
}


void loadNewParamArray(domFx_newparam_common_Array& newParamArrayElement, cfxParamable* paramable, cfxEffect* effect)
{

    // the newParam array contains parameters that are not in the shader code
    cfxUint newParamCount = (cfxUint) newParamArrayElement.getCount();

    // loop over the <newparam> objects, looking for <surface> params to extract.
    for (cfxUint currentNewParam = 0; currentNewParam < newParamCount; currentNewParam++)
	{
	    domFx_newparam_common* newParamElement = newParamArrayElement.get(currentNewParam);  

	    domFx_basic_type_common* paramInitializer = newParamElement->getFx_basic_type_common();
	    std::string name = paramInitializer->getContents().get(0)->getTypeName();

	    if(name == "fx_surface_common")
		{
			std::string init_from;
			domFx_surface_init_from_common_Array* initFromArray = getInitFromArray(paramInitializer->getSurface());
			if (initFromArray && initFromArray->getCount() > 0)
				init_from = initFromArray->get(0)->getValue().get(0).getID();
		    cfxSurface* surface = new cfxSurface( 
							 init_from,
							 paramInitializer->getSurface()->getFormat()->getValue()
							 );
		    effect->addNamedSurface(newParamElement->getSid(), surface);
		}
	    else
		{
		    // then use the element which holds the actual values to make the data
		    cfxData* newParamData = cfxDataMaker::makeDataForParam(paramInitializer, effect);

		    if (newParamData)
			{
			    //cfxPrint("Creating new param:  %s\n", newParamElement->getSid());

			    // make the parameter and set the data in the param
			    std::string newParamSemantic;
			    if (newParamElement->getSemantic())
				{
				    newParamSemantic = std::string(newParamElement->getSemantic()->getValue());
				}
			    
			    cfxNewParam* newParam = new cfxNewParam(newParamData, std::string(newParamElement->getSid()), effect, newParamSemantic);
			    
			    // load the param's annotations
			    loadAnnotateArray(newParamElement->getAnnotate_array(), newParam, effect);
			    
			    paramable->pushParam(newParam);
			}
		    else
			{
			    cfxPrint("Not creating new param because data was not made successfully:  %s\n", newParamElement->getSid());
			}

		}
	}
	  
}



void loadNewParamArray(domCg_newparam_Array& newParamArrayElement, cfxParamable* paramable, cfxEffect* effect)
{
    // the newParam array contains parameters that are not in the shader code
    cfxUint newParamCount = (cfxUint) newParamArrayElement.getCount();

    // loop over the <newparam> objects, looking for <surface> params to extract.
    for (cfxUint currentNewParam = 0; currentNewParam < newParamCount; currentNewParam++)
	{
	    domCg_newparam* newParamElement = newParamArrayElement.get(currentNewParam);  

	    domCg_param_type* paramInitializer = newParamElement->getCg_param_type();

		if ( !paramInitializer ) 
			continue; 
	    std::string name = paramInitializer->getContents().get(0)->getTypeName();

	    //cfxPrint("newparam %d of %d, named %s\n", currentNewParam, newParamCount, name.c_str());

	    if(name == "cg_surface_type")
		{
			std::string init_from;
			domFx_surface_init_from_common_Array* initFromArray = getInitFromArray(paramInitializer->getSurface());
			if (initFromArray && initFromArray->getCount() > 0)
				init_from = initFromArray->get(0)->getValue().get(0).getID();
		    cfxSurface* surface = new cfxSurface( 
							 init_from, 
							 paramInitializer->getSurface()->getFormat()->getValue()
							 );
		    effect->addNamedSurface(newParamElement->getSid(), surface);
		}
	    else
		{
		    // then use the element which holds the actual values to make the data
		    cfxData* newParamData = cfxDataMaker::makeDataForParam(paramInitializer, effect);
		    
		    if (newParamData)
			{
			    //cfxPrint("Creating new param:  %s\n", newParamElement->getSid());

			    // make the parameter and set the data in the param
			    std::string newParamSemantic;
			    if (newParamElement->getSemantic())
				{
				    newParamSemantic = std::string(newParamElement->getSemantic()->getValue());
				}
			    
			    cfxNewParam* newParam = new cfxNewParam(newParamData, std::string(newParamElement->getSid()), effect, newParamSemantic);
			    
			    // load the param's annotations
			    loadAnnotateArray(newParamElement->getAnnotate_array(), newParam, effect);
			    
			    paramable->pushParam(newParam);
			}
		    else
			{
			    cfxPrint("Not creating new param because data was not made successfully:  %s\n", newParamElement->getSid());
			}
		}
	}
	  
}


void loadSetParamArray(domInstance_effect::domSetparam_Array& setParamArrayElement, cfxParamable* paramable, cfxEffect* effect, cfxMaterial* material)
{
    // these parameters don't need to be created
    // they should exist already from shader code or newparam elements
    cfxUint setParamCount = (cfxUint) setParamArrayElement.getCount();
    // loop over the set params
    for (cfxUint currentSetParam = 0; currentSetParam < setParamCount; currentSetParam++)
	{
	    domInstance_effect::domSetparam* setParamElement = setParamArrayElement.get(currentSetParam);  

	    domFx_basic_type_common* paramInitializer = setParamElement->getFx_basic_type_common();

		if ( !paramInitializer )
			continue; 

	    std::string name = paramInitializer->getContents().get(0)->getTypeName();

	    if(name == "fx_surface_common")
		{
			std::string init_from;
			domFx_surface_init_from_common_Array* initFromArray = getInitFromArray(paramInitializer->getSurface());

			if (initFromArray && initFromArray->getCount() > 0)
				init_from = initFromArray->get(0)->getValue().get(0).getID();
			// lookup surface by name
			cfxSurface* surface = effect->getSurfaceByName(setParamElement->getRef());
		    cfxSurface* newSurface = new cfxSurface( 
							 init_from, 
							 paramInitializer->getSurface()->getFormat()->getValue(),
							 surface
							 );
			material->addSurface(newSurface);
			cfxPrint("Material surface with parent: %s %p\n", init_from.c_str(), surface);
		}
	    else
		{
			// then use the element which holds the actual values to make the data
			cfxData* setParamData = cfxDataMaker::makeDataForParam(setParamElement->getFx_basic_type_common(), effect);

			// make the parameter and set the data in the param
			cfxSetParam* setParam = new cfxSetParam(setParamData, setParamElement->getRef(), effect);

			paramable->pushParam(setParam);
		}
	}
     
}


void pushTechniqueInclude(cfxTechnique* technique, const char* includeSid, xsAnyURI &uri)
{
    char pathBuffer[1024];
    cfxBool pathOK = uri.getPath(pathBuffer, 1024);
    if (pathOK && includeSid)
	{
	    technique->pushInclude(includeSid, pathBuffer);
	}	
    else
	{	
	    cfxPrint("ERROR:  problem with path, probably too long, include not being added\n");
	}	
}


void loadProfileArray(domFx_profile_abstract_Array& profileArrayElement, cfxEffect* effect)
{
    // at least one supported profile is needed for the effect to do anything
    cfxUint profileCount = (cfxUint) profileArrayElement.getCount();

    cfxPrint("found %d profiles\n", profileCount);

    // loop over the profiles
    for (cfxUint currentProfile = 0; currentProfile < profileCount; currentProfile++)
	{
	    domFx_profile_abstract* profileElement = profileArrayElement.get(currentProfile);  
	    daeString profileName = profileElement->getTypeName();

	    cfxPrint("profile %d named %s\n", currentProfile, profileName);

	    // cg profile support
	    if (strcmp(profileName, COLLADA_ELEMENT_PROFILE_CG) == 0)
		{
		    // take the abstract profile and cast it to cg
		    // building with exceptions turned off so no rtti
		    // domProfile_CG* cgProfileElement = dynamic_cast<domProfile_CG*>(abstractProfileElement);
		    domProfile_CG* cgProfileElement = static_cast<domProfile_CG*>(profileElement);

		    daeString platformName = cgProfileElement->getPlatform();
		    cfxPrint("loadProfileArray(): platform %s\n", platformName);

			// Check if this is the platform we are interested in
			if (strcmp(platformName, cfxLoader::getPlatformString().c_str() ) == 0)
			{
			    // !!!tsl tweaked parameters go into materials, so a whole thing needs to be added for proper support  
			    // also, this adds newparam's to the effect, they should probably be added to the technique instead
			    cfxPrint("loadProfileArray():load newparams\n");
			    loadNewParamArray(cgProfileElement->getNewparam_array(), effect, effect);

			    cfxPrint("loadProfileArray():load technique\n");

			    // create and populate a cfxTechnique for every technique in the cg profile
			    cfxUint techniqueCount = (cfxUint) cgProfileElement->getTechnique_array().getCount();
			    // loop over the techniques
			    for (cfxUint currentTechnique = 0; currentTechnique < techniqueCount; currentTechnique++)
				{
				    domProfile_CG::domTechnique* techniqueElement = cgProfileElement->getTechnique_array().get(currentTechnique);  

				    // !!!tsl this should be updated to not check for null once sid is made a require attr in the schema
				    const char* techniqueSid = techniqueElement->getSid();

				    cfxPrint("technique %d\n", currentTechnique);

				    // create a cfxTechnique for every technique inside the cg profile
				    cfxTechnique* technique = new cfxTechnique(effect, techniqueSid ? techniqueSid : "");

				    // we don't do anything with code, mostly because we can't compile cg on ceb yet...

				    // includes are used to pair the name of the shader with its actual file name and path
				    // currently profile and technique are kind of rolled into one with cfxTechnique
				    // all the includes from both of these will be pushed onto each technique
				    // this is redundant and probably not the best representation 
				    // it used to be fine when profile was inside technique but now this is reversed and is less appropriate

				    cfxUint includeCount = 0;

				    // loop over the includes for the profile and push them onto the technique
				    domFx_include_common_Array &profileIncludeArrayElem = cgProfileElement->getInclude_array();
				    includeCount = (cfxUint) profileIncludeArrayElem.getCount();
                    cfxPrint(".......profile..includeCount %d\n", includeCount);
				    for (cfxUint currentInclude = 0; currentInclude < includeCount; currentInclude++)
					{
					    domFx_include_common* includeElement = profileIncludeArrayElem.get(currentInclude);
					    pushTechniqueInclude(technique, includeElement->getSid(), includeElement->getUrl());
					}

				    // loop over the includes for the technique and push them onto the technique
				    domFx_include_common_Array &techniqueIncludeArrayElem = techniqueElement->getInclude_array();
				    includeCount = (cfxUint) techniqueIncludeArrayElem.getCount();
                    cfxPrint("........technique.includeCount %d\n", includeCount);
				    for (cfxUint currentInclude = 0; currentInclude < includeCount; currentInclude++)
					{
					    domFx_include_common* includeElement = techniqueIncludeArrayElem.get(currentInclude);
					    pushTechniqueInclude(technique, includeElement->getSid(), includeElement->getUrl());
					}


				    // beware, profile and technique params are cg_params 
				    // these have way more type options and are currently unsupported
				    // all newparams currently must be in effect
				    // setparams are only valid on cg types, so their current implementation is useless until those types are added
	  

#if 0 // <surface> removed from <profile> and <technique>  - now a <newparam> type
				    // surfaces and images, currently only supporting surfaces IF it uses an image
				    // all surfaces end up in the technique right now
				    pushTechniqueSurfaces(cgProfileElement->getSurface_array(), technique);
				    pushTechniqueSurfaces(techniqueElement->getSurface_array(), technique);
#endif
						
				
				    // pass itself is ENORMOUS.
				    domProfile_CG::domTechnique::domPass_Array& passArrayElement = techniqueElement->getPass_array();
				    // at least one pass is needed for the technique to do anything 
				    cfxUint passCount = (cfxUint) passArrayElement.getCount();
				    // loop over the passes
				    for (cfxUint currentPass = 0; currentPass < passCount; currentPass++)
					{
					    cfxPrint("Here is a pass!\n");

					    domProfile_CG::domTechnique::domPass* passElement = passArrayElement.get(currentPass);  

					    // This should be a name, is Sid OK?
					    cfxPass* pass = new cfxPass(technique, std::string(passElement->getSid()));

					    // why is this not an array?  it should be.
					    // passElement->elemAnnotate
					    // should be
					    // loadAnnotateArray(passElement->getAnnotate_array(), setParam);

	  
					    // what is the meaning of these targets 
					    // passElement->elemColortarget_array
					    // passElement->elemDepthstenciltarget_array
					    // passElement->elemClearcolor_array
					    // passElement->elemCleardepth_array
					    // passElement->elemClearstencil_array

					    // now for drawing...  that isn't cgfx...  does it fit here???
					    // passElement->elemDraw_array

					    // handle the gl pipeline settings 
					    domGl_pipeline_settings_Array& glPipeSettingsArrayElement = passElement->getGl_pipeline_settings_array();
					    cfxUint glPipeSettingsCount = (cfxUint) glPipeSettingsArrayElement.getCount();
					    // loop over the gl pipeline settings
					    for (cfxUint currentGlPipeSettings = 0; currentGlPipeSettings < glPipeSettingsCount; currentGlPipeSettings++)
						{
						    domGl_pipeline_settings* glPipeSettingElement = glPipeSettingsArrayElement.get(currentGlPipeSettings);  

						    cfxGlPipelineSetting* setting = cfxGlPipelineSettingMaker::makeGlPipelineSetting(pass, glPipeSettingElement);
						    pass->pushSetting(setting);

						    // use the meta again to get the class name
						    // this definitely will be another map lookup to find another maker
						    // just like data i think...  only they can apply their own settings.
		  
						}

					    // more cg setparams...  where do these get applied???
					    // passElement->elemSetParam_array

					    // now the shaders
					    domProfile_CG::domTechnique::domPass::domShader_Array& shaderArrayElement = passElement->getShader_array();

					    // at least one shader is needed for the pass to do anything
					    cfxUint shaderCount = (cfxUint) shaderArrayElement.getCount();
					    // loop over the shaders
					    for (cfxUint currentShader = 0; currentShader < shaderCount; currentShader++)
						{
						    domProfile_CG::domTechnique::domPass::domShader* shaderElement = shaderArrayElement.get(currentShader);

						    domCg_pipeline_stage stage = shaderElement->getStage();

						    cfxShader* shader = NULL;

						    // use source (attrib of name) to match the include, may need to check for NULL
						    if (stage == CG_PIPELINE_STAGE_VERTEX)
							{
#ifndef _LIB
							    shader = new cfxShader(pass, std::string(shaderElement->getName()->getSource()), 
									std::string(shaderElement->getName()->getValue()), cgGLGetLatestProfile(CG_GL_VERTEX), cfxShader::VERTEX);
#else
								shader = new cfxShader(pass, std::string(shaderElement->getName()->getSource()), 
									std::string(shaderElement->getName()->getValue()), CG_PROFILE_UNKNOWN, cfxShader::VERTEX);
#endif
							}

						    else //if (stage == CG_PIPELINE_STAGE_FRAGMENT)
							{		    
#ifndef _LIB
							    shader = new cfxShader(pass, std::string(shaderElement->getName()->getSource()), 
									std::string(shaderElement->getName()->getValue()), cgGLGetLatestProfile(CG_GL_FRAGMENT), cfxShader::FRAGMENT);
#else
								shader = new cfxShader(pass, std::string(shaderElement->getName()->getSource()), 
									std::string(shaderElement->getName()->getValue()), CG_PROFILE_UNKNOWN, cfxShader::FRAGMENT);
#endif
							}

						    // load the shader's annotations
						    loadAnnotateArray(shaderElement->getAnnotate_array(), shader, effect);


						    // load the shader's parameters
						    domProfile_CG::domTechnique::domPass::domShader::domBind_Array& bindArrayElement = shaderElement->getBind_array();
		  
						    cfxUint bindCount = (cfxUint) bindArrayElement.getCount();
						    // loop over the bindings
						    for (cfxUint currentBind = 0; currentBind < bindCount; currentBind++)
							{
							    domProfile_CG::domTechnique::domPass::domShader::domBind* bindElement = bindArrayElement.get(currentBind);
							    cfxParam* param;
		      
							    // choose whether bind to param or value based on contents
							    daeElement* content = bindElement->getContents().get(0);
							    std::string dataTypeName = std::string(content->getTypeName());

							    if (dataTypeName == std::string(COLLADA_ELEMENT_PARAM))
								{
								    param = new cfxConnectParam(bindElement->getSymbol(), shader, bindElement->getParam()->getRef());
									//cfxPrint("param = %x\n", param);
								}
							    else
								{
								    // !!!tsl still have to add support for building data from all the cg types
								    cfxData* data = NULL; //cfxDataMaker::makeDataForParam(bindElement->getCg_param_type(), effect);
								    param = new cfxBindParam(bindElement->getSymbol(), shader, data);
								}

							    shader->pushParam(param);
							}		     

						    pass->pushShader(shader);
						}
					    technique->pushPass(pass);
					}
				    effect->pushTechnique(technique);

				}
			}
		}      
	}	  

}

bool hasCGProfile( domEffect *effect )
{
	cfxUint profileCount = (cfxUint) effect->getFx_profile_abstract_array().getCount();

	// loop over the profiles
    for (cfxUint currentProfile = 0; currentProfile < profileCount; currentProfile++)
	{
	    domFx_profile_abstract* profileElement = effect->getFx_profile_abstract_array().get(currentProfile);  
	    daeString profileName = profileElement->getTypeName();

	    // cg profile support
	    if (strcmp(profileName, COLLADA_ELEMENT_PROFILE_CG) == 0)
		{
			return true;
		}
	}
	return false;
}

