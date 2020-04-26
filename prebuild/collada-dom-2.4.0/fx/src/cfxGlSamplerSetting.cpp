/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#include <stdlib.h>
#include <stdio.h>

// !!!tsl request #defines for state names
//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

// User includes

#include <cfxGlSamplerSetting.h>
#include <cfxEffect.h>
#include <cfxParam.h>


// cfxGlSamplerSetting
cfxGlSamplerSetting::cfxGlSamplerSetting(cfxEffect* effect, const char* stateName)
{
  state = cgGetNamedSamplerState(effect->getContext(), stateName);
  if (state == 0)
    {
      printf("ERROR: Sampler state not found: %s\n", stateName);
    }
}

cfxGlSamplerSetting::~cfxGlSamplerSetting()
{
}



// cfxGlSamplerSettingWrapS
cfxGlSamplerSettingWrapS::cfxGlSamplerSettingWrapS(cfxEffect* effect, const char* stateName, int _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingWrapS::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetIntStateAssignment(assignment, value);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingWrapS::getType() const
{
	return cfxGlSamplerSetting::WRAP_S;
}

int cfxGlSamplerSettingWrapS::getValue() const
{
	return value;
}

// cfxGlSamplerSettingWrapT
cfxGlSamplerSettingWrapT::cfxGlSamplerSettingWrapT(cfxEffect* effect, const char* stateName, int _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingWrapT::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetIntStateAssignment(assignment, value);

      return true;
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingWrapT::getType() const
{
	return cfxGlSamplerSetting::WRAP_T;
}

int cfxGlSamplerSettingWrapT::getValue() const
{
	return value;
}

// cfxGlSamplerSettingMinFilter
cfxGlSamplerSettingMinFilter::cfxGlSamplerSettingMinFilter(cfxEffect* effect, const char* stateName, int _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingMinFilter::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetIntStateAssignment(assignment, value);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingMinFilter::getType() const
{
	return cfxGlSamplerSetting::MINFILTER;
}

int cfxGlSamplerSettingMinFilter::getValue() const
{
	return value;
}


// cfxGlSamplerSettingMagFilter
cfxGlSamplerSettingMagFilter::cfxGlSamplerSettingMagFilter(cfxEffect* effect, const char* stateName, int _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingMagFilter::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetIntStateAssignment(assignment, value);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingMagFilter::getType() const
{
	return cfxGlSamplerSetting::MAGFILTER;
}

int cfxGlSamplerSettingMagFilter::getValue() const
{
	return value;
}

// cfxGlSamplerSettingBorderColor
cfxGlSamplerSettingBorderColor::cfxGlSamplerSettingBorderColor(cfxEffect* effect, const char* stateName, cfxFloat4& _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingBorderColor::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetFloatArrayStateAssignment(assignment, &value.f0);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingBorderColor::getType() const
{
	return cfxGlSamplerSetting::BORDER_COLOR;
}

cfxFloat4 cfxGlSamplerSettingBorderColor::getValue() const
{
	return value;
}

// cfxGlSamplerSettingGenerateMipMap
cfxGlSamplerSettingGenerateMipMap::cfxGlSamplerSettingGenerateMipMap(cfxEffect* effect, const char* stateName, cfxBool _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingGenerateMipMap::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetBoolStateAssignment(assignment, value);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingGenerateMipMap::getType() const
{
	return cfxGlSamplerSetting::GENERATE_MIPMAP;
}

cfxBool cfxGlSamplerSettingGenerateMipMap::getValue() const
{
	return value;
}

// cfxGlSamplerSettingMaxMipLevel
cfxGlSamplerSettingMaxMipLevel::cfxGlSamplerSettingMaxMipLevel(cfxEffect* effect, const char* stateName, float _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingMaxMipLevel::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetFloatStateAssignment(assignment, value);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingMaxMipLevel::getType() const
{
	return cfxGlSamplerSetting::BORDER_COLOR;
}

float cfxGlSamplerSettingMaxMipLevel::getValue() const
{
	return value;
}

// cfxGlSamplerSettingLodBias
cfxGlSamplerSettingLodBias::cfxGlSamplerSettingLodBias(cfxEffect* effect, const char* stateName, float _value)
  : cfxGlSamplerSetting(effect, stateName),
    value(_value)
{
}

cfxBool cfxGlSamplerSettingLodBias::apply(const cfxParam* param)
{
  cfxBool result = false;

  if (state)
    {
      assignment = cgCreateSamplerStateAssignment(param->getParameter(), state);
      result = cgSetFloatStateAssignment(assignment, value);
    }

  return result;
}

cfxGlSamplerSetting::type_enum cfxGlSamplerSettingLodBias::getType() const
{
	return cfxGlSamplerSetting::BORDER_COLOR;
}

float cfxGlSamplerSettingLodBias::getValue() const
{
	return value;
}

