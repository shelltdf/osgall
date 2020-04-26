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

#include <cfxMaterial.h>
#include <cfxEffect.h>
#include <cfxParam.h>
#include <cfxSetParam.h>
#include <cfxSurface.h>
#include <cfxPlatform.h>
#include <cfxData.h>


// cfxMaterial
cfxMaterial::cfxMaterial(const std::string& _name, const std::string& _effectName, 
			 cfxEffect* _effect)
  : name(_name),
    effectName(_effectName),
    effect(_effect)
{
}
cfxMaterial::~cfxMaterial()
{
	for (size_t i=0; i< surfaces.size(); i++)
	{
		delete surfaces[i];
	}
	surfaces.clear();

    for ( size_t i = 0; i < paramArray.size(); ++i ) 
    {
        cfxSetParam* item = (cfxSetParam*)paramArray[i];
        delete item->getData();
        delete paramArray[i];
    }
    paramArray.clear();
}

const std::string& cfxMaterial::getName() const
{
  return name;
}
  
cfxEffect* cfxMaterial::getEffect() const
{
  return effect;
}
  
bool cfxMaterial::setParameterValues()
{
    if (effect)
	{
	    std::vector<cfxParam*>::iterator paramIter = paramArray.begin();
	    while (paramIter != paramArray.end())
		{
		    (*paramIter)->apply();
		    paramIter++;
		}
		std::vector<cfxSurface*>::iterator surfaceIter = surfaces.begin();
		while (surfaceIter != surfaces.end())
		{
			(*surfaceIter)->apply();
			surfaceIter++;
		}
	    return true;
	}
    else
	{
	    cfxPrint("Can not setParameterValues with NULL effect for material %s effect %s\n", name.c_str(), effectName.c_str());
	    return false;
	}
}


void cfxMaterial::setEffectPassState(unsigned int techniqueIndex, unsigned int passIndex)
{
    if (effect)
	{
	    effect->setPassState(techniqueIndex, passIndex);
	}
    else
	{
	    cfxPrint("Can not setPassState with NULL effect for material %s effect %s\n", name.c_str(), effectName.c_str());
	}
}


void cfxMaterial::resetEffectPassState(unsigned int techniqueIndex, unsigned int passIndex)
{
    if (effect)
	{
	    effect->resetPassState(techniqueIndex, passIndex);
	}
    else
	{
	    cfxPrint("Can not resetPassState with NULL effect for material %s effect %s\n", name.c_str(), effectName.c_str());
	}
}


unsigned int cfxMaterial::getEffectTechniqueCount()
{
    if (effect)
	{
	    return effect->getTechniqueCount();
	}
    else
	{
	    cfxPrint("Can not getEffectTechniqueCount with NULL effect for material %s effect %s\n", name.c_str(), effectName.c_str());
	    return 0;
	}
}


unsigned int cfxMaterial::getEffectPassCount(unsigned int techniqueIndex)
{
    if (effect)
	{
	    return effect->getPassCount(techniqueIndex);
	}
    else
	{
	    cfxPrint("Can not getEffectPassCount with NULL effect for material %s effect %s\n", name.c_str(), effectName.c_str());
	    return 0;
	}
}

const std::string &cfxMaterial::getEffectName() const
{
	return effectName;
}

void cfxMaterial::addSurface(cfxSurface* surface)
{
	surfaces.push_back(surface);
}

const std::vector<cfxSurface*> &cfxMaterial::getSurfaces() const
{
	return surfaces;
}
