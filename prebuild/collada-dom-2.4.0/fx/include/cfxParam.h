/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_PARAM_H
#define _CFX_PARAM_H


//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif
#include <string>

// cfxParam
class cfxParam
{
public:
	enum cfxParamTypeEnum {
		CFXBINDPARAM,
		CFXCONNECTPARAM,
		CFXNEWPARAM,
		CFXSETPARAM,
	};

public:

  cfxParam(const std::string& _name);
  virtual ~cfxParam();
 
  virtual bool apply() = 0;
  virtual bool validate() const = 0;

  CGparameter getParameter() const;
  virtual cfxParamTypeEnum getType() const = 0;

  const std::string &getName() const;
  
protected:
  
  std::string name;

  CGparameter parameter;

};


#endif // _CFX_PARAM_H
