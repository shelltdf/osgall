/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include <stdio.h>
#include "Crt/CrtEffect.h"
#include "Crt/CrtLight.h"
#include "Crt/CrtGeometry.h"
#include "Crt/CrtMaterial.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtPlatform.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtTexture.h"

// VBO Extension Definitions, From glext.h
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_STREAM_DRAW                    0x88E0
#define GL_WRITE_ONLY                     0x88B9

/*
void 	CrtPolyGroup::SetRender()
{
	if (_CrtRender.UsingVBOs())
	{
		glEnableClientState(GL_VERTEX_ARRAY); 
		_CrtRender.BindVBO(geometry->VBOIDs[eGeoPoints]);
		glVertexPointer ( 3, GL_FLOAT, 0, NULL);				

		glEnableClientState(GL_NORMAL_ARRAY);
		_CrtRender.BindVBO(geometry->VBOIDs[eGeoNormals]);
		glNormalPointer( GL_FLOAT, 0, NULL);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		_CrtRender.BindVBO(geometry->VBOIDs[eGeoTexCoord0]);
		glTexCoordPointer( 2, GL_FLOAT, 0, NULL);	
		return;
	}

	glEnableClientState(GL_VERTEX_ARRAY); 
	glVertexPointer ( 3, GL_FLOAT, 0, geometry->Points );				
	if (geometry->Normals) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, 0, geometry->Normals );
	}
	if (geometry->TexCoords[0]) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, geometry->TexCoords[0] );	
	}
}
*/

	// Temporary disable all VBOs stuffs

	// Are we using VBOs?
/*	if ( _CrtRender.UsingVBOs() )
	{
		for	( i=0 ; i<(NumTextures == 0 ? 1 : NumTextures) ; i++ )
		{
			// Texture coordinates for VBOs
			if	( VboIds[eGeoTexCoord0+i] )
			{
				// bind the vbo first 
				_CrtRender.BindVBO( VboIds[eGeoTexCoord0+i] ); 
				glTexCoordPointer( 2, GL_FLOAT, 0, NULL );	
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			else
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			// Normals for VBOs
			if	( VboIds[eGeoNormals] )
			{
				// non-skinned
				if(HasSkin)
				{
					_CrtRender.CopyVBOData( VboIds[eGeoNormals], Normals, sizeof( CrtVec3f )* NumTris * 3 ); 
				}
				_CrtRender.BindVBO( VboIds[eGeoNormals] ); 
				glNormalPointer( GL_FLOAT, 0, NULL );
				glEnableClientState(GL_NORMAL_ARRAY);
			}
			else
			{
				glDisableClientState(GL_NORMAL_ARRAY);
			}

			// Points for VBOs
			if ( VboIds[eGeoPoints] )
			{
				if(HasSkin)
				{
					_CrtRender.CopyVBOData( VboIds[eGeoPoints], Points, sizeof( CrtVec3f )* NumTris * 3 ); 
				}
				_CrtRender.BindVBO( VboIds[eGeoPoints] ); 
				glVertexPointer ( 3, GL_FLOAT, 0, NULL );				
			}

			glDrawArrays( GL_TRIANGLES, 0, NumTris*3 );
		}
	}
	else
    {
		// Non VBO path
		for	( i=0 ; i<(NumTextures == 0 ? 1 : NumTextures) ; i++ )
		{
			// Texture Coordinates
			if	(TexCoords[i] != 0)
			{
				//glClientActiveTexture(GL_TEXTURE0+i);
				glTexCoordPointer( 2, GL_FLOAT, 2*sizeof(GL_FLOAT), TexCoords[i] );
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			}
			else
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			// Normals
			if	(Normals != 0)
			{
				// Has normals
				glNormalPointer( GL_FLOAT, 3*sizeof(GL_FLOAT), Normals );
				glEnableClientState(GL_NORMAL_ARRAY);
			} 
			else if ( HasSkin && BindNormals )
			{
				// skinned with BindNormals
				glNormalPointer( GL_FLOAT, 3*sizeof(GL_FLOAT), BindNormals );
				glEnableClientState(GL_NORMAL_ARRAY); 
			}
			else
			{
				glDisableClientState(GL_NORMAL_ARRAY);
			}

			if ( !HasSkin && Points )
			{
				// Not skinned
				glVertexPointer ( 3, GL_FLOAT, 3*sizeof(GL_FLOAT), Points );
			}
			else if ( HasSkin && BindPoints && _CrtRender.UsingCg() )
			{
				// Skinned with bind points and CG
				glVertexPointer ( 3, GL_FLOAT, 3*sizeof(GL_FLOAT), BindPoints );
			}
			else
			{
				// everything else
				glVertexPointer ( 3, GL_FLOAT, 3*sizeof(GL_FLOAT), Points );
			}

			glDrawArrays( GL_TRIANGLES, 0, NumTris*3 );
		}
	}
 */
	// just for debugging 
	//DumpData(); 
/*
	if ( _CrtRender.GetRenderDebug() )
		RenderDebug(); 
};
void	CrtPolyGroup::DumpData()
{
	// for now just dumping skin data 

	if ( HasSkin ) 
	{
		for (CrtUInt i = 0 ; i < count * 3; i ++ )
		{
			if ( _CrtRender.UsingCg() )
			{
			// dump weight info 
				CrtPrint( "WeightCg %d W %f J %f Inf %d\n", i, SkinWeightsCg[i].x, SkinIndicesCg[i].x, 1 ); 
			}
			else
			{	
				// dump weight info 
				CrtPrint( "Weight %d W %f J %f Inf %d\n", i, Weights[i].Weights[0], Weights[i].Joints[0], Weights[i].NumInfluences ); 			
			}
		}
	}
}
*/

void 	CrtRender::SetMaterial( CrtMaterial * mat )
{
	CrtEffect *effect = mat->GetEffect();
	assert(effect);  // In COLLADA 1.4 a material is required to have a reference to an effect
	
	CrtColor4f	emission	=	effect->GetEmission();
	CrtColor4f	ambient		=	effect->GetAmbient();
	CrtColor4f	diffuse		=	effect->GetDiffuse();
	CrtColor4f	specular	=	effect->GetSpecular();
	CrtFloat	shininess	=   effect->GetShininess();
	if (shininess < 1.0) shininess = (CrtFloat) (shininess * 100.0); // if the scale is 0.0 to 1.0, make it 0.0 to 100.0

//	diffuse.a	=	effect->Transparency;
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION , (GLfloat*)&emission );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT  , (GLfloat*)&ambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE  , (GLfloat*)&diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR , (GLfloat*)&specular );
	glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, (GLfloat )shininess ); 

	if	( effect->Textures.empty() ) {
		glDisable(GL_TEXTURE_2D);
	}
	else {
		glEnable(GL_TEXTURE_2D);
		for (CrtUInt i=0; i<effect->Textures.size(); i++)
		{
//			glClientActiveTexture(GL_TEXTURE0+i);
			SetTexture( effect->Textures[i] );
		}
	}
	SetCurrentMaterial( mat );
}; 
void 	CrtRender::SetInstanceCamera( CrtInstanceCamera * inst, CrtInt32 sWidth, CrtInt32 sHeight )
{
	if (inst==NULL) return;
	// Recalculate the aspect in case the window size has changed.

	CrtFloat aspect;
	if ( sWidth > 0 && sHeight > 0 )
	{
		// If the window size is set, make the aspect ratio match it
		aspect = (CrtFloat)(sWidth)/(CrtFloat)(sHeight);
	}
	else if (inst->AbstractCamera->GetAspect() > 0.0)
	{
		// No window size set, use the aspect ratio from the camera
		aspect = inst->AbstractCamera->GetAspect();
	}
	else
	{
		// Otherwise default to 16 by 9 (HDTV)
		aspect = 16.0f/9.0f;
	}

	// Get the camera from the instance and set the projection matrix from it
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspectivef(inst->AbstractCamera->GetYFov(),
					aspect,
					inst->AbstractCamera->GetZNear(),
					inst->AbstractCamera->GetZFar());

	// Now get the node from the instance, and set the modelview matrix to the inverse of 
	// the node's matrix.
	glMatrixMode(GL_MODELVIEW);
	CrtMatrix mat;
	const CrtMatrix *ltow;
	ltow = inst->Parent->GetLocalToWorldMatrix();
	CrtMatrixCopy(*ltow, mat); 
	CrtMatrix3x4Invert( mat, mat);
	LoadMatrix( mat );
}
void	CrtRender::SetInstanceLight( CrtInstanceLight * lightInstance, CrtInt lightNumber )
{
	if (lightInstance==NULL) return;
	// NOTE: Remember the modelview matrix needs to be right before you call this, generally this
	// means calling SetInstanceCamera before calling SetInstanceLight

	// This handles the new lighting model, lightNumber is the GL light to set

	// Get the light position from the light instance's parent node's LocalToWorldMatrix
	// REFACTOR IN PROGRESS we should have an accessor for getting the eye position
	CrtMatrix * matrix = lightInstance->Parent->GetLocalToWorldMatrix();
	CrtVec4f  Position((*matrix)[12], (*matrix)[13], (*matrix)[14], 1.0f); 

	CrtColor4f color = lightInstance->AbstractLight->GetColor();

	// We only have one color in the light, try to set the gl light parameters sensiblly
	// !!!GAC this may oversaturate, might need to back down ambient/specular
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (GLfloat*)&color);
	switch(lightInstance->AbstractLight->GetType())
	{
		case eLIGHTTYPE_AMBIENT:
			glLightfv( GL_LIGHT0+lightNumber, GL_AMBIENT , (GLfloat*)&color);
//			glLightfv( GL_LIGHT0+lightNumber, GL_POSITION, (GLfloat*)&Position ); // not sure if ambient is positional
			break;
		case eLIGHTTYPE_DIRECTIONAL:
			{
			CrtVec3f  newdirection;//(0.0f, -1.0f, 0.0f);
			CrtVec3f  olddirection(0.0f, 0.0f, 1.0f);
			float * worldmatrix = (float *) lightInstance->Parent->GetInverseTransposeLocalToWorldMatrix();
			CrtMatrixTransform(worldmatrix, olddirection, newdirection);
			CrtVec4f  direction(newdirection.x, newdirection.y, newdirection.z, 0.0f);
			glLightfv( GL_LIGHT0+lightNumber, GL_POSITION, (GLfloat*)&direction );
			glLightfv( GL_LIGHT0+lightNumber, GL_DIFFUSE , (GLfloat*)&color);
			glLightfv( GL_LIGHT0+lightNumber, GL_SPECULAR, (GLfloat*)&color);
			glLightf ( GL_LIGHT0+lightNumber, GL_SPOT_CUTOFF,			(GLfloat)180.0f );
			glLightf ( GL_LIGHT0+lightNumber, GL_CONSTANT_ATTENUATION , (GLfloat)lightInstance->AbstractLight->ConstantAttenuation);
			glLightf ( GL_LIGHT0+lightNumber, GL_LINEAR_ATTENUATION,	(GLfloat)lightInstance->AbstractLight->LinearAttenuation );
			glLightf ( GL_LIGHT0+lightNumber, GL_QUADRATIC_ATTENUATION, (GLfloat)lightInstance->AbstractLight->QuadraticAttenuation);
			}
			break;
		case eLIGHTTYPE_POINT:
			glLightfv( GL_LIGHT0+lightNumber, GL_POSITION,				(GLfloat*)&Position );
			glLightfv( GL_LIGHT0+lightNumber, GL_DIFFUSE ,				(GLfloat*)&color);
			glLightfv( GL_LIGHT0+lightNumber, GL_SPECULAR,				(GLfloat*)&color);
			glLightf ( GL_LIGHT0+lightNumber, GL_SPOT_CUTOFF,			(GLfloat)180.0f );
			glLightf ( GL_LIGHT0+lightNumber, GL_CONSTANT_ATTENUATION , (GLfloat)lightInstance->AbstractLight->ConstantAttenuation);
			glLightf ( GL_LIGHT0+lightNumber, GL_LINEAR_ATTENUATION,	(GLfloat)lightInstance->AbstractLight->LinearAttenuation );
			glLightf ( GL_LIGHT0+lightNumber, GL_QUADRATIC_ATTENUATION, (GLfloat)lightInstance->AbstractLight->QuadraticAttenuation);
			break;
		case eLIGHTTYPE_SPOT:
			{
			CrtVec3f  olddirection(0.0f, 0.0f, -1.0f);
			CrtVec3f  newdirection;//(0.0f, -1.0f, 0.0f);
			float * worldmatrix = (float *) lightInstance->Parent->GetInverseTransposeLocalToWorldMatrix();
			CrtMatrixTransform(worldmatrix, olddirection, newdirection);
			CrtVec4f  direction(newdirection.x, newdirection.y, newdirection.z, 1.0f);
			glLightfv( GL_LIGHT0+lightNumber, GL_POSITION,		(GLfloat*)&Position );
			glLightfv( GL_LIGHT0+lightNumber, GL_DIFFUSE,		(GLfloat*)&color);
			glLightfv( GL_LIGHT0+lightNumber, GL_SPECULAR,		(GLfloat*)&color);
			glLightf ( GL_LIGHT0+lightNumber, GL_SPOT_CUTOFF,	(GLfloat ) lightInstance->AbstractLight->FalloffAngle);
			glLightf ( GL_LIGHT0+lightNumber, GL_SPOT_EXPONENT, (GLfloat ) lightInstance->AbstractLight->FalloffExponent );
			glLightfv( GL_LIGHT0+lightNumber, GL_SPOT_DIRECTION, (GLfloat*) &direction );
			glLightf ( GL_LIGHT0+lightNumber, GL_CONSTANT_ATTENUATION , (GLfloat)lightInstance->AbstractLight->ConstantAttenuation);
			glLightf ( GL_LIGHT0+lightNumber, GL_LINEAR_ATTENUATION   , (GLfloat)lightInstance->AbstractLight->LinearAttenuation );
			glLightf ( GL_LIGHT0+lightNumber, GL_QUADRATIC_ATTENUATION, (GLfloat)lightInstance->AbstractLight->QuadraticAttenuation);
			}
			break;
		default:
			break;
	}
	// Enable the light and lighting
	glEnable(GL_LIGHT0+lightNumber);

//	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

CrtBool	CrtRender::CreateTexture( CrtImage * texImageObj )
{	
//	(void)texImageObj; 	
	unsigned CrtInt32 tId = 0; 
	glGenTextures(1, &tId);

	// Create Nearest Filtered Texture
	glBindTexture(GL_TEXTURE_2D, tId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	
CrtPrint("Creating Texture... " ); 
	// get texImage 
	if	(texImageObj->GetFormat() == 3)
	{
#ifdef _WIN32
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texImageObj->GetWidth(), texImageObj->GetHeight(), 
			GL_RGB, GL_UNSIGNED_BYTE, texImageObj->GetImageData() );
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImageObj->GetWidth(), texImageObj->GetHeight(), 0, 
			GL_RGB, GL_UNSIGNED_BYTE, texImageObj->GetImageData() );
#endif
	}
	else if (texImageObj->GetFormat() == 4)
	{
#ifdef _WIN32
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, texImageObj->GetWidth(), texImageObj->GetHeight(), 
			GL_RGBA, GL_UNSIGNED_BYTE, texImageObj->GetImageData() );
#else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texImageObj->GetWidth(), texImageObj->GetHeight(), 0, 
			GL_RGBA, GL_UNSIGNED_BYTE, texImageObj->GetImageData() );
#endif
	}

CrtPrint(" done\n" ); 
	texImageObj->SetId( tId ); 


	return CrtTrue; 
}

CrtBool CrtRender::DeleteTexture(CrtImage * texImageObj )
{
	CrtUInt32 tId = texImageObj->GetId();
	if (tId != 0) {
		glDeleteTextures(1, &tId);
		texImageObj->SetId( 0 ); 
		return CrtTrue;
	}
	return CrtFalse;
}

CrtBool	CrtRender::SetTexture( CrtImage * texObj )
{
	if ((int) texObj->GetId() == -1 )
	{
		glDisable(GL_TEXTURE_2D);				
		return CrtFalse; 
	}
	else
	{
		// Enable Texture Mapping
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texObj->GetId());
	}

	return CrtTrue; 
}


CrtVoid	CrtRender::LoadMatrix( float * mat )
{
	glLoadMatrixf(mat);
}

CrtVoid	CrtRender::PushMatrix()
{
	glPushMatrix();
}

CrtVoid	CrtRender::MultMatrix( float * mat )
{
	glMultMatrixf(mat);
}

CrtVoid	CrtRender::PopMatrix()
{
	glPopMatrix();
}
#ifdef _WIN32
typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint *buffers);
//typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, int size, const GLvoid *data, GLenum usage);

// VBO Extension Function Pointers
PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;					// VBO Name Generation Procedure
PFNGLBINDBUFFERARBPROC glBindBuffer = NULL;					// VBO Bind Procedure
PFNGLBUFFERDATAARBPROC glBufferData = NULL;					// VBO Data Loading Procedure
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = NULL;				// VBO Deletion Procedure

CrtBool IsGLExtensionSupported( char* targetExtension )
{
	const unsigned char *extensions = NULL;
	const unsigned char *start;
	unsigned char *where, *terminator;

	// Extension names should not have spaces
	where = (unsigned char *) strchr( targetExtension, ' ' );
	if( where || *targetExtension == '\0' )
		return CrtFalse;

	// Get Extensions String
	extensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	start = extensions;
	for(;;)
	{
		where = (unsigned char *) strstr( (const char *) start, targetExtension );
		if( !where )
			break;
		terminator = where + strlen( targetExtension );
		if( where == start || *( where - 1 ) == ' ' )
			if( *terminator == ' ' || *terminator == '\0' )
				return CrtTrue;
		start = terminator;
	}
	return CrtFalse;
}


CrtBool	CrtRender::CheckForVBOs()
{
	CrtBool isSupported = IsGLExtensionSupported( "GL_ARB_vertex_buffer_object" );
	
	if ( isSupported )
	{
		// Get Pointers To The GL Functions
		glGenBuffers = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB");
		glBindBuffer = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB");
		glBufferData = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB");
		glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress("glDeleteBuffersARB");
	
		if ( glGenBuffers && 
			glBindBuffer && 
			glBufferData && 
			glDeleteBuffers )
			VBOsAvailable = CrtTrue; 		
	}
	else
	{
		UseVBOs = CrtFalse; 
	}

	return CrtTrue; 
}; 

#else

CrtBool	CrtRender::CheckForVBOs()
{
	VBOsAvailable = CrtTrue; 
	UseVBOs = CrtTrue; 

	CrtPrint(" COLLADA_RT Using VBOS %d \n", UsingVBOs() ); 
    return CrtTrue; 
}; 


#endif 
CrtInt	CrtRender::GenerateVBO()
{
	if ( UseVBOs && VBOsAvailable )
	{
		CrtUInt	vboID; 
		// Generate And Bind The Vertex Buffer
		glGenBuffers( 1, &vboID );					
		
		return vboID; 
	}
	return -1; 
}

CrtBool	CrtRender::CopyVBOData(CrtUInt type, CrtUInt vboID, CrtVoid * data, CrtInt size )
{
	if ( UseVBOs && VBOsAvailable && vboID > 0 )
	{		
		// Bind The Buffer
		glBindBuffer( type, vboID );			
		// Load The Data
		glBufferData( type, size, data, GL_STATIC_DRAW );

		return CrtTrue; 
	}
	return CrtFalse; 
}

CrtBool	CrtRender::BindVBO( CrtUInt type, CrtUInt vboID )
{
	if ( UseVBOs && VBOsAvailable )
	{
		glBindBuffer(type, vboID );
		return CrtTrue;
	}
	else 
		return CrtFalse; 
}
void	CrtRender::FreeVBO( CrtUInt vboUID )
{
	// Delete VBO
	if( UseVBOs && vboUID > 0 )
	{
		// Free The VBO Memory
		glDeleteBuffers( 1, &vboUID );
	}
}

/*
enum CrtGeoDataType 
{
	eGeoPoints,
	eGeoNormals,
	eGeoBiNormals,
	eGeoTangents,
	eGeoSkinIndices,
	eGeoSkinWeights,
    eGeoTriIndices,
	eGeoTexCoord0,
	eGeoTexCoord1,
	eGeoTexCoord2,
	eGeoTexCoord3,
	eGeoTexCoord4,
	eGeoTexCoord5,
	eGeoTexCoord6,
	eGeoTexCoord7,
};*/
/*
void	CrtPolyGroup::InitVBOs()
{
	if ( ! _CrtRender.UsingVBOs() )
		CrtAssert("VBOs aren't not Setup or not using VBOS!!", false ); 

	// points 
	if ( Points )
	{
		VboIds[eGeoPoints] = _CrtRender.GenerateVBO(); 
		_CrtRender.CopyVBOData( VboIds[eGeoPoints], Points, sizeof( CrtVec3f )* count * 3 ); 
		
		// we don't need this anymore if we are using Cg 	
		if ( _CrtRender.UsingCg() && !_CrtRender.GetRenderDebug() )
		{
			//CrtDeleteData( Points ); 		
		}

		//GLenum glError = glGetError();
		//CrtAssert("GL Polygon Render Error", glError == 0 );

	}

	// normals 
	if ( Normals )
	{
		VboIds[eGeoNormals] = _CrtRender.GenerateVBO(); 
		_CrtRender.CopyVBOData( VboIds[eGeoNormals], Normals, sizeof( CrtVec3f )* count * 3 ); 
		
		// we don't need this anymore if we are using Cg 	
		if ( _CrtRender.UsingCg()  && !_CrtRender.GetRenderDebug() )
		{
			//CrtDeleteData( Normals ); 
		}
		//GLenum glError = glGetError();
		//CrtAssert("GL Polygon Render Error", glError == 0 );

	}

	if ( Tangents )
	{
		VboIds[eGeoTangents] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData( VboIds[eGeoTangents], Tangents, sizeof( CrtVec3f ) * count  * 3 ); 
		
		//GLenum glError = glGetError();
		//CrtAssert("GL Polygon Render Error", glError == 0 );

		// we don't need this anymore if we are using Cg 	
		//if ( _CrtRender.UsingCg() )
		//	CrtDeleteData( Tangents ); 
	}

	// if there are bind pose points 
	// eventually need to make this an either or as in either points and bindPoints 
	// normals or bind normals, but because I am not pulling out the bindNormals right now 
	// I will leave it like this for now.  April 20, 05
	if ( HasSkin && BindPoints )
	{
		VboIds[eGeoSkinBindPoints] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData( VboIds[eGeoSkinBindPoints], BindPoints, sizeof( CrtVec3f )* count * 3 ); 
		
		// we don't need this anymore if we are using Cg 	
		if ( _CrtRender.UsingCg()  && !_CrtRender.GetRenderDebug() )
		{
			//CrtDeleteData( BindPoints ); 
		}
	}
	// lookie here... finally got this in.  Not the best solution, but this will work for now. 
	if ( HasSkin && BindNormals )
	{
		VboIds[eGeoSkinBindNormals] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData( VboIds[eGeoSkinBindNormals], BindNormals, sizeof( CrtVec3f )* count * 3 ); 
		
		// we don't need this anymore if we are using Cg 	
		if ( _CrtRender.UsingCg()  && !_CrtRender.GetRenderDebug() )
		{
			//CrtDeleteData( BindNormals ); 
		}
	}

	// Textures // right now just supporting one set 
	if ( TexCoords[0] )
	{
		VboIds[eGeoTexCoord0] = _CrtRender.GenerateVBO(); 
		_CrtRender.CopyVBOData( VboIds[eGeoTexCoord0], TexCoords[0], sizeof( CrtVec2f )* count * 3 ); 
		
		// don't need anymore in any case 
		//CrtDeleteData( TexCoords[0] ); 
	}

	// Skin indices 
	if ( SkinIndicesCg )
	{
		VboIds[eGeoSkinIndices] = _CrtRender.GenerateVBO(); 
		_CrtRender.CopyVBOData( VboIds[eGeoSkinIndices], SkinIndicesCg, sizeof( CrtVec4f )* count * 3 ); 
		
		// don't need anymore in any case 
		CrtDeleteData( SkinIndicesCg ); 
	}

	// Skin Weights 
	if ( SkinWeightsCg )
	{
		VboIds[eGeoSkinWeights] = _CrtRender.GenerateVBO(); 
		_CrtRender.CopyVBOData( VboIds[eGeoSkinWeights], SkinWeightsCg, sizeof( CrtVec4f )* count * 3 ); 
		
		// don't need anymore in any case 
		CrtDeleteData( SkinWeightsCg ); 
	}


};



void	CrtPolyGroup::FreeVBOs()
{
	for ( int i = 0; i < eGeoTexCoord7+1; i ++ )
	{
		_CrtRender.FreeVBO( VboIds[i] ); 
	}
}

void	CrtPolyGroup::RenderDebug()
{
#ifdef _WIN32
	glDisable(GL_LIGHTING);
	glColor3f(0.0f,0.5f,1.0f);	
		
	// Set Line Color To Blue
	// render the normals and tangents with the fixed function pipe 
	if ( Normals )
	{
		glBegin(GL_LINES);	
        for (CrtUInt i = 0; i < count * 3; i ++ )
		{			
	
				glVertex3d( Points[i].x, Points[i].y, Points[i].z );	
				glVertex3d( Points[i].x + Normals[i].x, 
					Points[i].y + Normals[i].y, 
					Points[i].z + Normals[i].z);	
		}		
		glEnd();
	}

	

	CrtFloat len = .2f; 
	glColor3f(1.0f,0.5f,0.0f);				// Set Line Color To Blue
	if ( Tangents )
		for ( int j = 0; j < NumTris * 3; j++ )
		{
			glBegin(GL_LINES);	
				glVertex3d( Points[j].x, Points[j].y, Points[j].z );	
				glVertex3d( Points[j].x + Tangents[j].x*len, 
					Points[j].y + Tangents[j].y*len, 
					Points[j].z + Tangents[j].z*len);	
			glEnd();
		}		

#endif
}
*/


// shadow map stuff 

#define GL_TEXTURE_COMPARE_MODE_ARB		0x884C
#define GL_COMPARE_R_TO_TEXTURE_ARB		0x884E
#define GL_CLAMP_TO_EDGE				0x812F
#define GL_DEPTH_COMPONENT              0x1902


CrtBool CrtRender::InitShadowMap()
{
	if ( ShadowMapInitialized )
		return CrtTrue; 

	// setup the depth texture
	glGenTextures  (1,&ShadowMapId);
	glBindTexture  (GL_TEXTURE_2D,ShadowMapId);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE_ARB,GL_COMPARE_R_TO_TEXTURE_ARB);

	glTexImage2D(GL_TEXTURE_2D,
				 0,GL_DEPTH_COMPONENT, ShadowMapWidth, ShadowMapHeight, 
				 0, GL_DEPTH_COMPONENT,	 GL_UNSIGNED_INT, NULL);
	// polygon offset, turned on when drawing to the shadow tex
	glPolygonOffset(1.1f,4.0f);
	// to read once and for all from the depth
	//glReadSurface(GL_DEPTH);
	//GLvoid * pixels = NULL; 
	//glReadPixels( 0, 0, ShadowMapWidth, ShadowMapHeight, GL_DEPTH_COMPONENT,
     //             GL_UNSIGNED_BYTE, pixels);
	// assumes only shadows use cull-face
	//glCullFace(GL_FRONT);

	ShadowMapInitialized = CrtTrue; 
	return CrtTrue; 
}



CrtVoid	CrtRender::SetupRenderingToShadowMap()
{
	// set tc flag so polygroups will know which shader to use 
	SetRenderingToShadowMap( CrtTrue );
	SetRenderingWithShadowMap( CrtFalse );
	
	// gl stuff to setup rendering to the shadow map 
	glEnable(GL_CULL_FACE);
	glViewport(0,0,ShadowMapWidth,ShadowMapHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	// update only z
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	// shift a bit
	glEnable(GL_POLYGON_OFFSET_FILL);

	PushLightViewMatrix();
     
}

CrtVoid	CrtRender::SetupRenderingWithShadowMap()
{
	//printf(" Setting Rendering with shadowmap \n" ); 

	// set tc flag so polygroups will know which shader to use 
	SetRenderingWithShadowMap( CrtTrue );
	SetRenderingToShadowMap( CrtFalse );

	glDisable(GL_CULL_FACE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, ShadowMapId);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0,0,
	//					0,0,ShadowMapWidth,ShadowMapHeight);
	// get depth buffer
	//glCopyTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,0,0,ShadowMapWidth,ShadowMapHeight, 0);

	glViewport(0,0,ScreenWidth,ScreenHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
        
	PopLightViewMatrix(); 

	// need to get the current camera and reset it again because we have since over written 
	// the model view setup by SceneUpdate
	if ( ActiveInstanceCamera )
	{
		// !!!GAC New camera handling, hopefully this will work the same as the old code
		SetInstanceCamera(ActiveInstanceCamera, ScreenWidth, ScreenHeight);
	}
	
}


CrtVoid 	CrtRender::PushLightViewMatrix()
{
	// should get the current light from the scene and get it 
	// position and what now but for just for now going to 
	// set a hard value 
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// Not implemented yet: need to look into perspective range
	gluPerspectivef(35.0f,ShadowMapWidth/ShadowMapHeight,1.f,30.f);

	// setup light view
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// light is directly above for now 
	gluLookAtf(  0,30,0, // eye (light)  position
		 0.0f,0.0f,0.0f,  // target
		 0.0f,1.0f,0.0f);	// up vector
}

CrtVoid 	CrtRender::PopLightViewMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


void	CrtNode::DrawLineToChildren()
{

#ifndef SN_TARGET_PS3 
	
	// recursively draw lines to each child node 
	GLboolean lightenabled = glIsEnabled(GL_LIGHTING);
	if (lightenabled) glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
		
	// Set Line Color To Blue
	glColor3f(1.0f,0.5f,1.0f);				
	glLineWidth( 3.0f ); 
	CrtNode * child = Children; 
	while ( child )
	{
		CrtVec3f pPos; 
		CrtVec3f pChild;
		pPos.x = LocalToWorldMatrix[M30];
		pPos.y = LocalToWorldMatrix[M31];
		pPos.z = LocalToWorldMatrix[M32];
		pChild.x = child->LocalToWorldMatrix[M30]; 	
		pChild.y = child->LocalToWorldMatrix[M31]; 	
		pChild.z = child->LocalToWorldMatrix[M32]; 	

		glBegin(GL_LINES);	
			glVertex3d( pPos.x, pPos.y, pPos.z );	
			glVertex3d( pChild.x, pChild.y, pChild.z );	
		glEnd();

		child = child->GetNext();
	}	

	glLineWidth( 1.0f ); 
	glEnable(GL_TEXTURE_2D);
	if (lightenabled) glEnable(GL_LIGHTING);

	child = Children; 
	while ( child )
	{
		child->DrawLineToChildren();
		child = child->GetNext();
	}
	
#endif 	

}; 
