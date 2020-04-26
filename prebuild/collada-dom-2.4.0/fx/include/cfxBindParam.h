/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_BINDPARAM_H
#define _CFX_BINDPARAM_H

#include <string>

#include <cfxParam.h>

class cfxShader;
class cfxData;


// cfxBindParam 
class cfxBindParam : public cfxParam
{
public:

  cfxBindParam(const std::string& _name, cfxShader* _shader, cfxData* _data);

  bool apply();
  bool validate() const;

  virtual cfxParam::cfxParamTypeEnum getType() const;
  const cfxData *getData() const;

protected:
  
  cfxShader* shader;
  cfxData* data;

};



#endif // _CFX_BINDPARAM_H
