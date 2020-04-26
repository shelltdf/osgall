/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes

#include <stdio.h>

// User includes

#include <cfxSampler.h>
#include <cfxEffect.h>
#include <cfxParam.h>
#include <cfxSurface.h>
#include <cfxGlSamplerSetting.h>
#include <cfxPlatform.h>

// cfxSampler
cfxSampler::cfxSampler(const std::string& _source, cfxEffect* _effect)
  : source(_source),
    textureId(0),
    effect(_effect),
    surface(NULL),
    generateMipmaps(true)
{
}
 
cfxSampler::~cfxSampler() 
{
	while (!settingArray.empty())
	{
		delete(settingArray[0]);
		settingArray.erase(settingArray.begin());
	}
}

bool cfxSampler::apply(cfxParam* param)
{
    // lookup source for the matching surface;
    if (surface == NULL)
	{
	    surface = effect->getSurfaceByName(source);

	    // only go through this if the surface is newly initialized
	    // this will update the textureId
	    // bind the texture and initialize the sampler states
	    // generate mipmaps if needed
	    if (surface)
		{
		    surface->addReferencingParam(param);

		    const std::string surfaceSource = surface->getInitFrom();
		    //cfxPrint("Found surface init from %p %s\n", surface, surfaceSource.c_str());

		    // get the texture object from the surface that is its source, or if that can't be found
		    // just try using texture 1 as the default which is definitely  not the solution but may 
		    // be interesting until this feature is fully supported

		    // the mechanism to pair a texture with a surface for use by a parameter is in place.
		    // the support for loading textures, as would probably be done by the surface is out
		    // of the scope of this library.  this is something that needs to exist
		    // in a lower layer that can do resource management.  this is something that 
		    // needs to exist in a layer that allows this other aspect of resource management.

		    // this uses a static map (which has its own set of disadvantages) from surface to lookup 
		    // texture ids from images that were loaded elsewhere and pushed into cfx
		    textureId = cfxSurface::getTexIdByImageId(surfaceSource);

		    if (textureId > 0)
			{
			    //cfxPrint("Set sampler state\n");
			    // this works on the currently bound texture object
			    // not important which texture unit is used, we just need to use one to
			    // allow cgSetSamplerState() to work properly. 
			    cfxPrint("going to bind %d\n", textureId);
			    glBindTexture(GL_TEXTURE_2D, textureId);

			    // apply the state settings 
			    std::vector<cfxGlSamplerSetting*>::iterator settingIter = settingArray.begin();
			    while (settingIter != settingArray.end())
				{
				    (*settingIter)->apply(param);
				    settingIter++;
				}

			    // calling this before generate mipmaps is better cause then the call
			    // to generate mipmaps already has the space set up for it to use
			    cgSetSamplerState(param->getParameter());

			    if (generateMipmaps)
			    {
#ifdef SN_TARGET_PS3
       			         glGenerateMipmapOES(GL_TEXTURE_2D);
#endif
                            }
			}
		    else
			{
			    cfxPrint("Texture not found for surface: %s\n", surfaceSource.c_str());
			    return false;
			}
		}
	}

    return true; 
}


GLuint cfxSampler::getTextureId() //const
{
    textureId = cfxSurface::getTexIdByImageId(surface->getInitFrom());
    return textureId;
}


void cfxSampler::pushSetting(cfxGlSamplerSetting* setting)
{
  settingArray.push_back(setting);
}

void cfxSampler::setGenerateMipmaps(bool _generateMipmaps)
{
    generateMipmaps = _generateMipmaps;
}

const std::string &cfxSampler::getSource() const 
{
	return source;
}

const std::vector<cfxGlSamplerSetting*> &cfxSampler::getSettingArray() const
{
	return settingArray;
}
