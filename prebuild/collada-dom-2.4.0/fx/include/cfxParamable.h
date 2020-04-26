/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_PARAMABLE_H
#define _CFX_PARAMABLE_H


#include <vector>

class cfxParam;


// cfxParamable
class cfxParamable
{
public:

  cfxParamable();
  virtual ~cfxParamable();

  void pushParam(cfxParam* param);

  const std::vector<cfxParam*> &getParamArray() const;

 protected:

  std::vector<cfxParam*> paramArray; // min 0

};


#endif // _CFX_PARAMABLE_H
