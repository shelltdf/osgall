/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtSkin.h"
#include "Crt/CrtUtils.h"


void	CrtSkinWeight::SortWeights()
{
	// need to sort if the number of weights is greater than 4 
	// just to prepare for the cg program 
	if ( NumWeights > 4 )
	{
		CrtFloat wTemp = 0;
		CrtInt	iTemp = 0; 

		// need to sort the weights with the biggest words the top 
		for (CrtUInt s = NumWeights-1; s > 0; s-- )
		{
			for (CrtUInt w = NumWeights-1; w > 0; w-- )
			{
				// if the current weight has a higher value than next swap the two 
				if ( Weights[w] > Weights[w-1] )
				{
					wTemp = Weights[w]; 
					iTemp = Joints[w]; 
					Weights[w] = Weights[w-1];
					Joints[w] = Joints[w-1];
					Weights[w-1] = wTemp;
					Joints[w-1] = iTemp; 
				}
			}
		}

		/// Test here just to make sure
		for (CrtUInt i = 0; i <  NumWeights - 1; i++ )
		{
			CrtAssert("Weight Indices werern't sorted properly \n", 
				Weights[i] >= Weights[i+1]) ;
		}	
	}
}

CrtVoid CrtSkin::FindJointNodes( CrtNode * sceneRoot ) 
{

	for (CrtUInt i = 0; i < JointCount; i++)
	{
		Joints[i].JointNode = FindJointNode( sceneRoot, Joints[i].Name ); 

		if ( Joints[i].JointNode )
		{
			Joints[i].JointNode->SetUsedForSkinning(); 	
			Joints[i].JointNode->SetInverseBindMatrix( IBindMats[i] );
			
			CrtPrint(" Skin Controller (%s) joint binding made to node (%s) \n", Name, Joints[i].Name ); 
		}
		else
		{
			CrtPrint(" Failed to make joint binding for Controller %s for Joint %s \n", Name, Joints[i].Name ); 
			return;
		}
	}
//	RenderReady = CrtTrue;
}

CrtNode * CrtSkin::FindJointNode( CrtNode * node, CrtChar * name )
{
	if (JointsType == CrtJoint::CrtJointsType_ID)
	{
		if (node->GetId()[0])
			if ( CrtICmp( node->GetId(), name ) )
				return node;
	
	} else if (JointsType == CrtJoint::CrtJointsType_SID)
	{
		if (node->GetSid()[0])
			if ( CrtICmp( node->GetSid(), name ) )
				return node;
	}
//	if ( CrtICmp( node->GetName(), name ) )
//		return node;
	
	// update any children 
	CrtNode * children = node->GetChild();

	while ( children )
	{
		CrtNode * node = FindJointNode( children, name );
		if ( node )
			return node; 
		children = children->GetNext(); 
	}

	return NULL; 
}

void	CrtSkin::AddIBindMats( CrtMatrix * iBindMatArray, CrtInt nMats )
{
	IBindMatCount = nMats; 

	// allocate for inverse bind matrices 
	IBindMats = CrtNewData( CrtMatrix, IBindMatCount );
	
	// allocate for combined matrix list as well 
	SkinningMats = CrtNewData( CrtMatrix, IBindMatCount ); 
	// need to move to useing this 3x4 so we can allow for more bones 
//	SkinningMats3x4 = CrtNewData( CrtMatrix3x4, IBindMatCount ); 

	for (CrtUInt i = 0; i < IBindMatCount; i++)
	{
		CrtMatrixCopy( iBindMatArray[i], IBindMats[i]);
	}	
}

void	CrtSkin::UpdateCombinedMats()
{
	// update the combined matrix from each joint 
	// into this centeralized combined matrix list.
	
	for (CrtUInt i = 0; i < JointCount; i++)
	{
		if (Joints[i].JointNode==NULL) continue;
		skin_animation_count += Joints[i].JointNode->GetNumAnimations();
		CrtFloat * skin_matrix = Joints[i].JointNode->GetSkinMatrix();
//		CrtMatrix temp_matrix = skin_matrix;
		CrtMatrixCopy( skin_matrix,  SkinningMats[i] );
//		if ( _CrtRender.UsingCg() )
//		{
//            CrtMatrixTranspose( SkinningMats[i], SkinningMats[i] ); 
//			CrtMatrixTranspose3x4( SkinningMats[i], SkinningMats3x4[i] ); 
//		}
		//CrtMatrixLoadIdentity( SkinningMats[i] ); 
	}
}

CrtVoid	CrtSkin::Update( CrtFloat)
{	
	if (RenderReady == CrtTrue) return; // if there are no animations, no need to update and ready to render 

	// copy the skinned matrices from the joint nodes 
	UpdateCombinedMats(); 
	
	for(CrtUInt v=0; v<base_geometry->vertexcount; v++) {//	int v = indexes[p];
		CrtUInt weight_index = base_geometry->SkinIndex[v];
		CrtUInt nbrInf = Weights[weight_index].NumWeights;
//		CrtAssert("SetSkin nbrInf > JointCount\n", nbrInf < JointCount);
		if( nbrInf )
		{
			CrtVec3f BSMVert; 
			CrtVec3f BSMNorm; 
			CrtVec3f transVert; 
			CrtVec3f weightVert;
			CrtVec3f transNorm; 
			CrtVec3f weightNorm; 
			CrtUInt joint = 0; 
			float weight = 0; 
			float weightTotal = 0; 
			float weightNormalized = 0; 	
			if (BindShapeMats)
			{
				CrtMatrix3x4TransformSkin( BindShapeMats, base_geometry->Points[v], BSMVert );
				CrtMatrix3x4TransformSkin( BindShapeMats, base_geometry->Normals[v], BSMNorm );
			}
			else 
				BSMVert = base_geometry->Points[v];
			for( CrtUInt j = 0; j < nbrInf ; j++)
			{
				joint =  Weights[weight_index].Joints[j];				
				weight = Weights[weight_index].Weights[j];

				CrtMatrix3x4TransformSkin( SkinningMats[joint], BSMVert, transVert );
				CrtMatrix3x4RotateSkin( SkinningMats[joint], BSMNorm, transNorm ); 

				weightVert += transVert * weight;  
				weightNorm += transNorm * weight;

				weightTotal += Weights[weight_index].Weights[j];
			}

			if (weightTotal != 1.0f)
			{
				weightNormalized = 1.0f/weightTotal;
				weightVert *= weightNormalized;
				weightNorm *= weightNormalized; 
			}
			skinned_geometry->Points[v] = weightVert;
			if ( skinned_geometry->Normals )
				skinned_geometry->Normals[v] = weightNorm; 
		}
		else
		{
			printf("No Influences \n" ); 
		}
	}
}

CrtVoid	CrtSkin::Draw(CrtNode *parentNode, CrtInstance * instance)
{
	skinned_geometry->Draw(parentNode, instance);
	if (skin_animation_count == 0) 
		RenderReady = CrtTrue;
	else 
		RenderReady = CrtFalse;
}

CrtSkin::~CrtSkin()
{
	CrtDeleteData( Joints ); 
	CrtDeleteData( IBindMats ); 
	CrtDeleteData( SkinningMats ); 
	CrtDeleteData( Joints ); 
	CrtDeleteData( Weights ); 
	CrtDelete(skinned_geometry);
}