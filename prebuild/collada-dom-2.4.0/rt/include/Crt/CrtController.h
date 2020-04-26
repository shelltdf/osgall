/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_CONTROLLER_H_
#define _CRT_CONTROLLER_H_

#include "Crt/CrtGeometry.h"
#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtTypes.h"
#include "Crt/CrtMatrix.h"
#include "Crt/CrtNode.h"

class CrtInstance;
class CrtController;

enum CrtController_Types
{
	CrtController_Skin_Types,
	CrtController_Morph_Types,
	CrtController_Max_Types,
};
class CrtInstanceController : public CrtInstance
{
public:
	CrtInstanceController() : AbstractController(0) {};
	~CrtInstanceController() {};
	CrtController * AbstractController;
//	CrtGeometry * AbstractGeometry;
};

class CrtController : public CrtBase
{
public:
	CrtController() {} ;
	virtual ~CrtController() {} ;

	virtual CrtVoid	Update( CrtFloat time) = 0;
	virtual CrtVoid	Draw(CrtNode *parentNode, CrtInstance * instance) = 0;
	CrtController_Types type;
	std::vector<string> skeletons;
};


#endif //_CRT_CONTROLLER_H_
