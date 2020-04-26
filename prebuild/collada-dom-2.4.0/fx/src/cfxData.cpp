/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes

#include <stdio.h>
#include <assert.h>

// User includes

#include <cfxData.h>
#include <cfxParam.h>
#include <cfxAnnotate.h>
#include <cfxSampler.h>

//#ifndef _LIB
#include <Cg/cg.h>
#include <Cg/cgGL.h>
//#else
//#include <cfxNoCg.h>
//#endif


// cfxData
bool cfxData::apply(cfxParam* param)
{ 
  (void)param;
  printf("ERROR!  Attempted to apply data of a type which is unsupported for parameters.\n");
  return false; 
}

bool cfxData::apply(cfxAnnotate* annotate)
{
  (void)annotate;
  printf("ERROR!  Attempted to apply data of a type which is unsupported for annotations.\n");
  return false; 
}


// cfxDataBool
cfxDataBool::cfxDataBool(const cfxBool& _data) 
  : data(_data)
{
}

CGtype cfxDataBool::getType() const
{ 
  return CG_BOOL; 
}

bool cfxDataBool::apply(cfxParam* param) 
{ 
  cgSetParameter1f(param->getParameter(), (float)data); 
  return true; 
}

bool cfxDataBool::apply(cfxAnnotate* annotate) 
{ 
  cgSetBoolAnnotation(annotate->getAnnotation(), data);
  return true; 
}

const cfxBool &cfxDataBool::getData() const
{
	return data;
}


// cfxDataBool1
cfxDataBool1::cfxDataBool1(const cfxBool1& _data) 
  : data(_data)
{
}

CGtype cfxDataBool1::getType() const
{ 
  return CG_BOOL1; 
}

bool cfxDataBool1::apply(cfxParam* param) 
{ 
  cgSetParameter1f(param->getParameter(), (float)data.b0); 
  return true; 
}

bool cfxDataBool1::apply(cfxAnnotate* annotate) 
{ 
  cgSetBoolAnnotation(annotate->getAnnotation(), data.b0);
  return true; 
}

const cfxBool1 &cfxDataBool1::getData() const
{
	return data;
}


// cfxDataBool2
cfxDataBool2::cfxDataBool2(const cfxBool2& _data) 
  : data(_data)
{
}

CGtype cfxDataBool2::getType() const
{ 
  return CG_BOOL2; 
}

bool cfxDataBool2::apply(cfxParam* param) 
{ 
  cgSetParameter2f(param->getParameter(), (float)data.b0, (float)data.b1); 
  return true; 
}

bool cfxDataBool2::apply(cfxAnnotate* annotate) 
{ 
  (void)annotate;
  printf("API to support annotations of type CG_BOOL2 not implemented yet.\n");
  return true; 
}

const cfxBool2 &cfxDataBool2::getData() const
{
	return data;
}


// cfxDataBool3
cfxDataBool3::cfxDataBool3(const cfxBool3& _data) 
  : data(_data)
{
}

CGtype cfxDataBool3::getType() const
{ 
  return CG_BOOL3; 
}

bool cfxDataBool3::apply(cfxParam* param) 
{ 
  cgSetParameter3f(param->getParameter(), (float)data.b0, (float)data.b1, (float)data.b2); 
  return true; 
}

bool cfxDataBool3::apply(cfxAnnotate* annotate) 
{ 
  (void)annotate;
  printf("API to support annotations of type CG_BOOL3 not implemented yet.\n");
  return true;
}

const cfxBool3 &cfxDataBool3::getData() const
{
	return data;
}


// cfxDataBool4
cfxDataBool4::cfxDataBool4(const cfxBool4& _data) 
  : data(_data)
{
}

CGtype cfxDataBool4::getType() const
{ 
  return CG_BOOL4; 
}

bool cfxDataBool4::apply(cfxParam* param) 
{ 
  cgSetParameter4f(param->getParameter(), (float)data.b0, (float)data.b1, (float)data.b2, (float)data.b3); 
  return true; 
}

bool cfxDataBool4::apply(cfxAnnotate* annotate)
{ 
  (void)annotate;
  printf("API to support annotations of type CG_BOOL4 not implemented yet.\n");
  return true; 
}

const cfxBool4 &cfxDataBool4::getData() const
{
	return data;
}


// cfxDataInt
cfxDataInt::cfxDataInt(const cfxInt& _data) 
  : data(_data)
{
}

CGtype cfxDataInt::getType() const
{ 
  return CG_INT; 
}

bool cfxDataInt::apply(cfxParam* param) 
{ 
  cgSetParameter1f(param->getParameter(), (float)data);   
  return true; 
}

bool cfxDataInt::apply(cfxAnnotate* annotate) 
{ 
  cgSetIntAnnotation(annotate->getAnnotation(), data);
  return true; 
}

const cfxInt &cfxDataInt::getData() const
{
	return data;
}


// cfxDataInt1
cfxDataInt1::cfxDataInt1(const cfxInt1& _data) 
  : data(_data)
{
}

CGtype cfxDataInt1::getType() const
{ 
  return CG_INT1; 
}

bool cfxDataInt1::apply(cfxParam* param) 
{ 
  cgSetParameter1f(param->getParameter(), (float)data.i0);   
  return true; 
}

bool cfxDataInt1::apply(cfxAnnotate* annotate) 
{ 
  cgSetIntAnnotation(annotate->getAnnotation(), data.i0);
  return true; 
}

const cfxInt1 &cfxDataInt1::getData() const
{
	return data;
}


// cfxDataInt2
cfxDataInt2::cfxDataInt2(const cfxInt2& _data) 
  : data(_data)
{
}

CGtype cfxDataInt2::getType() const
{ 
  return CG_INT2; 
}

bool cfxDataInt2::apply(cfxParam* param) 
{
  cgSetParameter2f(param->getParameter(), (float)data.i0, (float)data.i1);   
  return true; 
}

bool cfxDataInt2::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_INT2 not implemented yet.\n");
  return true; 
}

const cfxInt2 &cfxDataInt2::getData() const
{
	return data;
}


// cfxDataInt3
cfxDataInt3::cfxDataInt3(const cfxInt3& _data) 
  : data(_data)
{
}

CGtype cfxDataInt3::getType() const
{
  return CG_INT3; 
}

bool cfxDataInt3::apply(cfxParam* param) 
{ 
  cgSetParameter3f(param->getParameter(), (float)data.i0, (float)data.i1, (float)data.i2);   
  return true; 
}

bool cfxDataInt3::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_INT3 not implemented yet.\n");
  return true;
}

const cfxInt3 &cfxDataInt3::getData() const
{
	return data;
}


// cfxDataInt4
cfxDataInt4::cfxDataInt4(const cfxInt4& _data) 
  : data(_data)
{
}

CGtype cfxDataInt4::getType() const
{
  return CG_INT4; 
}

bool cfxDataInt4::apply(cfxParam* param) 
{
  cgSetParameter4f(param->getParameter(), (float)data.i0, (float)data.i1, (float)data.i2, (float)data.i3);   
  return true; 
}

bool cfxDataInt4::apply(cfxAnnotate* annotate)
{
  (void)annotate;
  printf("API to support annotations of type CG_INT4 not implemented yet.\n");
  return true; 
}

const cfxInt4 &cfxDataInt4::getData() const
{
	return data;
}


// cfxDataFloat
cfxDataFloat::cfxDataFloat(const cfxFloat& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat::getType() const
{
  return CG_FLOAT; 
}

bool cfxDataFloat::apply(cfxParam* param) 
{
  cgSetParameter1f(param->getParameter(), data);
  return true; 
}

bool cfxDataFloat::apply(cfxAnnotate* annotate) 
{
  cgSetFloatAnnotation(annotate->getAnnotation(), data);
  return true; 
}

const cfxFloat &cfxDataFloat::getData() const
{
	return data;
}


// cfxDataFloat1
cfxDataFloat1::cfxDataFloat1(const cfxFloat1& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat1::getType() const
{
  return CG_FLOAT; 
}

bool cfxDataFloat1::apply(cfxParam* param) 
{
  cgSetParameter1f(param->getParameter(), data.f0);
  return true; 
}

bool cfxDataFloat1::apply(cfxAnnotate* annotate) 
{
  cgSetFloatAnnotation(annotate->getAnnotation(), data.f0);
  return true; 
}

const cfxFloat1 &cfxDataFloat1::getData() const
{
	return data;
}


// cfxDataFloat2
cfxDataFloat2::cfxDataFloat2(const cfxFloat2& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat2::getType() const
{
  return CG_FLOAT2; 
}

bool cfxDataFloat2::apply(cfxParam* param) 
{
  cgSetParameter2f(param->getParameter(), data.f0, data.f1);
  return true; 
}

bool cfxDataFloat2::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_FLOAT2 not implemented yet.\n");
  return true; 
}

const cfxFloat2 &cfxDataFloat2::getData() const
{
	return data;
}


// cfxDataFloat3
cfxDataFloat3::cfxDataFloat3(const cfxFloat3& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat3::getType() const
{
  return CG_FLOAT3; 
}

bool cfxDataFloat3::apply(cfxParam* param) 
{
  cgSetParameter3f(param->getParameter(), data.f0, data.f1, data.f2);
  return true; 
}

bool cfxDataFloat3::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_FLOAT3 not implemented yet.\n");
  return true; 
}

const cfxFloat3 &cfxDataFloat3::getData() const
{
	return data;
}


// cfxDataFloat4
cfxDataFloat4::cfxDataFloat4(const cfxFloat4& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat4::getType() const
{
  return CG_FLOAT4; 
}

bool cfxDataFloat4::apply(cfxParam* param) 
{
  cgSetParameter4f(param->getParameter(), data.f0, data.f1, data.f2, data.f3);
  return true; 
}

bool cfxDataFloat4::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_FLOAT4 not implemented yet.\n");
  return true; 
}

const cfxFloat4 &cfxDataFloat4::getData() const
{
	return data;
}


// cfxDataFloat2x2
cfxDataFloat2x2::cfxDataFloat2x2(const cfxFloat2x2& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat2x2::getType() const
{
  return CG_FLOAT2x2; 
}

bool cfxDataFloat2x2::apply(cfxParam* param) 
{
  // using column major matrix setting because i think that is what is used by COLLADA
  cgSetMatrixParameterfc(param->getParameter(), &data.f00);
  return true; 
}

bool cfxDataFloat2x2::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_FLOAT2x2 not implemented yet.\n");
  return true; 
}

const cfxFloat2x2 &cfxDataFloat2x2::getData() const
{
	return data;
}


// cfxDataFloat3x3
cfxDataFloat3x3::cfxDataFloat3x3(const cfxFloat3x3& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat3x3::getType() const
{
  return CG_FLOAT3x3; 
}

bool cfxDataFloat3x3::apply(cfxParam* param)
{
  // using column major matrix setting because i think that is what is used by COLLADA
  cgSetMatrixParameterfc(param->getParameter(), &data.f00);
  return true; 
}

bool cfxDataFloat3x3::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_FLOAT3x3 not implemented yet.\n");
  return true; 
}

const cfxFloat3x3 &cfxDataFloat3x3::getData() const
{
	return data;
}


// cfxDataFloat4x4
cfxDataFloat4x4::cfxDataFloat4x4(const cfxFloat4x4& _data) 
  : data(_data)
{
}

CGtype cfxDataFloat4x4::getType() const
{
  return CG_FLOAT4x4; 
}

bool cfxDataFloat4x4::apply(cfxParam* param) 
{
  // using column major matrix setting because i think that is what is used by COLLADA
    cgSetMatrixParameterfc(param->getParameter(), &data.f00);
  return true; 
}

bool cfxDataFloat4x4::apply(cfxAnnotate* annotate) 
{
  (void)annotate;
  printf("API to support annotations of type CG_FLOAT4x4 not implemented yet.\n");
  return true; 
}

const cfxFloat4x4 &cfxDataFloat4x4::getData() const
{
	return data;
}


// cfxDataSampler1D
cfxDataSampler1D::cfxDataSampler1D(cfxSampler* _data)
  : data(_data)
{
}

CGtype cfxDataSampler1D::getType() const
{
  return CG_SAMPLER1D; 
}

bool cfxDataSampler1D::apply(cfxParam* /*param*/) 
{
  return true; 
}

const cfxSampler *cfxDataSampler1D::getData() const
{
	return data;
}

// cfxDataSampler2D
cfxDataSampler2D::cfxDataSampler2D(cfxSampler* _data)
  : data(_data)
{
}
cfxDataSampler2D::~cfxDataSampler2D()
{
	delete data;
}

CGtype cfxDataSampler2D::getType() const
{
  return CG_SAMPLER2D; 
}

bool cfxDataSampler2D::apply(cfxParam* param) 
{
  // get the texture ID from the sampler
  GLuint textureId = 0;

  // the sampler really should never be NULL
  assert(data);

  // call the sampler apply to 
  // 1) initialize texture id by resolving source
  // 2) have its states and stuff applied
  data->apply(param);

  textureId = data->getTextureId();

  // i think textureId should never be zero
      // set the parameter for this sampler's texture id
      // using managed textures replaces the need to call enabletextureparameter
  cgGLSetTextureParameter(param->getParameter(), textureId);   

  if (textureId == 0)
    {
      printf("ERROR:  sampler had texture Id of zero, states not set.\n");
    }

  return true; 
}

const cfxSampler *cfxDataSampler2D::getData() const
{
	return data;
}

// cfxDataSampler3D
cfxDataSampler3D::cfxDataSampler3D(cfxSampler* _data)
  : data(_data)
{
}

CGtype cfxDataSampler3D::getType() const
{
  return CG_SAMPLER3D; 
}

bool cfxDataSampler3D::apply(cfxParam* /*param*/) 
{
  return true; 
}

const cfxSampler *cfxDataSampler3D::getData() const
{
	return data;
}

// cfxDataSamplerCUBE
cfxDataSamplerCUBE::cfxDataSamplerCUBE(cfxSampler* _data)
  : data(_data)
{
}

CGtype cfxDataSamplerCUBE::getType() const
{
  return CG_SAMPLERCUBE; 
}

bool cfxDataSamplerCUBE::apply(cfxParam* /*param*/) 
{
  return true; 
}

const cfxSampler *cfxDataSamplerCUBE::getData() const
{
	return data;
}

// cfxDataString
cfxDataString::cfxDataString(const std::string& _data) 
  : data(_data)
{
}

CGtype cfxDataString::getType() const
{
  return CG_STRING; 
}

bool cfxDataString::apply(cfxAnnotate* annotate) 
{ 
  cgSetStringAnnotation(annotate->getAnnotation(), data.c_str());
  return true; 
}

const std::string &cfxDataString::getData() const
{
	return data;
}


