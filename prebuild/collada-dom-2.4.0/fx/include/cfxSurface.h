/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_SURFACE_H
#define _CFX_SURFACE_H

#include <string>
#include <map>
#include <vector>

#include <cfxPlatform.h>

class cfxParam;

// cfxSurface 
class cfxSurface
{
public:

  cfxSurface(const std::string& _init_from, const std::string& _format, cfxSurface* parentSurface = NULL);
  ~cfxSurface();

  bool apply();
  bool validate() const;

  const std::string& getInitFrom() const;
  const std::string& getFormat() const;

  void addReferencingParam(cfxParam* param);

  // load images in some other library
  // this must then be called for all loaded images so cfx can access their GL ids
  static void addImage(const std::string& imageId, GLuint texId);
  static void setDefaultTexture(GLuint texId);
  // to retrieve tex ids lookup by image id
  static GLuint getTexIdByImageId(const std::string& imageId);

  const cfxSurface* getParentSurface() const;
  std::vector<cfxParam*> referencingParams;
private:

  void setSurface(const std::string& _init_from, const std::string& _format);

  std::string init_from;
  std::string format;

  cfxSurface* parentSurface;

  static std::map<std::string, GLuint> mapImageIdToGlTextureObjectId;

  static GLuint defaultTexture;

};



#endif // _CFX_SURFACE_H
