/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <cfxPlatform.h>


// User includes

#include <cfxGlPipelineSetting.h>
#include <cfxPass.h>
#include <cfxTechnique.h>
#include <cfxEffect.h>

// cfxGlPipelineSetting
cfxGlPipelineSetting::cfxGlPipelineSetting(cfxPass* _pass, const char* stateName, int index)
  : pass(_pass)
{
  state = cgGetNamedState(_pass->getTechnique()->getEffect()->getContext(), stateName);
  if (state != 0)
    {
      printf("Got state for %s\n", stateName);
    }
  else
    {
      printf("ERROR:  state not found for %s\n", stateName);
    }

  if (index >= 0)
      {
	  assignment = cgCreateStateAssignmentIndex(_pass->getPass(), state, index);  
      }      
  else
      {
  assignment = cgCreateStateAssignment(_pass->getPass(), state);  
      }
}

cfxGlPipelineSetting::~cfxGlPipelineSetting()
{
}

// cfxGlPipelineSettingAlphaFunc
cfxGlPipelineSettingAlphaFunc::cfxGlPipelineSettingAlphaFunc(cfxPass* _pass, int _func, float _value)
  : cfxGlPipelineSetting(_pass, "AlphaFunc"),
    func(_func),
    value(_value)
{
}  

cfxBool cfxGlPipelineSettingAlphaFunc::apply() 
{
  float values[2];
  values[0] = (float)func;
  values[1] = value;
  cgSetFloatArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingAlphaFunc::getType() const
{
	return cfxGlPipelineSetting::ALPHA_FUNC;
}

int cfxGlPipelineSettingAlphaFunc::getFunc() const
{
	return func;
}

float cfxGlPipelineSettingAlphaFunc::getValue() const
{
	return value;
}


// cfxGlPipelineSettingBlendFunc
cfxGlPipelineSettingBlendFunc::cfxGlPipelineSettingBlendFunc(cfxPass* _pass, int _src, int _dst)
  : cfxGlPipelineSetting(_pass, "BlendFunc"),
    src(_src),
    dst(_dst)
{
}
  
cfxBool cfxGlPipelineSettingBlendFunc::apply() 
{
  int values[2];
  values[0] = src;
  values[1] = dst;

  if ( src && dst )
	cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingBlendFunc::getType() const
{
	return cfxGlPipelineSetting::BLEND_FUNC;
}

int cfxGlPipelineSettingBlendFunc::getSrc() const
{
	return src;
}

int cfxGlPipelineSettingBlendFunc::getDst() const
{
	return dst;
}


// cfxGlPipelineSettingBlendFuncSeparate
cfxGlPipelineSettingBlendFuncSeparate::cfxGlPipelineSettingBlendFuncSeparate(cfxPass* _pass, int _srcRGB, int _dstRGB, int _srcAlpha, int _dstAlpha)
  : cfxGlPipelineSetting(_pass, "BlendFuncSeparate"),
    srcRGB(_srcRGB),
    dstRGB(_dstRGB),
    srcAlpha(_srcAlpha),
    dstAlpha(_dstAlpha)
{
}
  
cfxBool cfxGlPipelineSettingBlendFuncSeparate::apply() 
{
  int values[4];
  values[0] = srcRGB;
  values[1] = dstRGB;
  values[2] = srcAlpha;
  values[3] = dstAlpha;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingBlendFuncSeparate::getType() const
{
	return cfxGlPipelineSetting::BLEND_FUNC_SEPARATE;
}

int cfxGlPipelineSettingBlendFuncSeparate::getSrcRGB() const
{
	return srcRGB;
}

int cfxGlPipelineSettingBlendFuncSeparate::getDstRGD() const
{
	return dstRGB;
}

int cfxGlPipelineSettingBlendFuncSeparate::getSrcAlpha() const
{
	return srcAlpha;
}

int cfxGlPipelineSettingBlendFuncSeparate::getDstAlpha() const
{
	return dstAlpha;
}


// cfxGlPipelineSettingBlendEquation
cfxGlPipelineSettingBlendEquation::cfxGlPipelineSettingBlendEquation(cfxPass* _pass, int _equation)
  : cfxGlPipelineSetting(_pass, "BlendEquation"),
    equation(_equation)
{
}
  
cfxBool cfxGlPipelineSettingBlendEquation::apply() 
{ 
  cgSetIntStateAssignment(assignment, equation);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingBlendEquation::getType() const
{
	return cfxGlPipelineSetting::BLEND_EQUATION;
}

int cfxGlPipelineSettingBlendEquation::getEquation() const
{
	return equation;
}


// cfxGlPipelineSettingBlendEquationSeparate
cfxGlPipelineSettingBlendEquationSeparate::cfxGlPipelineSettingBlendEquationSeparate(cfxPass* _pass, int _rgb, int _alpha)
  : cfxGlPipelineSetting(_pass, "BlendEquationSeparate"),
    rgb(_rgb),
    alpha(_alpha)
{
}
  
cfxBool cfxGlPipelineSettingBlendEquationSeparate::apply()
{  
  int values[2];
  values[0] = rgb;
  values[1] = alpha;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingBlendEquationSeparate::getType() const
{
	return cfxGlPipelineSetting::BLEND_EQUATION_SEPARATE;
}

int cfxGlPipelineSettingBlendEquationSeparate::getRgb() const
{
	return rgb;
}

int cfxGlPipelineSettingBlendEquationSeparate::getAlpha() const
{
	return alpha;
}


// cfxGlPipelineSettingBlendColor
cfxGlPipelineSettingBlendColor::cfxGlPipelineSettingBlendColor(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "BlendColor"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingBlendColor::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingBlendColor::getType() const
{
	return cfxGlPipelineSetting::BLEND_COLOR;
}

cfxFloat4 cfxGlPipelineSettingBlendColor::getValue() const
{
	return value;
}


// cfxGlPipelineSettingClearColor
cfxGlPipelineSettingClearColor::cfxGlPipelineSettingClearColor(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "ClearColor"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingClearColor::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingClearColor::getType() const
{
	return cfxGlPipelineSetting::CLEAR_COLOR;
}

cfxFloat4 cfxGlPipelineSettingClearColor::getValue() const
{
	return value;
}

// cfxGlPipelineSettingClearStencil
cfxGlPipelineSettingClearStencil::cfxGlPipelineSettingClearStencil(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "ClearStencil"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingClearStencil::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingClearStencil::getType() const
{
	return cfxGlPipelineSetting::CLEAR_STENCIL;
}

int cfxGlPipelineSettingClearStencil::getValue() const
{
	return value;
}

// cfxGlPipelineSettingClearDepth
cfxGlPipelineSettingClearDepth::cfxGlPipelineSettingClearDepth(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "ClearDepth"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingClearDepth::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingClearDepth::getType() const
{
	return cfxGlPipelineSetting::CLEAR_DEPTH;
}

float cfxGlPipelineSettingClearDepth::getValue() const
{
	return value;
}

// cfxGlPipelineSettingClipPlane
cfxGlPipelineSettingClipPlane::cfxGlPipelineSettingClipPlane(cfxPass* _pass, int _index, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "ClipPlane", _index),
    index(_index),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingClipPlane::apply()
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);	
  return true;
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingClipPlane::getType() const
{
	return cfxGlPipelineSetting::CLIP_PLANE;
}

int cfxGlPipelineSettingClipPlane::getIndex() const
{
	return index;
}

cfxFloat4 cfxGlPipelineSettingClipPlane::getValue() const
{
	return value;
}

// cfxGlPipelineSettingColorMask
cfxGlPipelineSettingColorMask::cfxGlPipelineSettingColorMask(cfxPass* _pass, cfxBool4& _value)
  : cfxGlPipelineSetting(_pass, "ColorMask"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingColorMask::apply()
{
  CGbool cgValues[4];
  cgValues[0] = value.b0;
  cgValues[1] = value.b1;
  cgValues[2] = value.b2;
  cgValues[3] = value.b3;
  cgSetBoolArrayStateAssignment(assignment, cgValues);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingColorMask::getType() const
{
	return cfxGlPipelineSetting::COLOR_MASK;
}

cfxBool4 cfxGlPipelineSettingColorMask::getValue() const
{
	return value;
}


// cfxGlPipelineSettingColorMaterial
cfxGlPipelineSettingColorMaterial::cfxGlPipelineSettingColorMaterial(cfxPass* _pass, int _face, int _mode)
  : cfxGlPipelineSetting(_pass, "ColorMaterial"),
    face(_face),
    mode(_mode)
{
}
  
cfxBool cfxGlPipelineSettingColorMaterial::apply() 
{
  int values[2];
  values[0] = face;
  values[1] = mode;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingColorMaterial::getType() const
{
	return cfxGlPipelineSetting::COLOR_MATERIAL;
}

int cfxGlPipelineSettingColorMaterial::getFace() const
{
	return face;
}

int cfxGlPipelineSettingColorMaterial::getMode() const
{
	return mode;
}


// cfxGlPipelineSettingCullFace
cfxGlPipelineSettingCullFace::cfxGlPipelineSettingCullFace(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "CullFace"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingCullFace::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingCullFace::getType() const
{
	return cfxGlPipelineSetting::CULL_FACE;
}

int cfxGlPipelineSettingCullFace::getValue() const
{
	return value;
}


// cfxGlPipelineSettingDepthFunc
cfxGlPipelineSettingDepthFunc::cfxGlPipelineSettingDepthFunc(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "DepthFunc"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingDepthFunc::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingDepthFunc::getType() const
{
	return cfxGlPipelineSetting::DEPTH_FUNC;
}

int cfxGlPipelineSettingDepthFunc::getValue() const
{
	return value;
}

// cfxGlPipelineSettingDepthMask
cfxGlPipelineSettingDepthMask::cfxGlPipelineSettingDepthMask(cfxPass* _pass, cfxBool _value)
  : cfxGlPipelineSetting(_pass, "DepthMask"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingDepthMask::apply() 
{
  cgSetBoolStateAssignment(assignment, value);
  return true;
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingDepthMask::getType() const
{
	return cfxGlPipelineSetting::DEPTH_MASK;
}

cfxBool cfxGlPipelineSettingDepthMask::getValue() const
{
	return value;
}


// cfxGlPipelineSettingDepthRange
cfxGlPipelineSettingDepthRange::cfxGlPipelineSettingDepthRange(cfxPass* _pass, cfxFloat2& _value)
  : cfxGlPipelineSetting(_pass, "DepthRange"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingDepthRange::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true;
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingDepthRange::getType() const
{
	return cfxGlPipelineSetting::DEPTH_RANGE;
}

cfxFloat2 cfxGlPipelineSettingDepthRange::getValue() const
{
	return value;
}


// cfxGlPipelineSettingFogMode
cfxGlPipelineSettingFogMode::cfxGlPipelineSettingFogMode(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "FogMode"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingFogMode::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingFogMode::getType() const
{
	return cfxGlPipelineSetting::FOG_MODE;
}

int cfxGlPipelineSettingFogMode::getValue() const
{
	return value;
}

// cfxGlPipelineSettingFogDensity
cfxGlPipelineSettingFogDensity::cfxGlPipelineSettingFogDensity(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "FogDensity"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingFogDensity::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingFogDensity::getType() const
{
	return cfxGlPipelineSetting::FOG_DENSITY;
}

float cfxGlPipelineSettingFogDensity::getValue() const
{
	return value;
}

// cfxGlPipelineSettingFogStart
cfxGlPipelineSettingFogStart::cfxGlPipelineSettingFogStart(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "FogStart"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingFogStart::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true;
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingFogStart::getType() const
{
	return cfxGlPipelineSetting::FOG_START;
}

float cfxGlPipelineSettingFogStart::getValue() const
{
	return value;
}

// cfxGlPipelineSettingFogEnd
cfxGlPipelineSettingFogEnd::cfxGlPipelineSettingFogEnd(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "FogEnd"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingFogEnd::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingFogEnd::getType() const
{
	return cfxGlPipelineSetting::FOG_END;
}

float cfxGlPipelineSettingFogEnd::getValue() const
{
	return value;
}

// cfxGlPipelineSettingFogColor
cfxGlPipelineSettingFogColor::cfxGlPipelineSettingFogColor(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "FogColor"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingFogColor::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingFogColor::getType() const
{
	return cfxGlPipelineSetting::FOG_COLOR;
}

cfxFloat4 cfxGlPipelineSettingFogColor::getValue() const
{
	return value;
}

// cfxGlPipelineSettingFrontFace
cfxGlPipelineSettingFrontFace::cfxGlPipelineSettingFrontFace(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "FrontFace"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingFrontFace::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingFrontFace::getType() const
{
	return cfxGlPipelineSetting::FRONT_FACE;
}

int cfxGlPipelineSettingFrontFace::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightModelAmbient
cfxGlPipelineSettingLightModelAmbient::cfxGlPipelineSettingLightModelAmbient(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "LightModelAmbient"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingLightModelAmbient::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightModelAmbient::getType() const
{
	return cfxGlPipelineSetting::LIGHT_MODEL_AMBEINT;
}

cfxFloat4 cfxGlPipelineSettingLightModelAmbient::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightAmbient
cfxGlPipelineSettingLightAmbient::cfxGlPipelineSettingLightAmbient(cfxPass* _pass, int _index, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "LightAmbient", _index),
    index(_index),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingLightAmbient::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightAmbient::getType() const
{
	return cfxGlPipelineSetting::LIGHT_AMBIENT;
}

int cfxGlPipelineSettingLightAmbient::getIndex() const
{
	return index;
}

cfxFloat4 cfxGlPipelineSettingLightAmbient::getValue() const
{
	return value;
}


// cfxGlPipelineSettingLightConstantAttenuation
cfxGlPipelineSettingLightConstantAttenuation::cfxGlPipelineSettingLightConstantAttenuation(cfxPass* _pass, int _index, float _value)
  : cfxGlPipelineSetting(_pass, "LightConstantAttenuation", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightConstantAttenuation::apply() 
{ 
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightConstantAttenuation::getType() const
{
	return cfxGlPipelineSetting::LIGHT_CONSTANT_ATTENUATION;
}

int cfxGlPipelineSettingLightConstantAttenuation::getIndex() const
{
	return index;
}

float cfxGlPipelineSettingLightConstantAttenuation::getValue() const
{
	return value;
}


// cfxGlPipelineSettingLightDiffuse
cfxGlPipelineSettingLightDiffuse::cfxGlPipelineSettingLightDiffuse(cfxPass* _pass, int _index, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "LightDiffuse", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightDiffuse::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightDiffuse::getType() const
{
	return cfxGlPipelineSetting::LIGHT_DIFFUSE;
}

int cfxGlPipelineSettingLightDiffuse::getIndex() const
{
	return index;
}

cfxFloat4 cfxGlPipelineSettingLightDiffuse::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightLinearAttenuation
cfxGlPipelineSettingLightLinearAttenuation::cfxGlPipelineSettingLightLinearAttenuation(cfxPass* _pass, int _index, float _value)
  : cfxGlPipelineSetting(_pass, "LightLinearAttenuation", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightLinearAttenuation::apply()
{
  cgSetFloatStateAssignment(assignment, value);
  return true;
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightLinearAttenuation::getType() const
{
	return cfxGlPipelineSetting::LIGHT_LINEAR_ATTENUATION;
}

int cfxGlPipelineSettingLightLinearAttenuation::getIndex() const
{
	return index;
}

float cfxGlPipelineSettingLightLinearAttenuation::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightPosition
cfxGlPipelineSettingLightPosition::cfxGlPipelineSettingLightPosition(cfxPass* _pass, int _index, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "LightPosition", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightPosition::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightPosition::getType() const
{
	return cfxGlPipelineSetting::LIGHT_POSITION;
}

int cfxGlPipelineSettingLightPosition::getIndex() const
{
	return index;
}

cfxFloat4 cfxGlPipelineSettingLightPosition::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightQuadraticAttenuation
cfxGlPipelineSettingLightQuadraticAttenuation::cfxGlPipelineSettingLightQuadraticAttenuation(cfxPass* _pass, int _index, float _value)
  : cfxGlPipelineSetting(_pass, "LightQuadraticAttenuation", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightQuadraticAttenuation::apply() 
{ 
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightQuadraticAttenuation::getType() const
{
	return cfxGlPipelineSetting::LIGHT_QUADRATIC_ATTENUATION;
}

int cfxGlPipelineSettingLightQuadraticAttenuation::getIndex() const
{
	return index;
}

float cfxGlPipelineSettingLightQuadraticAttenuation::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightSpecular
cfxGlPipelineSettingLightSpecular::cfxGlPipelineSettingLightSpecular(cfxPass* _pass, int _index, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "LightSpecular", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightSpecular::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightSpecular::getType() const
{
	return cfxGlPipelineSetting::LIGHT_SPECULAR;
}

int cfxGlPipelineSettingLightSpecular::getIndex() const
{
	return index;
}

cfxFloat4 cfxGlPipelineSettingLightSpecular::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightSpotCutoff
cfxGlPipelineSettingLightSpotCutoff::cfxGlPipelineSettingLightSpotCutoff(cfxPass* _pass, int _index, float _value)
  : cfxGlPipelineSetting(_pass, "LightSpotCutoff", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightSpotCutoff::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightSpotCutoff::getType() const
{
	return cfxGlPipelineSetting::LIGHT_SPOT_CUTOFF;
}

int cfxGlPipelineSettingLightSpotCutoff::getIndex() const
{
	return index;
}

float cfxGlPipelineSettingLightSpotCutoff::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightSpotDirection
cfxGlPipelineSettingLightSpotDirection::cfxGlPipelineSettingLightSpotDirection(cfxPass* _pass, int _index, cfxFloat3& _value)
  : cfxGlPipelineSetting(_pass, "LightSpotDirection", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightSpotDirection::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightSpotDirection::getType() const
{
	return cfxGlPipelineSetting::LIGHT_SPOT_DIRECTION;
}

int cfxGlPipelineSettingLightSpotDirection::getIndex() const
{
	return index;
}

cfxFloat3 cfxGlPipelineSettingLightSpotDirection::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLightSpotExponent
cfxGlPipelineSettingLightSpotExponent::cfxGlPipelineSettingLightSpotExponent(cfxPass* _pass, int _index, float _value)
  : cfxGlPipelineSetting(_pass, "LightSpotExponent", _index),
    index(_index),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLightSpotExponent::apply()
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLightSpotExponent::getType() const
{
	return cfxGlPipelineSetting::LIGHT_SPOT_EXPONENT;
}

int cfxGlPipelineSettingLightSpotExponent::getIndex() const
{
	return index;
}

float cfxGlPipelineSettingLightSpotExponent::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLineWidth
cfxGlPipelineSettingLineWidth::cfxGlPipelineSettingLineWidth(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "LineWidth"),
    value(_value)

{
}
  
cfxBool cfxGlPipelineSettingLineWidth::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLineWidth::getType() const
{
	return cfxGlPipelineSetting::LINE_WIDTH;
}

float cfxGlPipelineSettingLineWidth::getValue() const
{
	return value;
}

// cfxGlPipelineSettingLogicOp
cfxGlPipelineSettingLogicOp::cfxGlPipelineSettingLogicOp(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "LogicOp"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingLogicOp::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingLogicOp::getType() const
{
	return cfxGlPipelineSetting::LOGIC_OP;
}

int cfxGlPipelineSettingLogicOp::getValue() const
{
	return value;
}

// cfxGlPipelineSettingMaterialAmbient
cfxGlPipelineSettingMaterialAmbient::cfxGlPipelineSettingMaterialAmbient(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "MaterialAmbient"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingMaterialAmbient::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingMaterialAmbient::getType() const
{
	return cfxGlPipelineSetting::MATERIAL_AMBIENT;
}

cfxFloat4 cfxGlPipelineSettingMaterialAmbient::getValue() const
{
	return value;
}

// cfxGlPipelineSettingMaterialDiffuse
cfxGlPipelineSettingMaterialDiffuse::cfxGlPipelineSettingMaterialDiffuse(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "MaterialDiffuse"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingMaterialDiffuse::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingMaterialDiffuse::getType() const
{
	return cfxGlPipelineSetting::MATERIAL_DIFFUSE;
}

cfxFloat4 cfxGlPipelineSettingMaterialDiffuse::getValue() const
{
	return value;
}

// cfxGlPipelineSettingMaterialEmission
cfxGlPipelineSettingMaterialEmission::cfxGlPipelineSettingMaterialEmission(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "MaterialEmission"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingMaterialEmission::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingMaterialEmission::getType() const
{
	return cfxGlPipelineSetting::MATERIAL_EMISSION;
}

cfxFloat4 cfxGlPipelineSettingMaterialEmission::getValue() const
{
	return value;
}

// cfxGlPipelineSettingMaterialShininess
cfxGlPipelineSettingMaterialShininess::cfxGlPipelineSettingMaterialShininess(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "MaterialShininess"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingMaterialShininess::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingMaterialShininess::getType() const
{
	return cfxGlPipelineSetting::MATERIAL_SHININESS;
}

float cfxGlPipelineSettingMaterialShininess::getValue() const
{
	return value;
}

// cfxGlPipelineSettingMaterialSpecular
cfxGlPipelineSettingMaterialSpecular::cfxGlPipelineSettingMaterialSpecular(cfxPass* _pass, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "MaterialSpecular"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingMaterialSpecular::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingMaterialSpecular::getType() const
{
	return cfxGlPipelineSetting::MATERIAL_SPECULAR;
}

cfxFloat4 cfxGlPipelineSettingMaterialSpecular::getValue() const
{
	return value;
}

// cfxGlPipelineSettingModelViewMatrix
cfxGlPipelineSettingModelViewMatrix::cfxGlPipelineSettingModelViewMatrix(cfxPass* _pass, cfxFloat4x4& _value)
  : cfxGlPipelineSetting(_pass, "ModelViewMatrix"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingModelViewMatrix::apply()
{
  cgSetFloatArrayStateAssignment(assignment, &value.f00);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingModelViewMatrix::getType() const
{
	return cfxGlPipelineSetting::MODEL_VIEW_MATRIX;
}

cfxFloat4x4 cfxGlPipelineSettingModelViewMatrix::getValue() const
{
	return value;
}

// cfxGlPipelineSettingPointSize
cfxGlPipelineSettingPointSize::cfxGlPipelineSettingPointSize(cfxPass* _pass, float _value)
  : cfxGlPipelineSetting(_pass, "PointSize"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingPointSize::apply() 
{
  cgSetFloatStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingPointSize::getType() const
{
	return cfxGlPipelineSetting::POINT_SIZE;
}

float cfxGlPipelineSettingPointSize::getValue() const
{
	return value;
}


// cfxGlPipelineSettingPointSpriteCoordReplace
cfxGlPipelineSettingPointSpriteCoordReplace::cfxGlPipelineSettingPointSpriteCoordReplace(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "PointSpriteCoordReplace")
{
}
  
cfxBool cfxGlPipelineSettingPointSpriteCoordReplace::apply() 
{
  printf("ERROR:  Sprite pipeline settings unimplemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingPointSpriteCoordReplace::getType() const
{
	return cfxGlPipelineSetting::POINT_SPRITE_COORD_REPLACE;
}


// cfxGlPipelineSettingPointSpriteRMode
cfxGlPipelineSettingPointSpriteRMode::cfxGlPipelineSettingPointSpriteRMode(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "PointSpriteRMode")
{
}
  
cfxBool cfxGlPipelineSettingPointSpriteRMode::apply() 
{
  printf("ERROR:  Sprite pipeline settings unimplemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingPointSpriteRMode::getType() const
{
	return cfxGlPipelineSetting::POINT_SPRITE_R_MODE;
}


// cfxGlPipelineSettingPolygonMode
cfxGlPipelineSettingPolygonMode::cfxGlPipelineSettingPolygonMode(cfxPass* _pass, int _face, int _mode)
  : cfxGlPipelineSetting(_pass, "PolygonMode"),
    face(_face),
    mode(_mode)
{
}
  
cfxBool cfxGlPipelineSettingPolygonMode::apply() 
{
  int values[2];
  values[0] = face;
  values[1] = mode;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingPolygonMode::getType() const
{
	return cfxGlPipelineSetting::POLYGON_MODE;
}

int cfxGlPipelineSettingPolygonMode::getFace() const
{
	return face;
}

int cfxGlPipelineSettingPolygonMode::getMode() const
{
	return mode;
}


// cfxGlPipelineSettingPolygonOffset
cfxGlPipelineSettingPolygonOffset::cfxGlPipelineSettingPolygonOffset(cfxPass* _pass, cfxFloat2& _value)
  : cfxGlPipelineSetting(_pass, "PolygonOffset"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingPolygonOffset::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingPolygonOffset::getType() const
{
	return cfxGlPipelineSetting::POLYGON_OFFSET;
}

cfxFloat2 cfxGlPipelineSettingPolygonOffset::getValue() const
{
	return value;
}


// cfxGlPipelineSettingProjectionMatrix
cfxGlPipelineSettingProjectionMatrix::cfxGlPipelineSettingProjectionMatrix(cfxPass* _pass, cfxFloat4x4& _value)
  : cfxGlPipelineSetting(_pass, "ProjectionMatrix"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingProjectionMatrix::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f00);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingProjectionMatrix::getType() const
{
	return cfxGlPipelineSetting::PROJECTION_MATRIX;
}

cfxFloat4x4 cfxGlPipelineSettingProjectionMatrix::getValue() const
{
	return value;
}


// cfxGlPipelineSettingScissor
cfxGlPipelineSettingScissor::cfxGlPipelineSettingScissor(cfxPass* _pass, cfxInt4& _value)
  : cfxGlPipelineSetting(_pass, "Scissor"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingScissor::apply() 
{
  cgSetIntArrayStateAssignment(assignment, &value.i0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingScissor::getType() const
{
	return cfxGlPipelineSetting::SCISSOR;
}

cfxInt4 cfxGlPipelineSettingScissor::getValue() const
{
	return value;
}


// cfxGlPipelineSettingShadeModel
cfxGlPipelineSettingShadeModel::cfxGlPipelineSettingShadeModel(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "ShadeModel"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingShadeModel::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingShadeModel::getType() const
{
	return cfxGlPipelineSetting::SHADE_MODEL;
}

int cfxGlPipelineSettingShadeModel::getValue() const
{
	return value;
}

// cfxGlPipelineSettingStencilFunc
cfxGlPipelineSettingStencilFunc::cfxGlPipelineSettingStencilFunc(cfxPass* _pass, int _func, int _ref, int _mask)
  : cfxGlPipelineSetting(_pass, "StencilFunc"),
    func(_func),
    ref(_ref),
    mask(_mask)
{
}
  
cfxBool cfxGlPipelineSettingStencilFunc::apply() 
{
  int values[3];
  values[0] = func;
  values[1] = ref;
  values[2] = mask;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingStencilFunc::getType() const
{
	return cfxGlPipelineSetting::STENCIL_FUNC;
}

int cfxGlPipelineSettingStencilFunc::getFunc() const
{
	return func;
}

int cfxGlPipelineSettingStencilFunc::getRef() const
{
	return ref;
}

int cfxGlPipelineSettingStencilFunc::getMask() const
{
	return mask;
}

// cfxGlPipelineSettingStencilMask
cfxGlPipelineSettingStencilMask::cfxGlPipelineSettingStencilMask(cfxPass* _pass, int _value)
  : cfxGlPipelineSetting(_pass, "StencilMask"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingStencilMask::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingStencilMask::getType() const
{
	return cfxGlPipelineSetting::STENCIL_MASK;
}

int cfxGlPipelineSettingStencilMask::getValue() const
{
	return value;
}

// cfxGlPipelineSettingStencilOp
cfxGlPipelineSettingStencilOp::cfxGlPipelineSettingStencilOp(cfxPass* _pass, int _fail, int _zFail, int _zPass)
  : cfxGlPipelineSetting(_pass, "StencilOp"),
    fail(_fail),
    zFail(_zFail),
    zPass(_zPass)
{
}
  
cfxBool cfxGlPipelineSettingStencilOp::apply() 
{
  int values[3];
  values[0] = fail;
  values[1] = zFail;
  values[2] = zPass;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingStencilOp::getType() const
{
	return cfxGlPipelineSetting::STENCIL_OP;
}

int cfxGlPipelineSettingStencilOp::getFail() const
{
	return fail;
}

int cfxGlPipelineSettingStencilOp::getZFail() const
{
	return zFail;
}

int cfxGlPipelineSettingStencilOp::getZPass() const
{
	return zPass;
}

// cfxGlPipelineSettingStencilFuncSeparate
cfxGlPipelineSettingStencilFuncSeparate::cfxGlPipelineSettingStencilFuncSeparate(cfxPass* _pass, int _front, int _back, int _ref, int _mask)
  : cfxGlPipelineSetting(_pass, "StencilFuncSeparate"),
    front(_front),
    back(_back),
    ref(_ref),
    mask(_mask)
{
}
  
cfxBool cfxGlPipelineSettingStencilFuncSeparate::apply() 
{
  int values[4];
  values[0] = front;
  values[1] = back;
  values[2] = ref;
  values[3] = mask;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingStencilFuncSeparate::getType() const
{
	return cfxGlPipelineSetting::STENCIL_FUNC_SEPARATE;
}

int cfxGlPipelineSettingStencilFuncSeparate::getFront() const
{
	return front;
}

int cfxGlPipelineSettingStencilFuncSeparate::getBack() const
{
	return back;
}

int cfxGlPipelineSettingStencilFuncSeparate::getRef() const
{
	return ref;
}

int cfxGlPipelineSettingStencilFuncSeparate::getMask() const
{
	return mask;
}

// cfxGlPipelineSettingStencilMaskSeparate
cfxGlPipelineSettingStencilMaskSeparate::cfxGlPipelineSettingStencilMaskSeparate(cfxPass* _pass, int _face, int _mask)
  : cfxGlPipelineSetting(_pass, "StencilMaskSeparate"),
    face(_face),
    mask(_mask)
{
}
  
cfxBool cfxGlPipelineSettingStencilMaskSeparate::apply() 
{
  int values[2];
  values[0] = face;
  values[1] = mask;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingStencilMaskSeparate::getType() const
{
	return cfxGlPipelineSetting::STENCIL_MASK_SEPARATE;
}

int cfxGlPipelineSettingStencilMaskSeparate::getFace() const
{
	return face;
}

int cfxGlPipelineSettingStencilMaskSeparate::getMask() const
{
	return mask;
}


// cfxGlPipelineSettingStencilOpSeparate
cfxGlPipelineSettingStencilOpSeparate::cfxGlPipelineSettingStencilOpSeparate(cfxPass* _pass, int _face, int _fail, int _zFail, int _zPass)
  : cfxGlPipelineSetting(_pass, "StencilOpSeparate"),
    face(_face),
    fail(_fail),
    zFail(_zFail),
    zPass(_zPass)
{
}
  
cfxBool cfxGlPipelineSettingStencilOpSeparate::apply() 
{
  int values[4];
  values[0] = face;
  values[1] = fail;
  values[2] = zFail;
  values[3] = zPass;
  cgSetIntArrayStateAssignment(assignment, values);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingStencilOpSeparate::getType() const
{
	return cfxGlPipelineSetting::STENCIL_OP_SEPAREATE;
}

int cfxGlPipelineSettingStencilOpSeparate::getFace() const
{
	return face;
}

int cfxGlPipelineSettingStencilOpSeparate::getFail() const
{
	return fail;
}

int cfxGlPipelineSettingStencilOpSeparate::getZFail() const
{
	return zFail;
}

int cfxGlPipelineSettingStencilOpSeparate::getZPass() const
{
	return zPass;
}

// cfxGlPipelineSettingTexGenSMode
cfxGlPipelineSettingTexGenSMode::cfxGlPipelineSettingTexGenSMode(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenSMode")
{
}
  
cfxBool cfxGlPipelineSettingTexGenSMode::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenSMode::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_S_MODE;
}


// cfxGlPipelineSettingTexGenTMode
cfxGlPipelineSettingTexGenTMode::cfxGlPipelineSettingTexGenTMode(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenTMode")
{
}
  
cfxBool cfxGlPipelineSettingTexGenTMode::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenTMode::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_T_MODE;
}


// cfxGlPipelineSettingTexGenRMode
cfxGlPipelineSettingTexGenRMode::cfxGlPipelineSettingTexGenRMode(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenRMode")
{
}
  
cfxBool cfxGlPipelineSettingTexGenRMode::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenRMode::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_R_MODE;
}


// cfxGlPipelineSettingTexGenQMode
cfxGlPipelineSettingTexGenQMode::cfxGlPipelineSettingTexGenQMode(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenQMode")
{
}
  
cfxBool cfxGlPipelineSettingTexGenQMode::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenQMode::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_Q_MODE;
}


// cfxGlPipelineSettingTexGenSEyePlane
cfxGlPipelineSettingTexGenSEyePlane::cfxGlPipelineSettingTexGenSEyePlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenSEyePlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenSEyePlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenSEyePlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_S_EYE_PLANE;
}


// cfxGlPipelineSettingTexGenTEyePlane
cfxGlPipelineSettingTexGenTEyePlane::cfxGlPipelineSettingTexGenTEyePlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenTEyePlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenTEyePlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenTEyePlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_T_EYE_PLANE;
}


// cfxGlPipelineSettingTexGenREyePlane
cfxGlPipelineSettingTexGenREyePlane::cfxGlPipelineSettingTexGenREyePlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenREyePlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenREyePlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenREyePlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_R_EYE_PLANE;
}


// cfxGlPipelineSettingTexGenQEyePlane
cfxGlPipelineSettingTexGenQEyePlane::cfxGlPipelineSettingTexGenQEyePlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenQEyePlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenQEyePlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenQEyePlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_Q_EYE_PLANE;
}


// cfxGlPipelineSettingTexGenSObjectPlane
cfxGlPipelineSettingTexGenSObjectPlane::cfxGlPipelineSettingTexGenSObjectPlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenSObjectPlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenSObjectPlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenSObjectPlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_S_OBJECT_PLANE;
}


// cfxGlPipelineSettingTexGenTObjectPlane
cfxGlPipelineSettingTexGenTObjectPlane::cfxGlPipelineSettingTexGenTObjectPlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenTObjectPlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenTObjectPlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenTObjectPlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_T_OBJECT_PLANE;
}


// cfxGlPipelineSettingTexGenRObjectPlane
cfxGlPipelineSettingTexGenRObjectPlane::cfxGlPipelineSettingTexGenRObjectPlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenRObjectPlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenRObjectPlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenRObjectPlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_R_OBJECT_PLANE;
}


// cfxGlPipelineSettingTexGenQObjectPlane
cfxGlPipelineSettingTexGenQObjectPlane::cfxGlPipelineSettingTexGenQObjectPlane(cfxPass* _pass)
  : cfxGlPipelineSetting(_pass, "TexGenQObjectPlane")
{
}
  
cfxBool cfxGlPipelineSettingTexGenQObjectPlane::apply() 
{
  printf("ERROR:  TexGen state Assignments not implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexGenQObjectPlane::getType() const
{
	return cfxGlPipelineSetting::TEX_GEN_Q_OBJECT_PLANE;
}


// cfxGlPipelineSettingTexture2D
cfxGlPipelineSettingTexture2D::cfxGlPipelineSettingTexture2D(cfxPass* _pass, int _index, cfxSampler* _sampler)
  : cfxGlPipelineSetting(_pass, "Texture2D", _index),
    index(_index),
    sampler(_sampler)
{
}
  
cfxBool cfxGlPipelineSettingTexture2D::apply() 
{
  printf("ERROR:  Texture2D state Assignment not completely implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexture2D::getType() const
{
	return cfxGlPipelineSetting::TEXTURE_2D;
}

int cfxGlPipelineSettingTexture2D::getIndex() const
{
	return index;
}

const cfxSampler *cfxGlPipelineSettingTexture2D::getSampler() const
{
	return sampler;
}

// cfxGlPipelineSettingTexture3D
cfxGlPipelineSettingTexture3D::cfxGlPipelineSettingTexture3D(cfxPass* _pass, int _index, cfxSampler* _sampler)
  : cfxGlPipelineSetting(_pass, "Texture3D", _index),
    index(_index),
    sampler(_sampler)
{
}
  
cfxBool cfxGlPipelineSettingTexture3D::apply() 
{
  printf("ERROR:  Texture3D state Assignment not completely implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTexture3D::getType() const
{
	return cfxGlPipelineSetting::TEXTURE_3D;
}

int cfxGlPipelineSettingTexture3D::getIndex() const
{
	return index;
}

const cfxSampler *cfxGlPipelineSettingTexture3D::getSampler() const
{
	return sampler;
}

// cfxGlPipelineSettingTextureCubeMap
cfxGlPipelineSettingTextureCubeMap::cfxGlPipelineSettingTextureCubeMap(cfxPass* _pass, int _index, cfxSampler* _sampler)
  : cfxGlPipelineSetting(_pass, "TextureCubeMap", _index),
    index(_index),
    sampler(_sampler)

{
}
  
cfxBool cfxGlPipelineSettingTextureCubeMap::apply() 
{
  printf("ERROR:  TextureCubeMap state Assignment not completely implemented.\n");
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTextureCubeMap::getType() const
{
	return cfxGlPipelineSetting::TEXTURE_CUBE_MAP;
}

int cfxGlPipelineSettingTextureCubeMap::getIndex() const
{
	return index;
}

const cfxSampler *cfxGlPipelineSettingTextureCubeMap::getSampler() const
{
	return sampler;
}

// cfxGlPipelineSettingTextureEnvColor
cfxGlPipelineSettingTextureEnvColor::cfxGlPipelineSettingTextureEnvColor(cfxPass* _pass, int _index, cfxFloat4& _value)
  : cfxGlPipelineSetting(_pass, "TextureEnvColor", _index),
    index(_index),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingTextureEnvColor::apply() 
{
  cgSetFloatArrayStateAssignment(assignment, &value.f0);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTextureEnvColor::getType() const
{
	return cfxGlPipelineSetting::TEXTURE_ENV_COLOR;
}

int cfxGlPipelineSettingTextureEnvColor::getIndex() const
{
	return index;
}

cfxFloat4 cfxGlPipelineSettingTextureEnvColor::getValue() const
{
	return value;
}

// cfxGlPipelineSettingTextureEnvMode
cfxGlPipelineSettingTextureEnvMode::cfxGlPipelineSettingTextureEnvMode(cfxPass* _pass, int _index, int _value)
  : cfxGlPipelineSetting(_pass, "TextureEnvMode", _index),
    index(_index),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingTextureEnvMode::apply() 
{
  cgSetIntStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingTextureEnvMode::getType() const
{
	return cfxGlPipelineSetting::TEXTURE_ENV_MODE;
}

int cfxGlPipelineSettingTextureEnvMode::getIndex() const
{
	return index;
}

int cfxGlPipelineSettingTextureEnvMode::getValue() const
{
	return value;
}

// ENABLE / DISABLE settings


// cfxGlPipelineSettingAlphaTestEnable
cfxGlPipelineSettingAlphaTestEnable::cfxGlPipelineSettingAlphaTestEnable(cfxPass* _pass, cfxBool _value)
  : cfxGlPipelineSetting(_pass, "AlphaTestEnable"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingAlphaTestEnable::apply() 
{
  cgSetBoolStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingAlphaTestEnable::getType() const
{
	return cfxGlPipelineSetting::ALPHA_TEST_ENABLE;
}

cfxBool cfxGlPipelineSettingAlphaTestEnable::getValue() const
{
	return value;
}

// cfxGlPipelineSettingBlendEnable
cfxGlPipelineSettingBlendEnable::cfxGlPipelineSettingBlendEnable(cfxPass* _pass, cfxBool _value)
  : cfxGlPipelineSetting(_pass, "BlendEnable"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingBlendEnable::apply() 
{
  cgSetBoolStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingBlendEnable::getType() const
{
	return cfxGlPipelineSetting::BLEND_ENABLE;
}

cfxBool cfxGlPipelineSettingBlendEnable::getValue() const
{
	return value;
}

// cfxGlPipelineSettingCullFaceEnable
cfxGlPipelineSettingCullFaceEnable::cfxGlPipelineSettingCullFaceEnable(cfxPass* _pass, cfxBool _value)
  : cfxGlPipelineSetting(_pass, "CullFaceEnable"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingCullFaceEnable::apply() 
{
  cgSetBoolStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingCullFaceEnable::getType() const
{
	return cfxGlPipelineSetting::CULL_FACE_ENABLE;
}

cfxBool cfxGlPipelineSettingCullFaceEnable::getValue() const
{
	return value;
}

// cfxGlPipelineSettingDepthTestEnable
cfxGlPipelineSettingDepthTestEnable::cfxGlPipelineSettingDepthTestEnable(cfxPass* _pass, cfxBool _value)
  : cfxGlPipelineSetting(_pass, "DepthTestEnable"),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSettingDepthTestEnable::apply() 
{
  cgSetBoolStateAssignment(assignment, value);
  return true; 
}

cfxGlPipelineSetting::type_enum cfxGlPipelineSettingDepthTestEnable::getType() const
{
	return cfxGlPipelineSetting::DEPTH_TEST_ENABLE;
}

cfxBool cfxGlPipelineSettingDepthTestEnable::getValue() const
{
	return value;
}

#if 0 // template
// cfxGlPipelineSetting
cfxGlPipelineSetting::cfxGlPipelineSetting(cfxPass* _pass, cfxBool _value)
  : cfxGlPipelineSetting(_pass, ""),
    value(_value)
{
}
  
cfxBool cfxGlPipelineSetting::apply() 
{
  cgSetBoolStateAssignment(assignment, value);
  return true; 
}

#endif

