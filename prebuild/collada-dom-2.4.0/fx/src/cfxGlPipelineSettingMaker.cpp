/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#include <cfxGlPipelineSettingMaker.h>
#include <cfxGlPipelineSetting.h>

#include <cfxGlEnumMaps.h>

#include <dae/daeDomTypes.h>
#include <dom/domConstants.h>
#include <dom/domGl_pipeline_settings.h>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif


// cfxGlPipelineSettingMaker
std::map<std::string, cfxGlPipelineSettingMaker*> cfxGlPipelineSettingMaker::mapTypeNameToSettingMaker;

cfxGlPipelineSetting* cfxGlPipelineSettingMaker::makeGlPipelineSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  std::map<std::string, cfxGlPipelineSettingMaker*>::iterator iter;
  // here is the element which owns the actual data
  // it is a choice in schema, so there will be only one element in _contents that exists as the "chosen" item, 
  // which will be one of the elements in this class
  daeElement* dataElement = settingInitializer->getContents().get(0);
  // get typename from contents, use this to lookup the factory for the data
  daeString dataTypeName = dataElement->getTypeName();
  iter = mapTypeNameToSettingMaker.find(dataTypeName);
  if (iter == mapTypeNameToSettingMaker.end())
    {
      printf("ERROR:  Setting maker not found for:  %s\n", dataTypeName);
      return NULL;
    }
  printf("Making setting for:  %s\n", dataTypeName);
  return iter->second->makeSetting(pass, settingInitializer); 
}


// cfxGlPipelineSettingMakerAlphaFunc
cfxGlPipelineSettingMakerAlphaFunc::cfxGlPipelineSettingMakerAlphaFunc()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_ALPHA_FUNC, this));
}

cfxGlPipelineSettingMakerAlphaFunc cfxGlPipelineSettingMakerAlphaFunc::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerAlphaFunc::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domFunc = settingInitializer->getAlpha_func()->getFunc()->getValue();
  int func = cfxGlEnumMaps::getFuncTypeEnum(domFunc);
  
  float value = settingInitializer->getAlpha_func()->getValue()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingAlphaFunc(pass, func, value);
  return setting;
}


// cfxGlPipelineSettingMakerBlendFunc
cfxGlPipelineSettingMakerBlendFunc::cfxGlPipelineSettingMakerBlendFunc()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_BLEND_FUNC, this));
}

cfxGlPipelineSettingMakerBlendFunc cfxGlPipelineSettingMakerBlendFunc::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerBlendFunc::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domSrc = settingInitializer->getBlend_func()->getSrc()->getValue();
  int src = cfxGlEnumMaps::getBlendTypeEnum(domSrc);

  int domDst = settingInitializer->getBlend_func()->getDest()->getValue();
  int dst = cfxGlEnumMaps::getBlendTypeEnum(domDst);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingBlendFunc(pass, src, dst);
  return setting;
}


// cfxGlPipelineSettingMakerBlendFuncSeparate
cfxGlPipelineSettingMakerBlendFuncSeparate::cfxGlPipelineSettingMakerBlendFuncSeparate()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_BLEND_FUNC_SEPARATE, this));
}

cfxGlPipelineSettingMakerBlendFuncSeparate cfxGlPipelineSettingMakerBlendFuncSeparate::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerBlendFuncSeparate::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domSrcRGB = settingInitializer->getBlend_func_separate()->getSrc_rgb()->getValue();
  int srcRGB = cfxGlEnumMaps::getBlendTypeEnum(domSrcRGB);

  int domDstRGB = settingInitializer->getBlend_func_separate()->getDest_rgb()->getValue();
  int dstRGB = cfxGlEnumMaps::getBlendTypeEnum(domDstRGB);

  int domSrcAlpha = settingInitializer->getBlend_func_separate()->getSrc_alpha()->getValue();
  int srcAlpha = cfxGlEnumMaps::getBlendTypeEnum(domSrcAlpha);

  int domDstAlpha = settingInitializer->getBlend_func_separate()->getDest_alpha()->getValue();
  int dstAlpha = cfxGlEnumMaps::getBlendTypeEnum(domDstAlpha);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingBlendFuncSeparate(pass, srcRGB, dstRGB, srcAlpha, dstAlpha);
  return setting;
}


// cfxGlPipelineSettingMakerBlendEquation
cfxGlPipelineSettingMakerBlendEquation::cfxGlPipelineSettingMakerBlendEquation()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_BLEND_EQUATION, this));
}

cfxGlPipelineSettingMakerBlendEquation cfxGlPipelineSettingMakerBlendEquation::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerBlendEquation::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domEquation = settingInitializer->getBlend_equation()->getValue();
  int equation = cfxGlEnumMaps::getBlendEquationTypeEnum(domEquation);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingBlendEquation(pass, equation);
  return setting;
}


// cfxGlPipelineSettingMakerBlendEquationSeparate
cfxGlPipelineSettingMakerBlendEquationSeparate::cfxGlPipelineSettingMakerBlendEquationSeparate()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_BLEND_EQUATION_SEPARATE, this));
}

cfxGlPipelineSettingMakerBlendEquationSeparate cfxGlPipelineSettingMakerBlendEquationSeparate::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerBlendEquationSeparate::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domRgb = settingInitializer->getBlend_equation_separate()->getRgb()->getValue();
  int rgb = cfxGlEnumMaps::getBlendEquationTypeEnum(domRgb); 

  int domAlpha = settingInitializer->getBlend_equation_separate()->getAlpha()->getValue();
  int alpha = cfxGlEnumMaps::getBlendEquationTypeEnum(domAlpha);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingBlendEquationSeparate(pass, rgb, alpha);
  return setting;
}


// cfxGlPipelineSettingMakerBlendColor
cfxGlPipelineSettingMakerBlendColor::cfxGlPipelineSettingMakerBlendColor()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_BLEND_COLOR, this));
}

cfxGlPipelineSettingMakerBlendColor cfxGlPipelineSettingMakerBlendColor::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerBlendColor::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  domFloat4& domValue = settingInitializer->getBlend_color()->getValue();
  cfxFloat4 value;
  value.f0 = (cfxFloat) domValue.get(0);
  value.f1 = (cfxFloat) domValue.get(1);
  value.f2 = (cfxFloat) domValue.get(2);
  value.f3 = (cfxFloat) domValue.get(3);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingBlendColor(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerClearColor
cfxGlPipelineSettingMakerClearColor::cfxGlPipelineSettingMakerClearColor()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_CLEAR_COLOR, this));
}

cfxGlPipelineSettingMakerClearColor cfxGlPipelineSettingMakerClearColor::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerClearColor::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  domFloat4& domValue = settingInitializer->getClear_color()->getValue();
  cfxFloat4 value;
  value.f0 = (cfxFloat) domValue.get(0);
  value.f1 = (cfxFloat) domValue.get(1);
  value.f2 = (cfxFloat) domValue.get(2);
  value.f3 = (cfxFloat) domValue.get(3);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingClearColor(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerClearStencil
cfxGlPipelineSettingMakerClearStencil::cfxGlPipelineSettingMakerClearStencil()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_CLEAR_STENCIL, this));
}

cfxGlPipelineSettingMakerClearStencil cfxGlPipelineSettingMakerClearStencil::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerClearStencil::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxInt value = (cfxInt) settingInitializer->getClear_stencil()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingClearStencil(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerClearDepth
cfxGlPipelineSettingMakerClearDepth::cfxGlPipelineSettingMakerClearDepth()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_CLEAR_DEPTH, this));
}

cfxGlPipelineSettingMakerClearDepth cfxGlPipelineSettingMakerClearDepth::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerClearDepth::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat value = (cfxFloat) settingInitializer->getClear_depth()->getValue();
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingClearDepth(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerClipPlane
cfxGlPipelineSettingMakerClipPlane::cfxGlPipelineSettingMakerClipPlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_CLIP_PLANE, this));
}

cfxGlPipelineSettingMakerClipPlane cfxGlPipelineSettingMakerClipPlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerClipPlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index = settingInitializer->getClip_plane()->getIndex();

  domFloat4& domValue = settingInitializer->getClip_plane()->getValue();
  cfxFloat4 value;
  value.f0 = (cfxFloat) domValue.get(0);
  value.f1 = (cfxFloat) domValue.get(1);
  value.f2 = (cfxFloat) domValue.get(2);
  value.f3 = (cfxFloat) domValue.get(3);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingClipPlane(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerColorMask
cfxGlPipelineSettingMakerColorMask::cfxGlPipelineSettingMakerColorMask()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_COLOR_MASK, this));
}

cfxGlPipelineSettingMakerColorMask cfxGlPipelineSettingMakerColorMask::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerColorMask::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  domBool4& domValue = settingInitializer->getColor_mask()->getValue();
  cfxBool4 value;
  value.b0 = domValue.get(0);
  value.b1 = domValue.get(1);
  value.b2 = domValue.get(2);
  value.b3 = domValue.get(3);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingColorMask(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerColorMaterial
cfxGlPipelineSettingMakerColorMaterial::cfxGlPipelineSettingMakerColorMaterial()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_COLOR_MATERIAL, this));
}

cfxGlPipelineSettingMakerColorMaterial cfxGlPipelineSettingMakerColorMaterial::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerColorMaterial::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domFace = settingInitializer->getColor_material()->getFace()->getValue();
  int face = cfxGlEnumMaps::getFaceTypeEnum(domFace);

  int domMode = settingInitializer->getColor_material()->getMode()->getValue();
  int mode = cfxGlEnumMaps::getModeTypeEnum(domMode);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingColorMaterial(pass, face, mode);
  return setting;
}


// cfxGlPipelineSettingMakerCullFace
cfxGlPipelineSettingMakerCullFace::cfxGlPipelineSettingMakerCullFace()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_CULL_FACE, this));
}

cfxGlPipelineSettingMakerCullFace cfxGlPipelineSettingMakerCullFace::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerCullFace::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domValue = settingInitializer->getCull_face()->getValue();
  int value = cfxGlEnumMaps::getFaceTypeEnum(domValue);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingCullFace(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerDepthFunc
cfxGlPipelineSettingMakerDepthFunc::cfxGlPipelineSettingMakerDepthFunc()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_DEPTH_FUNC, this));
}

cfxGlPipelineSettingMakerDepthFunc cfxGlPipelineSettingMakerDepthFunc::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerDepthFunc::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domValue = settingInitializer->getDepth_func()->getValue();
  int value = cfxGlEnumMaps::getFuncTypeEnum( domValue );

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingDepthFunc(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerDepthMask
cfxGlPipelineSettingMakerDepthMask::cfxGlPipelineSettingMakerDepthMask()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_DEPTH_MASK, this));
}

cfxGlPipelineSettingMakerDepthMask cfxGlPipelineSettingMakerDepthMask::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerDepthMask::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  bool value = settingInitializer->getDepth_mask()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingDepthMask(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerDepthRange
cfxGlPipelineSettingMakerDepthRange::cfxGlPipelineSettingMakerDepthRange()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_DEPTH_RANGE, this));
}

cfxGlPipelineSettingMakerDepthRange cfxGlPipelineSettingMakerDepthRange::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerDepthRange::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat2 value;
  domFloat2& domValue = settingInitializer->getDepth_range()->getValue();
  value.f0 = (cfxFloat) domValue.get(0);
  value.f1 = (cfxFloat) domValue.get(1);

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingDepthRange(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerFogMode
cfxGlPipelineSettingMakerFogMode::cfxGlPipelineSettingMakerFogMode()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_FOG_MODE, this));
}

cfxGlPipelineSettingMakerFogMode cfxGlPipelineSettingMakerFogMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerFogMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int domValue = settingInitializer->getFog_mode()->getValue();
  int value = cfxGlEnumMaps::getFogModeTypeEnum( domValue );

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingFogMode(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerFogDensity
cfxGlPipelineSettingMakerFogDensity::cfxGlPipelineSettingMakerFogDensity()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_FOG_DENSITY, this));
}

cfxGlPipelineSettingMakerFogDensity cfxGlPipelineSettingMakerFogDensity::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerFogDensity::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  float value = (float)settingInitializer->getFog_density()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingFogDensity(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerFogStart
cfxGlPipelineSettingMakerFogStart::cfxGlPipelineSettingMakerFogStart()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_FOG_START, this));
}

cfxGlPipelineSettingMakerFogStart cfxGlPipelineSettingMakerFogStart::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerFogStart::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  float value = (float)settingInitializer->getFog_start()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingFogStart(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerFogEnd
cfxGlPipelineSettingMakerFogEnd::cfxGlPipelineSettingMakerFogEnd()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_FOG_END, this));
}

cfxGlPipelineSettingMakerFogEnd cfxGlPipelineSettingMakerFogEnd::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerFogEnd::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  float value = (float)settingInitializer->getFog_end()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingFogEnd(pass, value);
  return setting;
}


#if 0 
// these are currently unimplemented due to time constraints
// all that needs to be done is actually pull the values out of 
// the DOM and put them into the variables passed to the call to new

// cfxGlPipelineSettingMakerFogColor
cfxGlPipelineSettingMakerFogColor::cfxGlPipelineSettingMakerFogColor()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_FOG_COLOR, this));
}

cfxGlPipelineSettingMakerFogColor cfxGlPipelineSettingMakerFogColor::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerFogColor::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingFogColor(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerFrontFace
cfxGlPipelineSettingMakerFrontFace::cfxGlPipelineSettingMakerFrontFace()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_FRONT_FACE, this));
}

cfxGlPipelineSettingMakerFrontFace cfxGlPipelineSettingMakerFrontFace::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerFrontFace::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingFrontFace(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightModelAmbient
cfxGlPipelineSettingMakerLightModelAmbient::cfxGlPipelineSettingMakerLightModelAmbient()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_MODEL_AMBIENT, this));
}

cfxGlPipelineSettingMakerLightModelAmbient cfxGlPipelineSettingMakerLightModelAmbient::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightModelAmbient::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightModelAmbient(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightAmbient
cfxGlPipelineSettingMakerLightAmbient::cfxGlPipelineSettingMakerLightAmbient()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_AMBIENT, this));
}

cfxGlPipelineSettingMakerLightAmbient cfxGlPipelineSettingMakerLightAmbient::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightAmbient::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightAmbient(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightConstantAttenuation
cfxGlPipelineSettingMakerLightConstantAttenuation::cfxGlPipelineSettingMakerLightConstantAttenuation()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_CONSTANT_ATTENUATION, this));
}

cfxGlPipelineSettingMakerLightConstantAttenuation cfxGlPipelineSettingMakerLightConstantAttenuation::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightConstantAttenuation::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightConstantAttenuation(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightDiffuse
cfxGlPipelineSettingMakerLightDiffuse::cfxGlPipelineSettingMakerLightDiffuse()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_DIFFUSE, this));
}

cfxGlPipelineSettingMakerLightDiffuse cfxGlPipelineSettingMakerLightDiffuse::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightDiffuse::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightDiffuse(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightLinearAttenuation
cfxGlPipelineSettingMakerLightLinearAttenuation::cfxGlPipelineSettingMakerLightLinearAttenuation()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_LINEAR_ATTENUATION, this));
}

cfxGlPipelineSettingMakerLightLinearAttenuation cfxGlPipelineSettingMakerLightLinearAttenuation::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightLinearAttenuation::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightLinearAttenuation(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightPosition
cfxGlPipelineSettingMakerLightPosition::cfxGlPipelineSettingMakerLightPosition()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_POSITION, this));
}

cfxGlPipelineSettingMakerLightPosition cfxGlPipelineSettingMakerLightPosition::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightPosition::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightPosition(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightQuadraticAttenuation
cfxGlPipelineSettingMakerLightQuadraticAttenuation::cfxGlPipelineSettingMakerLightQuadraticAttenuation()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_QUADRATIC_ATTENUATION, this));
}

cfxGlPipelineSettingMakerLightQuadraticAttenuation cfxGlPipelineSettingMakerLightQuadraticAttenuation::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightQuadraticAttenuation::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightQuadraticAttenuation(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightSpecular
cfxGlPipelineSettingMakerLightSpecular::cfxGlPipelineSettingMakerLightSpecular()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_SPECULAR, this));
}

cfxGlPipelineSettingMakerLightSpecular cfxGlPipelineSettingMakerLightSpecular::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightSpecular::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightSpecular(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightSpotCutoff
cfxGlPipelineSettingMakerLightSpotCutoff::cfxGlPipelineSettingMakerLightSpotCutoff()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_SPOT_CUTOFF, this));
}

cfxGlPipelineSettingMakerLightSpotCutoff cfxGlPipelineSettingMakerLightSpotCutoff::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightSpotCutoff::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightSpotCutoff(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightSpotDirection
cfxGlPipelineSettingMakerLightSpotDirection::cfxGlPipelineSettingMakerLightSpotDirection()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_SPOT_DIRECTION, this));
}

cfxGlPipelineSettingMakerLightSpotDirection cfxGlPipelineSettingMakerLightSpotDirection::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightSpotDirection::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxFloat3 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightSpotDirection(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLightSpotExponent
cfxGlPipelineSettingMakerLightSpotExponent::cfxGlPipelineSettingMakerLightSpotExponent()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LIGHT_SPOT_EXPONENT, this));
}

cfxGlPipelineSettingMakerLightSpotExponent cfxGlPipelineSettingMakerLightSpotExponent::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLightSpotExponent::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLightSpotExponent(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerLineWidth
cfxGlPipelineSettingMakerLineWidth::cfxGlPipelineSettingMakerLineWidth()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LINE_WIDTH, this));
}

cfxGlPipelineSettingMakerLineWidth cfxGlPipelineSettingMakerLineWidth::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLineWidth::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLineWidth(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerLogicOp
cfxGlPipelineSettingMakerLogicOp::cfxGlPipelineSettingMakerLogicOp()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_LOGIC_OP, this));
}

cfxGlPipelineSettingMakerLogicOp cfxGlPipelineSettingMakerLogicOp::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerLogicOp::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingLogicOp(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerMaterialAmbient
cfxGlPipelineSettingMakerMaterialAmbient::cfxGlPipelineSettingMakerMaterialAmbient()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_MATERIAL_AMBIENT, this));
}

cfxGlPipelineSettingMakerMaterialAmbient cfxGlPipelineSettingMakerMaterialAmbient::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerMaterialAmbient::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingMaterialAmbient(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerMaterialDiffuse
cfxGlPipelineSettingMakerMaterialDiffuse::cfxGlPipelineSettingMakerMaterialDiffuse()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_MATERIAL_DIFFUSE, this));
}

cfxGlPipelineSettingMakerMaterialDiffuse cfxGlPipelineSettingMakerMaterialDiffuse::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerMaterialDiffuse::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingMaterialDiffuse(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerMaterialEmission
cfxGlPipelineSettingMakerMaterialEmission::cfxGlPipelineSettingMakerMaterialEmission()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_MATERIAL_EMISSION, this));
}

cfxGlPipelineSettingMakerMaterialEmission cfxGlPipelineSettingMakerMaterialEmission::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerMaterialEmission::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingMaterialEmission(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerMaterialShininess
cfxGlPipelineSettingMakerMaterialShininess::cfxGlPipelineSettingMakerMaterialShininess()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_MATERIAL_SHININESS, this));
}

cfxGlPipelineSettingMakerMaterialShininess cfxGlPipelineSettingMakerMaterialShininess::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerMaterialShininess::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingMaterialShininess(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerMaterialSpecular
cfxGlPipelineSettingMakerMaterialSpecular::cfxGlPipelineSettingMakerMaterialSpecular()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_MATERIAL_SPECULAR, this));
}

cfxGlPipelineSettingMakerMaterialSpecular cfxGlPipelineSettingMakerMaterialSpecular::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerMaterialSpecular::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingMaterialSpecular(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerModelViewMatrix
cfxGlPipelineSettingMakerModelViewMatrix::cfxGlPipelineSettingMakerModelViewMatrix()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_MODEL_VIEW_MATRIX, this));
}

cfxGlPipelineSettingMakerModelViewMatrix cfxGlPipelineSettingMakerModelViewMatrix::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerModelViewMatrix::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4x4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingModelViewMatrix(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerPointSize
cfxGlPipelineSettingMakerPointSize::cfxGlPipelineSettingMakerPointSize()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_POINT_SIZE, this));
}

cfxGlPipelineSettingMakerPointSize cfxGlPipelineSettingMakerPointSize::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerPointSize::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  float value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingPointSize(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerPointSpriteCoordReplace
cfxGlPipelineSettingMakerPointSpriteCoordReplace::cfxGlPipelineSettingMakerPointSpriteCoordReplace()
{
  //mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_POINT_SPRITE_COORD_REPLACE, this));
}

cfxGlPipelineSettingMakerPointSpriteCoordReplace cfxGlPipelineSettingMakerPointSpriteCoordReplace::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerPointSpriteCoordReplace::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingPointSpriteCoordReplace(pass);
  return setting;
}


// cfxGlPipelineSettingMakerPointSpriteRMode
cfxGlPipelineSettingMakerPointSpriteRMode::cfxGlPipelineSettingMakerPointSpriteRMode()
{
  //mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_POINT_SPRITE_R_MODE, this));
}

cfxGlPipelineSettingMakerPointSpriteRMode cfxGlPipelineSettingMakerPointSpriteRMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerPointSpriteRMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingPointSpriteRMode(pass);
  return setting;
}


// cfxGlPipelineSettingMakerPolygonMode
cfxGlPipelineSettingMakerPolygonMode::cfxGlPipelineSettingMakerPolygonMode()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_POLYGON_MODE, this));
}

cfxGlPipelineSettingMakerPolygonMode cfxGlPipelineSettingMakerPolygonMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerPolygonMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int face;
  int mode;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingPolygonMode(pass, face, mode);
  return setting;
}


// cfxGlPipelineSettingMakerPolygonOffset
cfxGlPipelineSettingMakerPolygonOffset::cfxGlPipelineSettingMakerPolygonOffset()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_POLYGON_OFFSET, this));
}

cfxGlPipelineSettingMakerPolygonOffset cfxGlPipelineSettingMakerPolygonOffset::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerPolygonOffset::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat2 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingPolygonOffset(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerProjectionMatrix
cfxGlPipelineSettingMakerProjectionMatrix::cfxGlPipelineSettingMakerProjectionMatrix()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_PROJECTION_MATRIX, this));
}

cfxGlPipelineSettingMakerProjectionMatrix cfxGlPipelineSettingMakerProjectionMatrix::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerProjectionMatrix::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxFloat4x4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingProjectionMatrix(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerScissor
cfxGlPipelineSettingMakerScissor::cfxGlPipelineSettingMakerScissor()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_SCISSOR, this));
}

cfxGlPipelineSettingMakerScissor cfxGlPipelineSettingMakerScissor::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerScissor::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxInt4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingScissor(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerShadeModel
cfxGlPipelineSettingMakerShadeModel::cfxGlPipelineSettingMakerShadeModel()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_SHADE_MODEL, this));
}

cfxGlPipelineSettingMakerShadeModel cfxGlPipelineSettingMakerShadeModel::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerShadeModel::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingShadeModel(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerStencilFunc
cfxGlPipelineSettingMakerStencilFunc::cfxGlPipelineSettingMakerStencilFunc()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_STENCIL_FUNC, this));
}

cfxGlPipelineSettingMakerStencilFunc cfxGlPipelineSettingMakerStencilFunc::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerStencilFunc::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int func;
  int ref;
  int mask;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingStencilFunc(pass, func, ref, mask);
  return setting;
}


// cfxGlPipelineSettingMakerStencilMask
cfxGlPipelineSettingMakerStencilMask::cfxGlPipelineSettingMakerStencilMask()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_STENCIL_MASK, this));
}

cfxGlPipelineSettingMakerStencilMask cfxGlPipelineSettingMakerStencilMask::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerStencilMask::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingStencilMask(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerStencilOp
cfxGlPipelineSettingMakerStencilOp::cfxGlPipelineSettingMakerStencilOp()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_STENCIL_OP, this));
}

cfxGlPipelineSettingMakerStencilOp cfxGlPipelineSettingMakerStencilOp::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerStencilOp::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int fail;
  int zFail;
  int zPass;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingStencilOp(pass, fail, zFail, zPass);
  return setting;
}


// cfxGlPipelineSettingMakerStencilFuncSeparate
cfxGlPipelineSettingMakerStencilFuncSeparate::cfxGlPipelineSettingMakerStencilFuncSeparate()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_STENCIL_FUNC_SEPARATE, this));
}

cfxGlPipelineSettingMakerStencilFuncSeparate cfxGlPipelineSettingMakerStencilFuncSeparate::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerStencilFuncSeparate::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int front;
  int back;
  int ref;
  int mask;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingStencilFuncSeparate(pass, front, back, ref, mask);
  return setting;
}


// cfxGlPipelineSettingMakerStencilMaskSeparate
cfxGlPipelineSettingMakerStencilMaskSeparate::cfxGlPipelineSettingMakerStencilMaskSeparate()
{
  //mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_STENCIL_MASK_SEPARATE, this));
}

cfxGlPipelineSettingMakerStencilMaskSeparate cfxGlPipelineSettingMakerStencilMaskSeparate::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerStencilMaskSeparate::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int face;
  int mask;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingStencilMaskSeparate(pass, face, mask);
  return setting;
}


// cfxGlPipelineSettingMakerStencilOpSeparate
cfxGlPipelineSettingMakerStencilOpSeparate::cfxGlPipelineSettingMakerStencilOpSeparate()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_STENCIL_OP_SEPARATE, this));
}

cfxGlPipelineSettingMakerStencilOpSeparate cfxGlPipelineSettingMakerStencilOpSeparate::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerStencilOpSeparate::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int face;
  int fail;	
  int zFail;
  int zPass;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingStencilOpSeparate(pass, face, fail, zFail, zPass);
  return setting;
}

#endif


#if 0 // all texgen makers unimplemented because not in COLLADA yet

// cfxGlPipelineSettingMakerTexGenSMode
cfxGlPipelineSettingMakerTexGenSMode::cfxGlPipelineSettingMakerTexGenSMode()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenSMode cfxGlPipelineSettingMakerTexGenSMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenSMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenSMode(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenTMode
cfxGlPipelineSettingMakerTexGenTMode::cfxGlPipelineSettingMakerTexGenTMode()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenTMode cfxGlPipelineSettingMakerTexGenTMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenTMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenTMode(pass, );
  return setting;
}

// cfxGlPipelineSettingMakerTexGenRMode
cfxGlPipelineSettingMakerTexGenRMode::cfxGlPipelineSettingMakerTexGenRMode()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenRMode cfxGlPipelineSettingMakerTexGenRMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenRMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenRMode(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenQMode
cfxGlPipelineSettingMakerTexGenQMode::cfxGlPipelineSettingMakerTexGenQMode()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenQMode cfxGlPipelineSettingMakerTexGenQMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenQMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenQMode(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenSEyePlane
cfxGlPipelineSettingMakerTexGenSEyePlane::cfxGlPipelineSettingMakerTexGenSEyePlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenSEyePlane cfxGlPipelineSettingMakerTexGenSEyePlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenSEyePlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenSEyePlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenTEyePlane
cfxGlPipelineSettingMakerTexGenTEyePlane::cfxGlPipelineSettingMakerTexGenTEyePlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenTEyePlane cfxGlPipelineSettingMakerTexGenTEyePlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenTEyePlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenTEyePlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenREyePlane
cfxGlPipelineSettingMakerTexGenREyePlane::cfxGlPipelineSettingMakerTexGenREyePlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenREyePlane cfxGlPipelineSettingMakerTexGenREyePlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenREyePlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenREyePlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenQEyePlane
cfxGlPipelineSettingMakerTexGenQEyePlane::cfxGlPipelineSettingMakerTexGenQEyePlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenQEyePlane cfxGlPipelineSettingMakerTexGenQEyePlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenQEyePlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenQEyePlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenSObjectPlane
cfxGlPipelineSettingMakerTexGenSObjectPlane::cfxGlPipelineSettingMakerTexGenSObjectPlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenSObjectPlane cfxGlPipelineSettingMakerTexGenSObjectPlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenSObjectPlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenSObjectPlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenTObjectPlane
cfxGlPipelineSettingMakerTexGenTObjectPlane::cfxGlPipelineSettingMakerTexGenTObjectPlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenTObjectPlane cfxGlPipelineSettingMakerTexGenTObjectPlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenTObjectPlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenTObjectPlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenRObjectPlane
cfxGlPipelineSettingMakerTexGenRObjectPlane::cfxGlPipelineSettingMakerTexGenRObjectPlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenRObjectPlane cfxGlPipelineSettingMakerTexGenRObjectPlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenRObjectPlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenRObjectPlane(pass, );
  return setting;
}


// cfxGlPipelineSettingMakerTexGenQObjectPlane
cfxGlPipelineSettingMakerTexGenQObjectPlane::cfxGlPipelineSettingMakerTexGenQObjectPlane()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMakerTexGenQObjectPlane cfxGlPipelineSettingMakerTexGenQObjectPlane::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexGenQObjectPlane::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexGenQObjectPlane(pass, );
  return setting;
}

#endif


#if 0 
// these are currently unimplemented due to time constraints
// all that needs to be done is actually pull the values out of 
// the DOM and put them into the variables passed to the call to new

// cfxGlPipelineSettingMakerTexture2D
cfxGlPipelineSettingMakerTexture2D::cfxGlPipelineSettingMakerTexture2D()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_TEXTURE2D, this));
}

cfxGlPipelineSettingMakerTexture2D cfxGlPipelineSettingMakerTexture2D::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexture2D::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxSampler* sampler;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexture2D(pass, index, sampler);
  return setting;
}


// cfxGlPipelineSettingMakerTexture3D
cfxGlPipelineSettingMakerTexture3D::cfxGlPipelineSettingMakerTexture3D()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_TEXTURE3D, this));
}

cfxGlPipelineSettingMakerTexture3D cfxGlPipelineSettingMakerTexture3D::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTexture3D::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxSampler* sampler;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTexture3D(pass, index, sampler);
  return setting;
}


// cfxGlPipelineSettingMakerTextureCubeMap
cfxGlPipelineSettingMakerTextureCubeMap::cfxGlPipelineSettingMakerTextureCubeMap()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_TEXTURECUBE, this));
}

cfxGlPipelineSettingMakerTextureCubeMap cfxGlPipelineSettingMakerTextureCubeMap::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTextureCubeMap::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  cfxSampler* sampler;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTextureCubeMap(pass, index, sampler);
  return setting;
}


// cfxGlPipelineSettingMakerTextureEnvColor
cfxGlPipelineSettingMakerTextureEnvColor::cfxGlPipelineSettingMakerTextureEnvColor()
{
  //mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_TEXTURE_ENV_COLOR, this));
}

cfxGlPipelineSettingMakerTextureEnvColor cfxGlPipelineSettingMakerTextureEnvColor::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTextureEnvColor::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;	
  cfxFloat4 value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTextureEnvColor(pass, index, value);
  return setting;
}


// cfxGlPipelineSettingMakerTextureEnvMode
cfxGlPipelineSettingMakerTextureEnvMode::cfxGlPipelineSettingMakerTextureEnvMode()
{
  //mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_TEXTURE_ENV_MODE, this));
}

cfxGlPipelineSettingMakerTextureEnvMode cfxGlPipelineSettingMakerTextureEnvMode::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerTextureEnvMode::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  int index;
  int value;

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingTextureEnvMode(pass, index, value);
  return setting;
}

#endif


// ENABLE / DISABLE settings


// cfxGlPipelineSettingMakerAlphaTestEnable
cfxGlPipelineSettingMakerAlphaTestEnable::cfxGlPipelineSettingMakerAlphaTestEnable()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_ALPHA_TEST_ENABLE, this));
}

cfxGlPipelineSettingMakerAlphaTestEnable cfxGlPipelineSettingMakerAlphaTestEnable::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerAlphaTestEnable::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  bool value = settingInitializer->getAlpha_test_enable()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingAlphaTestEnable(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerBlendEnable
cfxGlPipelineSettingMakerBlendEnable::cfxGlPipelineSettingMakerBlendEnable()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_BLEND_ENABLE, this));
}

cfxGlPipelineSettingMakerBlendEnable cfxGlPipelineSettingMakerBlendEnable::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerBlendEnable::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  bool value = settingInitializer->getBlend_enable()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingBlendEnable(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerCullFaceEnable
cfxGlPipelineSettingMakerCullFaceEnable::cfxGlPipelineSettingMakerCullFaceEnable()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_CULL_FACE_ENABLE, this));
}

cfxGlPipelineSettingMakerCullFaceEnable cfxGlPipelineSettingMakerCullFaceEnable::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerCullFaceEnable::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  bool value = settingInitializer->getCull_face_enable()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingCullFaceEnable(pass, value);
  return setting;
}


// cfxGlPipelineSettingMakerDepthTestEnable
cfxGlPipelineSettingMakerDepthTestEnable::cfxGlPipelineSettingMakerDepthTestEnable()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_DEPTH_TEST_ENABLE, this));
}

cfxGlPipelineSettingMakerDepthTestEnable cfxGlPipelineSettingMakerDepthTestEnable::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMakerDepthTestEnable::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  bool value = settingInitializer->getDepth_test_enable()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSettingDepthTestEnable(pass, value);
  return setting;
}


#if 0 // template
// cfxGlPipelineSettingMaker
cfxGlPipelineSettingMaker::cfxGlPipelineSettingMaker()
{
  mapTypeNameToSettingMaker.insert(std::make_pair(COLLADA_ELEMENT_, this));
}

cfxGlPipelineSettingMaker cfxGlPipelineSettingMaker::maker;

cfxGlPipelineSetting* cfxGlPipelineSettingMaker::makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer)
{
  bool value = settingInitializer->get_enable()->getValue();

  cfxGlPipelineSetting* setting = new cfxGlPipelineSetting(pass, value);
  return setting;
}
#endif




