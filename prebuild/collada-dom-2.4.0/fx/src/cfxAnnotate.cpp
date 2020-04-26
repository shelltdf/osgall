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
#include <assert.h>
#include <cfxPlatform.h>
// User includes

#include <cfxAnnotate.h>
#include <cfxData.h>
#include <cfxEffect.h>
#include <cfxParam.h>
#include <cfxPass.h>
#include <cfxTechnique.h>
#include <cfxShader.h>


// cfxAnnotate
cfxAnnotate::cfxAnnotate(cfxData* _data, const std::string& _name) 
  : data(_data),
    name(_name)
{
}
 
cfxAnnotate::~cfxAnnotate() 
{
}


bool cfxAnnotate::apply(const cfxEffect* effect)
{
  annotation = cgCreateEffectAnnotation(effect->getEffect(), name.c_str(), data->getType());
  data->apply(this);
  return true; 
}

bool cfxAnnotate::apply(const cfxParam* param)
{
  annotation = cgCreateParameterAnnotation(param->getParameter(), name.c_str(), data->getType());
  data->apply(this);
  return true; 
}

bool cfxAnnotate::apply(const cfxPass* pass)
{ 
  annotation = cgCreatePassAnnotation(pass->getPass(), name.c_str(), data->getType());  
  data->apply(this);
  return true; 
}

bool cfxAnnotate::apply(const cfxTechnique* technique)
{
  annotation = cgCreateTechniqueAnnotation(technique->getTechnique(), name.c_str(), data->getType());  
  data->apply(this);
  return true; 
}

bool cfxAnnotate::apply(const cfxShader* shader)
{
  annotation = cgCreateProgramAnnotation(shader->getProgram(), name.c_str(), data->getType());  
  data->apply(this);
  return true; 
}

CGannotation cfxAnnotate::getAnnotation() const
{
  return annotation;
}

const cfxData *cfxAnnotate::getData() const
{
	return data;
}

const std::string &cfxAnnotate::getName() const
{
	return name;
}
