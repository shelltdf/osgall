/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_LIGHT_H_
#define _CRT_LIGHT_H_

#include "Crt/CrtData.h"

class CrtLight;
class CrtNode;

/** The CrtInstanceLight class holds the information needed to make an instance of an abstract light.
 *  This maps to the <instance_light> tag in COLLADA
 *  NOTE: This assumes that all light instances that reference the same CrtLight are sharing the info
 *  in that abstract light, ie: if someone changes the stuff in the CrtLight all the instances derived
 *  from it will change.  We could avoid this by keeping a copy of CrtLight in the instance.
 */
class CrtInstanceLight
{
public:
	CrtNode		*Parent;			/**<< Node where this instance was instantiated */
	CrtLight	*AbstractLight;		/**<< The abstract light where the light parameters are stored*/
};

enum CrtLightType 
{
	eLIGHTTYPE_UNKNOWN,
	eLIGHTTYPE_AMBIENT = 1,
	eLIGHTTYPE_DIRECTIONAL,
	eLIGHTTYPE_POINT,
	eLIGHTTYPE_SPOT	
};

class CrtLight : public CrtBase 
{
public:
	CrtColor4f  Color;					/**< The color of the light (all types)*/
	CrtFloat	FalloffAngle;			/**< Falloff angle for spot light (spot) */
	CrtFloat	FalloffExponent;		/**< Falloff exponent for spot light (spot) */
	CrtFloat	ConstantAttenuation;	/**< Constant attenuation factor (point and spot)*/
	CrtFloat    LinearAttenuation;		/**< Linear attenuation factor (point and spot)*/
	CrtFloat	QuadraticAttenuation;	/**< Quadratic attenuation factor (point and spot) */

	CrtLight()
	{
		Color					=	CrtColor4f( 0.5f, 0.5f, 0.5f, 1.0f );
		// !!!GAC are these good values for defaults?
		FalloffAngle			= 180.0f;
		FalloffExponent			= 0.0f;
		ConstantAttenuation		= 1.0f;
		LinearAttenuation		= 0.0f;
		QuadraticAttenuation	= 0.0f;
		LightType				=	eLIGHTTYPE_POINT;
	}

	~CrtLight()
	{
        Destroy(); 
	}

protected:
	CrtLightType	LightType;

	void	Update( CrtInt light );

public:
	//---------------------- External Interfaces ---------------------------------//
	// This light definition is one of the few place that the interface will change
	// I will add a type for point, area, direction ect and so on as well as other 
	// information as fall off.  
	inline CrtLightType	GetType()					{ return LightType; }
	inline void			SetType(CrtLightType type)	{ LightType = type; }
	inline CrtColor4f	GetColor()					{return Color;}
	//---------------------- End External Interfaces ---------------------------------//
	
};

#endif //_CRT_LIGHT_H_


