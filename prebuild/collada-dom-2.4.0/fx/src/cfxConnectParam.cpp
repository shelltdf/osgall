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

#include <cfxConnectParam.h>
#include <cfxShader.h>
#include <cfxPass.h>
#include <cfxTechnique.h>
#include <cfxEffect.h>

// cfxConnectParam 
cfxConnectParam::cfxConnectParam(const std::string& _name, cfxShader* _shader, 
				     const std::string& _fromName)
  : cfxParam(_name), 
    shader(_shader), 
    fromName(_fromName)
{
  // dest parameter intended to be in a program
  parameter = cgGetNamedParameter(shader->getProgram(), name.c_str());
  if (parameter == 0)
    {
      // nothing to stop it from being in an effect
      // parameter = cgGetNamedEffectParameter(shader->getPass()->getTechnique()->getEffect()->getEffect(), name.c_str());
      parameter = shader->getPass()->getTechnique()->getEffect()->getParameterByName(name);
    }

  // source parameter is from an effect 
  // fromParameter = cgGetNamedEffectParameter(shader->getPass()->getTechnique()->getEffect()->getEffect(), fromName.c_str());
  fromParameter = shader->getPass()->getTechnique()->getEffect()->getParameterByName(fromName);

  if ( fromParameter )
  {
	printf("Connect to param %s %p from param %s %p\n", name.c_str(), parameter, fromName.c_str(), fromParameter);
	cgConnectParameter(fromParameter, parameter);
  }	
}

bool cfxConnectParam::apply()
{
  return true;
}

bool cfxConnectParam::validate() const
{
  return true;
}

cfxParam::cfxParamTypeEnum cfxConnectParam::getType() const
{
	return cfxParam::CFXCONNECTPARAM;
}

const std::string &cfxConnectParam::getFromName() const
{
	return fromName;
}

