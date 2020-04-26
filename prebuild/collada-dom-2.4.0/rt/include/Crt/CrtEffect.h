/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_EFFECT_H_
#define _CRT_EFFECT_H_ 

#include "Crt/CrtData.h"

const int MAX_IMAGES = 8; 
// This is a shell definition of an effect for now
// just to resolve the new binding for the 1.4 specifictaion
// between the imgaes, effects, and materials 

// This will be filled in more completely later RS

class CrtImage;

class CrtEffect : public CrtBase 
{
	
	CrtInt32	Type;
	
public:

	// this is part of the Phong lighting definition 
	// and will override the FF material setting  
	// Again this is a first implementation and should
	// be flush out further later. 

	CrtColor4f	Emission;
	CrtColor4f	Ambient;
	CrtColor4f	Diffuse;
	CrtColor4f	Specular;
	CrtFloat	Shininess;
	CrtFloat	Transparency;

	CrtColor4f	Reflective;
	CrtColor4f	Transparent;
	CrtColor4f	Absorption;
	CrtFloat	Reflectivity;
	CrtFloat	RefractiveIndex;
	
	std::vector<CrtImage*>	Textures;
	CrtUInt		Idx; 
public:

	CrtEffect()
	{
		Shininess = 0;
		Reflectivity = 0; 
		Transparency = 0;
		RefractiveIndex = 0; 
		Idx = 0; 

//		for ( int i = 0; i < MAX_IMAGES; i++)
//			Images[i] = NULL; 

		Type = 0; 
//		NumImages = 0; 

		// !!!GAC make the defaults all zeros 
		Emission = CrtColor3f(0.0f, 0.0f, 0.0f ); 
		Ambient = CrtColor3f(0.0f, 0.0f, 0.0f ); 
		Diffuse = CrtColor3f(0.0f, 0.0f, 0.0f );
		Specular = CrtColor3f(0.0f, 0.0f, 0.0f );
		Shininess = 40.0f;
		Transparency = 1.0f;
	}

	~CrtEffect()
	{
        Destroy(); 
	}

protected:
	friend class CrtScene; 

//	inline	void	AddImage( CrtImage * tex ){ Images[NumImages] = tex; NumImages++; }
	
public:

	inline	CrtColor4f	GetEmission(){ return Emission; }
	inline	CrtColor4f	GetAmbient(){ return Ambient; }
	inline	CrtColor4f	GetDiffuse(){ return Diffuse; }
	inline	CrtColor4f	GetSpecular(){ return Specular; }
	inline	CrtFloat	GetShininess(){ return Shininess; }
	inline	CrtFloat	GetTransparency(){ return Transparency; }
	inline	CrtInt		GetIdx(){ return Idx; } 

	inline	CrtColor4f	GetReflective(){ return Reflective; }
	inline	CrtColor4f	GetTransparent(){ return Transparent; }
	inline	CrtColor4f	GetAbsorption(){ return Absorption; }

//	inline	CrtImage	*GetImage(CrtInt n){ return Images[n]; }

	inline	CrtVoid		SetIdx( CrtInt i) { Idx = i; }
	
};

	

#endif 
