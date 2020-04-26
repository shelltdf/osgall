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
#include "Crt/CrtMorph.h"
#include "Crt/CrtGeometry.h"
#include "Crt/CrtAnimation.h"


CrtMorph::CrtMorph () 
{
	base_geometry = NULL;
	morph_type=MORPH_NORMAL_TYPE;
}
CrtMorph::~CrtMorph () 
{
}

CrtInt TargetMemberToIndex(const CrtChar * target_member)
{
	CrtChar index_str[CRT_MAX_TEMP_DATA];
	memset(index_str, 0, CRT_MAX_TEMP_DATA);
	for (int i=0; i<CrtLen(target_member); i++)
	{
		if (isdigit(target_member[i]))
		{
			index_str[i] = target_member[i];
		} else {
			break;
		}
	}
	return atoi(index_str);
}

CrtVoid	CrtMorph::Update( CrtFloat time)
{
	if (time != -1.0f)
	{
		if (Animations.size() > 0) 
		{
			for (size_t a=0; a<Animations.size(); a++)
			{
				CrtAnimation * Anim = Animations[a];
				for (size_t i=0; i<Anim->Channels.size(); i++)
				{
					CrtAnimChannel * channel = Anim->Channels[i];
					CrtChar * target_member = channel->GetTargetMember();
					CrtInt TargetMemberIndex = TargetMemberToIndex(target_member);
					Anim->AnimateChannelFloat(weights[TargetMemberIndex], Anim->Channels[i]->GetTarget(), 0, time);
				}
			}
		} else {
			return;
		}
	}

	// calculate one_minus_sum_of_weights
	CrtFloat base_weight=0;
	if (morph_type == MORPH_NORMAL_TYPE)
	{
		for(size_t i=0; i<weights.size(); i++)
		{
			base_weight += weights[i];
		}
		base_weight = 1.0f - base_weight;
	} else {
		base_weight = 1.0f;
	}


	for(size_t i=0; i<morphed_geometry->vertexcount; i++)
	{
		// morph points
		morphed_geometry->Points[i] = base_geometry->Points[i] * base_weight;
		for(size_t j=0; j<targets.size(); j++)
			morphed_geometry->Points[i] += targets[j]->Points[i] *  weights[j];

		// morph points
		morphed_geometry->Normals[i] = base_geometry->Normals[i] * base_weight;
		for(size_t j=0; j<targets.size(); j++)
			morphed_geometry->Normals[i] += targets[j]->Normals[i] *  weights[j];
	}
}

CrtVoid	CrtMorph::Draw(CrtNode *parentNode, CrtInstance * instance)
{
	morphed_geometry->Draw(parentNode, instance);
}

