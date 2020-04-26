/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtGeometry.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtScene.h"
#include "Crt/CrtMatrix.h"

#include "cfxLoader.h"
#include "cfxEffect.h"
#include "cfxMaterial.h"
#include "cfxPlatform.h"

float ticker = 0.0f;

CrtInputType	CrtPolyGroup::SetType( CrtChar * s )
{
	if ( CrtCmpIn( s, "VERTEX" ) )
		return ePOINT_SOURCE;
	if ( CrtCmpIn( s, "NORMAL" ) )
		return eNORMAL_SOURCE;
	if ( CrtCmpIn( s, "TEXCOORD" )|| CrtCmpIn( s, "UV" )) // until the schema is locked down got to suppor both
		return eTEXCOORD0_SOURCE;
	if ( CrtCmpIn( s, "TANGENT" ) )
		return eTANGENT_SOURCE;

	return eUNKNOWN; 
}

cfxMaterial*	CrtPolyGroup::SetupMaterialForDraw(CrtNode *parentNode, CrtInstance * instance)
{
	// This method contains code that is common to both Draw and DrawSkinned.  Was going to combine
	// Draw and DrawSkinned into one function, but decided to retain the two seperate functions so the
	// use of parameters would be properly checked.
	cfxMaterial* currentMaterial = 0;


	// resolve instance material
	for (unsigned int j=0; j<instance->MaterialInstances.size(); j++)
	{
		CrtInstanceMaterial* thisInstanceMaterial = instance->MaterialInstances[j];
		if (thisInstanceMaterial)
		{
			std::string symbol = thisInstanceMaterial->symbol;
			std::string target = thisInstanceMaterial->target;
			if (symbol == std::string(MaterialName)) {
				if (thisInstanceMaterial->targetMaterial)	// common material
				{
					_CrtRender.SetMaterial(thisInstanceMaterial->targetMaterial); 
					//Material = thisInstanceMaterial->targetMaterial;
					//break;
					return NULL;
				} else if (thisInstanceMaterial->targetcfxMaterial) // fx material
				{
					//TODO: we might have to reset FXMaterial back to 0 after we draw this.
//								FXMaterial = thisInstanceMaterial->targetcfxMaterial;
					currentMaterial = thisInstanceMaterial->targetcfxMaterial;
				}
			}
		}
	}

	if(currentMaterial)
	{
		// Pushes the setparam values into cgFX for this material
		currentMaterial->setParameterValues();

		// Get the effect pointer
		cfxEffect* currentEffect				= currentMaterial->getEffect();

		// See if there are any common semantic Parameters we need to set
		CGparameter worldviewprojectionParam	= currentEffect->getParameterBySemantic("WORLDVIEWPROJECTION");
		CGparameter viewinverseParam			= currentEffect->getParameterBySemantic("VIEWINVERSE");
		CGparameter worldParam					= currentEffect->getParameterBySemantic("WORLD");
		CGparameter worldinversetransposeParam	= currentEffect->getParameterBySemantic("WORLDINVERSETRANSPOSE");
		CGparameter lightPositionParam			= currentEffect->getParameterBySemantic("LIGHTPOSITION");
		CGparameter timeParam					= currentEffect->getParameterBySemantic("TIME");

		// WORLDVIEWPROJECTION is the world+view+projection matrix of this object which we get from GL
		if (worldviewprojectionParam)
		{
			cgGLSetStateMatrixParameter(worldviewprojectionParam, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
		}

		// VIEWINVERSE is the inverse of the view matrix which is the same as the camera's LocalToWorldMatrix
		if(viewinverseParam)
		{
			CrtInstanceCamera *inst = _CrtRender.GetActiveInstanceCamera();
			cgGLSetMatrixParameterfc( viewinverseParam, (const float *)(inst->Parent->GetLocalToWorldMatrix())); 
		}

		// WORLD is the localtoworld matrix for this object which we get from the scene graph
		if (worldParam)
		{
			cgGLSetMatrixParameterfc( worldParam, (const float *)(parentNode->GetLocalToWorldMatrix()));
//			cgGLSetStateMatrixParameter(worldParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
		}

		// WORLDINVERSETRANSPOSE is inversetransposelocaltoworld matrix for this object from the scene graph
		if (worldinversetransposeParam)
		{
			cgGLSetMatrixParameterfc( worldinversetransposeParam, (const float *)(parentNode->GetInverseTransposeLocalToWorldMatrix()));
//			cgGLSetStateMatrixParameter(worldinversetransposeParam, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_INVERSE);
		}

		// LIGHTPOSITION gets the position of the primary (nearist) light
		if (lightPositionParam)
		{
			assert(_CrtRender.GetScene()->GetLightInstanceCount());  // There should always be one
#if 0
			// !!!GAC EXPERIMENTAL this gets the closest light instance, need some way to get the parent node of this polygroup
			// !!!GAC EXPERIMENTAL may want to check to make sure NearistInstanceLight isn't null
			CrtInstanceLight *instanceLight = ??? parentnode->NearistInstanceLight;
#else
			CrtInstanceLight *instanceLight =_CrtRender.GetScene()->GetLightInstances(0);
#endif
			// Get the light position from the light instance's parent node's LocalToWorldMatrix
			// REFACTOR IN PROGRESS we should have an accessor for getting the position
			float *lightMatrix = (float *)instanceLight->Parent->GetLocalToWorldMatrix();

			CGtype lighttype = cgGetParameterType(lightPositionParam);
			if (lighttype == CG_FLOAT3)
			{
				cgSetParameter3f(lightPositionParam, lightMatrix[12], lightMatrix[13], lightMatrix[14]);
			}
			else if (lighttype == CG_FLOAT4)
			{
				cgSetParameter4f(lightPositionParam, lightMatrix[12], lightMatrix[13], lightMatrix[14], 0);
			}
		}

		// TIME gets a tickcount for driving animated shaders
		if (timeParam)
		{
			cgSetParameter1f(timeParam, ticker);
		}

		// Setup the state for the cfxMaterial
		currentMaterial->setEffectPassState(0,0);
		return currentMaterial;
	}
	return NULL;
}

CrtVoid 	CrtPolyGroup::Draw(CrtNode *parentNode, CrtInstance * instance)
{
	cfxMaterial* currentMaterial = SetupMaterialForDraw(parentNode, instance);

	Render();
	
	// Restore the old material state
	if(currentMaterial)
	{
		currentMaterial->resetEffectPassState(0,0);
	}
}

CrtGeometry::~CrtGeometry()
{
	while(!Groups.empty())
	{
		CrtDelete(Groups[0]);
		Groups.erase(Groups.begin());
	}
	
	if (Points)
	{
		CrtDeleteData(Points);
		Points = NULL;
	}
	if (Normals)
	{
		CrtDeleteData(Normals);
		Normals = NULL;
	}
	if (TexCoords[0])
	{
		CrtDeleteData(TexCoords[0]);
		TexCoords[0] = NULL;
	}
	if (SkinIndex)
	{
		CrtDeleteData(SkinIndex);
		SkinIndex = NULL;
	}

}

void 	CrtGeometry::SetRender()
{
	_CrtRender.range_data.SetRange(&Points, vertexcount);
	if (_CrtRender.UsingVBOs())
	{
		glEnableClientState(GL_VERTEX_ARRAY); 
		_CrtRender.BindVBO(GL_ARRAY_BUFFER, VBOIDs[eGeoPoints]);
		glVertexPointer ( 3, GL_FLOAT, 0, NULL);				

		glEnableClientState(GL_NORMAL_ARRAY);
		_CrtRender.BindVBO(GL_ARRAY_BUFFER, VBOIDs[eGeoNormals]);
		glNormalPointer( GL_FLOAT, 0, NULL);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		_CrtRender.BindVBO(GL_ARRAY_BUFFER, VBOIDs[eGeoTexCoord0]);
		glTexCoordPointer( 2, GL_FLOAT, 0, NULL);	

		_CrtRender.CopyVBOData(GL_ARRAY_BUFFER, VBOIDs[eGeoPoints],Points, vertexcount*3*sizeof(CrtFloat));
		_CrtRender.CopyVBOData(GL_ARRAY_BUFFER, VBOIDs[eGeoNormals],Normals, vertexcount*3*sizeof(CrtFloat));
		return;
	}

	glEnableClientState(GL_VERTEX_ARRAY); 
	glVertexPointer ( 3, GL_FLOAT, 0, Points );				
	if (Normals) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer( GL_FLOAT, 0, Normals );
	}
	if (TexCoords[0]) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, TexCoords[0] );	
	}
}

CrtLines::CrtLines()
{
	VBOID = 0;
}
CrtLines::~CrtLines()
{
	if (VBOID!=0)
	{
		_CrtRender.FreeVBO(VBOID);
		VBOID=0;
	}
	CrtDeleteData(indexes);
}
void 	CrtLines::Render()
{
	if (_CrtRender.UsingVBOs())
	{
		_CrtRender.BindVBO(GL_ELEMENT_ARRAY_BUFFER, VBOID);
		glDrawElements( GL_LINES, count * 2, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElements( GL_LINES, count * 2, GL_UNSIGNED_INT, indexes);
	}
}
void 	CrtLines::SetVBOs()
{
	VBOID = _CrtRender.GenerateVBO();
	_CrtRender.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, VBOID, indexes, count * 2 * sizeof(CrtFloat));
}

CrtTriangles::CrtTriangles()
{
	VBOID = 0;
}
CrtTriangles::~CrtTriangles()
{
	if (VBOID!=0)
	{
		_CrtRender.FreeVBO(VBOID);
		VBOID=0;
	}
	CrtDeleteData(indexes);
}
void 	CrtTriangles::Render()
{
	if (_CrtRender.UsingVBOs())
	{
		_CrtRender.BindVBO(GL_ELEMENT_ARRAY_BUFFER, VBOID);
		glDrawElements( GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElements( GL_TRIANGLES, count * 3, GL_UNSIGNED_INT, indexes);
	}
}
void 	CrtTriangles::SetVBOs()
{
	VBOID = _CrtRender.GenerateVBO();
	_CrtRender.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, VBOID, indexes, count * 3 * sizeof(CrtFloat));
}

CrtPolygons::CrtPolygons()
{
	VBOID = NULL;
}
CrtPolygons::~CrtPolygons()
{
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		if (VBOID!=0)
		{
			_CrtRender.FreeVBO(VBOID[i]);
			VBOID[i]=0;
		}
		CrtDeleteData(indexvector[i]);
	}
	CrtDeleteData(VBOID);
	indexvector.clear();
	countvector.clear();
}

void 	CrtPolygons::Render()
{
#ifndef SN_TARGET_PS3
	if (_CrtRender.UsingVBOs())
	{
		for (CrtUInt i=0; i< indexvector.size(); i++) {
			_CrtRender.BindVBO(GL_ELEMENT_ARRAY_BUFFER, VBOID[i]);
			glDrawElements( GL_POLYGON, countvector[i], GL_UNSIGNED_INT, 0);
		}
	}
	else
	{
		for (CrtUInt i=0; i< indexvector.size(); i++)
			glDrawElements( GL_POLYGON, countvector[i], GL_UNSIGNED_INT, indexvector[i]);
	}
#else
	CrtPrint("Polygons is not supported\n");
#endif
}
void 	CrtPolygons::SetVBOs()
{
	VBOID = CrtNewData(CrtUInt, indexvector.size());
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		VBOID[i] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, VBOID[i], indexvector[i], countvector[i] * sizeof(CrtFloat));
	}
}

CrtLinestrips::CrtLinestrips()
{
	VBOID = NULL;
}
CrtLinestrips::~CrtLinestrips()
{
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		if (VBOID!=0) {
			_CrtRender.FreeVBO(VBOID[i]);
			VBOID=0;
		}
		CrtDeleteData(indexvector[i]);
	}
	CrtDeleteData(VBOID);
	indexvector.clear();
	countvector.clear();
}
void 	CrtLinestrips::Render()
{
	if (_CrtRender.UsingVBOs())
	{
		for (CrtUInt i=0; i< indexvector.size(); i++) {
			_CrtRender.BindVBO(GL_ELEMENT_ARRAY_BUFFER, VBOID[i]);
			glDrawElements( GL_LINE_STRIP, countvector[i], GL_UNSIGNED_INT, 0);
		}
	}
	else
	{
		for (CrtUInt i=0; i< indexvector.size(); i++)
			glDrawElements( GL_LINE_STRIP, countvector[i], GL_UNSIGNED_INT, indexvector[i]);
	}
}
void 	CrtLinestrips::SetVBOs()
{
	VBOID = CrtNewData(CrtUInt, indexvector.size());
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		VBOID[i] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, VBOID[i], indexvector[i], countvector[i] * sizeof(CrtFloat));
	}
}

CrtTriStrips::CrtTriStrips()
{
	VBOID = NULL;
}
CrtTriStrips::~CrtTriStrips()
{
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		if (VBOID!=0)
		{
			_CrtRender.FreeVBO(VBOID[i]);
			VBOID=0;
		}
		CrtDeleteData(indexvector[i]);
	}
	CrtDeleteData(VBOID);
	indexvector.clear();
	countvector.clear();
}
void 	CrtTriStrips::Render()
{
	if (_CrtRender.UsingVBOs())
	{
		for (CrtUInt i=0; i< indexvector.size(); i++) {
			_CrtRender.BindVBO(GL_ELEMENT_ARRAY_BUFFER, VBOID[i]);
			glDrawElements( GL_TRIANGLE_STRIP, countvector[i], GL_UNSIGNED_INT, 0);
		}
	}
	else
	{
		for (CrtUInt i=0; i< indexvector.size(); i++)
			glDrawElements( GL_TRIANGLE_STRIP, countvector[i], GL_UNSIGNED_INT, indexvector[i]);
	}
}
void 	CrtTriStrips::SetVBOs()
{
	VBOID = CrtNewData(CrtUInt, indexvector.size());
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		VBOID[i] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, VBOID[i], indexvector[i], countvector[i] * sizeof(CrtFloat));
	}
}

CrtTriFans::CrtTriFans()
{
	VBOID = NULL;
}
CrtTriFans::~CrtTriFans()
{
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		if (VBOID!=0)
		{
			_CrtRender.FreeVBO(VBOID[i]);
			VBOID=0;
		}
		CrtDeleteData(indexvector[i]);
	}
	CrtDeleteData(VBOID);
	indexvector.clear();
	countvector.clear();
}
void 	CrtTriFans::Render()
{
	if (_CrtRender.UsingVBOs())
	{
		for (CrtUInt i=0; i< indexvector.size(); i++) {
			_CrtRender.BindVBO(GL_ELEMENT_ARRAY_BUFFER, VBOID[i]);
			glDrawElements( GL_TRIANGLE_FAN, countvector[i], GL_UNSIGNED_INT, 0);
		}
	}
	else
	{
		for (CrtUInt i=0; i< indexvector.size(); i++)
			glDrawElements( GL_TRIANGLE_STRIP, countvector[i], GL_UNSIGNED_INT, indexvector[i]);
	}
}
void 	CrtTriFans::SetVBOs()
{
	VBOID = CrtNewData(CrtUInt, indexvector.size());
	for (CrtUInt i=0; i< indexvector.size(); i++) {
		VBOID[i] = _CrtRender.GenerateVBO();
		_CrtRender.CopyVBOData(GL_ELEMENT_ARRAY_BUFFER, VBOID[i], indexvector[i], countvector[i] * sizeof(CrtFloat));
	}
}

CrtInstance::~CrtInstance() 
{
	while(!MaterialInstances.empty())
	{
		CrtDelete( MaterialInstances[0] );
		MaterialInstances.erase(MaterialInstances.begin());
	}
};
