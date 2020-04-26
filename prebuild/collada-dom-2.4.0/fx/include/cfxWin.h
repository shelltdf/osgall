/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_WIN_H_
#define _CFX_WIN_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glu.h>			
//#include <gl/glaux.h>	
//#ifndef _LIB
#include <Cg/cg.h>
#include <Cg/cgGL.h>
//#else
//#include <cfxNoCg.h>
//#endif

#include <assert.h>

#define CG_FILE_TYPE CG_SOURCE

inline void cfxPrint(const char* str, ...)
{
	va_list	valist;
	char		tempStr[4096]; 

	va_start(valist, str);				// LMessage is the last argument before the varargs
	vsprintf(tempStr, str, valist);
	va_end(valist);

	OutputDebugString( tempStr );
}

#endif
