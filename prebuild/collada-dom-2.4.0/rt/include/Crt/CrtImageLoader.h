/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_MAGICK_h_
#define _CRT_MAGICK_h_

#include "Crt/CrtTypes.h"
#include "Crt/CrtUtils.h"

class CrtImage; 

//CrtBool	CrtLoadImageWithMagick( CrtChar * file, CrtImage * texObj );
CrtBool	CrtLoadImage( CrtChar * file, CrtImage * texObj );

#endif 
