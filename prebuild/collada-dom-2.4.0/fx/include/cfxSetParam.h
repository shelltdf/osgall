/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_SETPARAM_H
#define _CFX_SETPARAM_H


#include <cfxParam.h>

class cfxEffect;
class cfxAnnotate;
class cfxData;

// cfxSetparam
class cfxSetParam : public cfxParam
{
public:
  
  cfxSetParam(cfxData* _data, const std::string& _name, cfxEffect* _effect);

  virtual bool apply();
  virtual bool validate() const;

  virtual cfxParam::cfxParamTypeEnum getType() const;
  const cfxData *getData() const;

protected:
  
  cfxEffect* effect;
  cfxData* data;

};

#endif // _CFX_SETPARAM_H

