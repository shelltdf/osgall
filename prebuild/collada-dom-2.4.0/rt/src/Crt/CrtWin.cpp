/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtCg.h"
#include "Crt/CrtPlatform.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtData.h"

CrtChar ShaderFilePrefix[CRT_MAX_URI_SIZE] = "shaders/advanced/"; 
CrtChar TextureFilePrefix[CRT_MAX_URI_SIZE]= "images/";
CrtChar BasePath[CRT_MAX_URI_SIZE] = ""; // not needed for windows 

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
	// Create a context for the CG programs we are going to load and validate it was successful
	CrtPrint("Creating CG context\n");
	cgContext = cgCreateContext();
	if (cgContext == NULL)
	{
		// BLahhh exit here 
		MessageBox(NULL, "Failed To Create Cg Context", "Error", MB_OK);
		return CrtFalse;													
	}

//	cgSetAutoCompile(cgContext, CG_COMPILE_MANUAL);

	// Register GL states (ugly crashes if you don't do this)
	cgGLRegisterStates(cgContext);

	// Get The Latest GL Vertex Profile
	cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);				
	cgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    
	// Validate Our Profile Determination Was Successful
	if (cgVertexProfile == CG_PROFILE_UNKNOWN || cgFragmentProfile == CG_PROFILE_UNKNOWN)
	{
		MessageBox(NULL, "Invalid profile type", "Error", MB_OK);
		return CrtFalse;			
	}

	// Set The Current Profile
	cgGLSetOptimalOptions(cgVertexProfile);
	cgGLSetOptimalOptions(cgFragmentProfile);

	// Check for errors
	if(!CrtCheckForCgError())
		return CrtFalse;

	return CrtTrue; 
}
CrtFloat CrtRender::GetTime()
{
	return timeGetTime() * .001f; 	
}

CrtInt CrtRender::LoadCgProgram( CrtChar * fileName, const CrtInt programType )
{
	CrtChar fullFileName[CRT_MAX_NAME_SIZE]; 
	
	sprintf(fullFileName, "%s%s", ShaderFilePrefix, fileName ); 
	//CrtCpy( fullFileName, fileName ); 

	CrtPrint("CrtRender::LoadCgProgram: Loading %s Shader From File \n", fileName ); 

	// Load And Compile The Vertex Shader From File
	if ( programType == CrtFragmentProgram )
		cgPrograms[NumCgPrograms] = cgCreateProgramFromFile(cgContext, CG_SOURCE, fullFileName, cgFragmentProfile, "main", 0);
	else
		cgPrograms[NumCgPrograms] = cgCreateProgramFromFile(cgContext, CG_SOURCE, fullFileName, cgVertexProfile, "main", 0);

	// Validate Success
	if (cgPrograms[NumCgPrograms] == NULL)
	{
		// Check for a Cg Error, If So switch to FixedFunction
		if ( ! CrtCheckForCgError() )
			return -1;

		// failed to load CgShader 
		CrtChar buff[100];
		sprintf(buff,"CrtRender::LoadCgProgram: Failed Compile CgFile %s \n", fileName ); 
		MessageBox(NULL, buff, "Cg Compile Error", MB_OK );   
		return -1;													
	}

	cgGLLoadProgram(cgPrograms[NumCgPrograms]);	
	NumCgPrograms ++;

	return NumCgPrograms - 1; 
}

PFNGLACTIVETEXTUREARBPROC glActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture = NULL;
CrtVoid	CrtInitExtentions()
{
	// get the arb extention 
	if ( !glClientActiveTexture || !glActiveTexture )
	{
		glActiveTexture = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");		
	}

	if ( !glClientActiveTexture || !glActiveTexture )
	{
		CrtPrint( " Needed Arb Entension was not found \n" ); 
		MessageBox(NULL, "Failed to find Needed ARB Entension!", "Error", MB_OK);
		CrtPrint( " This app will not be rendered with Cg \n" ); 
		MessageBox(NULL, "Switching to Fixed Function OpenGL \n", "Error", MB_OK ); 
		_CrtRender.DisableCgProfiles();
		_CrtRender.DisableCg(); 
		return; 
	}
}


