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

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

// User includes

#include <cfxSetParam.h>
#include <cfxEffect.h>
#include <cfxData.h>


// cfxSetparam
cfxSetParam::cfxSetParam(cfxData* _data, const std::string& _name, cfxEffect* _effect)
  : cfxParam(_name),
    effect(_effect),
    data(_data)
{
    if (effect)
	{
	    // !!!tsl this isn't supported properly yet
	    //parameter = cgGetNamedEffectParameter(effect->getEffect(), name.c_str());
	    
	    // this allows params created in the effect to be set later
	    parameter = effect->getParameterByName(name);
	    
	    printf("setparam created:  %s %p\n", name.c_str(), parameter);
	}
    else
	{
	    printf("setparam created but NULL effect so can not get effect parameter to set:  %s\n", name.c_str()); 
	}
}

bool cfxSetParam::apply()
{

    if (data && getParameter())
	{
	    data->apply(this);
	}
    
  return true;
}

bool cfxSetParam::validate() const
{
  return true;
}

cfxParam::cfxParamTypeEnum cfxSetParam::getType() const
{
	return cfxParam::CFXSETPARAM;
}

const cfxData *cfxSetParam::getData() const
{
	return data;
}

