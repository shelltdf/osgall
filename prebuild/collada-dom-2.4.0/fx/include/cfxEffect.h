/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_EFFECT_H
#define _CFX_EFFECT_H

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif
//#if defined(_POSIX_)

//include <GL/gl.h>
//#endif

#include <string>
#include <map>
#include "cfxTypes.h"
#include "cfxAnnotatable.h"
#include "cfxParamable.h"
#include "cfxSurface.h"
#include "cfxCode.h"
#include "cfxTechnique.h"

// cfxEffect
class cfxEffect : public cfxAnnotatable, public cfxParamable
{
public:

  cfxEffect(const std::string& _name, CGcontext _context); 
  ~cfxEffect();

  cfxBool apply();
  cfxBool validate() const;
  CGcontext getContext() const;
  CGeffect getEffect() const;
  const std::string& getName() const;

  void pushCode(cfxCode* code);
  void pushTechnique(cfxTechnique* technique);

  void setPassState(unsigned int techniqueIndex, unsigned int passIndex);
  void resetPassState(unsigned int techniqueIndex, unsigned int passIndex);
  unsigned int getTechniqueCount();
  unsigned int getPassCount(unsigned int techniqueIndex);

  // this is temporary until cg api gets upgraded to set effect parameter names
  void addNamedParameter(const std::string& name, CGparameter param);
  CGparameter getParameterByName(const std::string& name);

  void addNamedSurface(const std::string& name, cfxSurface* surface);
  cfxSurface* getSurfaceByName(const std::string& name);
  std::string getSurfaceName( const cfxSurface *surface );

  // this is temporary until cg api gets upgraded to set parameter semantics
  void addSemanticParameter(const std::string& semantic, CGparameter param);
  CGparameter getParameterBySemantic(const std::string& semantic);

  const std::vector<cfxTechnique*> &getTechniqueArray() const;

  const std::map<std::string, cfxSurface*> &getSurfaceMap() const;

private:

  std::vector<cfxCode*> codeArray;  // min 0
  std::vector<cfxTechnique*> techniqueArray;  // min 1

  std::string name;

  CGcontext context;
  CGeffect effect;

  // this is temporary until cg api gets upgraded to set effect parameter names
  std::map<std::string, CGparameter> mapNameToParameter;
  std::map<std::string, cfxSurface*> mapNameToSurfacePtr;
  // and semantics !?!
  std::map<std::string, CGparameter> mapSemanticToParameter;

};


#endif // _CFX_EFFECT_H

