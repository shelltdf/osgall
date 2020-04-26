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

// User includes

#include <cfxTechnique.h>
#include <cfxEffect.h>
#include <cfxAnnotate.h>
#include <cfxPass.h>
#include <cfxSurface.h>



cfxTechnique::cfxTechnique(cfxEffect* _effect, const std::string& _name)
  : effect(_effect),
    name(_name)
{
  technique = cgCreateTechnique(effect->getEffect(), name.c_str());
}

cfxTechnique::~cfxTechnique()
{
  for (size_t i=0; i<codeArray.size(); i++)
  {
	delete codeArray[i];
  }
  codeArray.clear();

  includeMap.clear();

  for (size_t i=0; i<passArray.size(); i++)
  {
	delete passArray[i];
  }
  passArray.clear();

  for (size_t i=0; i<surfaceArray.size(); i++)
  {
	delete surfaceArray[i];
  }
  surfaceArray.clear();
}

bool cfxTechnique::apply()
{
  std::vector<cfxAnnotate*>::iterator annotateIter = annotateArray.begin();
  while (annotateIter != annotateArray.end())
    {
      (*annotateIter)->apply(this);
      annotateIter++;
    }

  std::vector<cfxPass*>::iterator passIter = passArray.begin();
  while (passIter != passArray.end())
    {
      (*passIter)->apply();
      passIter++;
    }  

  return true;
}

bool cfxTechnique::validate() const
{
  return true;
}

cfxEffect* cfxTechnique::getEffect() const
{
  return effect;
}

CGtechnique cfxTechnique::getTechnique() const
{
  return technique;
}

const char* cfxTechnique::mapNameToFullPath(const std::string& name) const
{
  std::map<std::string, std::string>::const_iterator includeIter = includeMap.find(name);
  if (includeIter == includeMap.end())
    return NULL;
  return includeIter->second.c_str();
}


const char* cfxTechnique::getFullPathCg(unsigned int& numCg, unsigned int indexCg) const
{
    const char* fullpath = NULL;
    numCg = 0;
    std::map<std::string, std::string>::const_iterator includeIter = includeMap.begin();
    while (includeIter != includeMap.end())
	{
	    const std::string& testpath = includeIter->second;
	    if (((testpath.at(testpath.length()-3) == '.') && (testpath.at(testpath.length()-2) == 'c') && (testpath.at(testpath.length()-1) == 'g')) || 
		((testpath.at(testpath.length()-5) == '.') && (testpath.at(testpath.length()-4) == 'c') && (testpath.at(testpath.length()-3) == 'g') && (testpath.at(testpath.length()-2) == 'f') && (testpath.at(testpath.length()-1) == 'x')))
		{
		    if (numCg == indexCg)
			{
			    fullpath = testpath.c_str();
			}
		    numCg++;
		}
	    includeIter++;
	}
    return fullpath;
}


void cfxTechnique::pushPass(cfxPass* pass)
{
  passArray.push_back(pass);
}


void cfxTechnique::pushInclude(const std::string& name, const std::string& uri)
{
  includeMap.insert(make_pair(name, uri));
  printf("inserting pair into include map: %s %s\n", name.c_str(), uri.c_str());
}


void cfxTechnique::pushSurface(cfxSurface* surface)
{
  surfaceArray.push_back(surface);
}

void cfxTechnique::setPassState(unsigned int passIndex)
{
  if (passIndex >= passArray.size())
    {
      printf("ERROR: passIndex invalid:  %d\n", passIndex);
      return;
    }
  
  cgSetPassState(passArray[passIndex]->getPass());
}


void cfxTechnique::resetPassState(unsigned int passIndex)
{
  if (passIndex >= passArray.size())
    {
      printf("ERROR: passIndex invalid:  %d\n", passIndex);
      return;
    }
  
  cgResetPassState(passArray[passIndex]->getPass());
}


unsigned int cfxTechnique::getPassCount() const
{
  return (unsigned int)(passArray.size());
}

const std::map<std::string, std::string> &cfxTechnique::getIncludeMap() const
{
	return includeMap;
}

const std::vector<cfxSurface*> &cfxTechnique::getSurfaceArray() const
{
	return surfaceArray;
}
const std::string &cfxTechnique::getName() const
{
	return name;
}

const std::vector<cfxPass*> &cfxTechnique::getPassArray() const
{
	return passArray;
}
