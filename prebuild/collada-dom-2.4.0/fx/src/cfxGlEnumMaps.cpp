/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#include <cfxGlEnumMaps.h>

#include <cfxPlatform.h>


#if 0
int cfxGlEnumMaps::blendTypeEnums[GL_BLEND_TYPE_COUNT];
int cfxGlEnumMaps::faceTypeEnums[GL_FACE_TYPE_COUNT];
int cfxGlEnumMaps::blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_COUNT];
int cfxGlEnumMaps::funcTypeEnums[GL_FUNC_TYPE_COUNT];
int cfxGlEnumMaps::stencilOpTypeEnums[GL_STENCIL_OP_TYPE_COUNT];

int cfxGlEnumMaps::modeTypeEnums[GL_MATERIAL_TYPE_COUNT];
int cfxGlEnumMaps::fogModeTypeEnums[GL_FOG_TYPE_COUNT];
#endif

int cfxGlEnumMaps::samplerWrapTypeEnums[FX_SAMPLER_WRAP_COMMON_COUNT];
int cfxGlEnumMaps::samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_COUNT];

bool cfxGlEnumMaps::initialized = false;

cfxGlEnumMaps cfxGlEnumMaps::initializer;


int cfxGlEnumMaps::getBlendTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_BLEND_TYPE_COUNT)
	//    return -1;
	//  return blendTypeEnums[domEnum];
}

int cfxGlEnumMaps::getFaceTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_FACE_TYPE_COUNT)
	//    return -1;
	//  return faceTypeEnums[domEnum];
}


int cfxGlEnumMaps::getBlendEquationTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_BLEND_EQUATION_TYPE_COUNT)
	//    return -1;
	//  return blendEquationTypeEnums[domEnum];
}


int cfxGlEnumMaps::getFuncTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_FUNC_TYPE_COUNT)
	//    return -1;
	//  return funcTypeEnums[domEnum];
}


int cfxGlEnumMaps::getStencilOpTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_STENCIL_OP_TYPE_COUNT)
	//    return -1;
	//  return stencilOpTypeEnums[domEnum];
}


int cfxGlEnumMaps::getModeTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_MATERIAL_TYPE_COUNT)
	//    return -1;
	//  return modeTypeEnums[domEnum];
}


int cfxGlEnumMaps::getFogModeTypeEnum(int domEnum)
{
	return domEnum;

	//  if (domEnum < 0 || domEnum >= GL_FOG_TYPE_COUNT)
	//    return -1;
	//  return fogModeTypeEnums[domEnum];
}


int cfxGlEnumMaps::getSamplerWrapTypeEnum(int domEnum)
{
    if (domEnum < 0 || domEnum >= FX_SAMPLER_WRAP_COMMON_COUNT)
	return -1;
    return samplerWrapTypeEnums[domEnum];
}


int cfxGlEnumMaps::getSamplerFilterTypeEnum(int domEnum)
{
      if (domEnum < 0 || domEnum >= FX_SAMPLER_FILTER_COMMON_COUNT)
	  return -1;
      return samplerFilterTypeEnums[domEnum];
}



// deliberately doing explicit verbose initialization using both the dom and equivalent GL enums so mapping is less fragile 
cfxGlEnumMaps::cfxGlEnumMaps()
{

  if (!initialized)
    {
#if 0
      blendTypeEnums[GL_BLEND_TYPE_ZERO] = GL_ZERO;
      blendTypeEnums[GL_BLEND_TYPE_ONL_NEAREST_MIPMAP_LINEAE] = GL_ONE;
      blendTypeEnums[GL_BLEND_TYPE_SRC_COLOR] = GL_SRC_COLOR;
      blendTypeEnums[GL_BLEND_TYPE_ONE_MINUS_SRC_COLOR] = GL_ONE_MINUS_SRC_COLOR;
      blendTypeEnums[GL_BLEND_TYPE_DEST_COLOR] = GL_DST_COLOR;
      blendTypeEnums[GL_BLEND_TYPE_ONE_MINUS_DEST_COLOR] = GL_ONE_MINUS_DST_COLOR;
      blendTypeEnums[GL_BLEND_TYPE_SRC_ALPHA] = GL_SRC_ALPHA;
      blendTypeEnums[GL_BLEND_TYPE_ONE_MINUS_SRC_ALPHA] = GL_ONE_MINUS_SRC_ALPHA;
      //blendTypeEnums[GL_BLEND_TYPE_DST_ALPHA] = GL_DST_ALPHA;
      //blendTypeEnums[GL_BLEND_TYPE_ONE_MINUS_DST_ALPHA] = GL_ONE_MINUS_DST_ALPHA;
      //blendTypeEnums[GL_BLEND_TYPE_CONSTANT_COLOR] = GL_CONSTANT_COLOR;
      //blendTypeEnums[GL_BLEND_TYPE_ONE_MINUS_CONSTANT_COLOR] = GL_ONE_MINUS_CONSTANT_COLOR;
      //blendTypeEnums[GL_BLEND_TYPE_CONSTANT_ALPHA] = GL_CONSTANT_ALPHA;
      //blendTypeEnums[GL_BLEND_TYPE_ONE_MINUS_CONSTANT_ALPHA] = GL_ONE_MINUS_CONSTANT_ALPHA;
      blendTypeEnums[GL_BLEND_TYPE_SRC_ALPHA_SATURATE] = GL_SRC_ALPHA_SATURATE;

      faceTypeEnums[GL_FACE_TYPE_FRONT] = GL_FRONT;
      faceTypeEnums[GL_FACE_TYPE_BACK] = GL_BACK;
      faceTypeEnums[GL_FACE_TYPE_FRONT_AND_BACK] = GL_FRONT_AND_BACK;

      // i don't think this is right at all
      blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_FUNC_ADD] = GL_ADD;
	blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_FUNC_SUBTRACT] = GL_SUBTRACT;
	blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_FUNC_REVERSE_SUBTRACT] = GL_REVERSE_SUBTRACT;
	blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_MIN] = GL_MIN;
	blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_MAX] = GL_MAX;

      funcTypeEnums[GL_FUNC_TYPE_NEVER] = GL_NEVER;
      funcTypeEnums[GL_FUNC_TYPE_LESS] = GL_LESS;
      funcTypeEnums[GL_FUNC_TYPE_LEQUAL] = GL_LEQUAL;
      funcTypeEnums[GL_FUNC_TYPE_EQUAL] = GL_EQUAL;
      funcTypeEnums[GL_FUNC_TYPE_GREATER] = GL_GREATER;
      funcTypeEnums[GL_FUNC_TYPE_NOTEQUAL] = GL_NOTEQUAL;
      funcTypeEnums[GL_FUNC_TYPE_GEQUAL] = GL_GEQUAL;
      funcTypeEnums[GL_FUNC_TYPE_ALWAYS] = GL_ALWAYS;

      stencilOpTypeEnums[GL_STENCIL_OP_TYPE_KEEP] = GL_KEEP;
      stencilOpTypeEnums[GL_STENCIL_OP_TYPE_ZERO] = GL_ZERO;
      stencilOpTypeEnums[GL_STENCIL_OP_TYPE_REPLACE] = GL_REPLACE;
      stencilOpTypeEnums[GL_STENCIL_OP_TYPE_INCR] = GL_INCR;
      stencilOpTypeEnums[GL_STENCIL_OP_TYPE_DECR] = GL_DECR;
      stencilOpTypeEnums[GL_STENCIL_OP_TYPE_INVERT] = GL_INVERT;
      //stencilOpTypeEnums[GL_STENCIL_OP_TYPE_INCR_WRAP] = GL_INCR_WRAP;
      //stencilOpTypeEnums[GL_STENCIL_OP_TYPE_DECR_WRAP] = GL_DECR_WRAP;

      modeTypeEnums[domGl_pipeline_settings::domColor_material::domMode::MODE_EMISSION] = GL_EMISSION;
      //modeTypeEnums[domGl_pipeline_settings::domColor_material::domMode::MODE_AMBIENT] = GL_AMBIENT;
      //modeTypeEnums[domGl_pipeline_settings::domColor_material::domMode::MODE_DIFFUSE] = GL_DIFFUSE;
      //modeTypeEnums[domGl_pipeline_settings::domColor_material::domMode::MODE_SPECULAR] = GL_SPECULAR;
      modeTypeEnums[domGl_pipeline_settings::domColor_material::domMode::MODE_AMBIENT_AND_DIFFUSE] = GL_AMBIENT_AND_DIFFUSE;
      //modeTypeEnums[domGl_pipeline_settings::domColor_material::domMode::???] = GL_SHININESS;

      //fogModeTypeEnums[domGl_pipeline_settings::domFog_mode::FOG_MODE_LINEAR] = GL_LINEAR;
      fogModeTypeEnums[domGl_pipeline_settings::domFog_mode::FOG_MODE_EXP] = GL_FOG_TYPE_EXP;
      fogModeTypeEnums[domGl_pipeline_settings::domFog_mode::FOG_MODE_EXP2] = GL_FOG_TYPE_EXP2;
#endif

      int i;
      
      for (i = 0; i < FX_SAMPLER_WRAP_COMMON_CLAMP; i++)
	  {
	      samplerWrapTypeEnums[i] = -1;
	  }

      //samplerWrapTypeEnums[FX_SAMPLER_WRAP_COMMON_NONE] = ;
      samplerWrapTypeEnums[FX_SAMPLER_WRAP_COMMON_WRAP] = GL_REPEAT;
      //samplerWrapTypeEnums[FX_SAMPLER_WRAP_COMMON_MIRROR] = ;
      samplerWrapTypeEnums[FX_SAMPLER_WRAP_COMMON_CLAMP] = GL_CLAMP_TO_EDGE;

      for (i = 0; i < FX_SAMPLER_FILTER_COMMON_NONE; i++)
	  {
	      samplerFilterTypeEnums[i] = -1;
	  }

      //samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_NONE] = ;
      samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_NEAREST] = GL_NEAREST;
      samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_LINEAR] = GL_LINEAR;
      samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_NEAREST_MIPMAP_NEAREST] = GL_NEAREST_MIPMAP_NEAREST;
      samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_LINEAR_MIPMAP_NEAREST] = GL_LINEAR_MIPMAP_NEAREST;
      samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_NEAREST_MIPMAP_LINEAR] = GL_NEAREST_MIPMAP_LINEAR;
      samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_LINEAR_MIPMAP_LINEAR] = GL_LINEAR_MIPMAP_LINEAR;
    }
}


