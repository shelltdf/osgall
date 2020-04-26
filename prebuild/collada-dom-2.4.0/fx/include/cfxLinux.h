/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_LINUX_H_
#define _CFX_LINUX_H_

#include <stdio.h>
//#include <sys/param.h>
#include <stdarg.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>			
//#ifndef _LIB
#include <Cg/cg.h>
#include <Cg/cgGL.h>
//#else
//#include <cfxNoCg.h>
//#endif

#include <assert.h>

#define CG_FILE_TYPE CG_SOURCE

#define LINUX	1

inline void cfxPrint(const char* str, ...)
{
    va_list	valist;
    char	tempStr[4096];

    va_start(valist, str);
    vsprintf(tempStr, str, valist);
    va_end(valist);

    printf( tempStr );
//    OutputDebugString( tempStr );
}

#endif
