/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_ANNOTATE_H
#define _CFX_ANNOTATE_H


#include <string>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif


class cfxData;
class cfxEffect;
class cfxParam;
class cfxPass;
class cfxTechnique;
class cfxShader;


// cfxAnnotate
class cfxAnnotate
{
public:

  cfxAnnotate(cfxData* _data, const std::string& _name);
  virtual ~cfxAnnotate();
 
  virtual bool apply(const cfxEffect* effect);
  virtual bool apply(const cfxParam* param);
  virtual bool apply(const cfxPass* pass);
  virtual bool apply(const cfxTechnique* technique);
  virtual bool apply(const cfxShader* shader);

  CGannotation getAnnotation() const;

  const cfxData *getData() const;
  const std::string &getName() const;

protected:
  
  cfxData* data;

  std::string name;
  
  CGannotation annotation;

};


#endif // _CFX_ANNOTATE_H
