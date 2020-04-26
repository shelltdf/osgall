/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_NODE_H_
#define _CRT_NODE_H_

#include "Crt/CrtTypes.h"
#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtGeometry.h"
#include <vector>

class CrtCamera;
class CrtInstanceCamera;
class CrtGeometry;
class CrtLight;
class CrtInstanceLight;
class CrtInstanceGeometry;
class CrtInstanceController;

// --------------------- //
// Scene storage types   //
// --------------------- // 

const CrtInt MAX_ANIMS = 10; 

/**
 * This class represents a node in the RT scene graph
 */
class CrtNode: public CrtOrient 
{
	CrtNode				*Next;
//	CrtInstanceCamera	*InstanceCamera;	// REFACTOR IN PROGRESS needs to support multiple instances
//	CrtInstanceLight	*InstanceLight;		// REFACTOR IN PROGRESS needs to support multiple instances
	CrtInstanceLight	*NearistInstanceLight;
//	CrtGeometry			*Geometry;			// REFACTOR IN PROGRESS needs to support multiple instances
	CrtAnimation		*Animations[MAX_ANIMS];
	CrtInt				NumAnimations; 
	CrtNode				*Parent; 
	CrtNode				*Children;
	CrtInt32			NumChildren;
	CrtBool				UsedForSkinning; 
	CrtBool				InverseBindMatrixReady; 
	CrtBool 			OrientReady;
	CrtInt				Idx; 
	CrtInt				ParentIdx; 
	CrtInt				Depth; 
public:
//	std::vector <CrtInstanceCamera *> InstanceCameras;  
	std::vector <CrtInstanceGeometry *> InstanceGeometries;  
	std::vector <CrtInstanceController *> InstanceControllers;
	std::vector <CrtInstanceLight *> InstanceLights;

public:

	CrtNode():
	  Next(NULL),
	  NearistInstanceLight(NULL),
	  NumAnimations(0),
	  Parent(NULL),
	  Children(NULL),
	  NumChildren(0),
	  UsedForSkinning(CrtFalse),
	  InverseBindMatrixReady(CrtFalse),
	  OrientReady(CrtFalse),
	  Idx(-1),
	  ParentIdx(-1),
	  Depth(0)
	{
		Name[0] = 0;
		Id[0] = 0;
		Sid[0] = 0;
	}

	~CrtNode()
	{
		Destroy(); 
	}


	void	Destroy()
	{
		// need to delete any children or siblings
		if (Children)
			CrtDelete( Children ); 		
		if (Next)
			CrtDelete( Next ); 
	}


protected:
	friend class CrtScene; 
	friend class CrtSkin;

	inline	CrtNode	*SetNext( CrtNode * next )	{ Next = next; return this;}
	inline	void	SetName( const CrtChar * n ){ CrtCpy(Name, n); }
	inline	void	SetId( const CrtChar * n ){ CrtCpy(Id, n); }
	inline	void	SetSid( const CrtChar * n ){ CrtCpy(Sid, n); }
	inline	void	SetParent( CrtNode * node)	{ Parent = node; };

	void	SetChild( CrtNode * );
	void	AddObjectReference( CrtChar * name ); 	

	void	AddChild( CrtNode * node )
	{
		if (!Children )
		{
			Children = node; 
		}
		else
		{
			node->SetNext( Children ); 
			Children = node; 
		}
		NumChildren++; 

	}

//	inline	void	SetGeometry( CrtGeometry * geo ) { Geometry = geo; }
	inline	void	SetAnimation( CrtAnimation * anim ) { Animations[NumAnimations] = anim; NumAnimations++; }
	inline	void	SetUsedForSkinning() { UsedForSkinning = CrtTrue; } 
	inline	void	SetIdx( CrtInt id ){ Idx = id; }
	inline	void	SetParentIdx( CrtInt id ){ Idx = id; }
	inline	void	SetInverseBindMatrix( CrtMatrix imat )
	{
		memcpy( InverseBindMatrix, imat, sizeof( CrtMatrix ) ) ; 
		InverseBindMatrixReady = CrtTrue; 
	}
	
	void	DrawLineToChildren(); 
	

public:

	//---------------------- External Interfaces ---------------------------------//	
	// After you get the SceneRoot from the CrtScene.GetSceneRoot 
	// got throught he nodes and just check the type 
	// if the type is Crt_Geo the GetGeometry(); 
	// from there you will need to get the GeoGroups which will contain the materials,
	// Points, Normals, and the rest of the Geo data.  
	// To get the next sibling it's simply node = node->GetNext();  
	// until the node is null.  
	// Node there may also be animation attached to the node as well as geometry
	// so Do a GetAnimation to check for animation attached.  If not null try and get 
	// the Animation keys.  
	// Currently Animation refactoring isn't done but hope to have that completed soon. 
	// Finaly to get the orient check CrtData.h CrtOrient definition.  It describes 
	// how to handle getting a camera orient verse getting a regular node orient.  

	inline	CrtNode		*	GetNext() { return Next; }
	inline	CrtChar		*	GetName(){ return Name; }
	inline	CrtChar		*	GetId(){ return Id; }
	inline	CrtChar		*	GetSid(){ return Sid; }
//	inline	CrtNode		*	GetParent(){ return Parent; }
	inline	CrtNode		*	GetChild(){ return Children; }
	
//	inline	CrtGeometry *	GetGeometry(){ return Geometry; }
	inline	CrtInt			GetNumAnimations(){ return NumAnimations; } 
	inline	CrtAnimation*	GetAnimation( CrtInt i){return Animations[i]; }
	inline	CrtInt			GetIdx(){ return Idx; } 
//	inline	CrtInt			GetParentIdx(){ return ParentIdx; }
	inline  CrtInt			GetDepth(){ return Depth; }
	inline  CrtBool			GetIsUsedForSkinning(){ return UsedForSkinning; } 
		
	inline	CrtInt		GetNumChildren()
	{
		CrtInt	num = 0; 
		CrtNode * child = Children; 
		while( child )
		{
			num++;
			child = child->GetNext(); 
		}
		return num; 
	}

/*	inline void 		SetDepth()
	{
		Depth = 1; 
		CrtNode * par = Parent; 
		while ( par )
		{
			Depth++; 
			par = par->GetParent(); 
		}
	}
*/
//	CrtNode		*   GetNode( CrtChar * node ); 
//	CrtNode		*   GetNode( CrtInt nodeIdx ); 
	void	Update( CrtFloat time);
	void	UpdateOrient( CrtFloat time); 
	void	BuildLWMatrix( CrtFloat time );
	void	Render(); 
	
	//---------------------- End External Interfaces ---------------------------------//	

};

#endif // _CRT_NODE_H_


