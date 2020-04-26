/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_CODE_H
#define _CFX_CODE_H

#include <string>

class cfxEffect;

// cfxCode 
class cfxCode
{
public:

  cfxCode(cfxEffect* _effect, const std::string& _name, 
	  const std::string& _profile, const std::string& _code);
  ~cfxCode() {};

  bool apply();
  bool validate() const;


private:

  cfxEffect* effect;

  std::string name;
  std::string profile;
  std::string code;

  //CGprogram program;
};



#endif // _CFX_CODE_H
