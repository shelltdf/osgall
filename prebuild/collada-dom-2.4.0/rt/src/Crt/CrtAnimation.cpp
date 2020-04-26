/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtMatrix.h"
#include "Crt/CrtAnimation.h"
#include "Crt/CrtUtils.h"
#include "dae/daeSIDResolver.h"
#include <string>

CrtBool	CrtAnimSrc::Parse( const CrtChar * data )
{
	// parse data 
	const CrtChar * d = data; 
			
	// allocate for the keys 
	Array = CrtNewData( CrtFloat, Count ); 

	// copy the data 
	for( CrtUInt i = 0; i < Count; i++)
	{
		Array[i] = CrtGetFloatString( &d ); 
		//CrtPrint(" coping %d ",i);fflush(stdout);
	}

	return CrtTrue; 
}

CrtBool	CrtAnimChannel::AddSrc( CrtAnimSrc * channel )
{
	(void)channel;

	return CrtTrue; 
}

CrtFloat	CrtAnimChannel::Interpolate( CrtFloat time )
{
	(void)time; 
	float val = 0; 
	return val; 
}

const int ANIM_SAMPLE_RATE = 30; 

CrtVoid	CrtAnimation::GenerateKeys()
{
	CrtPrint("Generating Keys for Animation Channel %s \n", Name ); 

	// generate key frames for the channels in this animation 	
	CrtAnimChannel * chan = Channels[0]; 
	
	// allocating for generic key channels New way 
	AnimKeySets = CrtNewData( CrtKeySet, NumAnimChannels ); 
	//CrtPrint("AnimKeySets = %x,time\n", &AnimKeySets.Keys, &AnimKeySets.Time ); 
	CrtUInt animSet = 0; 
	for ( CrtUInt i = 0; i < Channels.size(); i++)
	{
		chan = Channels[i];
		CrtUInt NumKeys = chan->InputSrcPtr->Count;
		
		// new way //
		for ( CrtUInt t = 0; t < chan->GetNumElementTargets(); t++)
		{
			AnimKeySets[animSet+t].AllocateKeys( NumKeys ); 
		}
		
		// set the actual key info //
		for ( CrtUInt i = 0 ; i < NumKeys; i ++)
		{
			// fill in all the keys for each anim key set 
			CrtUInt numCh = chan->GetNumElementTargets();

			for ( CrtUInt ch = 0; ch < chan->GetNumElementTargets(); ch ++)
			{
				AnimKeySets[animSet+ch].Time[i] = chan->InputSrcPtr->Array[i];
				AnimKeySets[animSet+ch].Keys[i] = chan->OutputSrcPtr->Array[i*numCh+ch];
				
				if ( AnimKeySets[animSet+ch].Time[i] > EndTime )
					EndTime = AnimKeySets[animSet+ch].Time[i]; 

				// set the animKey in the channel for later interpolation 
				chan->SetKeySet( &AnimKeySets[animSet+ch], ch );  
			}			
		}

		// update the current animSet 
		animSet += chan->GetNumElementTargets(); 
	}
};

CrtVoid	CrtAnimation::Interp( CrtFloat & val, CrtKeySet * keySet, CrtFloat time )
{
	if ( ! keySet->Keys )
		return; 

	if ( time > keySet->Time[keySet->NumKeys-1] )
		val = keySet->Keys[keySet->NumKeys-1]; 
	else
	{
		// here need to get an actual interpolated value for this channel but for not just 
		// goint to return the first anim value 
		val = keySet->Keys[0];

		// need to first find out where the time lies in the keys 
		int next = -1;  
		int prev = -1;  
		for( int i = 0; i < keySet->NumKeys - 1; i ++)
		{
			if ( time >= keySet->Time[i] && time < keySet->Time[i+1] )
			{
				prev = i;
				next = i+1;
				break; 
			}
		}			

		if ( prev == -1 )
		{
			if ( time < keySet->Time[0] )
				val = keySet->Keys[0];
			else
				// if time is not in range just set to last key value 
				val = keySet->Keys[keySet->NumKeys-1];
		}
		else
		{	
			CrtFloat tSize = keySet->Time[next] - keySet->Time[prev];
			CrtFloat tDiff = time - keySet->Time[prev]; 
			CrtFloat tFactor = 1-((tSize-tDiff)/tSize);
			
			CrtFloat vSize = keySet->Keys[next] - keySet->Keys[prev];
			val = keySet->Keys[prev] + (vSize * tFactor);
		}


	}
}
CrtVoid 	CrtAnimation::AnimateChannelFloat( CrtFloat & value, CrtAnimTarget target, CrtUInt c, CrtFloat time )
{
	switch(target)
	{
	case eSource:
		Interp(value, &AnimKeySets[c], time);
		break;
	default:
		CrtPrint("Animation Target not found\n");
		break;
	}
}

CrtVoid 	CrtAnimation::AnimateChannel( CrtTransform * transform, CrtAnimTarget target, CrtUInt c, CrtFloat time )
{
	CrtVec4f & vec = transform->GetVecTrans();
	switch( target )
	{
	// new way 
	case eAnimTargetX:
	case eScaleXAxis:
	case eTransXAxis:
		Interp(vec.x, &AnimKeySets[c], time ); 
	break; 
	case eAnimTargetY:
	case eScaleYAxis:
	case eTransYAxis:
		Interp(vec.y, &AnimKeySets[c], time ); 
	break; 
	case eAnimTargetZ:
	case eScaleZAxis:
	case eTransZAxis:
		Interp(vec.z, &AnimKeySets[c], time ); 
	break; 
	case eRotXAxis:
	case eRotYAxis:
	case eRotZAxis:
	case eAnimTargetAngle: 
		Interp(vec.w, &AnimKeySets[c], time ); 
	break; 
	case eTranslate:
	case eScale:
	case eAnimTargetXYZ:
		Interp(vec.x, &AnimKeySets[c], time );
		Interp(vec.y, &AnimKeySets[c+1], time );
		Interp(vec.z, &AnimKeySets[c+2], time );
		break; 
	case eMatrix:
		{
		CrtMatrix & matrix = transform->GetMatrix();
		for (int m=0; m<16; m++)
			Interp(matrix[m], &AnimKeySets[c+m], time );
		CrtMatrixTranspose(matrix, matrix);
		break; 
		}
	default:
		CrtPrint("Animation Target not found\n");
		break; 
	}	
}; 

CrtAnimation::~CrtAnimation()
{
	while(!Sources.empty())
	{
		std::map<std::string, CrtAnimSrc*>::iterator iter = Sources.begin();
		CrtDelete(iter->second);
		Sources.erase(iter);
	}
	while(!Samplers.empty())
	{
		std::map<std::string, CrtAnimSampler*>::iterator iter = Samplers.begin();
		CrtDelete(iter->second);
		Samplers.erase(iter);
	}
	while(!Channels.empty())
	{
		CrtDelete( Channels[0] );
		Channels.erase(Channels.begin());
	}

	for ( CrtUInt t = 0; t < NumAnimChannels; t++)
	{
		AnimKeySets[t].DeallocateKeys(); 
	}
	CrtDeleteData(AnimKeySets);
}