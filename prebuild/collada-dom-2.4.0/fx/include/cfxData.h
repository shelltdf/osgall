/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_DATA_H
#define _CFX_DATA_H

#include <cfxTypes.h>
#include <string>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

class cfxParam;
class cfxAnnotate;
class cfxSampler;

// cfxData
class cfxData
{
 public:
 
  virtual ~cfxData() {}
  
  virtual CGtype getType() const = 0;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);  
};


// cfxDataBool
class cfxDataBool : public cfxData
{
 public:
  cfxDataBool(const cfxBool& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxBool &getData() const;

 protected:

  cfxBool data;
};


// cfxDataBool1
class cfxDataBool1 : public cfxData
{
 public:
  cfxDataBool1(const cfxBool1& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxBool1 &getData() const;

 protected:

  cfxBool1 data;
};


// cfxDataBool2
class cfxDataBool2 : public cfxData
{
 public:
  cfxDataBool2(const cfxBool2& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxBool2 &getData() const;

 protected:

  cfxBool2 data;
};


// cfxDataBool3
class cfxDataBool3 : public cfxData
{
 public:
  cfxDataBool3(const cfxBool3& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxBool3 &getData() const;

 protected:

  cfxBool3 data;
};


// cfxDataBool4
class cfxDataBool4 : public cfxData
{
 public:
  cfxDataBool4(const cfxBool4& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxBool4 &getData() const;

 protected:

  cfxBool4 data;
};


// cfxDataInt
class cfxDataInt : public cfxData
{
 public:
  cfxDataInt(const cfxInt& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxInt &getData() const;

 protected:

  cfxInt data;
};


// cfxDataInt1
class cfxDataInt1 : public cfxData
{
 public:
  cfxDataInt1(const cfxInt1& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);
  
  const cfxInt1 &getData() const;

 protected:

  cfxInt1 data;
};


// cfxDataInt2
class cfxDataInt2 : public cfxData
{
 public:
  cfxDataInt2(const cfxInt2& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxInt2 &getData() const;

 protected:

  cfxInt2 data;
};


// cfxDataInt3
class cfxDataInt3 : public cfxData
{
 public:
  cfxDataInt3(const cfxInt3& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxInt3 &getData() const;

 protected:

  cfxInt3 data;
};


// cfxDataInt4
class cfxDataInt4 : public cfxData
{
 public:
  cfxDataInt4(const cfxInt4& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxInt4 &getData() const;

 protected:

  cfxInt4 data;
};


// cfxDataFloat
class cfxDataFloat : public cfxData
{
 public:
  cfxDataFloat(const cfxFloat& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat &getData() const;

 protected:

  cfxFloat data;
};


// cfxDataFloat1
class cfxDataFloat1 : public cfxData
{
 public:
  cfxDataFloat1(const cfxFloat1& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat1 &getData() const;

 protected:

  cfxFloat1 data;
};


// cfxDataFloat2
class cfxDataFloat2 : public cfxData
{
 public:
  cfxDataFloat2(const cfxFloat2& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat2 &getData() const;

 protected:

  cfxFloat2 data;
};


// cfxDataFloat3
class cfxDataFloat3 : public cfxData
{
 public:
  cfxDataFloat3(const cfxFloat3& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat3 &getData() const;

 protected:

  cfxFloat3 data;
};


// cfxDataFloat4
class cfxDataFloat4 : public cfxData
{
 public:
  cfxDataFloat4(const cfxFloat4& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat4 &getData() const;

 protected:

  cfxFloat4 data;
};


// cfxDataFloat2x2
class cfxDataFloat2x2 : public cfxData
{
 public:
  cfxDataFloat2x2(const cfxFloat2x2& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat2x2 &getData() const;

 protected:

  cfxFloat2x2 data;
};


// cfxDataFloat3x3
class cfxDataFloat3x3 : public cfxData
{
 public:
  cfxDataFloat3x3(const cfxFloat3x3& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat3x3 &getData() const;

 protected:

  cfxFloat3x3 data;
};


// cfxDataFloat4x4
class cfxDataFloat4x4 : public cfxData
{
 public:
  cfxDataFloat4x4(const cfxFloat4x4& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);
  virtual bool apply(cfxAnnotate* annotate);

  const cfxFloat4x4 &getData() const;

 protected:

  cfxFloat4x4 data;
};


// cfxDataSampler1D
class cfxDataSampler1D : public cfxData
{
 public:
  cfxDataSampler1D(cfxSampler* _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);

  const cfxSampler *getData() const;

 protected:
  
  cfxSampler* data;
};


// cfxDataSampler2D
class cfxDataSampler2D : public cfxData
{
 public:
  cfxDataSampler2D(cfxSampler* _data);
  ~cfxDataSampler2D();
  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);

  const cfxSampler *getData() const;

 protected:

  cfxSampler* data;
};


// cfxDataSampler3D
class cfxDataSampler3D : public cfxData
{
 public:
  cfxDataSampler3D(cfxSampler* _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);

  const cfxSampler *getData() const;

 protected:
  
  cfxSampler* data;
};


// cfxDataSamplerCUBE
class cfxDataSamplerCUBE : public cfxData
{
 public:
  cfxDataSamplerCUBE(cfxSampler* _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxParam* param);

  const cfxSampler *getData() const;

 protected:

  cfxSampler* data;
};


// cfxDataString
class cfxDataString : public cfxData
{
 public:
  cfxDataString(const std::string& _data);

  virtual CGtype getType() const;
  virtual bool apply(cfxAnnotate* annotate);

  const std::string &getData() const;

 protected:

  std::string data;
};



#endif // _CFX_DATA_H
