/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_DATAMAKER_H
#define _CFX_DATAMAKER_H


#include <map>
#include <string>

#include <cfxTypes.h>
#include <cfxSampler.h>

class cfxData;
class domFx_basic_type_common;
class domCg_param_type;
class domFx_annotate_type_common;


// the data maker ends up needing to know about the effect 
// because samplers (which are just data) need the effect for context
class cfxEffect;

// cfxDataMaker
class cfxDataMaker
{
 protected:

  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect* effect);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect* effect);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect* effect);

  static std::map<std::string, cfxDataMaker*> mapTypeNameToDataMaker;
  
 public:

  virtual ~cfxDataMaker(){}
  static cfxData* makeDataForParam(domFx_basic_type_common* paramInitializer, cfxEffect*);
  static cfxData* makeDataForParam(domCg_param_type* paramInitializer, cfxEffect*);
  static cfxData* makeDataForAnnotate(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
  cfxSampler* sampler; 
};


// BASIC DATA MAKERS

// cfxDataMakerBool
class cfxDataMakerBool : public cfxDataMaker
{
 protected:
  cfxDataMakerBool();
  static cfxDataMakerBool maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerBool1
class cfxDataMakerBool1 : public cfxDataMaker
{
 protected:
  cfxDataMakerBool1();
  static cfxDataMakerBool1 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerBool2
class cfxDataMakerBool2 : public cfxDataMaker
{
 protected:
  cfxDataMakerBool2();
  static cfxDataMakerBool2 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerBool3
class cfxDataMakerBool3 : public cfxDataMaker
{
 protected:
  cfxDataMakerBool3();
  static cfxDataMakerBool3 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerBool4
class cfxDataMakerBool4 : public cfxDataMaker 
{
 protected:
  cfxDataMakerBool4();
  static cfxDataMakerBool4 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerInt
class cfxDataMakerInt : public cfxDataMaker 
{
 protected:
  cfxDataMakerInt();
  static cfxDataMakerInt maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerInt1
class cfxDataMakerInt1 : public cfxDataMaker 
{
 protected:
  cfxDataMakerInt1();
  static cfxDataMakerInt1 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerInt2
class cfxDataMakerInt2 : public cfxDataMaker
{
 protected:
  cfxDataMakerInt2();
  static cfxDataMakerInt2 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerInt3
class cfxDataMakerInt3 : public cfxDataMaker 
{
 protected:
  cfxDataMakerInt3();
  static cfxDataMakerInt3 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerInt4
class cfxDataMakerInt4 : public cfxDataMaker 
{
 protected:
  cfxDataMakerInt4();
  static cfxDataMakerInt4 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat
class cfxDataMakerFloat : public cfxDataMaker
{
 protected:
  cfxDataMakerFloat();
  static cfxDataMakerFloat maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat1
class cfxDataMakerFloat1 : public cfxDataMaker
{
 protected:
  cfxDataMakerFloat1();
  static cfxDataMakerFloat1 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat2
class cfxDataMakerFloat2 : public cfxDataMaker
{
 protected:
  cfxDataMakerFloat2();
  static cfxDataMakerFloat2 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat3
class cfxDataMakerFloat3 : public cfxDataMaker 
{
 protected:
  cfxDataMakerFloat3();
  static cfxDataMakerFloat3 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat4
class cfxDataMakerFloat4 : public cfxDataMaker 
{
 protected:
  cfxDataMakerFloat4();
  static cfxDataMakerFloat4 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat2x2
class cfxDataMakerFloat2x2 : public cfxDataMaker
{
 protected:
  cfxDataMakerFloat2x2();
  static cfxDataMakerFloat2x2 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat3x3
class cfxDataMakerFloat3x3 : public cfxDataMaker 
{
 protected:
  cfxDataMakerFloat3x3();
  static cfxDataMakerFloat3x3 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// cfxDataMakerFloat4x4
class cfxDataMakerFloat4x4 : public cfxDataMaker 
{
 protected:
  cfxDataMakerFloat4x4();
  static cfxDataMakerFloat4x4 maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


// PARAM DATA MAKERS


// cfxDataMakerSampler2D
class cfxDataMakerSampler2D : public cfxDataMaker
{
 protected:
  cfxDataMakerSampler2D();
  static cfxDataMakerSampler2D maker; 
 
  virtual cfxData* makeData(domFx_basic_type_common* paramInitializer, cfxEffect*);
  virtual cfxData* makeData(domCg_param_type* paramInitializer, cfxEffect*);
};


// ANNOTATE DATA MAKERS

// cfxDataMakerString
class cfxDataMakerString : public cfxDataMaker 
{
 protected:
  cfxDataMakerString();
  static cfxDataMakerString maker; 
 
  virtual cfxData* makeData(domFx_annotate_type_common* annotateInitializer, cfxEffect*);
};


#endif // _CFX_DATAMAKER_H
