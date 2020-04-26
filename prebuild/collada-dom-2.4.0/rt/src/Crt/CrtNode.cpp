/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtAnimation.h"
#include "Crt/CrtGeometry.h"
#include "Crt/CrtLight.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtScene.h"
#include "Crt/CrtController.h"
#include <float.h>

void 	CrtNode::Update( CrtFloat time)
{
	// skip this node if it is controlled by physics engine.
	if (UpdateLocalMatrix == CrtFalse && UpdateLocalToWorldMatrix == CrtFalse)
		return;

	// Update orientation matrices for this node

	UpdateOrient( time);

	// Update all the child nodes

	CrtNode * kids = Children;

	while( kids )
	{
		kids->Update( time ); 
		kids = kids->GetNext(); 
	}

/*#if 0
	// !!!GAC EXPERIMENTAL code to find the closest light to a node containing geometry
	// Doing it this way the light positions will be one frame behind all the other objects.
	// Avoiding this requires an extra scene-graph traversal

	// Only do this if there is geometry in this node
	if(Geometry)
	{
		float nearistLightInstanceDistance = FLT_MAX;
		NearistInstanceLight = 0;
		CrtScene *theScene = _CrtRender.GetScene();
		for(CrtInt i=0; i < theScene->NumLightInstances; i++)
		{
			CrtInstanceLight *instanceLight = theScene->GetLightInstances()[i];
			float *lightMatrix = (float *)instanceLight->Parent->GetLocalToWorldMatrix();
			float a = LocalToWorldMatrix[12] - lightMatrix[12];
			float b = LocalToWorldMatrix[13] - lightMatrix[13];
			float c = LocalToWorldMatrix[14] - lightMatrix[14];
			a = (a*a) + (b*b) + (c*c);
			if(a < nearistLightInstanceDistance)
			{
				NearistInstanceLight = instanceLight;
			}
		}
	}
#endif
*/
	// Create matrices that are later used by shaders, these updates should probably eventually be made lazy
	// NOTE: Because CrtMatrix3x4Invert doesn't copy over the last row to the output matrix, we have to do it.
	InverseLocalToWorldMatrix[M30] = InverseLocalToWorldMatrix[M31] = InverseLocalToWorldMatrix[M32] = 0.0;
	InverseLocalToWorldMatrix[M33] = 1.0;
	CrtMatrix3x4Invert( LocalToWorldMatrix, InverseLocalToWorldMatrix);
	CrtMatrixTranspose( InverseLocalToWorldMatrix, InverseTransposeLocalToWorldMatrix );
}

void CrtNode::BuildLWMatrix( CrtFloat time )
{
	// Update all the transforms that have animations (if any) 
	
	if ( NumAnimations > 0 && time >= 0 )
	{	
		// update each animated transform 
		for ( CrtUInt t = 0; t < Transforms.size(); t++)
		{
			if ( Transforms[t]->GetAnimation() )
			{
				// a single transform may have serveral animated channels attached 
				for ( CrtUInt c = 0; c < Transforms[t]->GetNumChannels(); c++)
				{
					CrtUInt channelIdx = Transforms[t]->GetChannelId(c); 
					CrtAnimation * Anim = Transforms[t]->GetAnimation(); 
					// interpolates the value based on the element that is to be targeted and the key frame info attached at load time
					Anim->AnimateChannel( Transforms[t], Anim->Channels[channelIdx]->GetTarget(), channelIdx, time );
//					Anim->AnimateChannel( Transforms[t]->GetVecTrans(), Anim->Channels[channelIdx]->GetTargetElement2(), channelIdx, time );
				}
			}
		}
		UpdateLocalMatrix = CrtTrue; // force update if there were animations
	} 
	
	// Process all the transforms in this node down into a single local transform matrix
	// !!!GAC if we aren't animating, this doesn't need to be calculated every time.

	if(UpdateLocalMatrix)
	{
		CrtMatrixLoadIdentity( LocalMatrix ); 
		for ( CrtUInt t = 0; t < Transforms.size(); t++)
		{
			switch ( Transforms[t]->GetType() )
			{
			case eCrtRotate:
				CrtMatrix4x4RotateAngleAxis( LocalMatrix, Transforms[t]->GetVecTrans() );									
				break;

			case eCrtTranslate:
				CrtMatrix4x4Translate( LocalMatrix, Transforms[t]->GetVecTrans() );									
				break;

			case eCrtScale:
				CrtMatrix4x4Scale( LocalMatrix, Transforms[t]->GetVecTrans() );									
				break;
			case eCrtMatrix:
				CrtMatrix4x4Mult( (CrtFloat *) Transforms[t]->GetMatrix() , LocalMatrix );
				break;
			case eCrtSkew:
			case eCrtLookAt:
			case eCrtTFormUnknown:
			default:
//				CrtPrint("*** This type of Transform is not supported yet\n"); // Do Later
				break; 
			}
		}
		UpdateLocalMatrix = CrtFalse;
	}

	if ( Parent )
	{
		// The node has a parent, concatenate the local transform matrix with it to fom our local-to-world
		// !!!GAC If we had a "parent is dirty" flag we could avoid this multiply a lot of the time
		CrtMatrixMult( LocalMatrix, Parent->LocalToWorldMatrix, LocalToWorldMatrix); 
	}
	else
	{
		// The node doesn't have a parent, so the local transform matrix is our local-to-world
		CrtMatrixCopy( LocalMatrix, LocalToWorldMatrix ); 		
	}

	// if this node is used for skinning need to update the combined 
	// bind inverse local to world matrix with the current local to world matrix 			
	if ( UsedForSkinning )
	{
		CrtMatrixMult( InverseBindMatrix, LocalToWorldMatrix, CombinedSkinMatrix ); 
	}
	else
		OrientReady = CrtTrue; 
}

void CrtNode::UpdateOrient( CrtFloat time )
{
	// update data with stored orient or if present by 
	//	it's animation  
	
/*	if ( !UpdateLocalToWorldMatrix )
	{
		// !!!GAC In the current code UpdateLocalToWorldMatrix is always true so this never executes
		// need to update the skin matrix if this is 
		// used for skinning 
		if ( UsedForSkinning )
			CrtMatrixMult( InverseBindMatrix, LocalToWorldMatrix, CombinedSkinMatrix ); 
				
		// either way return here 
		// job done zug zug 
		return; 
	}	
*/	BuildLWMatrix(time); 
	return; 
}

void 	CrtNode::Render()
{
	//CrtPrint(" Rendering Node %s Type %d \n", Name, (CrtInt32)Type ); 
	if ( _CrtRender.GetShowHiearchy() )
		DrawLineToChildren();
	
	// to concate to the camera which should be already set 	
	_CrtRender.PushMatrix();
	//CrtMatrixLoadIdentity(LocalToWorldMatrix);
	_CrtRender.MultMatrix(LocalToWorldMatrix); 
	_CrtRender.SetCurrentLMMat( LocalToWorldMatrix ); 
			
	for(CrtUInt i=0; i<InstanceGeometries.size(); i++)
	{
		CrtGeometry * geometry = InstanceGeometries[i]->AbstractGeometry;
		geometry->Draw(this, InstanceGeometries[i]);
//		gNumTris += geometry->GetTotalNumTris();
	}
	for(CrtUInt i=0; i<InstanceControllers.size(); i++)
	{
//		CrtGeometry * geometry = InstanceControllers[i]->AbstractGeometry;
		CrtController * controller = InstanceControllers[i]->AbstractController;
//		if (controller->GetRenderReady() == CrtTrue)
		controller->Draw(this, InstanceControllers[i]);
//		gNumTris += geometry->GetTotalNumTris();
	}
	//CrtPrint(" %s Rendering Children \n", Name ); 
	_CrtRender.PopMatrix(); 

    // Render All Children 
	if (Children)
		Children->Render();
	// Render All Siblings 
	if (Next)
		Next->Render(); 
}
/*
CrtNode*	CrtNode::GetNode( CrtChar * nodeName )
{
	// Check the current node if the name matches 
	if ( CrtCmpIn( Name, nodeName ))
		return this;

	CrtNode * node = NULL; 

	// Check the children if the name matches 
	if( Children )
		node = Children->GetNode( nodeName ); 

	if ( node )
		return node; 

	// Check the siblings if the name matches 
	if( Next )
		node = Next->GetNode( nodeName ); 
    	
	return node; 	
}; 

CrtNode*	CrtNode::GetNode( CrtInt nodeIdx )
{
	// Check the current node if the name matches 
	if ( nodeIdx == Idx )
		return this;

	CrtNode * node = NULL; 

	// Check the children if the name matches 
	if( Children )
		node = Children->GetNode( nodeIdx ); 

	if ( node )
		return node; 

	// Check the siblings if the name matches 
	if( Next )
		node = Next->GetNode( nodeIdx ); 
    	
	return node; 	
}; 
*/


/*

// I am considering switching to quats for the runtime but it's a work in progress 
// so please ignore for now 


//reading a rotation or computing a rotation from the animation channels:
//v is a vector which contains the X, Y and Z euler angles
//Code:
CrtQuat Rotate( CrtVec3f v )
{

	CrtVec3f tmp;
	CrtQuat qX; 
	CrtQuat qY; 
	CrtQuat qZ; 

	qX.Rotate( CrtVec3f(1.0f,0.0f,0.0f),v.x*CrtPI/180.0f);
	qY.Rotate( CrtVec3f(0.0f,1.0f,0.0f),v.y*CrtPI/180.0f);	
	qZ.Rotate( CrtVec3f(0.0f,0.0f,1.0f),v.z*CrtPI/180.0f);

	CrtQuat res = qZ*(qY*qX);
	return res; 
}

void CrtQuat::Rotate( CrtVec3f axis, CrtFloat theta )
{
	//If the axis of rotation is        (ax, ay, az)- must be a unit vector
	//and the angle is                  theta (radians)

	s   =   (CrtFloat)cos(theta/2);
	x   =   axis.x * (CrtFloat)sin(theta/2);
	y   =   axis.y * (CrtFloat)sin(theta/2);
	z   =   axis.z * (CrtFloat)sin(theta/2);
}
*/

