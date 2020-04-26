/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_MORPH_H_
#define _CRT_MORPH_H_

#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtTypes.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"
#include "Crt/CrtGeometry.h"
#include "Crt/CrtController.h"

enum MOPRH_TYPE
{
  MORPH_NORMAL_TYPE,
  MORPH_RELATIVE_TYPE,
  MORPH_TYPE_MAX_COUNT
};

class CrtMorph : public CrtController 
{
public:
	friend class CrtScene; 
	friend class CrtNode; 
	friend class CrtGeometry; 

	std::vector<CrtAnimation *>	Animations;

	CrtMorph ();
	~CrtMorph ();

	CrtVoid SetBaseGeometry(CrtGeometry * geo) {base_geometry = geo;};
	CrtGeometry * GetBaseGeometry() {return base_geometry;};

	CrtVoid SetMorphType(MOPRH_TYPE type) {morph_type = type;};
	MOPRH_TYPE GetMorphType() {return morph_type;};

	virtual CrtVoid	Update( CrtFloat time);
	virtual CrtVoid Draw(CrtNode *parentNode, CrtInstance * instance);

	vector<CrtGeometry*>	targets;
	vector<CrtFloat>		weights;
private:
	CrtGeometry *			base_geometry;
	MOPRH_TYPE				morph_type;
	CrtGeometry *			morphed_geometry;
};









#endif //_CRT_MORPH_H_

