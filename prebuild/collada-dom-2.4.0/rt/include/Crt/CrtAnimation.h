/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_ANIMATION_H_
#define _CRT_ANIMATION_H_

#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"

class CrtAnimSrc;
class CrtAnimSampler: public CrtBase
{
public:
	CrtAnimSampler() {};
	~CrtAnimSampler() 
	{
	};
	CrtChar		ID[CRT_MAX_ID_SIZE];
	map<string, CrtAnimSrc*> inputs;
};

class CrtAnimSrc: public CrtBase
{
protected:
	CrtChar		ID[CRT_MAX_ID_SIZE];
	CrtFloat	*	Array; 
	CrtUInt		Count; 
	
public:
friend class CrtScene; 
friend class CrtAnimation; 

	CrtAnimSrc()
	{
		ID[0] = 0;
		Array = NULL; 
		Count = 0; 
	}

	~CrtAnimSrc()
	{
		CrtDeleteData( Array ); 
	}
	
	CrtBool	Parse( const CrtChar * data );
	inline	CrtVoid	SetCount( CrtInt c ){ Count = c; };
};

enum CrtAnimTarget
{
	eAnimTargetX,
	eAnimTargetY,
	eAnimTargetZ,
	eAnimTargetXYZ, 
	eAnimTargetAngle,

	eTargetUnknown, 

	eRotXAxis,
	eRotYAxis,
	eRotZAxis,
	eRotate, 

	eScaleXAxis,
	eScaleYAxis,
	eScaleZAxis,
	eScale,

	eTransXAxis,
	eTransYAxis,
	eTransZAxis, 
	eTranslate, 

/*	eJointOrientX,
	eJointOrientY,
	eJointOrientZ,

	eRotateAxisX,
	eRotateAxisY,
	eRotateAxisZ,
*/
	eSource,
	eMatrix,
};

const CrtUInt MAX_KEY_SETS = 10;
struct CrtKeySet; 
class CrtAnimChannel: public CrtBase 
{
protected:

	CrtAnimSampler*  Sampler;
	
	CrtChar			TargetIDStr[CRT_MAX_ID_SIZE];
	CrtChar			TargetSIDStr[CRT_MAX_SID_SIZE];	
	CrtChar			TargetMemberStr[CRT_MAX_NAME_SIZE];	

	CrtAnimTarget	Target;
//	CrtAnimTarget	TargetElement;
	
	CrtAnimSrc *	InputSrcPtr; 
	CrtAnimSrc *	OutputSrcPtr; 

	CrtKeySet  *	Keys[MAX_KEY_SETS]; 
	CrtUInt			NumElementTargets; 

public:
	friend class CrtScene; 
	friend class CrtAnimation; 
	CrtAnimChannel()
	{
		InputSrcPtr = 0;
		OutputSrcPtr = 0; 	

		Sampler = NULL; 
		memset(TargetIDStr, 0 , CRT_MAX_ID_SIZE);
		memset(TargetSIDStr, 0 , CRT_MAX_SID_SIZE);
		memset(TargetMemberStr, 0 , CRT_MAX_NAME_SIZE);
		NumElementTargets = 0; 

		Target = eTargetUnknown;

		for ( CrtUInt i = 0; i < MAX_KEY_SETS; i++)
			Keys[i] = NULL; 
	}

	~CrtAnimChannel()
	{
				
	}

//	inline CrtVoid	SetInput( const CrtChar * input )		{ CrtCpy( Input, input ); };  
//	inline CrtVoid	SetOutput( const CrtChar * output )		{ CrtCpy( Output, output ); }; 
	inline CrtVoid	SetTargetID( const CrtChar * target )	{ CrtCpy( TargetIDStr, target); }
	inline CrtVoid	SetTargetSID ( const CrtChar * target ) { CrtCpy( TargetSIDStr, target); }
	inline CrtVoid	SetSampler( CrtAnimSampler * sampler )	{ Sampler=sampler;/*CrtCpy( Sampler, sampler);*/}
//	inline CrtVoid	SetChannel( const CrtChar * channel )	{ CrtCpy( Channel, channel); }
	inline CrtVoid	SetInputSrc( CrtAnimSrc * inputPtr )	{ InputSrcPtr = inputPtr; }
	inline CrtVoid	SetOutputSrc( CrtAnimSrc * outputPtr )	{ OutputSrcPtr = outputPtr; }
	inline CrtVoid	SetTargetMember( const CrtChar*target )	{ CrtCpy( TargetMemberStr, target); }
	inline CrtVoid	SetTarget( CrtAnimTarget target )		{ Target = target; }
	
	inline CrtVoid	SetKeySet( CrtKeySet * set, CrtUInt i ) 
	{ 
		if (i<MAX_KEY_SETS)
			Keys[i] = set; 	
	}

	inline CrtAnimSampler * GetSampler(){return Sampler; }
//	inline CrtChar * GetChannel(){return Channel; }	
//	inline CrtChar * GetInput(){return Input;    }	
//	inline CrtChar * GetOutput(){return Output; }
	
	inline CrtAnimTarget GetTarget(){ return Target; }
//	inline CrtAnimTarget GetTargetElement2(){ return TargetElement; }
	inline CrtChar *	GetTargetID() { return TargetIDStr; }
	inline CrtChar *	GetTargetSID(){ return TargetSIDStr; }
	inline CrtChar *	GetTargetMember(){ return TargetMemberStr; }
	inline CrtUInt		GetNumElementTargets(){ return NumElementTargets; }
//	inline CrtAnimTarget	GetTarget() { return Target;}

	CrtBool		AddSrc( CrtAnimSrc * channel );
	CrtFloat	Interpolate( CrtFloat time ); 
//	CrtVoid		ParseElement( const CrtChar * id, const CrtChar * sid ); 
//	CrtVoid		ParseElementTarget( CrtChar * elementTargetString );


};

struct CrtAnimKey
{
	CrtOrient	Orient;
	CrtFloat		Time; 
	CrtBool		HasRot; 
	CrtBool		HasTrans; 

	CrtAnimKey()
	{
		Orient = CrtOrient(); 
		Time = 0; 
		HasRot = CrtFalse;  
		HasTrans = CrtFalse;  
	}
};

struct	CrtKeySet
{
	CrtInt			NumKeys; 
	CrtFloat		*	Keys; 
	CrtFloat		*	Time; 	

	CrtKeySet()
	{
		NumKeys = 0; 
		Keys = NULL;
		Time = NULL; 
	}

	CrtBool	AllocateKeys( CrtInt nKeys )
	{
		Keys = CrtNewData( CrtFloat, nKeys ); 	
		Time = CrtNewData( CrtFloat, nKeys ); 
		NumKeys = nKeys; 

		for( int i = 0; i < nKeys; i++)
		{
			Keys[i] = 0;
		}
		return CrtTrue; 
	}
	CrtVoid DeallocateKeys( )
	{
		CrtDeleteData(Keys);
		CrtDeleteData(Time);
	}
};

class CrtAnimation: public CrtBase 
{
	CrtUInt			NumAnimChannels; // multipule elements can be targeted by one channel. 

	CrtKeySet *		AnimKeySets; 
	CrtFloat		EndTime; 

	CrtFloat		SampleRate; 
	
	map<string, CrtAnimSampler*> Samplers; 
	map<string, CrtAnimSrc*> Sources; 

	CrtChar		    Target[CRT_MAX_NAME_SIZE]; 
	CrtChar			TargetTransform[CRT_MAX_NAME_SIZE];
	CrtChar			TargetTransformElement[CRT_MAX_NAME_SIZE]; 

	// export data 
	CrtAnimKey	  * Keys; 
	CrtInt			NumKeys; 
	
	CrtBool			HasRotation;
	CrtBool			HasTranslation; 
	CrtBool			HasScale;
	CrtBool			HasSource;
	CrtBool			HasMatrix;
	CrtBool			FoundTarget; 
protected:
	vector<CrtAnimChannel*> Channels;

public:

	CrtAnimation()
	{	
		AnimKeySets = NULL;  
		NumAnimChannels=0; 
		SampleRate = 30; // fps; 

		Target[0]	= 0; 

		Keys = NULL;
		NumKeys = 0; 

		HasRotation = CrtFalse;
		HasTranslation = CrtFalse; 
		HasScale = CrtFalse; 
		HasMatrix = CrtFalse;
		EndTime		= 0; 

		FoundTarget = CrtFalse; 	
	}
	
	~CrtAnimation();

protected:
	friend class CrtScene; 
	friend class CrtNode; 
	friend class CrtMorph;

	CrtVoid	ResolveChannel(CrtAnimSampler * sampler, const char * target ); 
	
	inline  CrtVoid  SetTarget( const CrtChar * t ){CrtCpy(Target,t); }
	inline	CrtChar * GetTarget(){	return Target; }
	inline	CrtChar * GetTransformTarget(){	return TargetTransform; }
	inline	CrtChar * GetTransformTargetElement(){	return TargetTransformElement; }

	CrtVoid	GenerateKeys();

	CrtVoid 	AnimateChannel( CrtTransform * transform, CrtAnimTarget target, CrtUInt c, CrtFloat time );
	CrtVoid 	AnimateChannelFloat( CrtFloat & value, CrtAnimTarget target, CrtUInt c, CrtFloat time );

    CrtVoid		Interp( CrtFloat & val, CrtKeySet * keySet, CrtFloat time );

	CrtVoid 	SetFoundTarget() { FoundTarget = CrtTrue; };
	CrtBool		HasFoundTarget() { return FoundTarget; }
public:
	

	//---------------------- External Interfaces ---------------------------------//	
	inline	CrtBool		HasAxisRots(){ return HasRotation; } 
	inline	CrtBool		HasTrans(){ return HasTranslation; }
	inline	CrtBool		HasScales(){ return HasScale; }

	inline  CrtFloat		GetEndTime(){ return EndTime; }; 

	//---------------------- End External Interfaces ---------------------------------//


};

#endif 

