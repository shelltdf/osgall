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

#include <cfxPass.h>
#include <cfxAnnotate.h>
#include <cfxShader.h>
#include <cfxTechnique.h>
#include <cfxGlPipelineSetting.h>


// cfxPass 
cfxPass::cfxPass(cfxTechnique* _technique, const std::string& _name)
  : technique(_technique),
    name(_name)
{
  pass = cgCreatePass(technique->getTechnique(), name.c_str());
}
cfxPass::~cfxPass()
{
	while (!settingArray.empty())
	{
		delete(settingArray[0]);
		settingArray.erase(settingArray.begin());
	}

	while (!shaderArray.empty())
	{
		delete(shaderArray[0]);
		shaderArray.erase(shaderArray.begin());
	}

}
bool cfxPass::apply()
{
  std::vector<cfxGlPipelineSetting*>::iterator settingIter = settingArray.begin();
  while (settingIter != settingArray.end())
    {
      (*settingIter)->apply();
      settingIter++;
    }

  std::vector<cfxShader*>::iterator shaderIter = shaderArray.begin();
  while (shaderIter != shaderArray.end())
    {
      (*shaderIter)->apply();
      shaderIter++;
    }

  return true;
}

bool cfxPass::validate() const
{
  return true;
}

void cfxPass::pushSetting(cfxGlPipelineSetting* setting)
{
  settingArray.push_back(setting);
  printf("setting pushed onto pass\n");
}

void cfxPass::pushShader(cfxShader* shader)
{
  shaderArray.push_back(shader);
  printf("shader pushed onto pass\n");
}

cfxTechnique* cfxPass::getTechnique() const
{
  return technique;
}

CGpass cfxPass::getPass() const
{
  return pass;
}

const std::string &cfxPass::getName() const
{
	return name;
}

const std::vector<cfxGlPipelineSetting*> &cfxPass::getSettingArray() const
{
	return settingArray;
}

const std::vector<cfxShader*> &cfxPass::getShaderArray() const
{
	return shaderArray;
}
