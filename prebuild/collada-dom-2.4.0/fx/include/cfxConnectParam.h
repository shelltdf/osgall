/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_CONNECTPARAM_H
#define _CFX_CONNECTPARAM_H

#include <string>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

#include <cfxParam.h>

class cfxShader;

// cfxConnectParam 
class cfxConnectParam : public cfxParam
{
public:

  cfxConnectParam(const std::string& _name, cfxShader* _shader, 
		  const std::string& _fromName);

  bool apply();
  bool validate() const;

  virtual cfxParam::cfxParamTypeEnum getType() const;
  const std::string &getFromName() const;


private:

  cfxShader* shader;

  std::string fromName;

  CGparameter fromParameter;
};



#endif // _CFX_CONNECTPARAM_H
