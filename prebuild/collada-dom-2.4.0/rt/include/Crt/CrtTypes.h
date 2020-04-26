/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_TYPES_
#define _CRT_TYPES_

//#include <dae/daeTypes.h>

#define CrtChar     char 
#define CrtByte		char
#define CrtInt32	int
#define CrtInt16	short
#define CrtInt       CrtInt32 

#define CrtUChar	unsigned CrtByte
#define CrtUByte	unsigned CrtByte
#define CrtUInt32	unsigned CrtInt32
#define CrtUInt16	unsigned CrtInt16
#define CrtUInt		CrtUInt32

#define CrtFloat     float
#define CrtDouble    double 

#define CrtVoid		void 
#define CrtError	CrtInt32

enum CrtBool
{
	CrtFalse = 0,
	CrtTrue
};

typedef CrtFloat CrtMatrix[16];
typedef CrtFloat CrtMatrix3x4[12];

const int MAX_STRING_SIZE = 256; 

#define CrtError_OK			0
#define CrtError_Skin		-10


#endif // _CRT_TYPES_

