/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_SKIN_DATA_H_
#define _CRT_SKIN_DATA_H_

#include "Crt/CrtGeometry.h"
#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtTypes.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtController.h"

class CrtController;

class CrtSkinWeight
{
public:
	CrtUInt		NumWeights;
	CrtFloat	*	Weights;
	CrtUInt		*	Joints; 
	
	CrtSkinWeight()
	{
		NumWeights = 0;
		Weights = NULL;
		Joints = NULL;
	}

	~CrtSkinWeight()
	{
		CrtDeleteData( Weights ); 
		CrtDeleteData( Joints ); 
	}

	// temp data for parsing 
/*	CrtUInt	*	WeightIndices; 
	CrtUInt	*	JointIndices; 

	inline void FreeTempData() 
	{
		CrtDeleteData( WeightIndices );
		CrtDeleteData( JointIndices ); 		
	}

	inline void FreeWeightData()
	{
		CrtDeleteData( Weights ); 
		CrtDeleteData( Joints ); 
	}
*/
	// parses and allocates weights and indices 
//	void	Parse( const CrtChar * data ); 
//	void	ResolveIndices( const CrtFloat * weights, CrtInt wCount, const CrtInt * joints, CrtInt jCount );
	void	SortWeights();
};

class CrtNode; 
class CrtJoint
{
public:
	CrtJoint() {JointNode=NULL;};
	CrtChar	Name[CRT_MAX_NAME_SIZE];	
	CrtNode *JointNode;	
	enum CrtJointsType { CrtJointsType_SID, CrtJointsType_ID};
};

class CrtGeometry; 

class CrtSkin: public CrtController 
{
protected:

	CrtJoint		 *	Joints;
	CrtJoint::CrtJointsType	JointsType;
	CrtBool			RenderReady;
	CrtSkinWeight *	Weights;

	CrtMatrix		 BindShapeMats;
	CrtMatrix	 *  IBindMats;
	CrtMatrix	 *  SkinningMats; 
//	CrtMatrix3x4  *  SkinningMats3x4;

	CrtUInt			WeightCount; 
	CrtUInt			JointCount;
	CrtUInt			IBindMatCount;
	CrtGeometry *	skinned_geometry;
	CrtGeometry *   base_geometry;
	CrtUInt			skin_animation_count;
	
	friend class CrtGeometry; 
	friend class CrtScene; 
public:

	CrtSkin()
	{
		WeightCount = 0; 
		JointCount = 0; 
		IBindMatCount = 0; 
		RenderReady = CrtFalse;
		Joints = NULL; 
		Weights = NULL; 
		CrtMatrixLoadIdentity(BindShapeMats); 
		IBindMats = NULL; 
		SkinningMats = NULL; 
//		SkinningMats3x4 = NULL; 
		skin_animation_count = 0;
		Id[0] = 0; 

		base_geometry = NULL;
		skinned_geometry = NULL; 
	}

	~CrtSkin();

	inline	CrtGeometry	*GetGeometry()								{ return base_geometry; } 
	inline	CrtBool		GetRenderReady()	{ return RenderReady;} 

	void	AddBindPos( CrtChar * BindPosArray, CrtInt nValues );

	void	AddIBindMats( CrtMatrix * iBindMatArray, CrtInt nValues );

	CrtVoid FindJointNodes( CrtNode * sceneRoot ); 
	CrtNode* FindJointNode( CrtNode * sceneRoot, CrtChar * name );

	void	UpdateCombinedMats(); 

	inline	CrtInt		GetWeightCount()	{ return WeightCount; }
	inline	CrtJoint	*GetJoints()		{ return Joints; }
	inline  CrtInt		GetJointCount()		{ return JointCount; } 

	virtual CrtVoid	Update( CrtFloat time);
	virtual CrtVoid	Draw(CrtNode *parentNode, CrtInstance * instance);
};
#endif //_CRT_SKIN_DATA_H_



