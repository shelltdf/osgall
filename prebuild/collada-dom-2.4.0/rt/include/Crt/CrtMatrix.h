/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_MATRIX_H_
#define _CRT_MATRIX_H_

#include "Crt/CrtTypes.h"
#include "Crt/CrtData.h"
#include <string.h>

inline void CrtMatrixLoadIdentity(CrtMatrix LDstMtx)
{
	LDstMtx[M00] = 1.0;LDstMtx[M01] = 0.0;LDstMtx[M02] = 0.0;LDstMtx[M03] = 0.0;
	LDstMtx[M10] = 0.0;LDstMtx[M11] = 1.0;LDstMtx[M12] = 0.0;LDstMtx[M13] = 0.0;
	LDstMtx[M20] = 0.0;LDstMtx[M21] = 0.0;LDstMtx[M22] = 1.0;LDstMtx[M23] = 0.0;
	LDstMtx[M30] = 0.0;LDstMtx[M31] = 0.0;LDstMtx[M32] = 0.0;LDstMtx[M33] = 1.0;
}
inline void CrtMatrixCopy( const CrtMatrix LSrcMtx, CrtMatrix LDstMtx)
{
	memcpy(LDstMtx, LSrcMtx, sizeof(CrtMatrix));
}

void CrtMatrixRotateTranslate(CrtMatrix LMatrix, CrtVec4f RotX, CrtVec4f RotY, CrtVec4f RotZ, CrtVec3f Trans );
void CrtMatrix3x4Invert(CrtMatrix LM, CrtMatrix LMI);
void CrtMatrix3x3TransformVV( CrtMatrix mMatrix,  
							CrtFloat mXIn, CrtFloat mYIn, CrtFloat mZIn, 
							CrtFloat &mXOut, CrtFloat &mYOut, CrtFloat &mZOut);
void CrtMatrixTranspose( const CrtMatrix LSrcMtx, CrtMatrix LDstMtx );
void CrtMatrixTranspose3x4( const CrtMatrix LSrcMtx, CrtMatrix3x4 LDstMtx );
void CrtMatrixToQuat( const CrtMatrix rotMat, CrtQuat & RotQuat ); 

inline void CrtMatrixTransform( CrtMatrix Matrix, const CrtVec3f & v, CrtVec3f & tv)
{
	tv.x = v.x*Matrix[M00] + v.y*Matrix[M10] + v.z*Matrix[M20] + Matrix[M30];
	tv.y = v.x*Matrix[M01] + v.y*Matrix[M11] + v.z*Matrix[M21] + Matrix[M31];
	tv.z = v.x*Matrix[M02] + v.y*Matrix[M12] + v.z*Matrix[M22] + Matrix[M32];
}

inline void CrtMatrixRotate( CrtMatrix Matrix, const CrtVec3f & v, CrtVec3f & tv)
{
	tv.x = v.x*Matrix[M00] + v.y*Matrix[M10] + v.z*Matrix[M20];
	tv.y = v.x*Matrix[M01] + v.y*Matrix[M11] + v.z*Matrix[M21];
	tv.z = v.x*Matrix[M02] + v.y*Matrix[M12] + v.z*Matrix[M22];
}

inline void CrtMatrix3x4TransformSkin( CrtMatrix Matrix, const CrtVec3f & v, CrtVec3f & tv)
{
	tv.x = v.x*Matrix[M00] + v.y*Matrix[M10] + v.z*Matrix[M20] + Matrix[M30];
	tv.y = v.x*Matrix[M01] + v.y*Matrix[M11] + v.z*Matrix[M21] + Matrix[M31];
	tv.z = v.x*Matrix[M02] + v.y*Matrix[M12] + v.z*Matrix[M22] + Matrix[M32];
}

inline void CrtMatrix3x4RotateSkin( CrtMatrix Matrix, const CrtVec3f & v, CrtVec3f & tv)
{
	tv.x = v.x*Matrix[M00] + v.y*Matrix[M10] + v.z*Matrix[M20];
	tv.y = v.x*Matrix[M01] + v.y*Matrix[M11] + v.z*Matrix[M21];
	tv.z = v.x*Matrix[M02] + v.y*Matrix[M12] + v.z*Matrix[M22];
}


void CrtMatrix4x4RotateAngleAxis(CrtMatrix LMatrix, CrtFloat LAxisX, CrtFloat LAxisY, CrtFloat LAxisZ, CrtFloat LAngle);

void CrtMatrix4x4RotateAngleAxis(CrtMatrix LMatrix, CrtVec4f AxisRot);
void CrtMatrix4x4Translate(CrtMatrix LMatrix, CrtVec4f Trans);
void CrtMatrix4x4Scale(CrtMatrix LMatrix, CrtVec4f Scale); 

void CrtMatrixMult( const CrtMatrix LMtx1, const CrtMatrix LMtx2, CrtMatrix LDstMtx);
void CrtMatrix4x4Mult(CrtMatrix LSrcMtx, CrtMatrix LDestMtx);
void CrtMatrixTranslate(CrtMatrix LMatrix, CrtFloat LX, CrtFloat LY, CrtFloat LZ);
void CrtQuaternionToMatrix(CrtQuat * LQ, CrtFloat * LMatrix);

#endif // _CRT_MATRIX_H_


