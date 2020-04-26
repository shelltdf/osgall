/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFXOSX_H
#define _CFXOSX_H

#include <stdio.h>
#include <stdarg.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <OpenGL/glu.h>			
#include <CG/cg.h>
#include <CG/cgGL.h>


#include <assert.h>

#define CG_FILE_TYPE CG_SOURCE

//#define LINUX	1

inline void cfxPrint(const char* str, ...)
{
    va_list	valist;
    char	tempStr[4096];

    va_start(valist, str);
    vsprintf(tempStr, str, valist);
    va_end(valist);

    printf( tempStr );
}

#endif
