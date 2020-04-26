/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_GLENUMMAPS_H
#define _CFX_GLENUMMAPS_H

#include <dom/domTypes.h>
#include <dom/domGl_pipeline_settings.h>

class cfxGlEnumMaps
{
 public:

  static int getBlendTypeEnum(int domEnum);
  static int getFaceTypeEnum(int domEnum);
  static int getBlendEquationTypeEnum(int domEnum);
  static int getFuncTypeEnum(int domEnum);
  static int getStencilOpTypeEnum(int domEnum);

  static int getModeTypeEnum(int domEnum);
  static int getFogModeTypeEnum(int domEnum);

  static int getSamplerWrapTypeEnum(int domEnum);
  static int getSamplerFilterTypeEnum(int domEnum);

 protected:
	
  
  cfxGlEnumMaps();

  // no longer need to map gl types as the dom has the right values
#if 0
  static int blendTypeEnums[GL_BLEND_TYPE_COUNT];
  static int faceTypeEnums[GL_FACE_TYPE_COUNT];
  static int blendEquationTypeEnums[GL_BLEND_EQUATION_TYPE_COUNT];
  static int funcTypeEnums[GL_FUNC_TYPE_COUNT];
  static int stencilOpTypeEnums[GL_STENCIL_OP_TYPE_COUNT];

  static int modeTypeEnums[GL_MATERIAL_TYPE_COUNT];
  static int fogModeTypeEnums[GL_FOG_TYPE_COUNT];
#endif

  // fx types need mapping to gl because the dom uses its own internal values 
  static int samplerWrapTypeEnums[FX_SAMPLER_WRAP_COMMON_COUNT];
  static int samplerFilterTypeEnums[FX_SAMPLER_FILTER_COMMON_COUNT];


  static bool initialized;

  static cfxGlEnumMaps initializer;
};



#endif // _CFX_GLENUMMAPS_H

