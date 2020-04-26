/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_GLPIPELINESETTINGMAKER_H
#define _CFX_GLPIPELINESETTINGMAKER_H

#include <map>
#include <string>

class cfxGlPipelineSetting;
class cfxPass;

class domGl_pipeline_settings;


// cfxGlPipelineSettingMaker
class cfxGlPipelineSettingMaker
{
 protected:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer) = 0;
  static std::map<std::string, cfxGlPipelineSettingMaker*> mapTypeNameToSettingMaker;
 public:
  virtual ~cfxGlPipelineSettingMaker() {}
  static cfxGlPipelineSetting* makeGlPipelineSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerAlphaFunc
class cfxGlPipelineSettingMakerAlphaFunc : public cfxGlPipelineSettingMaker
{
 protected:
  cfxGlPipelineSettingMakerAlphaFunc();
  static cfxGlPipelineSettingMakerAlphaFunc maker;
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerBlendFunc
class cfxGlPipelineSettingMakerBlendFunc : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerBlendFunc();
  static cfxGlPipelineSettingMakerBlendFunc maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerBlendFuncSeparate
class cfxGlPipelineSettingMakerBlendFuncSeparate : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerBlendFuncSeparate();
  static cfxGlPipelineSettingMakerBlendFuncSeparate maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerBlendEquation
class cfxGlPipelineSettingMakerBlendEquation : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerBlendEquation();
  static cfxGlPipelineSettingMakerBlendEquation maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerBlendEquationSeparate
class cfxGlPipelineSettingMakerBlendEquationSeparate : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerBlendEquationSeparate();
  static cfxGlPipelineSettingMakerBlendEquationSeparate maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerBlendColor
class cfxGlPipelineSettingMakerBlendColor : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerBlendColor();
  static cfxGlPipelineSettingMakerBlendColor maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerClearColor
class cfxGlPipelineSettingMakerClearColor : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerClearColor();
  static cfxGlPipelineSettingMakerClearColor maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerClearStenciil
class cfxGlPipelineSettingMakerClearStencil : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerClearStencil();
  static cfxGlPipelineSettingMakerClearStencil maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerClearDepth
class cfxGlPipelineSettingMakerClearDepth : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerClearDepth();
  static cfxGlPipelineSettingMakerClearDepth maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerClipPlane
class cfxGlPipelineSettingMakerClipPlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerClipPlane();
  static cfxGlPipelineSettingMakerClipPlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerColorMask
class cfxGlPipelineSettingMakerColorMask : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerColorMask();
  static cfxGlPipelineSettingMakerColorMask maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerColorMaterial
class cfxGlPipelineSettingMakerColorMaterial : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerColorMaterial();
  static cfxGlPipelineSettingMakerColorMaterial maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerCullFace
class cfxGlPipelineSettingMakerCullFace : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerCullFace();
  static cfxGlPipelineSettingMakerCullFace maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerDepthFunc
class cfxGlPipelineSettingMakerDepthFunc : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerDepthFunc();
  static cfxGlPipelineSettingMakerDepthFunc maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerDepthMask
class cfxGlPipelineSettingMakerDepthMask : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerDepthMask();
  static cfxGlPipelineSettingMakerDepthMask maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerDepthRange
class cfxGlPipelineSettingMakerDepthRange : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerDepthRange();
  static cfxGlPipelineSettingMakerDepthRange maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerFogMode
class cfxGlPipelineSettingMakerFogMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerFogMode();
  static cfxGlPipelineSettingMakerFogMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerFogDensity
class cfxGlPipelineSettingMakerFogDensity : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerFogDensity();
  static cfxGlPipelineSettingMakerFogDensity maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerFogStart
class cfxGlPipelineSettingMakerFogStart : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerFogStart();
  static cfxGlPipelineSettingMakerFogStart maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerFogEnd
class cfxGlPipelineSettingMakerFogEnd : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerFogEnd();
  static cfxGlPipelineSettingMakerFogEnd maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerFogColor
class cfxGlPipelineSettingMakerFogColor : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerFogColor();
  static cfxGlPipelineSettingMakerFogColor maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerFrontFace
class cfxGlPipelineSettingMakerFrontFace : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerFrontFace();
  static cfxGlPipelineSettingMakerFrontFace maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightModelAmbient
class cfxGlPipelineSettingMakerLightModelAmbient : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightModelAmbient();
  static cfxGlPipelineSettingMakerLightModelAmbient maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightAmbient
class cfxGlPipelineSettingMakerLightAmbient : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightAmbient();
  static cfxGlPipelineSettingMakerLightAmbient maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightConstantAttenuation
class cfxGlPipelineSettingMakerLightConstantAttenuation : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightConstantAttenuation();
  static cfxGlPipelineSettingMakerLightConstantAttenuation maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightDiffuse
class cfxGlPipelineSettingMakerLightDiffuse : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightDiffuse();
  static cfxGlPipelineSettingMakerLightDiffuse maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightLinearAttenuation
class cfxGlPipelineSettingMakerLightLinearAttenuation : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightLinearAttenuation();
  static cfxGlPipelineSettingMakerLightLinearAttenuation maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightPosition
class cfxGlPipelineSettingMakerLightPosition : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightPosition();
  static cfxGlPipelineSettingMakerLightPosition maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightQuadraticAttenuation
class cfxGlPipelineSettingMakerLightQuadraticAttenuation : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightQuadraticAttenuation();
  static cfxGlPipelineSettingMakerLightQuadraticAttenuation maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightSpecular
class cfxGlPipelineSettingMakerLightSpecular : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightSpecular();
  static cfxGlPipelineSettingMakerLightSpecular maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightSpotCutoff
class cfxGlPipelineSettingMakerLightSpotCutoff : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightSpotCutoff();
  static cfxGlPipelineSettingMakerLightSpotCutoff maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightSpotDirection
class cfxGlPipelineSettingMakerLightSpotDirection : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightSpotDirection();
  static cfxGlPipelineSettingMakerLightSpotDirection maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLightSpotExponent
class cfxGlPipelineSettingMakerLightSpotExponent : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLightSpotExponent();
  static cfxGlPipelineSettingMakerLightSpotExponent maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLineWidth
class cfxGlPipelineSettingMakerLineWidth : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLineWidth();
  static cfxGlPipelineSettingMakerLineWidth maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerLogicOp
class cfxGlPipelineSettingMakerLogicOp : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerLogicOp();
  static cfxGlPipelineSettingMakerLogicOp maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerMaterialAmbient
class cfxGlPipelineSettingMakerMaterialAmbient : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerMaterialAmbient();
  static cfxGlPipelineSettingMakerMaterialAmbient maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerMaterialDiffuse
class cfxGlPipelineSettingMakerMaterialDiffuse : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerMaterialDiffuse();
  static cfxGlPipelineSettingMakerMaterialDiffuse maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerMaterialEmission
class cfxGlPipelineSettingMakerMaterialEmission : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerMaterialEmission();
  static cfxGlPipelineSettingMakerMaterialEmission maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerMaterialShininess
class cfxGlPipelineSettingMakerMaterialShininess : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerMaterialShininess();
  static cfxGlPipelineSettingMakerMaterialShininess maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerMaterialSpecular
class cfxGlPipelineSettingMakerMaterialSpecular : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerMaterialSpecular();
  static cfxGlPipelineSettingMakerMaterialSpecular maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerModelViewMatrix
class cfxGlPipelineSettingMakerModelViewMatrix : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerModelViewMatrix();
  static cfxGlPipelineSettingMakerModelViewMatrix maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerPointSize
class cfxGlPipelineSettingMakerPointSize : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerPointSize();
  static cfxGlPipelineSettingMakerPointSize maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerPointSpriteCoordReplace
class cfxGlPipelineSettingMakerPointSpriteCoordReplace : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerPointSpriteCoordReplace();
  static cfxGlPipelineSettingMakerPointSpriteCoordReplace maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerPointSpriteRMode
class cfxGlPipelineSettingMakerPointSpriteRMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerPointSpriteRMode();
  static cfxGlPipelineSettingMakerPointSpriteRMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerPolygonMode
class cfxGlPipelineSettingMakerPolygonMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerPolygonMode();
  static cfxGlPipelineSettingMakerPolygonMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerPolygonOffset
class cfxGlPipelineSettingMakerPolygonOffset : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerPolygonOffset();
  static cfxGlPipelineSettingMakerPolygonOffset maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerProjectionMatrix
class cfxGlPipelineSettingMakerProjectionMatrix : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerProjectionMatrix();
  static cfxGlPipelineSettingMakerProjectionMatrix maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerScissor
class cfxGlPipelineSettingMakerScissor : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerScissor();
  static cfxGlPipelineSettingMakerScissor maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerShadeModel
class cfxGlPipelineSettingMakerShadeModel : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerShadeModel();
  static cfxGlPipelineSettingMakerShadeModel maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerStencilFunc
class cfxGlPipelineSettingMakerStencilFunc : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerStencilFunc();
  static cfxGlPipelineSettingMakerStencilFunc maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerStencilMask
class cfxGlPipelineSettingMakerStencilMask : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerStencilMask();
  static cfxGlPipelineSettingMakerStencilMask maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerStencilOp
class cfxGlPipelineSettingMakerStencilOp : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerStencilOp();
  static cfxGlPipelineSettingMakerStencilOp maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerStencilFuncSeparate
class cfxGlPipelineSettingMakerStencilFuncSeparate : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerStencilFuncSeparate();
  static cfxGlPipelineSettingMakerStencilFuncSeparate maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerStencilMaskSeparate
class cfxGlPipelineSettingMakerStencilMaskSeparate : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerStencilMaskSeparate();
  static cfxGlPipelineSettingMakerStencilMaskSeparate maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerStencilOpSeparate
class cfxGlPipelineSettingMakerStencilOpSeparate : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerStencilOpSeparate();
  static cfxGlPipelineSettingMakerStencilOpSeparate maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenSMode
class cfxGlPipelineSettingMakerTexGenSMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenSMode();
  static cfxGlPipelineSettingMakerTexGenSMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenTMode
class cfxGlPipelineSettingMakerTexGenTMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenTMode();
  static cfxGlPipelineSettingMakerTexGenTMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenRMode
class cfxGlPipelineSettingMakerTexGenRMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenRMode();
  static cfxGlPipelineSettingMakerTexGenRMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenQMode
class cfxGlPipelineSettingMakerTexGenQMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenQMode();
  static cfxGlPipelineSettingMakerTexGenQMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenSEyePlane
class cfxGlPipelineSettingMakerTexGenSEyePlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenSEyePlane();
  static cfxGlPipelineSettingMakerTexGenSEyePlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenTEyePlane
class cfxGlPipelineSettingMakerTexGenTEyePlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenTEyePlane();
  static cfxGlPipelineSettingMakerTexGenTEyePlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenREyePlane
class cfxGlPipelineSettingMakerTexGenREyePlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenREyePlane();
  static cfxGlPipelineSettingMakerTexGenREyePlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenQEyePlane
class cfxGlPipelineSettingMakerTexGenQEyePlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenQEyePlane();
  static cfxGlPipelineSettingMakerTexGenQEyePlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenSObjectPlane
class cfxGlPipelineSettingMakerTexGenSObjectPlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenSObjectPlane();
  static cfxGlPipelineSettingMakerTexGenSObjectPlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenTObjectPlane
class cfxGlPipelineSettingMakerTexGenTObjectPlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenTObjectPlane();
  static cfxGlPipelineSettingMakerTexGenTObjectPlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenRObjectPlane
class cfxGlPipelineSettingMakerTexGenRObjectPlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenRObjectPlane();
  static cfxGlPipelineSettingMakerTexGenRObjectPlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexGenQObjectPlane
class cfxGlPipelineSettingMakerTexGenQObjectPlane : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexGenQObjectPlane();
  static cfxGlPipelineSettingMakerTexGenQObjectPlane maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexture2D
class cfxGlPipelineSettingMakerTexture2D : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexture2D();
  static cfxGlPipelineSettingMakerTexture2D maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTexture3D
class cfxGlPipelineSettingMakerTexture3D : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTexture3D();
  static cfxGlPipelineSettingMakerTexture3D maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTextureCubeMap
class cfxGlPipelineSettingMakerTextureCubeMap : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTextureCubeMap();
  static cfxGlPipelineSettingMakerTextureCubeMap maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTextureEnvColor
class cfxGlPipelineSettingMakerTextureEnvColor : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTextureEnvColor();
  static cfxGlPipelineSettingMakerTextureEnvColor maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerTextureEnvMode
class cfxGlPipelineSettingMakerTextureEnvMode : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerTextureEnvMode();
  static cfxGlPipelineSettingMakerTextureEnvMode maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// ENABLE / DISABLE settings


// cfxGlPipelineSettingMakerAlphaTestEnable
class cfxGlPipelineSettingMakerAlphaTestEnable : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerAlphaTestEnable();
  static cfxGlPipelineSettingMakerAlphaTestEnable maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerBlendEnable
class cfxGlPipelineSettingMakerBlendEnable : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerBlendEnable();
  static cfxGlPipelineSettingMakerBlendEnable maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerCullFaceEnable
class cfxGlPipelineSettingMakerCullFaceEnable : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerCullFaceEnable();
  static cfxGlPipelineSettingMakerCullFaceEnable maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


// cfxGlPipelineSettingMakerDepthTestEnable
class cfxGlPipelineSettingMakerDepthTestEnable : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMakerDepthTestEnable();
  static cfxGlPipelineSettingMakerDepthTestEnable maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};


#if 0 // template
// cfxGlPipelineSettingMaker
class cfxGlPipelineSettingMaker : public cfxGlPipelineSettingMaker
{
 private:
  cfxGlPipelineSettingMaker();
  static cfxGlPipelineSettingMaker maker;
 public:
  virtual cfxGlPipelineSetting* makeSetting(cfxPass* pass, domGl_pipeline_settings* settingInitializer);
};
#endif


#endif // _CFX_GLPIPELINESETTINGMAKER_H

