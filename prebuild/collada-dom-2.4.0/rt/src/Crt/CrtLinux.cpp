/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include <sys/time.h>

#include "Crt/CrtCg.h"
#include "Crt/CrtPlatform.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtUtils.h"

CrtChar ShaderFilePrefix[CRT_MAX_NAME_SIZE] = "shaders/advanced/"; 
CrtChar TextureFilePrefix[CRT_MAX_NAME_SIZE]= "images/";
CrtChar BasePath[CRT_MAX_NAME_SIZE]= "";

CrtVoid CrtInitPaths()
{
	
	CrtCpy( ShaderFilePrefix, "shaders/advanced/" ); 
	CrtCpy( TextureFilePrefix, "images/" ); 
}
CrtBool CrtExitCg()
{
	CrtPrint("destroying CG context\n");
	_CrtRender.DisableCgProfiles();
	_CrtRender.DestroyCg();
	return CrtFalse;
}
CrtBool CrtInitCg()
{
	CrtPrint("creating CG context\n");
	cgContext = cgCreateContext();
	if(cgContext == NULL)
	{
		printf("Failed to create CG context\n");
		return CrtFalse;
	}

	// Register GL states
	cgGLRegisterStates(cgContext);
	
	// Get the latest GL Vertex Profile
	cgVertexProfile   = cgGLGetLatestProfile(CG_GL_VERTEX);    // Was hardcoded to CG_PROFILE_SCE_VP_TYPEB
	cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);  // Was hardcoded to CG_PROFILE_SCE_FP_TYPEB

	// Make sure the profiles came back valid
	if(cgVertexProfile == CG_PROFILE_UNKNOWN || cgFragmentProfile == CG_PROFILE_UNKNOWN)
	{
		CrtPrint("Invalid profile type returned from cgGLGetLatestProfile\n");
		return CrtFalse;
	}
	
	// Set Optimal Options for this profile 
	cgGLSetOptimalOptions(cgVertexProfile);
	cgGLSetOptimalOptions(cgFragmentProfile);
	
	// Check for Cg errors
	if ( !CrtCheckForCgError() )
		return CrtFalse;

	CrtPrint("CG context created\n");

	return CrtTrue; 
}

CrtFloat CrtRender::GetTime(void)
{
 struct timeval		LTV;
 struct timezone	LTZ;

 if(gettimeofday(&LTV, &LTZ) != -1)
 {
    long long LTime = LTV.tv_sec;

    LTime *= 1000000; 
    LTime += LTV.tv_usec;
    unsigned long time = LTime & 0xFFFFFFFF;
    return((CrtFloat)time/1000000);
 }
 return(0.0);
}

CrtInt CrtRender::LoadCgProgram( CrtChar * fileName, const CrtInt programType )
{
	CrtChar fullFileName[CRT_MAX_NAME_SIZE]; 
	
	CrtCpy( fullFileName, ShaderFilePrefix );
	CrtCat( fullFileName, fileName ); 

	CrtPrint(" Loading %s Shader From Binary \n", fullFileName ); 

	// get the name extention 
	CrtChar * ext = CrtGetExtention( fullFileName );                 
	
	if ( !CrtCmpIn( ext, ".cg" ) && !CrtCmpIn( ext, ".CG" ) )
	{
		CrtPrint("CrtRender::LoadCgProgram: Invalid File Name %s \n", fullFileName ); 
		return -1; 
	}
	// Load And Compile The Vertex Shader From File
	if ( programType == CrtFragmentProgram )
	{	
		CrtCpy( ext, ".cg" ); 
		cgPrograms[NumCgPrograms] = cgCreateProgramFromFile(cgContext, CG_SOURCE, fullFileName, cgFragmentProfile, NULL, NULL);
	}
	else
	{
		CrtCpy( ext, ".cg" ); 
		cgPrograms[NumCgPrograms] = cgCreateProgramFromFile(cgContext, CG_SOURCE, fullFileName, cgVertexProfile, "main", 0);
	}

	// Validate Success
	if (cgPrograms[NumCgPrograms] == NULL)
	{
		// Check for a Cg Error, If So switch to FixedFunction
		if ( ! CrtCheckForCgError() )
			return -1;

		// failed to load CgShader 
		CrtChar buff[CRT_MAX_NAME_SIZE];
		sprintf(buff," Shader Load Failed %s \n", fileName ); 
		return -1;													
	}
	
	CrtPrint(" Shader ID %d \n", cgPrograms[NumCgPrograms] ); 

	cgGLLoadProgram(cgPrograms[NumCgPrograms]);	
	NumCgPrograms ++;

	return NumCgPrograms - 1; 
}

CrtVoid	CrtInitExtentions(){};

