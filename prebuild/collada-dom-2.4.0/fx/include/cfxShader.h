/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_SHADER_H
#define _CFX_SHADER_H

#include <string>
#include <cfxAnnotatable.h>
#include <cfxParamable.h>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

class cfxPass;

// cfxShader 
class cfxShader : public cfxAnnotatable, public cfxParamable
{
public:
	enum type_enum {
		VERTEX,
		FRAGMENT
	};
public:

  cfxShader(cfxPass* _pass, const std::string& _source, const std::string& _name, CGprofile _target, type_enum type);
  ~cfxShader();
  bool apply();
  bool validate() const;

  cfxPass* getPass() const;

  CGprogram getProgram() const;

  const std::string &getName() const;
  const std::string &getSource() const;
  const CGprofile &getTarget() const;
  type_enum getOriginalType() const;

private:

  cfxPass* pass;

  std::string source;
  std::string name;

  CGprofile target;
  CGprogram program;
  CGstate state;
  CGstateassignment assignment;

  type_enum originalType;
};

#endif // _CFX_SHADER_H

