/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_PASS_H
#define _CFX_PASS_H

#include <string>
#include <vector>

#include <cfxAnnotatable.h>
#include <cfxParamable.h>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

class cfxGlPipelineSetting;
class cfxShader;
class cfxTechnique;

// cfxPass 
class cfxPass : public cfxAnnotatable, public cfxParamable
{
public:

  cfxPass(cfxTechnique* _technique, const std::string& _name);
  ~cfxPass();

  bool apply();
  bool validate() const;

  void pushSetting(cfxGlPipelineSetting* setting);
  void pushShader(cfxShader* shader);

  cfxTechnique* getTechnique() const;

  CGpass getPass() const;

  const std::string &getName() const;
  const std::vector<cfxGlPipelineSetting*> &getSettingArray() const;
  const std::vector<cfxShader*> &getShaderArray() const;

private:

  cfxTechnique* technique;

  std::string name;
 
  std::vector<cfxGlPipelineSetting*> settingArray;  // min 0
  
  std::vector<cfxShader*> shaderArray;  // min 0
  
  CGpass pass;

};

#endif // _CFX_PASS_H

