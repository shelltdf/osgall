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

#include <cfxParam.h>


// cfxParam
cfxParam::cfxParam(const std::string& _name)
  : name(_name)
{
}
 
cfxParam::~cfxParam()
{
}

CGparameter cfxParam::getParameter() const
{
  return parameter;
}

const std::string &cfxParam::getName() const 
{
	return name;
}

 

