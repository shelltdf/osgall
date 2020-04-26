/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CFX_ANNOTATABLE_H
#define _CFX_ANNOTATABLE_H


#include <string>
#include <vector>


class cfxAnnotateData;
class cfxAnnotate;



// cfxAnnotatable
class cfxAnnotatable
{
public:

  cfxAnnotatable();
  virtual ~cfxAnnotatable();
 
  void pushAnnotate(cfxAnnotate* annotate);

  const std::vector<cfxAnnotate*> &getAnnotateArray() const;

 protected:

  std::vector<cfxAnnotate*> annotateArray; // min 0

};


#endif // _CFX_ANNOTATABLE_H
