/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes
#include <cfxPlatform.h>
#include <stdio.h>
#include <cstdlib>
#include <iostream>

// User includes

#include <cfxNewParam.h>
#include <cfxData.h>
#include <cfxEffect.h>
#include <cfxAnnotate.h>
#include <cfxEffect.h>
#include <cfxPlatform.h>

// cfxNewparam
cfxNewParam::cfxNewParam(cfxData* _data, const std::string& _name, cfxEffect* _effect,
			 const std::string& _semantic)
  : cfxParam(_name),
    effect(_effect),
    data(_data),
    semantic(_semantic)
{
    // ideally, data should always exist, but if the type isn't supported yet, it might be handy not to segfault
    if (data)
	{
	parameter = cgCreateEffectParameter(effect->getEffect(), name.c_str(), data->getType());
	}

  // this is temporary until cg api gets upgraded to set effect parameter names
  effect->addNamedParameter(name, parameter);

  cfxPrint("New param %s %p\n", name.c_str(), parameter);

  if (!semantic.empty())
    {
	// !!!tsl whoa, this is unimplemented too?!
	cgSetParameterSemantic(parameter, semantic.c_str());
      // this is temporary until cg api gets upgraded to set parameter semantics
      effect->addSemanticParameter(semantic, parameter);
    }

  
}

bool cfxNewParam::apply()
{
  std::vector<cfxAnnotate*>::iterator annotateIter = annotateArray.begin();
  while (annotateIter != annotateArray.end())
    {
      (*annotateIter)->apply(this);
      annotateIter++;
    }

  //cfxPrint("setting the data for parameter: %s\n", name.c_str());
    // ideally, data should always exist, but if the type isn't supported yet, it might be handy not to segfault
    if (data)
	{
	    data->apply(this);
	}

  return true;
}

bool cfxNewParam::validate() const
{
  return true;
}

cfxParam::cfxParamTypeEnum cfxNewParam::getType() const
{
	return cfxParam::CFXNEWPARAM;
}

const std::string &cfxNewParam::getSemantic() const
{
	return semantic;
}

const cfxData *cfxNewParam::getData() const
{
	return data;
}
