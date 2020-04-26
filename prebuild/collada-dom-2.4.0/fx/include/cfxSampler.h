/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_SAMPLER_H
#define _CFX_SAMPLER_H

#include <string>
#include <vector>

#include <cfxPlatform.h>

class cfxEffect;
class cfxParam;
class cfxGlSamplerSetting;
class cfxSurface;

// cfxSampler
class cfxSampler
{
 public:

  cfxSampler(const std::string& _source, cfxEffect* _effect);
  virtual ~cfxSampler();

  bool apply(cfxParam* param);

  GLuint getTextureId();// const;

  void pushSetting(cfxGlSamplerSetting* setting);

  void setGenerateMipmaps(bool _generateMipmaps);

  const std::string &getSource() const;
  const std::vector<cfxGlSamplerSetting*> &getSettingArray() const;

 protected:
  
  std::string source;
  GLuint textureId;
  
  cfxEffect* effect;
  cfxSurface* surface;

  bool generateMipmaps;

  std::vector<cfxGlSamplerSetting*> settingArray; // min 0

};



#endif // _CFX_SAMPLER_H
