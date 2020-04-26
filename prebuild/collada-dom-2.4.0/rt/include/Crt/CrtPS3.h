/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef CRT_PS3
#define CRT_PS3

#include "Crt/CrtTypes.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

#include <GLES/gl.h>
#include <GLES/glext.h>

//#include <stddef.h>
#include <sys/raw_spu.h>
#include <sys/sys_time.h>

extern CGcontext	cgContext;
extern CGprofile	cgVertexProfile, cgFragmentProfile; 

#define gluLookAt gluLookAtf

CrtVoid CrtInitPaths();
CrtBool CrtInitCg();
CrtBool CrtResetCg();
		   
const int MB_OK = 1; 
inline void MessageBox( void * , char *, char *, int ){};

#endif 
