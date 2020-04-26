/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_NEWPARAM_H
#define _CFX_NEWPARAM_H


#include <cfxParam.h>
#include <cfxAnnotatable.h>

class cfxEffect;
class cfxAnnotate;
class cfxData;

// cfxNewParam
class cfxNewParam : public cfxParam, public cfxAnnotatable
{
public:

  cfxNewParam(cfxData* _data, const std::string& _name, cfxEffect* _effect,
	      const std::string& _semantic);
 
  virtual bool apply();
  virtual bool validate() const;

  virtual cfxParam::cfxParamTypeEnum getType() const;
  const std::string &getSemantic() const;
  const cfxData *getData() const;
  cfxEffect* effect;
  cfxData* data;

protected:
  
  std::string semantic;

};

#endif // _CFX_NEWPARAM_H

