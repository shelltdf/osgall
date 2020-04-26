/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_CG_H_
#define _CRT_CG_H_ 

#include "Crt/CrtPlatform.h"
#include "Crt/CrtTypes.h"

const int	CrtFragmentProgram = 1;
const int	CrtVertexProgram = 0; 
const int	MAX_SKIN_JOINTS = 80; 
const int	MAX_NBR_PROGRAMS = 10; 

extern CrtChar ShaderFilePrefix[]; 
extern CrtChar TextureFilePrefix[];
extern CrtChar BasePath[];

CrtBool		CrtInitCg();
CrtBool		CrtCheckForCgError();
CrtVoid		CrtInitExtentions();
CrtBool		CrtExitCg();

extern		CrtInt		NumCgPrograms;
extern		CGprogram	cgPrograms[MAX_NBR_PROGRAMS];
extern		CGprofile	cgVertexProfile, cgFragmentProfile;

#endif 
