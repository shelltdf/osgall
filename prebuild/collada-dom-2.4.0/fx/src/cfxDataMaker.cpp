/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#include <cfxDataMaker.h>
#include <cfxData.h>
#include <cfxEffect.h>
#include <cfxGlEnumMaps.h>
#include <cfxSampler.h>
#include <cfxGlSamplerSetting.h>

#include <dom/domFx_basic_type_common.h>
#include <dom/domCg_param_type.h>
#include <dom/domFx_annotate_type_common.h>
#include <dom/domConstants.h>


// cfxDataMaker
std::map<std::string, cfxDataMaker*> cfxDataMaker::mapTypeNameToDataMaker;

cfxData* cfxDataMaker::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  printf("No support for common parameter data of type:  %s\n", paramInitializer->getContents().get(0)->getTypeName());
  return NULL;
}


cfxData* cfxDataMaker::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
  printf("No support for cg parameter data of type:  %s\n", paramInitializer->getContents().get(0)->getTypeName());
  return NULL;
}


cfxData* cfxDataMaker::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  printf("No support for common annotation data of type:  %s\n", annotateInitializer->getContents().get(0)->getTypeName());
  return NULL;
}

cfxData* cfxDataMaker::makeDataForParam(domFx_basic_type_common* paramInitializer, cfxEffect* effect)
{
    //printf("make data for fx common param\n");
  std::map<std::string, cfxDataMaker*>::iterator iter;
  // here is the element which owns the actual data
  // it is a choice in schema, so there will be only one element in _contents that exists as the "chosen" item, 
  // which will be one of the elements in this class
  daeElement* dataElement = paramInitializer->getContents().get(0);
  // get typename from contents, use this to lookup the factory for the data
  //printf("param for type %s\n", dataElement->getTypeName());
  daeString dataTypeName = dataElement->getTypeName();
  iter = mapTypeNameToDataMaker.find(dataTypeName);
  if (iter == mapTypeNameToDataMaker.end())
    {
	printf("ERROR:  cfxDataMaker not found for common param type name %s\n", dataTypeName);
      return NULL;
    }
  return iter->second->makeData(paramInitializer, effect); 
}

cfxData* cfxDataMaker::makeDataForParam(domCg_param_type* paramInitializer, cfxEffect* effect)
{
    //printf("make data for CG param\n");
  std::map<std::string, cfxDataMaker*>::iterator iter;
  // here is the element which owns the actual data
  // it is a choice in schema, so there will be only one element in _contents that exists as the "chosen" item, 
  // which will be one of the elements in this class
  daeElement* dataElement = paramInitializer->getContents().get(0);
  // get typename from contents, use this to lookup the factory for the data
  //printf("param for type %s\n", dataElement->getTypeName());
  daeString dataTypeName = dataElement->getTypeName();
  iter = mapTypeNameToDataMaker.find(dataTypeName);
  if (iter == mapTypeNameToDataMaker.end())
    {
	printf("ERROR:  cfxDataMaker not found for cg param type name %s\n", dataTypeName);
      return NULL;
    }
  return iter->second->makeData(paramInitializer, effect); 
}

cfxData* cfxDataMaker::makeDataForAnnotate(domFx_annotate_type_common* annotateInitializer, cfxEffect* effect)
{
    //printf("make data for common annotate\n");
  std::map<std::string, cfxDataMaker*>::iterator iter;
  // then here is the element which owns the actual data
  // it is a choice in schema, so there will be only one element in _contents that exists as the "chosen" item, 
  // which will be one of the elements in this class
  daeElement* dataElement = annotateInitializer->getContents().get(0);
  // get typename from contents, use this to lookup the factory for the data
  daeString dataTypeName = dataElement->getTypeName();
  iter = mapTypeNameToDataMaker.find(dataTypeName);
  if (iter == mapTypeNameToDataMaker.end())
    {
	printf("ERROR:  cfxDataMaker not found for common annotate type name %s\n", dataTypeName);
      return NULL;
    }
  return iter->second->makeData(annotateInitializer, effect);
}


// BASIC DATA MAKERS

// cfxDataMakerBool
cfxDataMakerBool::cfxDataMakerBool()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_BOOL, this));
}

cfxDataMakerBool cfxDataMakerBool::maker;

cfxDataBool* newDataBool(const domBool& db)
{
  cfxBool b;
  b = db;
  cfxDataBool* data = new cfxDataBool(b);
  return data;
}

cfxData* cfxDataMakerBool::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataBool(paramInitializer->getBool()->getValue());
}

cfxData* cfxDataMakerBool::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataBool(annotateInitializer->getBool()->getValue());
}


// cfxDataMakerBool1
cfxDataMakerBool1::cfxDataMakerBool1()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_BOOL1, this));
}

cfxDataMakerBool1 cfxDataMakerBool1::maker;

cfxDataBool1* newDataBool1(const domBool& db)
{
  cfxBool1 b1;
  b1.b0 = db;
  cfxDataBool1* data = new cfxDataBool1(b1);
  return data;
}

cfxData* cfxDataMakerBool1::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataBool1(paramInitializer->getBool()->getValue());
}

cfxData* cfxDataMakerBool1::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataBool1(annotateInitializer->getBool()->getValue());
}


// cfxDataMakerBool2
cfxDataMakerBool2::cfxDataMakerBool2()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_BOOL2, this));
}

cfxDataMakerBool2 cfxDataMakerBool2::maker;

cfxDataBool2* newDataBool2(const domBool2& db2)
{
  cfxBool2 b2;
  b2.b0 = db2.get(0);
  b2.b1 = db2.get(1);
  cfxDataBool2* data = new cfxDataBool2(b2);
  return data;
}

cfxData* cfxDataMakerBool2::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataBool2(paramInitializer->getBool2()->getValue());
}

cfxData* cfxDataMakerBool2::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataBool2(annotateInitializer->getBool2()->getValue());
}


// cfxDataMakerBool3
cfxDataMakerBool3::cfxDataMakerBool3()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_BOOL3, this));
}

cfxDataMakerBool3 cfxDataMakerBool3::maker;

cfxDataBool3* newDataBool3(const domBool3& db3)
{
  cfxBool3 b3;
  b3.b0 = db3.get(0);
  b3.b1 = db3.get(1);
  b3.b2 = db3.get(2);
  cfxDataBool3* data = new cfxDataBool3(b3);
  return data;
}

cfxData* cfxDataMakerBool3::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataBool3(paramInitializer->getBool3()->getValue());
}

cfxData* cfxDataMakerBool3::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataBool3(annotateInitializer->getBool3()->getValue());
}


// cfxDataMakerBool4
cfxDataMakerBool4::cfxDataMakerBool4()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_BOOL4, this));
}

cfxDataMakerBool4 cfxDataMakerBool4::maker;

cfxDataBool4* newDataBool4(const domBool4& db4)
{
  cfxBool4 b4;
  b4.b0 = db4.get(0);
  b4.b1 = db4.get(1);
  b4.b2 = db4.get(2);
  b4.b3 = db4.get(3);
  cfxDataBool4* data = new cfxDataBool4(b4);
  return data;
}

cfxData* cfxDataMakerBool4::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataBool4(paramInitializer->getBool4()->getValue());
}

cfxData* cfxDataMakerBool4::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataBool4(annotateInitializer->getBool4()->getValue());
}


// cfxDataMakerInt
cfxDataMakerInt::cfxDataMakerInt()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_INT, this));
}

cfxDataMakerInt cfxDataMakerInt::maker;

cfxDataInt* newDataInt(const domInt& di)
{
  cfxInt i;
  i = (cfxInt) di;
  cfxDataInt* data = new cfxDataInt(i);
  return data;
}

cfxData* cfxDataMakerInt::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataInt(paramInitializer->getInt()->getValue());
}

cfxData* cfxDataMakerInt::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataInt(annotateInitializer->getInt()->getValue());
}


// cfxDataMakerInt1
cfxDataMakerInt1::cfxDataMakerInt1()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_INT1, this));
}

cfxDataMakerInt1 cfxDataMakerInt1::maker;

cfxDataInt1* newDataInt1(const domInt& di)
{
  cfxInt1 i1;
  i1.i0 = (cfxInt) di;
  cfxDataInt1* data = new cfxDataInt1(i1);
  return data;
}

cfxData* cfxDataMakerInt1::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataInt1(paramInitializer->getInt()->getValue());
}

cfxData* cfxDataMakerInt1::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataInt1(annotateInitializer->getInt()->getValue());
}


// cfxDataMakerInt2
cfxDataMakerInt2::cfxDataMakerInt2()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_INT2, this));
}

cfxDataMakerInt2 cfxDataMakerInt2::maker;

cfxDataInt2* newDataInt2(const domInt2& di2)
{
  cfxInt2 i2;
  i2.i0 = (cfxInt) di2.get(0);
  i2.i1 = (cfxInt) di2.get(1);
  cfxDataInt2* data = new cfxDataInt2(i2);
  return data;
}

cfxData* cfxDataMakerInt2::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataInt2(paramInitializer->getInt2()->getValue());
}

cfxData* cfxDataMakerInt2::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataInt2(annotateInitializer->getInt2()->getValue());
}


// cfxDataMakerInt3
cfxDataMakerInt3::cfxDataMakerInt3()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_INT3, this));
}

cfxDataMakerInt3 cfxDataMakerInt3::maker;

cfxDataInt3* newDataInt3(const domInt3& di3)
{
  cfxInt3 i3;
  i3.i0 = (cfxInt) di3.get(0);
  i3.i1 = (cfxInt) di3.get(1);
  i3.i2 = (cfxInt) di3.get(2);
  cfxDataInt3* data = new cfxDataInt3(i3);
  return data;
}

cfxData* cfxDataMakerInt3::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataInt3(paramInitializer->getInt3()->getValue());
}

cfxData* cfxDataMakerInt3::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataInt3(annotateInitializer->getInt3()->getValue());
}


// cfxDataMakerInt4
cfxDataMakerInt4::cfxDataMakerInt4()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_INT4, this));
}

cfxDataMakerInt4 cfxDataMakerInt4::maker;

cfxDataInt4* newDataInt4(const domInt4& di4)
{
  cfxInt4 i4;
  i4.i0 = (cfxInt) di4.get(0);
  i4.i1 = (cfxInt) di4.get(1);
  i4.i2 = (cfxInt) di4.get(2);
  i4.i3 = (cfxInt) di4.get(3);
  cfxDataInt4* data = new cfxDataInt4(i4);
  return data;
}

cfxData* cfxDataMakerInt4::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataInt4(paramInitializer->getInt4()->getValue());
}

cfxData* cfxDataMakerInt4::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataInt4(annotateInitializer->getInt4()->getValue());
}


// cfxDataMakerFloat
cfxDataMakerFloat::cfxDataMakerFloat()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT, this));
}

cfxDataMakerFloat cfxDataMakerFloat::maker;

cfxDataFloat* newDataFloat(const domFloat& df)
{
  cfxFloat f;
  f = (cfxFloat) df;
  cfxDataFloat* data = new cfxDataFloat(f);
  return data;
}

cfxData* cfxDataMakerFloat::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat(paramInitializer->getFloat()->getValue());
}

cfxData* cfxDataMakerFloat::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
  return newDataFloat(paramInitializer->getFloat()->getValue());
}

cfxData* cfxDataMakerFloat::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat(annotateInitializer->getFloat()->getValue());
}


// cfxDataMakerFloat1
cfxDataMakerFloat1::cfxDataMakerFloat1()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT1, this));
}

cfxDataMakerFloat1 cfxDataMakerFloat1::maker;

cfxDataFloat1* newDataFloat1(const domFloat& df)
{
  cfxFloat1 f1;
  f1.f0 = (cfxFloat) df;
  cfxDataFloat1* data = new cfxDataFloat1(f1);
  return data;
}

cfxData* cfxDataMakerFloat1::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat1(paramInitializer->getFloat()->getValue());
}

cfxData* cfxDataMakerFloat1::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat1(annotateInitializer->getFloat()->getValue());
}


// cfxDataMakerFloat2
cfxDataMakerFloat2::cfxDataMakerFloat2()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT2, this));
}

cfxDataMakerFloat2 cfxDataMakerFloat2::maker;

cfxDataFloat2* newDataFloat2(const domFloat2& df2)
{
  cfxFloat2 f2;
  f2.f0 = (cfxFloat) df2.get(0);
  f2.f1 = (cfxFloat) df2.get(1);
  cfxDataFloat2* data = new cfxDataFloat2(f2);
  return data;
}

cfxData* cfxDataMakerFloat2::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat2(paramInitializer->getFloat2()->getValue());
}

cfxData* cfxDataMakerFloat2::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
    // this can not go through the helper method because in cg param type, 
    // dom float 4 is actually defined to be a float where as in commmon,
    // it is defined to be double.  the conversion is not automatic because
    // the dom float 4 is actually a templatized container so the types won't convert.
    //  return newDataFloat3(paramInitializer->getFloat3()->getValue());
    cfxFloat2 f2;
    f2.f0 = paramInitializer->getFloat2()->getValue().get(0);
    f2.f1 = paramInitializer->getFloat2()->getValue().get(1);
    cfxDataFloat2* data = new cfxDataFloat2(f2);
    return data;
}

cfxData* cfxDataMakerFloat2::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat2(annotateInitializer->getFloat2()->getValue());
}


// cfxDataMakerFloat3
cfxDataMakerFloat3::cfxDataMakerFloat3()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT3, this));
}

cfxDataMakerFloat3 cfxDataMakerFloat3::maker;

cfxDataFloat3* newDataFloat3(const domFloat3& df3)
{
  cfxFloat3 f3;
  f3.f0 = (cfxFloat) df3.get(0);
  f3.f1 = (cfxFloat) df3.get(1);
  f3.f2 = (cfxFloat) df3.get(2);
  cfxDataFloat3* data = new cfxDataFloat3(f3);
  return data;
}

cfxData* cfxDataMakerFloat3::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat3(paramInitializer->getFloat3()->getValue());
}

cfxData* cfxDataMakerFloat3::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
    // this can not go through the helper method because in cg param type, 
    // dom float 4 is actually defined to be a float where as in commmon,
    // it is defined to be double.  the conversion is not automatic because
    // the dom float 4 is actually a templatized container so the types won't convert.
    //  return newDataFloat3(paramInitializer->getFloat3()->getValue());
    cfxFloat3 f3;
    f3.f0 = paramInitializer->getFloat3()->getValue().get(0);
    f3.f1 = paramInitializer->getFloat3()->getValue().get(1);
    f3.f2 = paramInitializer->getFloat3()->getValue().get(2);
    cfxDataFloat3* data = new cfxDataFloat3(f3);
    return data;
}

cfxData* cfxDataMakerFloat3::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat3(annotateInitializer->getFloat3()->getValue());
}


// cfxDataMakerFloat4
cfxDataMakerFloat4::cfxDataMakerFloat4()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT4, this));
}

cfxDataMakerFloat4 cfxDataMakerFloat4::maker;

cfxDataFloat4* newDataFloat4(const domFloat4& df4)
{
  cfxFloat4 f4;
  f4.f0 = (cfxFloat) df4.get(0);
  f4.f1 = (cfxFloat) df4.get(1);
  f4.f2 = (cfxFloat) df4.get(2);
  f4.f3 = (cfxFloat) df4.get(3);
  cfxDataFloat4* data = new cfxDataFloat4(f4);
  return data;
}

cfxData* cfxDataMakerFloat4::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat4(paramInitializer->getFloat4()->getValue());
}

cfxData* cfxDataMakerFloat4::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
    // this can not go through the helper method because in cg param type, 
    // dom float 4 is actually defined to be a float where as in commmon,
    // it is defined to be double.  the conversion is not automatic because
    // the dom float 4 is actually a templatized container so the types won't convert.
    //  return newDataFloat4(paramInitializer->getFloat4()->getValue());
  cfxFloat4 f4;
  f4.f0 = paramInitializer->getFloat4()->getValue().get(0);
  f4.f1 = paramInitializer->getFloat4()->getValue().get(1);
  f4.f2 = paramInitializer->getFloat4()->getValue().get(2);
  f4.f3 = paramInitializer->getFloat4()->getValue().get(3);
  cfxDataFloat4* data = new cfxDataFloat4(f4);
  return data;
}

cfxData* cfxDataMakerFloat4::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat4(annotateInitializer->getFloat4()->getValue());
}





// cfxDataMakerFloat2x2
cfxDataMakerFloat2x2::cfxDataMakerFloat2x2()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT2X2, this));
}

cfxDataMakerFloat2x2 cfxDataMakerFloat2x2::maker;

cfxDataFloat2x2* newDataFloat2x2(const domFloat2x2& df2x2)
{
  cfxFloat2x2 f2x2;
  f2x2.f00 = (cfxFloat) df2x2.get(0);
  f2x2.f01 = (cfxFloat) df2x2.get(1);
  f2x2.f10 = (cfxFloat) df2x2.get(2);
  f2x2.f11 = (cfxFloat) df2x2.get(3);
  cfxDataFloat2x2* data = new cfxDataFloat2x2(f2x2);
  return data;
}

cfxData* cfxDataMakerFloat2x2::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat2x2(paramInitializer->getFloat2x2()->getValue());
}

cfxData* cfxDataMakerFloat2x2::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat2x2(annotateInitializer->getFloat2x2()->getValue());
}


// cfxDataMakerFloat3x3
cfxDataMakerFloat3x3::cfxDataMakerFloat3x3()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT3X3, this));
}

cfxDataMakerFloat3x3 cfxDataMakerFloat3x3::maker;

cfxDataFloat3x3* newDataFloat3x3(const domFloat3x3& df3x3)
{
  cfxFloat3x3 f3x3;
  f3x3.f00 = (cfxFloat) df3x3.get(0);
  f3x3.f01 = (cfxFloat) df3x3.get(1);
  f3x3.f02 = (cfxFloat) df3x3.get(2);
  f3x3.f10 = (cfxFloat) df3x3.get(3);
  f3x3.f11 = (cfxFloat) df3x3.get(4);
  f3x3.f12 = (cfxFloat) df3x3.get(5);
  f3x3.f20 = (cfxFloat) df3x3.get(6);
  f3x3.f21 = (cfxFloat) df3x3.get(7);
  f3x3.f22 = (cfxFloat) df3x3.get(8);
  cfxDataFloat3x3* data = new cfxDataFloat3x3(f3x3);
  return data;
}

cfxData* cfxDataMakerFloat3x3::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat3x3(paramInitializer->getFloat3x3()->getValue());
}

cfxData* cfxDataMakerFloat3x3::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat3x3(annotateInitializer->getFloat3x3()->getValue());
}


// cfxDataMakerFloat4x4
cfxDataMakerFloat4x4::cfxDataMakerFloat4x4()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FLOAT4X4, this));
}

cfxDataMakerFloat4x4 cfxDataMakerFloat4x4::maker;

cfxDataFloat4x4* newDataFloat4x4(const domFloat4x4& df4x4)
{
  cfxFloat4x4 f4x4;
  f4x4.f00 = (cfxFloat) df4x4.get(0);
  f4x4.f01 = (cfxFloat) df4x4.get(1);
  f4x4.f02 = (cfxFloat) df4x4.get(2);
  f4x4.f03 = (cfxFloat) df4x4.get(3);
  f4x4.f10 = (cfxFloat) df4x4.get(4);
  f4x4.f11 = (cfxFloat) df4x4.get(5);
  f4x4.f12 = (cfxFloat) df4x4.get(6);
  f4x4.f13 = (cfxFloat) df4x4.get(7);
  f4x4.f20 = (cfxFloat) df4x4.get(8);
  f4x4.f21 = (cfxFloat) df4x4.get(9);
  f4x4.f22 = (cfxFloat) df4x4.get(10);
  f4x4.f23 = (cfxFloat) df4x4.get(11);
  f4x4.f30 = (cfxFloat) df4x4.get(12);
  f4x4.f31 = (cfxFloat) df4x4.get(13);
  f4x4.f32 = (cfxFloat) df4x4.get(14);
  f4x4.f33 = (cfxFloat) df4x4.get(15);
  cfxDataFloat4x4* data = new cfxDataFloat4x4(f4x4);
  return data;
}

cfxData* cfxDataMakerFloat4x4::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  return newDataFloat4x4(paramInitializer->getFloat4x4()->getValue());
}

cfxData* cfxDataMakerFloat4x4::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
    // this can not go through the helper method because in cg param type, 
    // dom float 4x4 is actually defined to be a float where as in commmon,
    // it is defined to be double.  the conversion is not automatic because
    // the dom float 4 is actually a templatized container so the types won't convert.
    // return newDataFloat4x4(paramInitializer->getFloat4x4()->getValue());
  cfxFloat4x4 f4x4;
  f4x4.f00 = paramInitializer->getFloat4x4()->getValue().get(0);
  f4x4.f01 = paramInitializer->getFloat4x4()->getValue().get(1);
  f4x4.f02 = paramInitializer->getFloat4x4()->getValue().get(2);
  f4x4.f03 = paramInitializer->getFloat4x4()->getValue().get(3);
  f4x4.f10 = paramInitializer->getFloat4x4()->getValue().get(4);
  f4x4.f11 = paramInitializer->getFloat4x4()->getValue().get(5);
  f4x4.f12 = paramInitializer->getFloat4x4()->getValue().get(6);
  f4x4.f13 = paramInitializer->getFloat4x4()->getValue().get(7);
  f4x4.f20 = paramInitializer->getFloat4x4()->getValue().get(8);
  f4x4.f21 = paramInitializer->getFloat4x4()->getValue().get(9);
  f4x4.f22 = paramInitializer->getFloat4x4()->getValue().get(10);
  f4x4.f23 = paramInitializer->getFloat4x4()->getValue().get(11);
  f4x4.f30 = paramInitializer->getFloat4x4()->getValue().get(12);
  f4x4.f31 = paramInitializer->getFloat4x4()->getValue().get(13);
  f4x4.f32 = paramInitializer->getFloat4x4()->getValue().get(14);
  f4x4.f33 = paramInitializer->getFloat4x4()->getValue().get(15);
  cfxDataFloat4x4* data = new cfxDataFloat4x4(f4x4);
  return data;
}

cfxData* cfxDataMakerFloat4x4::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  return newDataFloat4x4(annotateInitializer->getFloat4x4()->getValue());
}

// PARAM DATA MAKERS

// cfxDataMakerSampler2D
cfxDataMakerSampler2D::cfxDataMakerSampler2D()
{
  //cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_SAMPLER, this));
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair("fx_sampler2D_common", this));
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair("cg_sampler2D", this));
}

cfxDataMakerSampler2D cfxDataMakerSampler2D::maker;

cfxData* cfxDataMakerSampler2D::makeData(domFx_basic_type_common* paramInitializer, cfxEffect* effect)
{
  sampler = new cfxSampler(paramInitializer->getSampler2D()->getSource()->getValue(), effect); 

  if (paramInitializer->getSampler2D()->getWrap_s())
    {
      int domEnum = paramInitializer->getSampler2D()->getWrap_s()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerWrapTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingWrapS(effect, "WrapS", glEnum));
    }

  if (paramInitializer->getSampler2D()->getWrap_t())
    {
      int domEnum = paramInitializer->getSampler2D()->getWrap_t()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerWrapTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingWrapT(effect, "WrapT", glEnum));
    }

  if (paramInitializer->getSampler2D()->getMinfilter())
    {
      int domEnum = paramInitializer->getSampler2D()->getMinfilter()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerFilterTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingMinFilter(effect, "MinFilter", glEnum));

      // the default for mipmap generation is true
      // turn it off if the min filter is GL_LINEAR or GL_NEAREST
      if (glEnum == GL_LINEAR || glEnum == GL_NEAREST)
	  sampler->setGenerateMipmaps(false);
    }

  if (paramInitializer->getSampler2D()->getMagfilter())
    {
      int domEnum = paramInitializer->getSampler2D()->getMagfilter()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerFilterTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingMagFilter(effect, "MagFilter", glEnum));
    }

  if (paramInitializer->getSampler2D()->getMipmap_bias())
    {
      float value = paramInitializer->getSampler2D()->getMipmap_bias()->getValue();
      sampler->pushSetting(new cfxGlSamplerSettingLodBias(effect, "LodBias", value));      
    }

  return new cfxDataSampler2D(sampler);
}



cfxData* cfxDataMakerSampler2D::makeData(domCg_param_type* paramInitializer, cfxEffect* effect)
{
  sampler = new cfxSampler(paramInitializer->getSampler2D()->getSource()->getValue(), effect); 

  if (paramInitializer->getSampler2D()->getWrap_s())
    {
      int domEnum = paramInitializer->getSampler2D()->getWrap_s()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerWrapTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingWrapS(effect, "WrapS", glEnum));
    }

  if (paramInitializer->getSampler2D()->getWrap_t())
    {
      int domEnum = paramInitializer->getSampler2D()->getWrap_t()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerWrapTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingWrapT(effect, "WrapT", glEnum));
    }

  if (paramInitializer->getSampler2D()->getMinfilter())
    {
      int domEnum = paramInitializer->getSampler2D()->getMinfilter()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerFilterTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingMinFilter(effect, "MinFilter", glEnum));

      // the default for mipmap generation is true
      // turn it off if the min filter is GL_LINEAR or GL_NEAREST
      if (glEnum == GL_LINEAR || glEnum == GL_NEAREST)
	  sampler->setGenerateMipmaps(false);
    }

  if (paramInitializer->getSampler2D()->getMagfilter())
    {
      int domEnum = paramInitializer->getSampler2D()->getMagfilter()->getValue();
      int glEnum = cfxGlEnumMaps::getSamplerFilterTypeEnum(domEnum);
      sampler->pushSetting(new cfxGlSamplerSettingMagFilter(effect, "MagFilter", glEnum));
    }

#if 0 
  if (paramInitializer->getSampler2D()->getBorder_color())
    {
      cfxFloat4 value;
      value.f0 = paramInitializer->getSampler2D()->getBorder_color()->getValue().get(0);
      value.f1 = paramInitializer->getSampler2D()->getBorder_color()->getValue().get(1);
      value.f2 = paramInitializer->getSampler2D()->getBorder_color()->getValue().get(2);
      value.f3 = paramInitializer->getSampler2D()->getBorder_color()->getValue().get(3);
      sampler->pushSetting(new cfxGlSamplerSettingBorderColor(effect, "BorderColor", value));      
    }

  if (paramInitializer->getSampler2D()->getMipmap_generate())
    {
      bool value = paramInitializer->getSampler2D()->getMipmap_generate()->getValue();
      sampler->pushSetting(new cfxGlSamplerSettingGenerateMipMap(effect, "GenerateMipMap", value));      
    }

  if (paramInitializer->getSampler2D()->getMipmap_maxlevel())
    {
      float value = paramInitializer->getSampler2D()->getMipmap_maxlevel()->getValue();
      sampler->pushSetting(new cfxGlSamplerSettingMaxMipLevel(effect, "MaxMipLevel", value));      
    }
#endif

  if (paramInitializer->getSampler2D()->getMipmap_bias())
    {
      float value = paramInitializer->getSampler2D()->getMipmap_bias()->getValue();
      sampler->pushSetting(new cfxGlSamplerSettingLodBias(effect, "LodBias", value));      
    }

  return new cfxDataSampler2D(sampler);
}


// ANNOTATE DATA MAKERS

// cfxDataMakerString
cfxDataMakerString::cfxDataMakerString()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_STRING, this));
}

cfxDataMakerString cfxDataMakerString::maker;

// not supporting string parameters because they can't really connect to any program params
// so seem a bit useless and are not supported in the cg runtime on PS3 anyway
#if 0 
cfxData* cfxDataMakerString::makeData(domCg_param_type* paramInitializer, cfxEffect*)
{
    cfxDataString* data;
    if (paramInitializer->getString()->getValue())
	{
	    data = new cfxDataString(paramInitializer->getString()->getValue());
	}
    else
	{
	    data = new cfxDataString("");
	}
  return data;
}
#endif

cfxData* cfxDataMakerString::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  cfxDataString* data = new cfxDataString(annotateInitializer->getString()->getValue());
  return data;
}




#if 0 
// cfxDataMakerBasicType
cfxDataMakerBasicType::cfxDataMakerBasicType()
{
  cfxDataMaker::mapTypeNameToDataMaker.insert(std::make_pair(COLLADA_ELEMENT_FX_BASIC_TYPE_COMMON, this));
}

cfxDataMakerBasicType cfxDataMakerBasicType::maker;

cfxData* cfxDataMakerBasicType::makeData(domFx_basic_type_common* paramInitializer, cfxEffect*)
{
  std::map<std::string, cfxDataMaker*>::iterator iter;
  // the element which has the actual data is inside basic type common
  daeElement* dataElement = paramInitializer->getFx_basic_type_common()->getContents().get(0);
  // get typename from contents, use this to lookup the factory for the data
  daeString dataTypeName = dataElement->getTypeName();
  iter = mapTypeNameToDataMaker.find(dataTypeName);
  if (iter == mapTypeNameToDataMaker.end())
    {
      // print error message
      return NULL;
    }
  cfxDataMaker* basicMaker = iter->second;
  return basicMaker->makeData(paramInitializer); 
}

cfxData* cfxDataMakerBasicType::makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*)
{
  std::map<std::string, cfxDataMaker*>::iterator iter;
  // the element which has the actual data is inside basic type common
  daeElement* dataElement = annotateInitializer->getFx_basic_type_common()->getContents().get(0);
  // get typename from contents, use this to lookup the factory for the data
  daeString dataTypeName = dataElement->getTypeName();
  iter = mapTypeNameToDataMaker.find(dataTypeName);
  if (iter == mapTypeNameToDataMaker.end())
    {
      // print error message
      return NULL;
    }
  return iter->second->makeData(annotateInitializer); 
}
#endif


