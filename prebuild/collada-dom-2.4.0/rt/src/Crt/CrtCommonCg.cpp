/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtCg.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtPlatform.h"
#include "Crt/CrtScene.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtTargaLoader.h"

#define USE_CG

#ifndef USE_CG

CrtBool CrtRender::InitCg(){ return CrtTrue; };
CrtBool CrtRender::DestroyCg(){ return CrtTrue; };
CrtVoid CrtPolyGroup::RenderCg(){};
CrtVoid	CrtPolyGroup::RenderSkinnedCg( CrtVec3f * bindPos, CrtVec3f * bindNorms, CrtJoint * joints, CrtInt nbrBindPoints, CrtInt nbrJoints ){};
CrtVoid	CrtPolyGroup::BuildTangents(){}; 

#else // USE_CG

#ifdef _WIN32 

#pragma comment( lib, "cg.lib" )										// Search For Cg.lib While Linking
#pragma comment( lib, "cggl.lib" )										// Search For CgGL.lib While Linking

#endif 

CrtBool		CrtInitCg();
CrtInt		NumCgPrograms = 0;
CGprogram	cgPrograms[MAX_NBR_PROGRAMS];
CGprofile	cgVertexProfile, cgFragmentProfile;
CGcontext	cgContext;  // !!!GAC might want to make this part of CrtRender, though that would make render dependent on cg

// skin vertex program parameters 
CGparameter	skinVertexModelViewProjMatrix; 
CGparameter	skinLightModelViewProjMatrix; 
CGparameter skinVertexSkinMatrixStack;

// static vertex program parameters 
CGparameter	staticVertexModelViewMatrix; 
CGparameter staticVertexModelViewProjMatrix; 
CGparameter staticVertexLightViewProjMatrix; 

// phong fragment vertex program parameters 
CGparameter fragmentGlobalAmbient;
CGparameter fragmentLightColor;
CGparameter fragmentLightPosition;
CGparameter fragmentEyePosition;
CGparameter fragmentMtrlAmbient;
CGparameter fragmentMtrlDiffuse;
CGparameter fragmentMtrlSpecular;
CGparameter fragmentShininess;

// shadow fragment program 
CGparameter shadowFragmentLookupMap;
CGparameter	shadowFragmentShadowMap;

GLuint ShadowLookupTexId = 0;

// static normalmap program parameters 
CGparameter staticNormalLightPosition; 
CGparameter staticNormalEyePosition; 
CGparameter staticNormalModelViewProj; 

// fragment normalmap program parameters 
CGparameter fragmentNormalMap;
CGparameter fragmentNormalizeCubeMap; 
CGparameter fragmentNormalMapMtrlAmbient;
CGparameter fragmentNormalMapGlobalAmbient;

GLuint	FragmentNormalMapId = 0; 
GLuint	FragmentNormalizeCubeMapId = 0; 

void callback()
{
	CGerror err;
	printf("%s", cgGetLastErrorString(&err));
}

CrtBool	CrtCheckForCgError()
{
	CGerror Error = cgGetError();

	if ( Error != CG_NO_ERROR )
	{    
		// Show A Message Box Explaining What Went Wrong
		CrtPrint( "**** Cg Error %s ***** \n", (char *)cgGetErrorString(Error));
		MessageBox(NULL, (char *)cgGetErrorString(Error), "Error", MB_OK);
		CrtPrint( "**** Switching to Fixed Function OpenGL ***** \n" );

		MessageBox(NULL, "Switching to Fixed Function OpenGL \n", "Error", MB_OK ); 
		_CrtRender.DisableCgProfiles();
		_CrtRender.DisableCg(); 

		return CrtFalse; 
	}
	return CrtTrue;
}

/*
CrtVoid	CrtPolyGroup::BuildSkinDataCg( )
{
	// allocate for SkinWeightsCg and SkinIndicesCg
	SkinIndicesCg = CrtNewData( CrtVec4f, count * 3 );
	SkinWeightsCg = CrtNewData( CrtVec4f, count * 3 ); 

	// need to sort weights from biggest to smallest 
	// this will only be done once and make sure 
	// we have no more than 4 weights and only the largest 
	// weights 
	

	// go through weight list and make the max 4
	// and normalize the weights and pack in the the 
	// new float structs 

	CrtFloat normalFactor = 0; 

	CrtInt numOverMaxInfluences = 0; 
	for(CrtUInt v = 0;v < count * 3; v++)
	{
		normalFactor = 0;
		// max the number of influences to 4
        for (CrtUInt i = 0; i < 4; i++)
		{
			// total up the influence as we copy the data 
			if ( i < Weights[v].NumInfluences )				
			{	
				normalFactor += Weights[v].Weights[i];
				SkinWeightsCg[v].Set(i,Weights[v].Weights[i]); 
				SkinIndicesCg[v].Set(i,(CrtFloat)Weights[v].Joints[i]);
			}
			else 
			{
				// init to zero for remainders 
				SkinWeightsCg[v].Set(i,0);
				SkinIndicesCg[v].Set(i,0);
			}

			
		}

		//CrtAssert("Number of Weight Influences > 4", Weights[v].NumInfluences <= 4 ); 
		//if ( Weights[v].NumInfluences > 4 )
		{
			numOverMaxInfluences ++;

			normalFactor = 1.0f/ normalFactor; 

			CrtFloat epsilon = .0002f; 

			CrtFloat newFactor = 0; 

			if ( normalFactor < 1.0f - epsilon || normalFactor > 1.0f + epsilon )
			{
				// time to normalize the influences we have 
				for( int n = 0; n < 4; n ++ )
				{
					if ( n < 4 - 1 && Weights[v].NumInfluences > 4 )
					{
						// just a little sanity check here 
						CrtAssert("CrtRenderCg.cpp Weight Indices werern't sorted properly \n", 
							SkinWeightsCg[v].Get(n) >= SkinWeightsCg[v].Get(n+1)) ;
					}

					SkinWeightsCg[v].Set( n, SkinWeightsCg[v].Get(n) * normalFactor ); 
					newFactor += SkinWeightsCg[v].Get(n);
				}
				CrtAssert("Assertion Failed for Weight renormalization in CrtRenderCg.cpp \n", 
					newFactor < 1.0f + epsilon && newFactor > 1.0f - epsilon );
			}
		}
	}
	
	if ( _CrtRender.UsingVBOs() )
		InitVBOs();

	DataReadyForCg = CrtTrue; 
}
CrtVoid	CrtPolyGroup::BuildTangents()
{
	CrtVec3f *tan1 = CrtNewData( CrtVec3f, (count * 3) * 2 );
	CrtVec3f *tan2 = tan1 + count * 3;

	Tangents = CrtNewData( CrtVec3f, count * 3 ); 
	memset( tan1,0, count * 3 * sizeof(CrtVec3f ) * 2 ); 

	// if no tex coord can't do normal mapping 
    if ( !TexCoords[0] )
	{	
		_CrtRender.SetUsingNormalMaps( CrtFalse ); 
		return; 
	}

	for (CrtUInt a = 0; a < count; a++)
	{

		CrtInt i1 = (a * 3);
		CrtInt i2 = (a * 3)+1;
		CrtInt i3 = (a * 3)+2;

		const CrtVec3f& v1 = Points[i1];
		const CrtVec3f& v2 = Points[i2];
		const CrtVec3f& v3 = Points[i3];

		const CrtVec2f& w1 = TexCoords[0][i1];
		const CrtVec2f& w2 = TexCoords[0][i2];
		const CrtVec2f& w3 = TexCoords[0][i3];

		CrtFloat x1 = v2.x - v1.x;
		CrtFloat x2 = v3.x - v1.x;
		CrtFloat y1 = v2.y - v1.y;
		CrtFloat y2 = v3.y - v1.y;
		CrtFloat z1 = v2.z - v1.z;
		CrtFloat z2 = v3.z - v1.z;

		CrtFloat s1 = w2.x - w1.x;
		CrtFloat s2 = w3.x - w1.x;
		CrtFloat t1 = w2.y - w1.y;
		CrtFloat t2 = w3.y - w1.y;

		CrtFloat r = 1.0F / (s1 * t2 - s2 * t1);
		CrtVec3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
		(t2 * z1 - t1 * z2) * r);
		CrtVec3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
		(s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;

	}

//	CrtUInt count = count * 3;
	for (CrtUInt a = 0; a < count * 3; a++)
	{
		const CrtVec3f& n = Normals[a];
		const CrtVec3f& t = tan1[a];

		// Gram-Schmidt orthogonalize
		Tangents[a] = (t - n * (n * t)).Normalize();
		Tangents[a].Normalize(); 
	}
	CrtDeleteData( tan1 );	
}
*/

#if 0 // !!!GAC Save this block of code for reference, contains old Cg based shadow and normal map rendering setup
CrtVoid	CrtPolyGroup::RenderCg()
{
	// init extentions if pc 
	CrtInitExtentions();

	//CrtPrint("Render Cg" ); 

	// enable cg profiles 
	_CrtRender.EnableCgProfiles();

	

	if	(Points == 0 && !_CrtRender.UsingVBOs() )
		return;

	
	// for now normalmaps and shadowmaps usage is 
	// mutually exclusive 
	if ( _CrtRender.UsingNormalMaps() )
	{
		
		// need to set the tangents in the texcoord pointer lists 
		if ( _CrtRender.UsingVBOs() && VboIds[eGeoTangents] != 0 )
		{
			// set the weights in texcoord 1 
			glClientActiveTexture(GL_TEXTURE1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			_CrtRender.BindVBO( VboIds[eGeoTangents] ); 
			glTexCoordPointer( 3, GL_FLOAT, 0, NULL );

			//GLenum glError = glGetError();
			//CrtAssert("GL Polygon Render Error", glError == 0 );

			// reset active texture to 0 
			glClientActiveTexture(GL_TEXTURE0);
			
			//glError = glGetError();
			//CrtAssert("GL Polygon Render Error", glError == 0 );
		}
		else
		{
			// set the weights in texcoord 1 
			glClientActiveTexture(GL_TEXTURE1);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer( 3, GL_FLOAT, 0, Tangents );

			//GLenum glError = glGetError();
			//CrtAssert("GL Polygon Render Error", glError == 0 );

			// reset active texture to 0 
			glClientActiveTexture(GL_TEXTURE0);
			
			//glError = glGetError();
			//CrtAssert("GL Polygon Render Error", glError == 0 );
		}

		// set using 
		_CrtRender.SetCgNormalMapStaticProgram();
		_CrtRender.SetCgNormalMapFragmentProgram(); 
	}
	else
	if ( _CrtRender.UsingShadowMaps() )
	{
		
		if ( _CrtRender.GetRenderingToShadowMap())
		{
			_CrtRender.SetCgDefaultFragmentProgram();
			_CrtRender.SetCgDefaultStaticProgram();
		}
		else
		if ( _CrtRender.GetRenderingWithShadowMap() )
		{ 
			// push on to the stack our light view projection matrix 
			// from our current light 
			_CrtRender.PushLightViewMatrix(); 

			// need to setup the vertex light view projection 
			// matrix for the shadowing pass in each vertex program 
			cgGLSetStateMatrixParameter( staticVertexLightViewProjMatrix, 
					 CG_GL_MODELVIEW_PROJECTION_MATRIX,
					CG_GL_MATRIX_IDENTITY ); 

			// pops it off the stack 
			_CrtRender.PopLightViewMatrix(); 

			_CrtRender.SetCgDefaultStaticProgram();
            // set the special shadowing fragment shader			
			_CrtRender.SetCgShadowMapFragmentProgram();
		}
	}
	else
	{
		
		// business as usuall 
        _CrtRender.SetCgDefaultFragmentProgram();
		_CrtRender.SetCgDefaultStaticProgram();
	}

	
	
	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return;



	// set the standard stuff and call draw arrays 
	Render(); 


	if ( _CrtRender.UsingShadowMaps())
	{
		cgGLDisableTextureParameter(shadowFragmentShadowMap);
		cgGLDisableTextureParameter(shadowFragmentLookupMap );
	}
	
	_CrtRender.DisableCgProfiles();

	if ( _CrtRender.GetRenderDebug() )
		RenderDebug(); 
};





CrtVoid	CrtPolyGroup::RenderSkinnedCg( CrtVec3f * bindPos, CrtVec3f * bindNorms, CrtJoint * joints, CrtInt nbrBindPoints, CrtInt nbrJoints )
{
	// just to get rid of the warnings 
	(void)bindPos;
	(void)bindNorms;
	(void)joints;
	(void)nbrBindPoints; 	

	// if pc init extentions if not already 
	CrtInitExtentions();

	// if first time through.. need to build weight data for the skinning program. 
	if ( !DataReadyForCg )
		BuildSkinDataCg(); 

	// Check for a Cg Error, If So switch to FixedFunction
	if ( ! CrtCheckForCgError() )
		return;

	// enable cg profiles 
	_CrtRender.EnableCgProfiles();

   //printf("Set Static Program CgStuff \n" ); 
	if ( _CrtRender.UsingShadowMaps() )
	{
		if ( _CrtRender.GetRenderingToShadowMap())
		{
			 _CrtRender.SetCgDefaultFragmentProgram();
		}
		else
		if ( _CrtRender.GetRenderingWithShadowMap() )
		{ 
			  // push on to the stack our light view projection matrix 
			  // from our current light 
			  _CrtRender.PushLightViewMatrix(); 
			
			  // need to setup the vertex light view projection 
			  // matrix for the shadowing pass in each vertex program 
			  cgGLSetStateMatrixParameter( skinLightModelViewProjMatrix, 
					   CG_GL_MODELVIEW_PROJECTION_MATRIX,
					  CG_GL_MATRIX_IDENTITY ); 
			
			  // pops it off the stack 
			  _CrtRender.PopLightViewMatrix(); 
			
			  // set the special shadowing fragment shader
			  _CrtRender.SetCgShadowMapFragmentProgram(); 	
		}
	}
	else
	{
		// business as usuall 
		_CrtRender.SetCgDefaultFragmentProgram();
	}

	_CrtRender.SetCgDefaultSkinProgram(); 
	
	// Check for a Cg Error, If So switch to FixedFunction
	if ( ! CrtCheckForCgError() )
		return;

	CrtAssert( "TOO MANY JOINTS FOR THE SKIN SHADER ! \n", nbrJoints < MAX_SKIN_JOINTS );

	// need to switch to this 3x4 matrix list 
	cgGLSetParameterArray4f(skinVertexSkinMatrixStack, 0, nbrJoints*3, (const float *)SkinMatrixStack3x4);  

     //CrtVec4f * buildMats = (CrtVec4f *)SkinMatrixStack3x4;
	//for( int i = 0; i < nbrJoints * 3; i++)	
	//	cgGLSetParameter4fv( skinVertexSkinMatrixStack[i],	(float*)&buildMats[i]);  

	if ( ! CrtCheckForCgError() )
		return;

	// need to set the joints and weights in tex coord 1 and 2 
	if ( _CrtRender.UsingVBOs() )
	{
		// set the weights in texcoord 1 
		glClientActiveTexture(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		_CrtRender.BindVBO( VboIds[eGeoSkinWeights] ); 
		glTexCoordPointer( 4, GL_FLOAT, 0, NULL );

		//GLenum glError = glGetError();
		//CrtAssert("GL Polygon Render Error", glError == 0 );

		// set the indices in texcoord 2 
		glClientActiveTexture(GL_TEXTURE2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		_CrtRender.BindVBO( VboIds[eGeoSkinIndices] ); 
		glTexCoordPointer( 4, GL_FLOAT, 0, NULL );
		
		// reset active texture to 0 
		glClientActiveTexture(GL_TEXTURE0);
		
		//glError = glGetError();
		//CrtAssert("GL Polygon Render Error", glError == 0 );
	}
	else
	{
		// set the weights in texcoord 1 
		glClientActiveTexture(GL_TEXTURE1);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 4, GL_FLOAT, 0, SkinWeightsCg );

		GLenum glError = glGetError();
		CrtAssert("GL Polygon Render Error", glError == 0 );

		// set the indices in texcoord 2 
		glClientActiveTexture(GL_TEXTURE2);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 4, GL_FLOAT, 0, SkinIndicesCg );
		
		// reset active texture to 0 
		glClientActiveTexture(GL_TEXTURE0);
		
		glError = glGetError();
		CrtAssert("GL Polygon Render Error", glError == 0 );
	}

	if	(Points == 0 && !_CrtRender.UsingVBOs() )
		return;

	Render(); 

	if ( _CrtRender.UsingShadowMaps())
	{
		cgGLDisableTextureParameter(shadowFragmentShadowMap);
		cgGLDisableTextureParameter(shadowFragmentLookupMap );
	}

	_CrtRender.DisableCgProfiles(); 

};
#endif

#define GL_CLAMP_TO_EDGE				0x812F

CrtBool CrtRender::InitCg()
{
	if ( CgInitialized ) 
		return CrtTrue; 

	cgSetErrorCallback(callback);

	// Initalize the Cg Context 
	CrtInitCg(); 
	
	SetupCgDefaultStaticProgram();
	SetupCgDefaultSkinProgram(); 
	SetupCgDefaultFragmentProgram(); 
	SetupCgNormalMapStaticProgram(); 
	SetupCgNormalMapSkinProgram(); 
	SetupCgNormalMapFragmentProgram();
	
	//SetupCgShadowMapFragmentProgram();
	//SetupCgShadowMapStaticProgram();
	//SetupCgShadowMapSkinProgram();
	

	CgInitialized = CrtTrue; 
	return CrtTrue; 
}

// -------------------
// cg setup programs 
// -------------------

CrtBool	CrtRender::SetupCgDefaultStaticProgram()
{
	// load the static vertex shader 
	CgStaticVertexProgramId		= LoadCgProgram( "staticVertex.cg", CrtVertexProgram );
	if ( CgStaticVertexProgramId == -1 )
		return CrtFalse;

	// Get Static Vertex Parameters 
	staticVertexModelViewProjMatrix	= cgGetNamedParameter( cgPrograms[CgStaticVertexProgramId], "modelViewProj");
	staticVertexLightViewProjMatrix = cgGetNamedParameter( cgPrograms[CgStaticVertexProgramId], "lightViewProj");
	staticVertexModelViewMatrix = cgGetNamedParameter( cgPrograms[CgStaticVertexProgramId], "modelView" ); 

	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;

	return CrtTrue;
}

CrtBool	CrtRender::SetupCgDefaultSkinProgram()
{
	// load the default vertex skin shader 
   	CgSkinVertexProgramId		= LoadCgProgram( "skin4mVertex.cg", CrtVertexProgram ); 
	if ( CgSkinVertexProgramId == -1 )
		return CrtFalse;

	// Get Skin Vertex Parameters 
	skinVertexModelViewProjMatrix	= cgGetNamedParameter( cgPrograms[CgSkinVertexProgramId], "modelViewProj");   
	skinLightModelViewProjMatrix 	= cgGetNamedParameter( cgPrograms[CgSkinVertexProgramId], "lightViewProj");   
	skinVertexSkinMatrixStack	= cgGetNamedParameter( cgPrograms[CgSkinVertexProgramId], "boneMats"); 

	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;
	return CrtTrue; 
}

CrtBool	CrtRender::SetupCgDefaultFragmentProgram()
{
	// load the default phong fragment program 
	CgPhongFragmentProgramId	= LoadCgProgram( "phongFragment.cg", CrtFragmentProgram ); 

	if ( CgPhongFragmentProgramId == -1 )
		return CrtFalse; 

	// Get Fragment Parameters 
	fragmentGlobalAmbient	= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "globalAmbient");
	fragmentLightColor		= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "lightColor");		
	fragmentLightPosition	= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "lightPosition");
	fragmentEyePosition		= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "eyePosition");
	fragmentMtrlAmbient		= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "mtrlAmbient");
	fragmentMtrlDiffuse		= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "mtrlDiffuse");
	fragmentMtrlSpecular	= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "mtrlSpecular");
	fragmentShininess		= cgGetNamedParameter( cgPrograms[CgPhongFragmentProgramId], "shininess");

	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;

	return CrtTrue; 
}

CrtBool	CrtRender::SetupCgShadowMapFragmentProgram()
{
	// add any other program loading here 
	// load the default Shadow fragment shader here 
	CgPhongFragmentShadowProgramId	= LoadCgProgram( "shadowFragment.cg", CrtFragmentProgram ); 
	if ( CgPhongFragmentShadowProgramId == -1 )
		return CrtFalse;

		// get the look map param handle 
	shadowFragmentLookupMap = cgGetNamedParameter( cgPrograms[CgPhongFragmentShadowProgramId], "LookupMap");
	CrtPrint("sampler : got %p\n", shadowFragmentLookupMap);
	//CrtPrint("LookupMap on unit %x\n", cgGLGetTextureEnum(shadowFragmentLookup) - GL_TEXTURE0);
	
	// get the shadow map param handle 
	shadowFragmentShadowMap = cgGetNamedParameter( cgPrograms[CgPhongFragmentShadowProgramId], "shadow");
	CrtPrint("fShadow : got %p\n", shadowFragmentShadowMap);
	//CrtPrint("shadowmap on unit %x\n", cgGLGetTextureEnum(shadowFragmentShadow) -  GL_TEXTURE0);
	
	// load the lookup jitter texture 
	CrtTgaFile * tga = CrtLoadTargaFromFile( "examples/Aniso2.tga" );

	if ( tga )
	{
		glGenTextures(1, &ShadowLookupTexId);
		glBindTexture(GL_TEXTURE_2D, ShadowLookupTexId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if ( tga && tga->BBP == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tga->Header.ImageWidth, 
			tga->Header.ImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tga->RGBAData);
		else 
			if ( tga && tga->BBP == 24 )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
				tga->Header.ImageWidth, tga->Header.ImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tga->RGBData);
	}	
	else
	{
		CrtPrint("Missing Lookup Texture Aniso.tga for shadow mapping lookup \n");	
		return CrtFalse;
	}
	
	//CrtDeleteData(data);
	cgGLSetTextureParameter(shadowFragmentLookupMap, ShadowLookupTexId);

	
	printf(" --------------------------------------\n "); 

	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;
	return CrtTrue; 
}

CrtBool	CrtRender::SetupCgNormalMapStaticProgram()
{
	CgStaticNormalMapId = LoadCgProgram( "normalMapVertex.cg", CrtVertexProgram );
	//CgStaticNormalMapId = LoadCgProgram( "NormalTest/bumpSimple.cg", CrtVertexProgram );
	// get handles to the params 
	staticNormalLightPosition = cgGetNamedParameter( cgPrograms[CgStaticNormalMapId], "lightPosition"); 
	staticNormalEyePosition   = cgGetNamedParameter( cgPrograms[CgStaticNormalMapId], "eyePosition");  
	staticNormalModelViewProj = cgGetNamedParameter( cgPrograms[CgStaticNormalMapId], "modelViewProj");

	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;
	return CrtTrue;
}

CrtBool	CrtRender::SetupCgNormalMapFragmentProgram()
{
	CgFragmentNormalMapId = LoadCgProgram( "normalMapFragment.cg", CrtFragmentProgram );  
	// get handles to the params 
	fragmentNormalMap		= cgGetNamedParameter( cgPrograms[CgFragmentNormalMapId], "normalMap"); 
	fragmentNormalizeCubeMap	= cgGetNamedParameter( cgPrograms[CgFragmentNormalMapId], "normalizeCube");
	fragmentNormalMapMtrlAmbient	= cgGetNamedParameter( cgPrograms[CgFragmentNormalMapId], "mtrlAmbient"); 
	fragmentNormalMapGlobalAmbient	= cgGetNamedParameter( cgPrograms[CgFragmentNormalMapId], "globalAmbient"); 	
	//cgGLSetManageTextureParameters(cgContext,1);

	// load normal map 
	// load normal map 
	CrtChar detailNormalMap[CRT_MAX_NAME_SIZE];
	sprintf(detailNormalMap, "%sSeymourPlane140AnimLight_Textures/planeNormalMap.tga", TextureFilePrefix ); 
	CrtTgaFile * tga = CrtLoadTargaFromFile( detailNormalMap );

	if ( tga ) 
	{
		glGenTextures(1, &FragmentNormalMapId);
		glBindTexture(GL_TEXTURE_2D, FragmentNormalMapId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		if (tga->BBP == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
			tga->Header.ImageWidth, tga->Header.ImageHeight, 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, tga->RGBAData);
	
		if (tga->BBP == 24)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
			tga->Header.ImageWidth, tga->Header.ImageHeight, 0, 
			GL_RGB, GL_UNSIGNED_BYTE, 
			tga->RGBData);
	}
	else
	{
		CrtPrint("Normal map planeNormalMap.tga isn't the 24 or 32 bit or was not found \n" );	
		return CrtFalse;
	}
	cgGLSetTextureParameter(fragmentNormalMap, FragmentNormalMapId);
	
	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;
	return CrtTrue; 
}

CrtBool	CrtRender::SetupCgNormalMapSkinProgram()
{
	CgSkin4mNormalMapId = -1; 
	// Check for a Cg Error, If So switch to FixedFunction
	if ( !CrtCheckForCgError() )
		return CrtFalse;
	return CrtTrue; 
}



CrtBool CrtRender::SetCgDefaultStaticProgram()
{
	if ( CgInitialized )
	{
		//printf(" Setting Cg Static Program \n" ); 

		// Bind Our Vertex Program To The Current State
		cgGLBindProgram(cgPrograms[CgStaticVertexProgramId]);
                
		// Set The Modelview Matrix Of Our Shader To Our OpenGL Modelview Matrix
		cgGLSetStateMatrixParameter(staticVertexModelViewProjMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		//cgGLSetStateMatrixParameter(staticVertexModelViewMatrix, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
		cgGLSetMatrixParameterfc( staticVertexModelViewMatrix, (const float *)GetCurrentLMMat() );  
				
		return CrtTrue; 
	}

	return CrtFalse; 
}

CrtBool CrtRender::SetCgDefaultSkinProgram()
{

	if ( CgInitialized )
	{
		//printf(" Setting Cg Skin Program \n" ); 

		// Bind Our Vertex Program To The Current State
		cgGLBindProgram(cgPrograms[CgSkinVertexProgramId]);
		
		// Set The Modelview Matrix Of Our Shader To Our OpenGL Modelview Matrix
		cgGLSetStateMatrixParameter(skinVertexModelViewProjMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		return CrtTrue; 
	}

	return CrtFalse; 
}

CrtBool CrtRender::SetCgDefaultFragmentProgram()
{
	if ( CgInitialized )
	{
		//CrtPrint(" Setting Cg Phong Program \n" ); 
		// Bind out Fragment Program to the Current State 
		cgGLBindProgram(cgPrograms[CgPhongFragmentProgramId]);

		// !!!GAC This code is setup to only handle one light, so get the first instance from the scene
		assert(Scene->GetLightInstanceCount());  // There should always be one
		CrtInstanceLight *instanceLight = Scene->GetLightInstances(0);

		// Get the light position from the light instance's parent node's LocalToWorldMatrix
		// REFACTOR IN PROGRESS we should have an accessor for getting the position
		float *lightMatrix = (float *)instanceLight->Parent->GetLocalToWorldMatrix();
		CrtVec3f lightPosition(
			lightMatrix[12],
			lightMatrix[13],
			lightMatrix[14]); 

		// Get Fragment Parameters 
		CrtMaterial *material = _CrtRender.GetCurrentMaterial();
		CrtInstanceCamera *instanceCamera = _CrtRender.GetActiveInstanceCamera();
		
		assert(instanceCamera);  // There should always be a camera instance

		// Get the eye position from the camera instance's parent node's LocalToWorldMatrix
		// REFACTOR IN PROGRESS we should have an accessor for getting the eye position
		float *matrix = (float *)instanceCamera->Parent->GetLocalToWorldMatrix();
			CrtVec3f eyePosition(
			matrix[12],
			matrix[13],
			matrix[14]); 

		// Get the material parameters, establish some defaults for things without materials
		// !!!GAC maybe crtSceneRead should make a default material and bind polys without materials to it?
		CrtColor3f	mtrlAmbientColor(0.0f, 0.0f, 0.0f);  
		CrtColor4f	mtrlDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		CrtColor4f	mtrlSpecularColor(0.5f, 0.5f, 0.5f, 1.0f); 
		CrtFloat	mtrlShininess = 5.0f; 

		// Override defaults if there is a material.
		if ( material )
		{
			mtrlAmbientColor	= CrtColor3f( material->GetAmbient() );  
			mtrlDiffuseColor	= CrtColor4f( material->GetDiffuse() );
			mtrlSpecularColor	= CrtColor4f( material->GetSpecular() ); 
			mtrlShininess		= material->GetShininess(); 
		}
		
		
		CrtColor4f	lightColor = instanceLight->AbstractLight->GetColor();
		
		// Set the light and material information in the shader
		cgGLSetParameter3fv( fragmentEyePosition,	(float*)&eyePosition);
		// mtrl info 
		cgGLSetParameter3fv( fragmentMtrlAmbient,	(float*)&mtrlAmbientColor); 
		cgGLSetParameter3fv( fragmentMtrlDiffuse,	(float*)&mtrlDiffuseColor);   
		cgGLSetParameter3fv( fragmentMtrlSpecular,  (float*)&mtrlSpecularColor);
		cgGLSetParameter1f(  fragmentShininess,		mtrlShininess ); 
		// light info 		
		cgGLSetParameter3fv( fragmentLightPosition,	(float*)&lightPosition); 
		cgGLSetParameter3fv( fragmentLightColor,	(float*)&lightColor ); 
		// global info 
		CrtColor4f globalAmbient= CrtColor4f( .2f,.2f,.2f,.2f ); 
		cgGLSetParameter3fv( fragmentGlobalAmbient,	(float*)&globalAmbient ); 

		
		if ( !CrtCheckForCgError() ) 
			return CrtFalse;
		
		return CrtTrue; 
	}
	return CrtFalse; 
}

CrtBool	CrtRender::SetCgShadowMapFragmentProgram()
{
	if ( CgInitialized )
	{
		//printf(" Setting Cg Fragment Shadow Program \n" ); 

        // set the shadow map parameter 
		cgGLSetTextureParameter(shadowFragmentShadowMap, ShadowMapId );	
		cgGLEnableTextureParameter(shadowFragmentLookupMap );
        cgGLEnableTextureParameter(shadowFragmentShadowMap);

		// Bind shadow Fragment Program to the Current State 
		cgGLBindProgram(cgPrograms[CgPhongFragmentShadowProgramId]);
		
	}
	return CrtTrue; 	
};

// cg bind programs and perrender setup 
CrtBool	CrtRender::SetCgNormalMapStaticProgram()
{
	if ( CgInitialized )
	{
		//printf(" Setting Cg Fragment Shadow Program \n" ); 
		CrtInstanceCamera *instanceCamera = _CrtRender.GetActiveInstanceCamera();

		// !!!GAC This code is setup to only handle one light, so get the first instance from the scene
		assert(Scene->GetLightInstanceCount());  // There should always be one
		CrtInstanceLight *instanceLight = Scene->GetLightInstances(0);

		// Get the light position from the light instance's parent node's LocalToWorldMatrix
		// REFACTOR IN PROGRESS we should have an accessor for getting the position
		float *lightMatrix = (float *)instanceLight->Parent->GetLocalToWorldMatrix();
		CrtVec3f lightPosition(
			lightMatrix[12],
			lightMatrix[13],
			lightMatrix[14]); 
		
		// Get the eye position from the camera instance's parent node's LocalToWorldMatrix
		// REFACTOR IN PROGRESS we should have an accessor for getting the eye position
		float *matrix = (float *)instanceCamera->Parent->GetLocalToWorldMatrix();
			CrtVec3f eyePosition(
			matrix[12],
			matrix[13],
			matrix[14]); 
		
	    // set the shadow map parameter 
		cgGLSetParameter3fv(staticNormalLightPosition, (float *)&lightPosition );	
		cgGLSetParameter3fv(staticNormalEyePosition, (float *)&eyePosition );	
		cgGLSetStateMatrixParameter(staticNormalModelViewProj, 
			CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
			
		// Bind shadow Fragment Program to the Current State 
		cgGLBindProgram(cgPrograms[CgStaticNormalMapId]);
	}

	return CrtTrue; 
}

CrtBool	CrtRender::SetCgNormalMapFragmentProgram()
{
	if ( CgInitialized )
	{
		
		//cgGLSetTextureParameter(fragmentNormalMap, FragmentNormalMapId);
		//cgGLEnableTextureParameter(fragmentNormalMap );
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, FragmentNormalMapId);

		// Get Fragment Parameters 
		CrtMaterial *mat = _CrtRender.GetCurrentMaterial(); 
		CrtColor3f	mtrlAmbientColor;  
		
		if ( mat )
		{
			mtrlAmbientColor = CrtColor3f( mat->GetAmbient() );  			
		}
		
		cgGLSetParameter3fv( fragmentNormalMapMtrlAmbient, (float*)&mtrlAmbientColor); 
		// global info 
		CrtColor4f globalAmbient= CrtColor4f( .2f,.2f,.2f,.2f ); 
		cgGLSetParameter3fv( fragmentNormalMapGlobalAmbient,	(float*)&globalAmbient ); 

		// Bind normal fragment Program to the Current State 
		cgGLBindProgram(cgPrograms[CgFragmentNormalMapId]);
	}
	return CrtTrue; 
}

CrtBool CrtRender::EnableCgProfiles()
{
	if ( CgInitialized )
	{	
		// enable the fragment profile 
		cgGLEnableProfile(cgVertexProfile);	
		cgGLEnableProfile(cgFragmentProfile); 
	}
	return CrtFalse; 
}

CrtBool  CrtRender::DisableCgProfiles()
{
	if ( CgInitialized )
	{
		// Disable Our Vertex Profile
		cgGLDisableProfile(cgVertexProfile);	
		cgGLDisableProfile(cgFragmentProfile);	
		return CrtTrue; 
	}

	return CrtFalse;
}

CrtBool	CrtRender::DestroyCg()
{
	// Destroy Our Cg Context And All Programs Contained Within It
	if (cgContext != NULL)
	{
		cgDestroyContext(cgContext);
		cgContext = NULL;
		return CrtTrue; 
	}
	return CrtFalse;
}

#endif // USE_CG

