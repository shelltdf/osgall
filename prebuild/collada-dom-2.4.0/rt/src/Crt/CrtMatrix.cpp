/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtMatrix.h"
#include "Crt/CrtUtils.h"

//========================================================
// Print contents of Matrix
//========================================================
void CrtMatrixPrint(CrtMatrix LMtx)
{
	CrtPrint("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",
		LMtx[M00], LMtx[M10], LMtx[M20], LMtx[M30],
		LMtx[M01], LMtx[M11], LMtx[M21], LMtx[M31],
		LMtx[M02], LMtx[M12], LMtx[M22], LMtx[M32],
		LMtx[M03], LMtx[M13], LMtx[M23], LMtx[M33]);
}

void CrtMatrixTranspose( const CrtMatrix LSrcMtx, CrtMatrix LDstMtx )
{
	CrtMatrix outMat;
	CrtMatrixCopy( LSrcMtx, outMat ); 
	
	LDstMtx[M00] = outMat[M00];
	LDstMtx[M01] = outMat[M10];
	LDstMtx[M02] = outMat[M20];
	LDstMtx[M03] = outMat[M30];
		
	LDstMtx[M10] = outMat[M01];
	LDstMtx[M11] = outMat[M11];
	LDstMtx[M12] = outMat[M21];
	LDstMtx[M13] = outMat[M31];

	LDstMtx[M20] = outMat[M02];
	LDstMtx[M21] = outMat[M12];
	LDstMtx[M22] = outMat[M22];
	LDstMtx[M23] = outMat[M32];

	LDstMtx[M30] = outMat[M03];
	LDstMtx[M31] = outMat[M13];
	LDstMtx[M32] = outMat[M23];
	LDstMtx[M33] = outMat[M33];
}

void CrtMatrixTranspose3x4( const CrtMatrix LSrcMtx, CrtMatrix3x4 LDstMtx )
{
	CrtMatrix outMat;
	CrtMatrixCopy( LSrcMtx, outMat ); 
	
	LDstMtx[M00] = outMat[M00];
	LDstMtx[M01] = outMat[M10];
	LDstMtx[M02] = outMat[M20];
	LDstMtx[M03] = outMat[M30];
		
	LDstMtx[M10] = outMat[M01];
	LDstMtx[M11] = outMat[M11];
	LDstMtx[M12] = outMat[M21];
	LDstMtx[M13] = outMat[M31];

	LDstMtx[M20] = outMat[M02];
	LDstMtx[M21] = outMat[M12];
	LDstMtx[M22] = outMat[M22];
	LDstMtx[M23] = outMat[M32];
}


void CrtMatrixMult(const CrtMatrix LMtx1, const CrtMatrix LMtx2, CrtMatrix LDstMtx)
{
	CrtFloat	LM1_00, LM1_01, LM1_02, LM1_03,
		LM1_10, LM1_11, LM1_12, LM1_13,
		LM1_20, LM1_21, LM1_22, LM1_23,
		LM1_30, LM1_31, LM1_32, LM1_33,
		LM2_00, LM2_01, LM2_02, LM2_03,
		LM2_10, LM2_11, LM2_12, LM2_13,
		LM2_20, LM2_21, LM2_22, LM2_23,
		LM2_30, LM2_31, LM2_32, LM2_33;


	LM1_00 = LMtx1[M00];LM1_01 = LMtx1[M01];LM1_02 = LMtx1[M02];LM1_03 = LMtx1[M03];
	LM1_10 = LMtx1[M10];LM1_11 = LMtx1[M11];LM1_12 = LMtx1[M12];LM1_13 = LMtx1[M13];
	LM1_20 = LMtx1[M20];LM1_21 = LMtx1[M21];LM1_22 = LMtx1[M22];LM1_23 = LMtx1[M23];
	LM1_30 = LMtx1[M30];LM1_31 = LMtx1[M31];LM1_32 = LMtx1[M32];LM1_33 = LMtx1[M33];

	LM2_00 = LMtx2[M00];LM2_01 = LMtx2[M01];LM2_02 = LMtx2[M02];LM2_03 = LMtx2[M03];
	LM2_10 = LMtx2[M10];LM2_11 = LMtx2[M11];LM2_12 = LMtx2[M12];LM2_13 = LMtx2[M13];
	LM2_20 = LMtx2[M20];LM2_21 = LMtx2[M21];LM2_22 = LMtx2[M22];LM2_23 = LMtx2[M23];
	LM2_30 = LMtx2[M30];LM2_31 = LMtx2[M31];LM2_32 = LMtx2[M32];LM2_33 = LMtx2[M33];

	LDstMtx[M00] = LM1_00*LM2_00 + LM1_01*LM2_10 + LM1_02*LM2_20 + LM1_03*LM2_30;
	LDstMtx[M01] = LM1_00*LM2_01 + LM1_01*LM2_11 + LM1_02*LM2_21 + LM1_03*LM2_31;
	LDstMtx[M02] = LM1_00*LM2_02 + LM1_01*LM2_12 + LM1_02*LM2_22 + LM1_03*LM2_32;
	LDstMtx[M03] = LM1_00*LM2_03 + LM1_01*LM2_13 + LM1_02*LM2_23 + LM1_03*LM2_33;

	LDstMtx[M10] = LM1_10*LM2_00 + LM1_11*LM2_10 + LM1_12*LM2_20 + LM1_13*LM2_30;
	LDstMtx[M11] = LM1_10*LM2_01 + LM1_11*LM2_11 + LM1_12*LM2_21 + LM1_13*LM2_31;
	LDstMtx[M12] = LM1_10*LM2_02 + LM1_11*LM2_12 + LM1_12*LM2_22 + LM1_13*LM2_32;
	LDstMtx[M13] = LM1_10*LM2_03 + LM1_11*LM2_13 + LM1_12*LM2_23 + LM1_13*LM2_33;

	LDstMtx[M20] = LM1_20*LM2_00 + LM1_21*LM2_10 + LM1_22*LM2_20 + LM1_23*LM2_30;
	LDstMtx[M21] = LM1_20*LM2_01 + LM1_21*LM2_11 + LM1_22*LM2_21 + LM1_23*LM2_31;
	LDstMtx[M22] = LM1_20*LM2_02 + LM1_21*LM2_12 + LM1_22*LM2_22 + LM1_23*LM2_32;
	LDstMtx[M23] = LM1_20*LM2_03 + LM1_21*LM2_13 + LM1_22*LM2_23 + LM1_23*LM2_33;

	LDstMtx[M30] = LM1_30*LM2_00 + LM1_31*LM2_10 + LM1_32*LM2_20 + LM1_33*LM2_30;
	LDstMtx[M31] = LM1_30*LM2_01 + LM1_31*LM2_11 + LM1_32*LM2_21 + LM1_33*LM2_31;
	LDstMtx[M32] = LM1_30*LM2_02 + LM1_31*LM2_12 + LM1_32*LM2_22 + LM1_33*LM2_32;
	LDstMtx[M33] = LM1_30*LM2_03 + LM1_31*LM2_13 + LM1_32*LM2_23 + LM1_33*LM2_33;
}

void CrtMatrix4x4Mult(CrtMatrix LSrcMtx, CrtMatrix LDestMtx)
{
	CrtFloat	L00, L01, L02, L03, L10, L11, L12, L13,
			L20, L21, L22, L23, L30, L31, L32, L33;

	// !!!GAC as an experiment, reorder the operations so the lifetime of the temps is limited
	// !!!GAC the compiler should do this, but I want to make sure it's doing its job.

	L00 = LDestMtx[M00];
	L10 = LDestMtx[M10];
	L20 = LDestMtx[M20];
	L30 = LDestMtx[M30];
	LDestMtx[M00] = LSrcMtx[M00]*L00+LSrcMtx[M01]*L10+LSrcMtx[M02]*L20+LSrcMtx[M03]*L30;
	LDestMtx[M10] = LSrcMtx[M10]*L00+LSrcMtx[M11]*L10+LSrcMtx[M12]*L20+LSrcMtx[M13]*L30;
	LDestMtx[M20] = LSrcMtx[M20]*L00+LSrcMtx[M21]*L10+LSrcMtx[M22]*L20+LSrcMtx[M23]*L30;
	LDestMtx[M30] = LSrcMtx[M30]*L00+LSrcMtx[M31]*L10+LSrcMtx[M32]*L20+LSrcMtx[M33]*L30;

	L01 = LDestMtx[M01];
	L11 = LDestMtx[M11];
	L21 = LDestMtx[M21];
	L31 = LDestMtx[M31];
	LDestMtx[M01] = LSrcMtx[M00]*L01+LSrcMtx[M01]*L11+LSrcMtx[M02]*L21+LSrcMtx[M03]*L31;
	LDestMtx[M11] = LSrcMtx[M10]*L01+LSrcMtx[M11]*L11+LSrcMtx[M12]*L21+LSrcMtx[M13]*L31;
	LDestMtx[M21] = LSrcMtx[M20]*L01+LSrcMtx[M21]*L11+LSrcMtx[M22]*L21+LSrcMtx[M23]*L31;
	LDestMtx[M31] = LSrcMtx[M30]*L01+LSrcMtx[M31]*L11+LSrcMtx[M32]*L21+LSrcMtx[M33]*L31;
	
	L02 = LDestMtx[M02];
	L12 = LDestMtx[M12];
	L22 = LDestMtx[M22];
	L32 = LDestMtx[M32];
	LDestMtx[M02] = LSrcMtx[M00]*L02+LSrcMtx[M01]*L12+LSrcMtx[M02]*L22+LSrcMtx[M03]*L32;
	LDestMtx[M12] = LSrcMtx[M10]*L02+LSrcMtx[M11]*L12+LSrcMtx[M12]*L22+LSrcMtx[M13]*L32;
	LDestMtx[M22] = LSrcMtx[M20]*L02+LSrcMtx[M21]*L12+LSrcMtx[M22]*L22+LSrcMtx[M23]*L32;
	LDestMtx[M32] = LSrcMtx[M30]*L02+LSrcMtx[M31]*L12+LSrcMtx[M32]*L22+LSrcMtx[M33]*L32;
	
	L03 = LDestMtx[M03];
	L13 = LDestMtx[M13];
	L23 = LDestMtx[M23];
	L33 = LDestMtx[M33];
	LDestMtx[M03] = LSrcMtx[M00]*L03+LSrcMtx[M01]*L13+LSrcMtx[M02]*L23+LSrcMtx[M03]*L33;
	LDestMtx[M13] = LSrcMtx[M10]*L03+LSrcMtx[M11]*L13+LSrcMtx[M12]*L23+LSrcMtx[M13]*L33;
	LDestMtx[M23] = LSrcMtx[M20]*L03+LSrcMtx[M21]*L13+LSrcMtx[M22]*L23+LSrcMtx[M23]*L33;
	LDestMtx[M33] = LSrcMtx[M30]*L03+LSrcMtx[M31]*L13+LSrcMtx[M32]*L23+LSrcMtx[M33]*L33;
}


void CrtMatrixConcat(const CrtMatrix LSrcParMtx, const CrtMatrix LSrcChildMtx, CrtMatrix LDestMtx)
{
	CrtFloat	L00, L01, L02, L03, L10, L11, L12, L13,
			L20, L21, L22, L23, L30, L31, L32, L33;

	L00 = LSrcChildMtx[M00];L01 = LSrcChildMtx[M01];L02 = LSrcChildMtx[M02];L03 = LSrcChildMtx[M03];
	L10 = LSrcChildMtx[M10];L11 = LSrcChildMtx[M11];L12 = LSrcChildMtx[M12];L13 = LSrcChildMtx[M13];
	L20 = LSrcChildMtx[M20];L21 = LSrcChildMtx[M21];L22 = LSrcChildMtx[M22];L23 = LSrcChildMtx[M23];
	L30 = LSrcChildMtx[M30];L31 = LSrcChildMtx[M31];L32 = LSrcChildMtx[M32];L33 = LSrcChildMtx[M33];

	LDestMtx[M00] = LSrcParMtx[M00]*L00+LSrcParMtx[M01]*L10+LSrcParMtx[M02]*L20+LSrcParMtx[M03]*L30;
	LDestMtx[M01] = LSrcParMtx[M00]*L01+LSrcParMtx[M01]*L11+LSrcParMtx[M02]*L21+LSrcParMtx[M03]*L31;
	LDestMtx[M02] = LSrcParMtx[M00]*L02+LSrcParMtx[M01]*L12+LSrcParMtx[M02]*L22+LSrcParMtx[M03]*L32;
	LDestMtx[M03] = LSrcParMtx[M00]*L03+LSrcParMtx[M01]*L13+LSrcParMtx[M02]*L23+LSrcParMtx[M03]*L33;

	LDestMtx[M10] = LSrcParMtx[M10]*L00+LSrcParMtx[M11]*L10+LSrcParMtx[M12]*L20+LSrcParMtx[M13]*L30;
	LDestMtx[M11] = LSrcParMtx[M10]*L01+LSrcParMtx[M11]*L11+LSrcParMtx[M12]*L21+LSrcParMtx[M13]*L31;
	LDestMtx[M12] = LSrcParMtx[M10]*L02+LSrcParMtx[M11]*L12+LSrcParMtx[M12]*L22+LSrcParMtx[M13]*L32;
	LDestMtx[M13] = LSrcParMtx[M10]*L03+LSrcParMtx[M11]*L13+LSrcParMtx[M12]*L23+LSrcParMtx[M13]*L33;

	LDestMtx[M20] = LSrcParMtx[M20]*L00+LSrcParMtx[M21]*L10+LSrcParMtx[M22]*L20+LSrcParMtx[M23]*L30;
	LDestMtx[M21] = LSrcParMtx[M20]*L01+LSrcParMtx[M21]*L11+LSrcParMtx[M22]*L21+LSrcParMtx[M23]*L31;
	LDestMtx[M22] = LSrcParMtx[M20]*L02+LSrcParMtx[M21]*L12+LSrcParMtx[M22]*L22+LSrcParMtx[M23]*L32;
	LDestMtx[M23] = LSrcParMtx[M20]*L03+LSrcParMtx[M21]*L13+LSrcParMtx[M22]*L23+LSrcParMtx[M23]*L33;

	LDestMtx[M30] = LSrcParMtx[M30]*L00+LSrcParMtx[M31]*L10+LSrcParMtx[M32]*L20+LSrcParMtx[M33]*L30;
	LDestMtx[M31] = LSrcParMtx[M30]*L01+LSrcParMtx[M31]*L11+LSrcParMtx[M32]*L21+LSrcParMtx[M33]*L31;
	LDestMtx[M32] = LSrcParMtx[M30]*L02+LSrcParMtx[M31]*L12+LSrcParMtx[M32]*L22+LSrcParMtx[M33]*L32;
	LDestMtx[M33] = LSrcParMtx[M30]*L03+LSrcParMtx[M31]*L13+LSrcParMtx[M32]*L23+LSrcParMtx[M33]*L33;
}


void CrtQuaternionToMatrix(CrtQuat * LQ, CrtFloat * LMatrix)
{
	CrtFloat	LS, LXS, LYS, LZS, LWX, LWY, LWZ, LXX, LXY, LXZ, LYY, LYZ, LZZ;

	LS = 2.0f/(LQ->x*LQ->x+LQ->y*LQ->y+LQ->z*LQ->z+LQ->w*LQ->w);

	LXS = LQ->x*LS;LYS = LQ->y*LS;LZS = LQ->z*LS;
	LWX = LQ->w*LXS;LWY = LQ->w*LYS;LWZ = LQ->w*LZS;
	LXX = LQ->x*LXS;LXY = LQ->x*LYS;LXZ = LQ->x*LZS;
	LYY = LQ->y*LYS;LYZ = LQ->y*LZS;LZZ = LQ->z*LZS;

	LMatrix[M00] = 1.0f-(LYY+LZZ);
	LMatrix[M01] = LXY+LWZ;
	LMatrix[M02] = LXZ-LWY;

	LMatrix[M10] = LXY-LWZ;
	LMatrix[M11] = 1.0f-(LXX+LZZ);
	LMatrix[M12] = LYZ+LWX;

	LMatrix[M20] = LXZ+LWY;
	LMatrix[M21] = LYZ-LWX;
	LMatrix[M22] = 1.0f-(LXX+LYY);

	LMatrix[M03] = 0.0f;
	LMatrix[M13] = 0.0f;
	LMatrix[M23] = 0.0f;
	LMatrix[M30] = 0.0f;
	LMatrix[M31] = 0.0;LMatrix[M32] = 0.0;LMatrix[M33] = 1.0f;
}

void CrtQuaternionToMatrix(CrtVec4f * LQ, CrtFloat * LMatrix)
{
	CrtFloat	LS, LXS, LYS, LZS, LWX, LWY, LWZ, LXX, LXY, LXZ, LYY, LYZ, LZZ;

	LS = 2.0f/(LQ->x*LQ->x+LQ->y*LQ->y+LQ->z*LQ->z+LQ->w*LQ->w);

	LXS = LQ->x*LS;LYS = LQ->y*LS;LZS = LQ->z*LS;
	LWX = LQ->w*LXS;LWY = LQ->w*LYS;LWZ = LQ->w*LZS;
	LXX = LQ->x*LXS;LXY = LQ->x*LYS;LXZ = LQ->x*LZS;
	LYY = LQ->y*LYS;LYZ = LQ->y*LZS;LZZ = LQ->z*LZS;

	LMatrix[M00] = 1.0f-(LYY+LZZ);
	LMatrix[M01] = LXY+LWZ;
	LMatrix[M02] = LXZ-LWY;

	LMatrix[M10] = LXY-LWZ;
	LMatrix[M11] = 1.0f-(LXX+LZZ);
	LMatrix[M12] = LYZ+LWX;

	LMatrix[M20] = LXZ+LWY;
	LMatrix[M21] = LYZ-LWX;
	LMatrix[M22] = 1.0f-(LXX+LYY);

	LMatrix[M03] = 0.0f;
	LMatrix[M13] = 0.0f;
	LMatrix[M23] = 0.0f;
	LMatrix[M30] = 0.0f;
	LMatrix[M31] = 0.0;LMatrix[M32] = 0.0;LMatrix[M33] = 1.0f;
}

CrtFloat CrtPI = 3.14159f;

void CrtMatrix4x4RotateAngleAxis(CrtMatrix LMatrix, CrtFloat LAxisX, CrtFloat LAxisY, CrtFloat LAxisZ, CrtFloat LAngle)
{
	CrtMatrix	LRotMatrix;
	CrtVec4f	LQuaternion;
	CrtFloat	LAngleSin = (CrtFloat)sin(LAngle*0.5f*CrtDEGREES_TO_RADIANS),
			LAngleCos = (CrtFloat)cos(LAngle*0.5f*CrtDEGREES_TO_RADIANS);


	CrtMatrixLoadIdentity(LRotMatrix);
	LQuaternion.x = LAxisX*LAngleSin;
	LQuaternion.y = LAxisY*LAngleSin;
	LQuaternion.z = LAxisZ*LAngleSin;
	LQuaternion.w = LAngleCos;
	CrtQuaternionToMatrix(&LQuaternion, LRotMatrix);

	CrtMatrix4x4Mult(LRotMatrix, LMatrix);
}

void CrtMatrix4x4RotateAngleAxis(CrtMatrix LMatrix, CrtVec4f AxisRot)
{
	CrtMatrix	LRotMatrix;
	CrtVec4f	LQuaternion;

	// Build the rotation matix based off of the axis rotation passed in 
	CrtFloat	LAngleSin = (CrtFloat)sin(AxisRot.w*0.5f*CrtDEGREES_TO_RADIANS),
			LAngleCos = (CrtFloat)cos(AxisRot.w*0.5f*CrtDEGREES_TO_RADIANS);


	CrtMatrixLoadIdentity(LRotMatrix);
	LQuaternion.x = AxisRot.x*LAngleSin;
	LQuaternion.y = AxisRot.y*LAngleSin;
	LQuaternion.z = AxisRot.z*LAngleSin;
	LQuaternion.w = LAngleCos;
	CrtQuaternionToMatrix(&LQuaternion, LRotMatrix);

	// concate to previously passed in matrix 
	CrtMatrix4x4Mult(LRotMatrix, LMatrix);
}

void CrtMatrix4x4Translate(CrtMatrix LMatrix, CrtVec4f translate)
{
	CrtMatrix	TMatrix;
	
	// Build the translation matrix 
	CrtMatrixLoadIdentity(TMatrix);
	TMatrix[M30] = translate.x; 
	TMatrix[M31] = translate.y; 
	TMatrix[M32] = translate.z;

	// concatinate to previously passed in matrix 
	CrtMatrix4x4Mult(TMatrix, LMatrix);
}

void CrtMatrix4x4Scale(CrtMatrix LMatrix, CrtVec4f scale)
{
	CrtMatrix	TMatrix;
	
	// Build the translation matrix 
	CrtMatrixLoadIdentity(TMatrix);
	TMatrix[M00] = scale.x; 
	TMatrix[M11] = scale.y; 
	TMatrix[M22] = scale.z;

	// concatinate to previously passed in matrix 
	CrtMatrix4x4Mult(TMatrix, LMatrix);
}




void CrtMatrixTranslate(CrtMatrix LMatrix, CrtFloat LX, CrtFloat LY, CrtFloat LZ)
{
	CrtFloat	LDX, LDY, LDZ;

	LDX = LX*LMatrix[M00]+LY*LMatrix[M10]+LZ*LMatrix[M20];
	LDY = LX*LMatrix[M01]+LY*LMatrix[M11]+LZ*LMatrix[M21];
	LDZ = LX*LMatrix[M02]+LY*LMatrix[M12]+LZ*LMatrix[M22];

	LMatrix[M30] += LDX;
	LMatrix[M31] += LDY;
	LMatrix[M32] += LDZ;
}

void CrtMatrixRotateTranslate(CrtMatrix LMatrix, CrtVec4f RotX, CrtVec4f RotY, CrtVec4f RotZ, CrtVec3f Trans )
{
	CrtMatrixLoadIdentity( LMatrix ); 
	CrtMatrixTranslate( LMatrix, Trans.x, Trans.y, Trans.z  ); 
	CrtMatrix4x4RotateAngleAxis( LMatrix, RotZ.x, RotZ.y, RotZ.z, RotZ.w);
	CrtMatrix4x4RotateAngleAxis( LMatrix, RotY.x, RotY.y, RotY.z, RotY.w);
	CrtMatrix4x4RotateAngleAxis( LMatrix, RotX.x, RotX.y, RotX.z, RotX.w);			
	
}

void CrtMatrix3x3TransformVV( CrtMatrix mMatrix,  CrtFloat mXIn, CrtFloat mYIn, CrtFloat mZIn, 
												CrtFloat &mXOut, CrtFloat &mYOut, CrtFloat &mZOut)
{
	CrtFloat	mX = mXIn, mY = mYIn, mZ = mZIn;
	mXOut = mX*(mMatrix)[M00] + mY*(mMatrix)[M10] + mZ*(mMatrix)[M20];
	mYOut = mX*(mMatrix)[M01] + mY*(mMatrix)[M11] + mZ*(mMatrix)[M21];
	mZOut = mX*(mMatrix)[M02] + mY*(mMatrix)[M12] + mZ*(mMatrix)[M22];
}

void CrtMatrix3x4Invert(CrtMatrix LM, CrtMatrix LMI)
{
	// !!!GAC Because this doesn't copy over the last column of the source to the dest, if the output matrix
	// !!!GAC isn't initialized it may turn out a bogus 4x4 matrix, should this function set that column?

	CrtFloat	LM00 = LM[M00], LM01 = LM[M01], LM02 = LM[M02],	// Use temporary storage, in case LM == LMI
		LM10 = LM[M10], LM11 = LM[M11], LM12 = LM[M12],
		LM20 = LM[M20], LM21 = LM[M21], LM22 = LM[M22],
		LInvD = 1.0f / (LM22*LM11*LM00 - LM22*LM10*LM01 - LM21*LM12*LM00 + LM21*LM10*LM02 + LM20*LM12*LM01 - LM20*LM11*LM02);

	LMI[M00] =  (LM22*LM11 - LM21*LM12) * LInvD;
	LMI[M01] = (-LM22*LM01 + LM21*LM02) * LInvD;
	LMI[M02] =  (LM12*LM01 - LM11*LM02) * LInvD;
	LMI[M10] = (-LM22*LM10 + LM20*LM12) * LInvD;
	LMI[M11] =  (LM22*LM00 - LM20*LM02) * LInvD;
	LMI[M12] = (-LM12*LM00 + LM10*LM02) * LInvD;
	LMI[M20] =  (LM21*LM10 - LM20*LM11) * LInvD;
	LMI[M21] = (-LM21*LM00 + LM20*LM01) * LInvD;
	LMI[M22] =  (LM11*LM00 - LM10*LM01) * LInvD;

	CrtMatrix3x3TransformVV(LMI,  -LM[M30], -LM[M31], -LM[M32],  LMI[M30], LMI[M31], LMI[M32]);
}

#define MtxPos(r, c)	(c*4+r)

enum
{
 E3dX = 0, E3dY, E3dZ, E3dW, E3dARBITRARY
};

#define E3dS	E3dX
#define E3dT	E3dY
#define E3dR	E3dZ
#define E3dQ	E3dW

int	E3d_NXT[3] = { E3dY, E3dZ, E3dX };

/*void CrtMatrixToQuat( const CrtMatrix rotMat, CrtQuat & RotQuat )
{
	CrtFloat	LTr, LS;
	CrtFloat	LQ[3];
	CrtInt		LI, LJ, LK;


	LTr = rotMat[M00]+rotMat[M11]+rotMat[M22];
	if(LTr > 0.0)
	{
		LS = (CrtFloat)sqrt(LTr+1.0f);
		RotQuat.w = LS*0.5f;
		LS = 0.5f/LS;

		RotQuat.x = (rotMat[M12]-rotMat[M21])*LS;
		RotQuat.y = (rotMat[M20]-rotMat[M02])*LS;
		RotQuat.z = (rotMat[M01]-rotMat[M10])*LS;
	}
	else
	{
		LI = E3dX;
		if(rotMat[M11] > rotMat[M00]) LI = E3dY;
		if(rotMat[M22] > rotMat[E3dMtxPos(LI, LI)]) LI = E3dZ;

		LJ = E3d_NXT[LI];LK = E3d_NXT[LJ];
		LS = (CrtFloat)sqrt(rotMat[E3dMtxPos(LI, LI)]-rotMat[E3dMtxPos(LJ, LJ)]-rotMat[E3dMtxPos(LK, LK)]+1.0f);
		LQ[LI] = LS*0.5f;LS = 0.5f/LS;
		LQ[LJ] = (rotMat[E3dMtxPos(LI, LJ)]+rotMat[E3dMtxPos(LJ, LI)])*LS;
		LQ[LK] = (rotMat[E3dMtxPos(LI, LK)]+rotMat[E3dMtxPos(LK, LI)])*LS;

		RotQuat.x = LQ[0];
		RotQuat.y = LQ[1];
		RotQuat.z = LQ[2];
		RotQuat.w = (rotMat[E3dMtxPos(LJ, LK)]-rotMat[E3dMtxPos(LK, LJ)])*LS;
	}
	
}; */


void CrtMatrixToQuat( const CrtMatrix rotMat, CrtQuat & RotQuat )
{
/*	CrtFloat	LTr, LS;
	CrtFloat	LQ[3];
	CrtInt		LI, LJ, LK;


	LTr = rotMat[M00]+rotMat[M11]+rotMat[M22];
	if(LTr > 0.0)
	{
		LS = (CrtFloat)sqrt(LTr+1.0f);
		RotQuat.w = LS*0.5f;
		LS = 0.5f/LS;

		RotQuat.x = (rotMat[M12]-rotMat[M21])*LS;
		RotQuat.y = (rotMat[M20]-rotMat[M02])*LS;
		RotQuat.z = (rotMat[M01]-rotMat[M10])*LS;
	}
	else
	{
		LI = E3dX;
		if(rotMat[M11] > rotMat[M00]) LI = E3dY;
		if(rotMat[M22] > rotMat[E3dMtxPos(LI, LI)]) LI = E3dZ;

		LJ = E3d_NXT[LI];LK = E3d_NXT[LJ];
		LS = (CrtFloat)sqrt(rotMat[E3dMtxPos(LI, LI)]-rotMat[E3dMtxPos(LJ, LJ)]-rotMat[E3dMtxPos(LK, LK)]+1.0f);
		LQ[LI] = LS*0.5f;LS = 0.5f/LS;
		LQ[LJ] = (rotMat[E3dMtxPos(LI, LJ)]+rotMat[E3dMtxPos(LJ, LI)])*LS;
		LQ[LK] = (rotMat[E3dMtxPos(LI, LK)]+rotMat[E3dMtxPos(LK, LI)])*LS;

		RotQuat.x = LQ[0];
		RotQuat.y = LQ[1];
		RotQuat.z = LQ[2];
		RotQuat.w = (rotMat[E3dMtxPos(LJ, LK)]-rotMat[E3dMtxPos(LK, LJ)])*LS;
	}*/
	    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    CrtFloat fTrace = rotMat[M00] + rotMat[M11] + rotMat[M22];
    CrtFloat fRoot;

    if ( fTrace > 0.0f )
    {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = (CrtFloat)sqrt(fTrace + 1.0f);  // 2w
        RotQuat.w = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;  // 1/(4w)
        //RotQuat.x = (rotMat[M21]-rotMat[M12])*fRoot;
        //RotQuat.y = (rotMat[M02]-rotMat[M20])*fRoot;
        //RotQuat.z = (rotMat[M10]-rotMat[M01])*fRoot;
		 RotQuat.x = (rotMat[M12]-rotMat[M21])*fRoot;
        RotQuat.y = (rotMat[M20]-rotMat[M02])*fRoot;
        RotQuat.z = (rotMat[M01]-rotMat[M10])*fRoot;
    }
    else
    {
		int Next[3] = { 1, 2, 0 };

        // |w| <= 1/2
        int i = 0;
		int iMatPos=0; 
		int jMatPos=0; 
		int kMatPos=0; 

        if ( rotMat[M11] > rotMat[M00] )
            i = 1;
		iMatPos = MtxPos(i,i); 

        if ( rotMat[M22] > rotMat[iMatPos] )
            i = 2;

        int j = Next[i];
        int k = Next[j];
			
		iMatPos = MtxPos(i,i); 
		jMatPos = MtxPos(j,j);
		kMatPos = MtxPos(k,k); 

		fRoot = (CrtFloat)sqrt(rotMat[iMatPos]-rotMat[jMatPos]-rotMat[kMatPos]+ 1.0f);
        CrtFloat * QXYZ[3] = { &RotQuat.x, &RotQuat.y, &RotQuat.z };
        *QXYZ[i] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        RotQuat.w = (rotMat[MtxPos(k,j)]-rotMat[MtxPos(j,k)])*fRoot;
        *QXYZ[j] = (rotMat[MtxPos(j,i)]+rotMat[MtxPos(i,j)])*fRoot;
        *QXYZ[k] = (rotMat[MtxPos(k,i)]+rotMat[MtxPos(i,k)])*fRoot;
    }
	
}; 

