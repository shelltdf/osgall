/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef CRT_OSX
#define CRT_OSX

#include <Crt/CrtTypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

#define GL_GLEXT_PROTOTYPES
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
//#include <OpenGL/glextt.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef APIENTRYP
#define APIENTRYP APIENTRY *
#endif
#ifndef GLAPI
#define GLAPI extern
#endif
	
	
#ifndef GLsizeiptr
typedef long            GLsizeiptr;
#endif 

GLAPI void APIENTRY glGenBuffers (GLsizei, GLuint *);
GLAPI void APIENTRY glBindBuffer (GLenum, GLuint);
GLAPI void APIENTRY glBufferData (GLenum, GLsizeiptr, const GLvoid *, GLenum);
GLAPI void APIENTRY glDeleteBuffers (GLsizei, const GLuint *);

#ifdef __cplusplus
}
#endif


#include <Cg/cg.h>	
#include <Cg/cgGL.h>	


extern CGcontext	cgContext;
extern CGprofile	cgVertexProfile, cgFragmentProfile;

#define	gluPerspectivef gluPerspective
#define	gluLookAtf	gluLookAt

// Multitexture constants
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2

#ifdef WIN32
#ifndef glActiveTexture
extern PFNGLACTIVETEXTUREARBPROC glActiveTexture;
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
#endif

#ifndef glClientActiveTexture
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture;
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);
#endif
#endif

CrtVoid CrtInitPaths();




// Imitate MessageBox
//
#define	MB_OK	0x01

static void MessageBox(char* LSomething, char* LMessage, char* LTypeStr, int LButtons)
{
 (void)LSomething;
 (void)LButtons;
printf("%s() %s %s\n", __FUNCTION__, LTypeStr, LMessage);fflush(stdout);
}


#endif	// CRT_LINUX
