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

#include <cfxCode.h>


// cfxCode 
cfxCode::cfxCode(cfxEffect* _effect, const std::string& _name, 
		 const std::string& _profile, const std::string& _code)
  : effect(_effect), 
    name(_name),
    profile(_profile),
    code(_code)
{}

bool cfxCode::apply()
{
  // there is nothing to do with code until the platform supports compilation at run time

  /*
  program = cgCreateProgram(effect->getContext(), 
			    CGenum program_type,
			    code.c_str(),
			    CGprofile profile,
			    const char *entry,
			    const char **args);
  */

  return true;
}

bool cfxCode::validate() const
{
  return true;
}

