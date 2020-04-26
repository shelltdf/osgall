/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_MATERIAL_H_
#define _CRT_MATERIAL_H_

const int MAX_TEXTURES_PER_PASS = 10; 

class CrtEffect;
class CrtTexture;

class CrtMaterial : public CrtBase 
{
	CrtInt32			Type;
	
public:
	CrtInt		PassCount; 	

	CrtShader*	Shader;

	CrtColor3f	Emission;
	CrtColor3f	Ambient;
	CrtColor3f	Diffuse;
	CrtColor3f	Specular;
	CrtFloat	Shininess;
	CrtFloat	Transparency;

	CrtColor3f	Reflective;
	CrtColor3f	Transparent;
	CrtColor3f	Absorption;
	CrtFloat	Reflectivity;
	CrtFloat	RefractiveIndex;
	
	CrtTexture*	Textures[MAX_TEXTURES_PER_PASS];
	CrtEffect * Effect; 
	CrtInt		NumTextures; 
	CrtInt		Idx; 
public:

	CrtMaterial()
	{
		Shininess = 0;
		Reflectivity = 0; 
		Transparency = 0;
		RefractiveIndex = 0; 
		Idx = 0; 

		for ( int i = 0; i < MAX_TEXTURES_PER_PASS; i++)
			Textures[i] = NULL; 
		
		Effect = NULL; 

		Shader = NULL; 

		Type = 0; 
		NumTextures = 0; 

		Emission = CrtColor3f(.0f, .0f, .0f ); 
		Ambient = CrtColor3f(.25f, .25f, .25f ); 
		Diffuse = CrtColor3f(1.0f, 1.0f, 1.0f );
		Specular = CrtColor3f(.95f, .95f, .95f );
		Shininess = 40.0f;
		Transparency = 1.0f;
	}

	~CrtMaterial()
	{
        Destroy(); 
	}

protected:
friend class CrtScene; 

	inline	void	AddTexture( CrtTexture * tex ){ Textures[NumTextures] = tex; NumTextures++; }
	inline	void	SetEffect( CrtEffect * effect) { Effect = effect; }; 
	
public:
	//---------------------- External Interfaces ---------------------------------//
	// These lighting values are what comes from collada.. You will need to decide what 
	// you can and can't use.  Collada files often don't export all these paremeters.

	inline	CrtColor3f	GetEmission(){ return Emission; }
	inline	CrtColor3f	GetAmbient(){ return Ambient; }
	inline	CrtColor3f	GetDiffuse(){ return Diffuse; }
	inline	CrtColor3f	GetSpecular(){ return Specular; }
	inline	CrtFloat	GetShininess(){ return Shininess; }
	inline	CrtFloat	GetTransparency(){ return Transparency; }
	inline	CrtInt		GetIdx(){ return Idx; } 

	inline	CrtColor3f	GetReflective(){ return Reflective; }
	inline	CrtColor3f	GetTransparent(){ return Transparent; }
	inline	CrtColor3f	GetAbsorption(){ return Absorption; }

	inline	CrtTexture	*GetTexture(CrtInt n){ return Textures[n]; }
	inline	CrtEffect	*GetEffect(){ return Effect; }

	inline	CrtVoid		SetIdx( CrtInt i) { Idx = i; }

	//---------------------- End External Interfaces -----------------------------//
	
};


#endif // _CRT_MATERIAL_H_


