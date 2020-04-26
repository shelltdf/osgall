/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtData.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtMatrix.h"

CrtOrient::CrtOrient():
		UpdateLocalMatrix(CrtTrue),
		UpdateLocalToWorldMatrix(CrtTrue)
{
	CrtMatrixLoadIdentity(LocalMatrix);
	CrtMatrixLoadIdentity(LocalToWorldMatrix);
	CrtMatrixLoadIdentity(InverseLocalToWorldMatrix);
	CrtMatrixLoadIdentity(InverseTransposeLocalToWorldMatrix);
	CrtMatrixLoadIdentity(InverseBindMatrix);
	CrtMatrixLoadIdentity(CombinedSkinMatrix);
}

void	CrtQuat::Set( CrtVec4f axisRot )
{
	// convert the axis rot to quat 
	w = cos( axisRot.w/2.0f); 
	x = axisRot.x * sin(axisRot.w/2);
	y = axisRot.y * sin(axisRot.w/2);
	z = axisRot.z * sin(axisRot.w/2);
}

void	CrtVec4f::Set( CrtQuat * quat )
{
	CrtFloat s = (CrtFloat)sqrt( 1 - (quat->w*quat->w)); 
	w = 2 * acos(quat->w);
	if ( s < .001)
	{		
		x = quat->x;  
		y = quat->y;  
		z = quat->z;  
	}
	else
	{
		x = quat->x / s;  
		y = quat->y / s;  
		z = quat->z / s;  
	}
}

CrtVoid	CrtTransform::SetTranslate( CrtVec4f trans )
{
	Vector = trans; 
}

CrtVoid	CrtTransform::SetRotate( CrtVec4f rotate )
{
	Vector = rotate; 
}

CrtVoid	CrtTransform::SetScale( CrtVec4f scale )
{
	Vector = scale; 
}

CrtVoid	CrtTransform::SetLookAt( CrtVec3f lookAt[] )
{
	LookAt[eCrtLook] = lookAt[eCrtLook];
	LookAt[eCrtEye] = lookAt[eCrtEye];
	LookAt[eCrtUp] = lookAt[eCrtUp];
}

CrtVoid	CrtTransform::SetMatrix( CrtMatrix m )
{
	for ( int i = 0; i < 16; i ++ )
		Matrix[i] = m[i]; 
}

CrtVoid	CrtTransform::SetSid( CrtChar * sid )
{
	CrtCpy( Sid, sid ); 
}

CrtTFormType CrtTransform::GetType( CrtChar * nameType )
{
	// Return a Crt enum for the type of transform, things that are not transforms like <instance_
	// are returned as eCrtTFormUnknown and will be handled by other code.

	if      ( CrtICmp( nameType, "rotate" ) )
		return (eCrtRotate); 
	else if ( CrtICmp( nameType, "translate" ) )
		return (eCrtTranslate); 
	else if ( CrtICmp( nameType, "matrix" ) ) 
		return (eCrtMatrix); 
	else if ( CrtICmp( nameType, "scale" ) ) 
		return (eCrtScale); 
	else if ( CrtICmp( nameType, "lookat" ) ) 
		return (eCrtLookAt); 
	else if ( CrtICmp( nameType, "skew" ) ) 
		return (eCrtSkew); 
	else 
		return (eCrtTFormUnknown); 
}

