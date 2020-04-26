/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// User includes

#include <cfxEffect.h>
#include <cfxCode.h>
#include <cfxParam.h>
#include <cfxNewParam.h>
#include <cfxTechnique.h>
#include <cfxAnnotate.h>
#include <cfxPass.h>
#include <cfxData.h>

#include <assert.h>

// cfxEffect
cfxEffect::cfxEffect(const std::string& _name, CGcontext _context) 
  : name(_name),
    context(_context)
{
  // do lazy context creation, but this may make it difficult to call 
  // cgDestroyContext later... 
  if (context == NULL)
    context = cgCreateContext(); 
	// I think cg would always return an error if context was not created so this is probably redundant
	assert(context);

	// create the new effect and set its name
	effect = cgCreateEffect(context, NULL, NULL);
	cgSetEffectName( effect, name.c_str() ); 

	assert(effect);
}
cfxEffect::~cfxEffect()
{
#ifndef SN_TARGET_PS3
	cgDestroyEffect(effect);
#endif
	for (size_t i=0; i<codeArray.size(); i++)
	{
		delete codeArray[i];
	}
	codeArray.clear();

	for (size_t i=0; i<techniqueArray.size(); i++)
	{
		delete techniqueArray[i];
	}
	techniqueArray.clear();

	std::map<std::string, cfxSurface*>::iterator iter=mapNameToSurfacePtr.begin();
    for (size_t i=0; i<paramArray.size(); i++)
	{
        cfxNewParam* p = (cfxNewParam*)paramArray[i];
		delete p->data;

        // Check if param was used by cfxSurface object, if so, don't delet it yet, let ~cfxSurface() delete it
        bool found = false;
        for (iter=mapNameToSurfacePtr.begin(); iter != mapNameToSurfacePtr.end(); iter++)
        {
            	for (size_t i=0; i<iter->second->referencingParams.size(); i++)
                    if (iter->second->referencingParams[i] == p)
                            found = true;
        }
	    if (!found) 
            delete p;
	}
	paramArray.clear();
  // this is temporary until cg api gets upgraded to set effect parameter names

	mapNameToParameter.clear();
	iter=mapNameToSurfacePtr.begin();
	for (iter=mapNameToSurfacePtr.begin(); iter != mapNameToSurfacePtr.end(); iter++)
	{
		delete (iter->second);
	}
	mapNameToSurfacePtr.clear();
	mapSemanticToParameter.clear();
}

cfxBool cfxEffect::apply()
{
  std::vector<cfxAnnotate*>::iterator annotateIter = annotateArray.begin();
  while (annotateIter != annotateArray.end())
    {
      (*annotateIter)->apply(this);
      annotateIter++;
    }

  std::vector<cfxParam*>::iterator paramIter = paramArray.begin();
  while (paramIter != paramArray.end())
    {
      (*paramIter)->apply();
      paramIter++;
    }

  // // // moving technique earlier so the connection can happen before the set because 
  // // // propagation of parameter value is not quite implemented properly yet.
  std::vector<cfxTechnique*>::iterator techniqueIter = techniqueArray.begin();
  while (techniqueIter != techniqueArray.end())
    {
      (*techniqueIter)->apply();
      techniqueIter++;
    }

  // i think there should be a better place for this, but until i know what that is...
  techniqueIter = techniqueArray.begin();
  while (techniqueIter != techniqueArray.end())
    {
      cfxBool valid = cgValidateTechnique((*techniqueIter)->getTechnique());

      if (valid)
	{
	  printf("VALID TECHNIQUE\n");
	}
      else
	{
	  printf("ERROR:  INVALID TECHNIQUE\n");
	}

      techniqueIter++;
    }

  return true;
}

cfxBool cfxEffect::validate() const
{
  return true;
}

CGcontext cfxEffect::getContext() const
{
  return context;
}

CGeffect cfxEffect::getEffect() const
{
  return effect;
}
  
const std::string& cfxEffect::getName() const
{
  return name;
}
  
void cfxEffect::pushCode(cfxCode* code)
{
  codeArray.push_back(code);
}
  
void cfxEffect::pushTechnique(cfxTechnique* technique)
{

  printf("PushTechnique\n");
  techniqueArray.push_back(technique);
}

  

void cfxEffect::setPassState(cfxUint techniqueIndex, cfxUint passIndex)
{
  if (techniqueIndex >= techniqueArray.size())
    {
	  printf("setPassState passIndex=%d\n", passIndex);
      printf("ERROR: techniqueIndex invalid:  %d\n", techniqueIndex);
      return;
    }
  
  techniqueArray[techniqueIndex]->setPassState(passIndex);
}


void cfxEffect::resetPassState(cfxUint techniqueIndex, cfxUint passIndex)
{
  if (techniqueIndex >= techniqueArray.size())
    {
	  printf("resetPassState passIndex=%d\n", passIndex);
      printf("ERROR: techniqueIndex invalid:  %d\n", techniqueIndex);
      return;
    }
  
  techniqueArray[techniqueIndex]->resetPassState(passIndex);
}


cfxUint cfxEffect::getTechniqueCount()
{
    return (cfxUint) techniqueArray.size();
}


cfxUint cfxEffect::getPassCount(cfxUint techniqueIndex)
{
  if (techniqueIndex >= techniqueArray.size())
    {
	  printf("getPassCount\n");
      printf("ERROR: techniqueIndex invalid:  %d\n", techniqueIndex);
      return 0;
    }
  
  return (techniqueArray[techniqueIndex]->getPassCount());
}


// this is temporary until cg api gets upgraded to set effect parameter names
void cfxEffect::addNamedParameter(const std::string& name, CGparameter param)
{
  mapNameToParameter.insert(make_pair(name, param));
}


CGparameter cfxEffect::getParameterByName(const std::string& name)
{
  CGparameter param = 0;
#if 1
  std::map<std::string, CGparameter>::iterator iter = mapNameToParameter.find(name);
  if (iter != mapNameToParameter.end())
    {
      param = iter->second;
    }
  //printf("got effect param:  %s %d\n", name.c_str(), param);
#else
  param = cgGetNamedEffectParameter(effect, name.c_str());
#endif
  return param;
}


void cfxEffect::addNamedSurface(const std::string& name, cfxSurface* surface)
{
  mapNameToSurfacePtr.insert(make_pair(name, surface));
  printf("added surface to map of name %s\n", name.c_str());
}

cfxSurface* cfxEffect::getSurfaceByName(const std::string& name)
{
  cfxSurface* surface = 0;
  std::map<std::string, cfxSurface*>::iterator iter = mapNameToSurfacePtr.find(name);
  if (iter != mapNameToSurfacePtr.end())
    {
      surface = iter->second;
    }
  //printf("got effect surface:  %s %p\n", name.c_str(), surface);
  return surface;
}


void cfxEffect::addSemanticParameter(const std::string& semantic, CGparameter param)
{
    printf("adding semantic pair %s %p\n", semantic.c_str(), param);
    mapSemanticToParameter.insert(make_pair(semantic, param));
}


CGparameter cfxEffect::getParameterBySemantic(const std::string& semantic)
{
  CGparameter param = 0;
#if 1
  std::map<std::string, CGparameter>::iterator iter = mapSemanticToParameter.find(semantic);
  if (iter != mapSemanticToParameter.end())
    {
      param = iter->second;
    }
  //printf("got semantic param:  %s %p\n", semantic.c_str(), param);
#else
  param = cgGetEffectParameterBySemantic(effect, semantic.c_str());
#endif
  return param;
}

const std::vector<cfxTechnique*> &cfxEffect::getTechniqueArray() const
{
	return techniqueArray;
}

const std::map<std::string, cfxSurface*> &cfxEffect::getSurfaceMap() const
{
	return mapNameToSurfacePtr;
}

std::string cfxEffect::getSurfaceName( const cfxSurface *surface )
{
	std::map<std::string, cfxSurface *>::iterator iter = mapNameToSurfacePtr.begin();
	while (iter != mapNameToSurfacePtr.end() )
	{
		if ( iter->second == surface )
		{
			return iter->first;
		}
		++iter;
	}
	return "";
}
