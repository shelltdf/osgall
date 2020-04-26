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

#include <cfxBindParam.h>
#include <cfxShader.h>
#include <cfxData.h>

// cfxBindParam 
cfxBindParam::cfxBindParam(const std::string& _name, cfxShader* _shader, cfxData* _data)
  : cfxParam(_name),
    shader(_shader),
    data(_data)
{
}

bool cfxBindParam::apply()
{
  parameter = cgGetNamedParameter(shader->getProgram(), name.c_str());

  if (data)
    {
      data->apply(this);
    }
  else
    {
      printf("Value settings not implemented yet for bind params - data must support all cg types for this.\n");
    }

  return true;
}

bool cfxBindParam::validate() const
{
  return true;
}

cfxParam::cfxParamTypeEnum cfxBindParam::getType() const
{
	return cfxParam::CFXBINDPARAM;
}

const cfxData *cfxBindParam::getData() const
{
	return data;
}
