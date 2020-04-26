/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// User includes

#include <cfxParamable.h>


// cfxParamable
cfxParamable::cfxParamable() 
{
}
 
cfxParamable::~cfxParamable() 
{
}
 
void cfxParamable::pushParam(cfxParam* param)
{
  paramArray.push_back(param);
}

const std::vector<cfxParam*> &cfxParamable::getParamArray() const
{
	return paramArray;
}
