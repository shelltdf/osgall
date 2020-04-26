/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef CRT_WIN
#define CRT_WIN

#include <Crt/CrtTypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#include <windows.h>
#include <cg\cg.h>	
#include <cg\cggl.h>	

#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glaux.h>

extern CGcontext	cgContext;
extern CGprofile	cgVertexProfile, cgFragmentProfile;

#define	gluPerspectivef gluPerspective
#define	gluLookAtf	gluLookAt

// Multitexture constants
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2

typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);

extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;

CrtVoid CrtInitPaths();

#endif 
