/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_GLPIPELINESETTING_H
#define _CFX_GLPIPELINESETTING_H

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

#include <cfxTypes.h>

class cfxPass;
class cfxSampler;

// cfxGlPipelineSetting
class cfxGlPipelineSetting
{
public:
	enum type_enum {
		ALPHA_FUNC,
		BLEND_FUNC,
		BLEND_FUNC_SEPARATE,
		BLEND_EQUATION,
		BLEND_EQUATION_SEPARATE,
		BLEND_COLOR,
		CLEAR_COLOR,
		CLEAR_STENCIL,
		CLEAR_DEPTH,
		CLIP_PLANE,
		COLOR_MASK,
		COLOR_MATERIAL,
		CULL_FACE,
		DEPTH_FUNC,
		DEPTH_MASK,
		DEPTH_RANGE,
		FOG_MODE,
		FOG_DENSITY,
		FOG_START,
		FOG_END,
		FOG_COLOR,
		FRONT_FACE,
		LIGHT_MODEL_AMBEINT,
		LIGHT_AMBIENT,
		LIGHT_CONSTANT_ATTENUATION,
		LIGHT_DIFFUSE,
		LIGHT_LINEAR_ATTENUATION,
		LIGHT_POSITION,
		LIGHT_QUADRATIC_ATTENUATION,
		LIGHT_SPECULAR,
		LIGHT_SPOT_CUTOFF,
		LIGHT_SPOT_DIRECTION,
		LIGHT_SPOT_EXPONENT,
		LINE_WIDTH,
		LOGIC_OP,
		MATERIAL_AMBIENT,
		MATERIAL_DIFFUSE,
		MATERIAL_EMISSION,
		MATERIAL_SHININESS,
		MATERIAL_SPECULAR,
		MODEL_VIEW_MATRIX,
		POINT_SIZE,
		POINT_SPRITE_COORD_REPLACE,
		POINT_SPRITE_R_MODE,
		POLYGON_MODE,
		POLYGON_OFFSET,
		PROJECTION_MATRIX,
		SCISSOR,
		SHADE_MODEL,
		STENCIL_FUNC,
		STENCIL_MASK,
		STENCIL_OP,
		STENCIL_FUNC_SEPARATE,
		STENCIL_MASK_SEPARATE,
		STENCIL_OP_SEPAREATE,
		TEX_GEN_S_MODE,
		TEX_GEN_T_MODE,
		TEX_GEN_R_MODE,
		TEX_GEN_Q_MODE,
		TEX_GEN_S_EYE_PLANE,
		TEX_GEN_T_EYE_PLANE,
		TEX_GEN_R_EYE_PLANE,
		TEX_GEN_Q_EYE_PLANE,
		TEX_GEN_S_OBJECT_PLANE,
		TEX_GEN_T_OBJECT_PLANE,
		TEX_GEN_R_OBJECT_PLANE,
		TEX_GEN_Q_OBJECT_PLANE,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE_MAP,
		TEXTURE_ENV_COLOR,
		TEXTURE_ENV_MODE,
		ALPHA_TEST_ENABLE,
		BLEND_ENABLE,
		CULL_FACE_ENABLE,
		DEPTH_TEST_ENABLE
	};

 public:

  cfxGlPipelineSetting(cfxPass* _pass, const char* stateName, int index=-1);
  virtual ~cfxGlPipelineSetting();

  virtual cfxBool apply() = 0;

  virtual type_enum getType() const = 0;

 protected:

  cfxPass* pass;

  CGstate state;
  CGstateassignment assignment;

};


// cfxGlPipelineSettingAlphaFunc
class cfxGlPipelineSettingAlphaFunc : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingAlphaFunc(cfxPass* _pass, int _func, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFunc() const;
  float getValue() const;

 protected:
  
  int func;
  float value;
};


// cfxGlPipelineSettingBlendFunc
class cfxGlPipelineSettingBlendFunc : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingBlendFunc(cfxPass* _pass, int _src, int _dst);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getSrc() const;
  int getDst() const;

 protected:

  int src;
  int dst;	
};


// cfxGlPipelineSettingBlendFuncSeparate
class cfxGlPipelineSettingBlendFuncSeparate : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingBlendFuncSeparate(cfxPass* _pass, int _srcRGB, int _dstRGB, int _srcAlpha, int _dstAlpha);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getSrcRGB() const;
  int getDstRGD() const;
  int getSrcAlpha() const;
  int getDstAlpha() const;

 protected:

  int srcRGB;
  int dstRGB;
  int srcAlpha;
  int dstAlpha;
};


// cfxGlPipelineSettingBlendEquation
class cfxGlPipelineSettingBlendEquation : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingBlendEquation(cfxPass* _pass, int _equation);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getEquation() const;

 protected:

  int equation;
};


// cfxGlPipelineSettingBlendEquationSeparate
class cfxGlPipelineSettingBlendEquationSeparate : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingBlendEquationSeparate(cfxPass* _pass, int _rgb, int _alpha);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getRgb() const;
  int getAlpha() const;

 protected:

  int rgb;
  int alpha;
};


// cfxGlPipelineSettingBlendColor
class cfxGlPipelineSettingBlendColor : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingBlendColor(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingClearColor
class cfxGlPipelineSettingClearColor : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingClearColor(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingClearStencil
class cfxGlPipelineSettingClearStencil : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingClearStencil(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingClearDepth
class cfxGlPipelineSettingClearDepth : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingClearDepth(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingClipPlane
class cfxGlPipelineSettingClipPlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingClipPlane(cfxPass* _pass, int _index, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat4 getValue() const;

 protected:

  int index;
  cfxFloat4 value;
};


// cfxGlPipelineSettingColorMask
class cfxGlPipelineSettingColorMask : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingColorMask(cfxPass* _pass, cfxBool4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool4 getValue() const;

 protected:

  cfxBool4 value;
};


// cfxGlPipelineSettingColorMaterial
class cfxGlPipelineSettingColorMaterial  : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingColorMaterial(cfxPass* _pass, int _face, int _mode);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFace() const;
  int getMode() const;

 protected:

  int face;
  int mode;
};


// cfxGlPipelineSettingCullFace
class cfxGlPipelineSettingCullFace : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingCullFace(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingDepthFunc
class cfxGlPipelineSettingDepthFunc : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingDepthFunc(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingDepthMask
class cfxGlPipelineSettingDepthMask : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingDepthMask(cfxPass* _pass, cfxBool _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool getValue() const;

 protected:

  cfxBool value;
};


// cfxGlPipelineSettingDepthRange
class cfxGlPipelineSettingDepthRange : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingDepthRange(cfxPass* _pass, cfxFloat2& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat2 getValue() const;

 protected:

  cfxFloat2 value;
};


// cfxGlPipelineSettingFogMode
class cfxGlPipelineSettingFogMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingFogMode(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingFogDensity
class cfxGlPipelineSettingFogDensity : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingFogDensity(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingFogStart
class cfxGlPipelineSettingFogStart : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingFogStart(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingFogEnd
class cfxGlPipelineSettingFogEnd : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingFogEnd(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingFogColor
class cfxGlPipelineSettingFogColor : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingFogColor(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingFrontFace
class cfxGlPipelineSettingFrontFace : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingFrontFace(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingLightModelAmbient
class cfxGlPipelineSettingLightModelAmbient : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightModelAmbient(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingLightAmbient
class cfxGlPipelineSettingLightAmbient : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightAmbient(cfxPass* _pass, int _index, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat4 getValue() const;

 protected:

  int index;
  cfxFloat4 value;
};


// cfxGlPipelineSettingLightConstantAttenuation
class cfxGlPipelineSettingLightConstantAttenuation : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightConstantAttenuation(cfxPass* _pass, int _index, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  float getValue() const;

 protected:

  int index;
  float value;
};


// cfxGlPipelineSettingLightDiffuse
class cfxGlPipelineSettingLightDiffuse : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightDiffuse(cfxPass* _pass, int _index, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat4 getValue() const;

 protected:

  int index;
  cfxFloat4 value;
};


// cfxGlPipelineSettingLightLinearAttenuation
class cfxGlPipelineSettingLightLinearAttenuation : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightLinearAttenuation(cfxPass* _pass, int _index, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  float getValue() const;

 protected:

  int index;
  float value;
};


// cfxGlPipelineSettingLightPosition
class cfxGlPipelineSettingLightPosition : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightPosition(cfxPass* _pass, int _index, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat4 getValue() const;

 protected:

  int index;
  cfxFloat4 value;
};


// cfxGlPipelineSettingLightQuadraticAttenuation
class cfxGlPipelineSettingLightQuadraticAttenuation : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightQuadraticAttenuation(cfxPass* _pass, int _index, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  float getValue() const;

 protected:

  int index;
  float value;
};


// cfxGlPipelineSettingLightSpecular
class cfxGlPipelineSettingLightSpecular : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightSpecular(cfxPass* _pass, int _index, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat4 getValue() const;

 protected:

  int index;
  cfxFloat4 value;
};


// cfxGlPipelineSettingLightSpotCutoff
class cfxGlPipelineSettingLightSpotCutoff : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightSpotCutoff(cfxPass* _pass, int _index, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  float getValue() const;

 protected:

  int index;
  float value;
};


// cfxGlPipelineSettingLightSpotDirection
class cfxGlPipelineSettingLightSpotDirection : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightSpotDirection(cfxPass* _pass, int _index, cfxFloat3& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat3 getValue() const;

 protected:

  int index;
  cfxFloat3 value;
};


// cfxGlPipelineSettingLightSpotExponent
class cfxGlPipelineSettingLightSpotExponent : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLightSpotExponent(cfxPass* _pass, int _index, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  float getValue() const;

 protected:

  int index;
  float value;
};


// cfxGlPipelineSettingLineWidth
class cfxGlPipelineSettingLineWidth : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLineWidth(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingLogicOp
class cfxGlPipelineSettingLogicOp : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingLogicOp(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingMaterialAmbient
class cfxGlPipelineSettingMaterialAmbient : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingMaterialAmbient(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingMaterialDiffuse
class cfxGlPipelineSettingMaterialDiffuse : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingMaterialDiffuse(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingMaterialEmission
class cfxGlPipelineSettingMaterialEmission : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingMaterialEmission(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingMaterialShininess
class cfxGlPipelineSettingMaterialShininess : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingMaterialShininess(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingMaterialSpecular
class cfxGlPipelineSettingMaterialSpecular : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingMaterialSpecular(cfxPass* _pass, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4 getValue() const;

 protected:

  cfxFloat4 value;
};


// cfxGlPipelineSettingModelViewMatrix
class cfxGlPipelineSettingModelViewMatrix : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingModelViewMatrix(cfxPass* _pass, cfxFloat4x4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4x4 getValue() const;

 protected:

  cfxFloat4x4 value;
};


// cfxGlPipelineSettingPointSize
class cfxGlPipelineSettingPointSize : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingPointSize(cfxPass* _pass, float _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  float getValue() const;

 protected:

  float value;
};


// cfxGlPipelineSettingPointSpriteCoordReplace
class cfxGlPipelineSettingPointSpriteCoordReplace : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingPointSpriteCoordReplace(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingPointSpriteRMode
class cfxGlPipelineSettingPointSpriteRMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingPointSpriteRMode(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingPolygonMode
class cfxGlPipelineSettingPolygonMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingPolygonMode(cfxPass* _pass, int _face, int _mode);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFace() const;
  int getMode() const;

 protected:

  int face;
  int mode;
};


// cfxGlPipelineSettingPolygonOffset
class cfxGlPipelineSettingPolygonOffset : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingPolygonOffset(cfxPass* _pass, cfxFloat2& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat2 getValue() const;

 protected:

  cfxFloat2 value;
};


// cfxGlPipelineSettingProjectionMatrix
class cfxGlPipelineSettingProjectionMatrix : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingProjectionMatrix(cfxPass* _pass, cfxFloat4x4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxFloat4x4 getValue() const;

 protected:

  cfxFloat4x4 value;
};


// cfxGlPipelineSettingScissor
class cfxGlPipelineSettingScissor : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingScissor(cfxPass* _pass, cfxInt4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxInt4 getValue() const;

 protected:

  cfxInt4 value;
};


// cfxGlPipelineSettingShadeModel
class cfxGlPipelineSettingShadeModel : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingShadeModel(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingStencilFunc
class cfxGlPipelineSettingStencilFunc : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingStencilFunc(cfxPass* _pass, int _func, int _ref, int _mask);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFunc() const;
  int getRef() const;
  int getMask() const;

 protected:

  int func;
  int ref;
  int mask;
};


// cfxGlPipelineSettingStencilMask
class cfxGlPipelineSettingStencilMask : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingStencilMask(cfxPass* _pass, int _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getValue() const;

 protected:

  int value;
};


// cfxGlPipelineSettingStencilOp
class cfxGlPipelineSettingStencilOp : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingStencilOp(cfxPass* _pass, int _fail, int _zFail, int _zPass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFail() const;
  int getZFail() const;
  int getZPass() const;

 protected:

  int fail;
  int zFail;
  int zPass;
};


// cfxGlPipelineSettingStencilFuncSeparate
class cfxGlPipelineSettingStencilFuncSeparate : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingStencilFuncSeparate(cfxPass* _pass, int _front, int _back, int _ref, int _mask);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFront() const;
  int getBack() const;
  int getRef() const;
  int getMask() const;

 protected:

  int front;
  int back;
  int ref;
  int mask;
};


// cfxGlPipelineSettingStencilMaskSeparate
class cfxGlPipelineSettingStencilMaskSeparate : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingStencilMaskSeparate(cfxPass* _pass, int _face, int _mask);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFace() const;
  int getMask() const;

 protected:

  int face;
  int mask;
};


// cfxGlPipelineSettingStencilOpSeparate
class cfxGlPipelineSettingStencilOpSeparate : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingStencilOpSeparate(cfxPass* _pass, int _face, int _fail, int _zFail, int _zPass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getFace() const;
  int getFail() const;
  int getZFail() const;
  int getZPass() const;

 protected:

  int face;
  int fail;
  int zFail;
  int zPass;
};


// cfxGlPipelineSettingTexGenSMode
class cfxGlPipelineSettingTexGenSMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenSMode(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenTMode
class cfxGlPipelineSettingTexGenTMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenTMode(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenRMode
class cfxGlPipelineSettingTexGenRMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenRMode(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenQMode
class cfxGlPipelineSettingTexGenQMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenQMode(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenSEyePlane
class cfxGlPipelineSettingTexGenSEyePlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenSEyePlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenTEyePlane
class cfxGlPipelineSettingTexGenTEyePlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenTEyePlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenREyePlane
class cfxGlPipelineSettingTexGenREyePlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenREyePlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenQEyePlane
class cfxGlPipelineSettingTexGenQEyePlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenQEyePlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenSObjectPlane
class cfxGlPipelineSettingTexGenSObjectPlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenSObjectPlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenTObjectPlane
class cfxGlPipelineSettingTexGenTObjectPlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenTObjectPlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenRObjectPlane
class cfxGlPipelineSettingTexGenRObjectPlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenRObjectPlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexGenQObjectPlane
class cfxGlPipelineSettingTexGenQObjectPlane : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexGenQObjectPlane(cfxPass* _pass);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

 protected:

};


// cfxGlPipelineSettingTexture2D
class cfxGlPipelineSettingTexture2D : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexture2D(cfxPass* _pass, int _index, cfxSampler* _sampler);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  const cfxSampler *getSampler() const;

 protected:

  int index;
  cfxSampler* sampler;
};


// cfxGlPipelineSettingTexture3D
class cfxGlPipelineSettingTexture3D : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTexture3D(cfxPass* _pass, int _index, cfxSampler* _sampler);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  const cfxSampler *getSampler() const;

 protected:

  int index;
  cfxSampler* sampler;
};


// cfxGlPipelineSettingTextureCubeMap
class cfxGlPipelineSettingTextureCubeMap : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTextureCubeMap(cfxPass* _pass, int _index, cfxSampler* _sampler);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  const cfxSampler *getSampler() const;

 protected:

  int index;
  cfxSampler* sampler;
};


// cfxGlPipelineSettingTextureEnvColor
class cfxGlPipelineSettingTextureEnvColor : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTextureEnvColor(cfxPass* _pass, int _index, cfxFloat4& _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  cfxFloat4 getValue() const;

 protected:

  int index;
  cfxFloat4 value;
};


// cfxGlPipelineSettingTextureEnvMode
class cfxGlPipelineSettingTextureEnvMode : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingTextureEnvMode(cfxPass* _pass, int _index, int _mode);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  int getIndex() const;
  int getValue() const;

 protected:

  int index;
  int value;
};


// ENABLE / DISABLE settings


// cfxGlPipelineSettingAlphaTestEnable
class cfxGlPipelineSettingAlphaTestEnable : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingAlphaTestEnable(cfxPass* _pass, cfxBool _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool getValue() const;

 protected:

  cfxBool value;
};


// cfxGlPipelineSettingBlendEnable
class cfxGlPipelineSettingBlendEnable : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingBlendEnable(cfxPass* _pass, cfxBool _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool getValue() const;

 protected:

  cfxBool value;
};


// cfxGlPipelineSettingCullFaceEnable
class cfxGlPipelineSettingCullFaceEnable : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingCullFaceEnable(cfxPass* _pass, cfxBool _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool getValue() const;

 protected:

  cfxBool value;
};


// cfxGlPipelineSettingDepthTestEnable
class cfxGlPipelineSettingDepthTestEnable : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSettingDepthTestEnable(cfxPass* _pass, cfxBool _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool getValue() const;

 protected:

  cfxBool value;
};


#if 0
// cfxGlPipelineSetting
class cfxGlPipelineSetting : public cfxGlPipelineSetting
{
 public:

  cfxGlPipelineSetting(cfxPass* _pass, cfxBool _value);  
  virtual cfxBool apply();

  virtual type_enum getType() const;

  cfxBool getValue() const;

 protected:

  cfxBool value;
};
#endif


#endif // _CFX_GLPIPELINESETTING_H


