/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_DATA_H_
#define _CRT_DATA_H_

#include "Crt/CrtTypes.h"
#include "Crt/CrtMemory.h"

#include <math.h>
#include <stddef.h>
#include <string>
#include <string.h>
#include <vector>
#include <stdio.h>
#include <map>
#include <vector>
using namespace std;

#define MAX_STRING_SIZE 512

#define CRT_MAX_ID_SIZE		MAX_STRING_SIZE
#define CRT_MAX_SID_SIZE	MAX_STRING_SIZE 
#define CRT_MAX_TEMP_DATA	MAX_STRING_SIZE
#define CRT_MAX_NAME_SIZE	MAX_STRING_SIZE 
#define CRT_MAX_TEX_COORDS	8
#define CRT_MAX_URI_SIZE	MAX_STRING_SIZE

inline CrtFloat	CrtClampf(	CrtFloat v, CrtFloat min = 0.0f, CrtFloat max = 1.0f )
{
	if	(v < min)
		return	min;
	else if	(v > max)
		return	max;
	else
		return	v;
}

// --------------------- //
// General storage types //
// --------------------- // 

struct CrtVec2f
{
	CrtFloat x,y; 

	CrtVec2f()
	{
		x = y = 0; 
	}

	CrtVec2f( CrtFloat xx, CrtFloat yy )
	{
		x = xx; y = yy; 
	}

	inline CrtBool operator ==( CrtVec2f v)
	{
		if ( x != v.x )
			return CrtFalse; 
		if ( y != v.y )
			return CrtFalse; 
		return CrtTrue; 
	}

	inline CrtBool operator !=( CrtVec2f v )
	{
		if ( x != v.x )
			return CrtTrue; 
		if ( y != v.y )
			return CrtTrue; 
		return CrtFalse; 
	}
};


struct CrtVec3f
{
	CrtFloat x,y,z; 

	CrtVec3f()
	{
		x = y = z = 0; 
	}

	CrtVec3f( CrtFloat xx, CrtFloat yy, CrtFloat zz )
	{
		x = xx; y = yy; z = zz; 
	}

	inline CrtVec3f operator +(const CrtVec3f& v) const 
		{ 	return (CrtVec3f(x + v.x, y + v.y, z + v.z)); }
	inline CrtVec3f operator +(const CrtVec2f& v) const
		{	return (CrtVec3f(x + v.x, y + v.y, z)); }
	inline CrtVec3f operator -(const CrtVec3f& v) const
		{	return (CrtVec3f(x - v.x, y - v.y, z - v.z)); }
	inline CrtVec3f operator -(const CrtVec2f& v) const
		{	return (CrtVec3f(x - v.x, y - v.y, z)); }
	inline CrtVec3f operator *(float t) const
		{	return (CrtVec3f(x * t, y * t, z * t));}
	inline CrtVec3f & operator +=(const CrtVec3f& v)
		{	x += v.x; y += v.y;	z += v.z;	return (*this); }
	inline CrtVec3f & operator *=( CrtFloat t)
		{	x *= t; y *= t;	z *= t;	return (*this); }
	inline CrtFloat operator *(const CrtVec3f& v) const
		{	return (x * v.x + y * v.y + z * v.z); }
	inline CrtVec3f& operator /=( CrtFloat t)
	{
		CrtFloat v = 1.0F / t;
		x *= v; y *= v;  z *= v;
		return (*this);
	}

	inline CrtVec3f& Normalize(void) 
		{ 	return (*this /= sqrtf(x * x + y * y + z * z)); 	}

	inline CrtBool operator ==( CrtVec3f v)
	{
		if ( x != v.x )
			return CrtFalse; 
		if ( y != v.y )
			return CrtFalse; 
		if ( z != v.z )
			return CrtFalse; 
		return CrtTrue; 
	}

	inline CrtBool operator !=( CrtVec3f v )
	{
		if ( x != v.x )
			return CrtTrue; 
		if ( y != v.y )
			return CrtTrue; 
		if ( z != v.z )
			return CrtTrue; 
		return CrtFalse; 
	}

	inline CrtFloat DotProduct( const CrtVec3f * v1, const CrtVec3f * v2 )
	{
		return ( v1->x * v2->x + v1->y * v2->y + v1->z * v2->z ); 
	}

	inline CrtVec3f CrossProduct( const CrtVec3f * v1, const CrtVec3f * v2 )
	{
		CrtVec3f v3;  
			
		v3.x = v1->y*v2->z - v1->z*v2->y;
		v3.y = v1->z*v2->x - v1->x*v2->z;
		v3.z = v1->x*v2->y - v1->y*v2->x;
		
		return v3; 
	}; 	
};

struct CrtQuat; 
struct CrtVec4f
{
	CrtFloat x,y,z,w; 

	CrtVec4f()
	{
		x = y = z = w = 0; 
	}

	CrtVec4f( CrtFloat xx, CrtFloat yy, CrtFloat zz, CrtFloat ww )
	{
		x = xx; y = yy; z = zz; w = ww;
	}

	inline void Set( CrtInt i, CrtFloat f )
	{
		switch( i )
		{
		case 0: x = f; 	break;
		case 1: y = f;  break; 
		case 2:	z = f; 	break; 
		case 3:	w = f; 	break; 
		};
	}

	inline CrtFloat Get( CrtInt i )
	{
		switch( i )
		{
		case 0:	return x; 			
		case 1: return y; 
		case 2: return z; 			
		case 3: return w; 			
		};
		return 0; 
	}

	void Set( CrtQuat *); 
};

struct CrtVec4i
{
	CrtInt32 x,y,z,w; 

	CrtVec4i()
	{
		x = y = z = w = 0; 
	}

	CrtVec4i( CrtInt32 xx, CrtInt32 yy, CrtInt32 zz, CrtInt32 ww )
	{
		x = xx; y = yy; z = zz; ww = w;
	}

};




struct CrtColor3f
{
	CrtFloat r,g,b;

	CrtColor3f()
	{
		r = g = b = 1;
	}

	CrtColor3f( CrtFloat rr, CrtFloat gg, CrtFloat bb )
	{
		r = rr; g = gg; b = bb;
	}

	void	Clamp( CrtFloat min=0, CrtFloat max=1.0f )
	{
		r	=	CrtClampf( r, min, max ); 
		g	=	CrtClampf( g, min, max ); 
		b	=	CrtClampf( b, min, max ); 
	}
};



struct CrtColor4f
{
	CrtFloat r,g,b,a;

	CrtColor4f()
	{
		r = g = b = a = 1;
	}

	CrtColor4f( CrtFloat rr, CrtFloat gg, CrtFloat bb, CrtFloat aa )
	{
		r = rr; g = gg; b = bb; a = aa;
	}

	CrtColor4f( CrtColor3f * c )
	{
		r = c->r; g = c->g; b = c->b; a = 1.0f; 
	}

	CrtColor4f( CrtColor3f c )
	{
		r = c.r; g = c.g; b = c.b; a = 1.0f; 
	}

	void	Clamp( CrtFloat min=0, CrtFloat max=1.0f )
	{
		r	=	CrtClampf( r, min, max ); 
		g	=	CrtClampf( g, min, max ); 
		b	=	CrtClampf( b, min, max ); 
		a	=	CrtClampf( a, min, max ); 
	}
};

struct CrtColor3i
{
	CrtUChar r,g,b;

	CrtColor3i()
	{
		r = g = b = (CrtUChar)255;
	}

	CrtColor3i( CrtUChar rr, CrtUChar gg, CrtUChar bb )
	{
		r = rr; g = gg; b = bb; 
	}

	CrtColor3i( CrtColor3f c )
	{
		r = (CrtUChar)(c.r* 255); 
		g = (CrtUChar)(c.g*255); 
		b = (CrtUChar)(c.b*255); 		
	}

	CrtColor3i( CrtColor4f c )
	{
		r = (CrtUChar)(c.r* 255); 
		g = (CrtUChar)(c.g*255); 
		b = (CrtUChar)(c.b*255); 
	}

};


struct CrtColor4i
{
	CrtUChar r,g,b,a;

	CrtColor4i()
	{
		r = g = b = a = (CrtUChar)255;
	}

	CrtColor4i( CrtUChar rr, CrtUChar gg, CrtUChar bb, CrtUChar aa )
	{
		r = rr; g = gg; b = bb; a = aa;
	}

	CrtColor4i( CrtColor4f c )
	{
		r = (CrtUChar)(c.r* 255); 
		g = (CrtUChar)(c.g*255); 
		b = (CrtUChar)(c.b*255); 
		a = (CrtChar)(c.a*255);
	}

	CrtColor4i( CrtColor3f c )
	{
		r = (CrtUChar)(c.r* 255); 
		g = (CrtUChar)(c.g*255); 
		b = (CrtUChar)(c.b*255); 
		a = 1;
	}

};



struct CrtQuat
{
	CrtFloat x,y,z,w; 
	
	CrtQuat()
	{
		x = y = z = 0; w = 1.0f; 
	}

	CrtQuat( CrtFloat X, CrtFloat Y, CrtFloat Z, CrtFloat W )
	{
		x = X, y = Y, z = Z, w = W; 
	}  

	//Update orient 2 q1 * q2 
	inline CrtQuat& operator *= ( const CrtQuat& q2 )
	{
	   float A, B, C, D, E, F, G, H;
	   A = (w + x) * (q2.w + q2.x);
	   B = (z - y) * (q2.y - q2.z);
	   C = (w - x) * (q2.y + q2.z);
	   D = (y + z) * (q2.w - q2.x);
	   E = (x + z) * (q2.x + q2.y);
	   F = (x - z) * (q2.x - q2.y);
	   G = (w + y) * (q2.w - q2.z);
	   H = (w - y) * (q2.w + q2.z);

	   x =  A - (  E + F + G + H ) * .5f;
	   y =  C + (  E - F + G - H ) * .5f;
	   z =  D + (  E - F - G + H ) * .5f;
	   w =  B + ( -E - F + G + H ) * .5f;
	   return *this;
	}

/*	inline void Rotate( CrtVec3f erot)
	{
		float cr, cp, cy, sr, sp, sy, cpcy, spsy;

		CrtFloat roll =  erot.x; 
		CrtFloat pitch = erot.y;
		CrtFloat yaw =   erot.z; 

		// calculate trig identities
		cr = cos(roll/2.0f);

		cp = cos(pitch/2.0f);
		cy = cos(yaw/2.0f);

		sr = sin(roll/2.0f);
		sp = sin(pitch/2.0f);
		sy = sin(yaw/2.0f);

		cpcy = cp * cy;
		spsy = sp * sy;

		s = cr * cpcy + sr * spsy;
		x = sr * cpcy - cr * spsy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;

	}*/

	void Rotate( CrtVec3f axis, CrtFloat theta );
	void Set( CrtVec4f axisRot ); 

	inline CrtFloat Length()
	{
		return (CrtFloat)sqrt( x*x + y*y + z*z + w*w);
	}
	inline void Normalize()
	{
		CrtFloat Len = Length();

		if ( Len > 1e-06f )
		{
			CrtFloat ILen = (1.0f)/Len;
			x *= ILen;
			y *= ILen;
			z *= ILen;
			w *= ILen;
		}
		else
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}    
	}
};

inline CrtQuat operator * ( const CrtQuat &q1, const CrtQuat &q2 )
{
   CrtQuat tmp(q1);
   tmp*=q2;
   return tmp;
}


// --------------------- //
// Library storage types //
// --------------------- // 

CrtChar * CrtCpy( CrtChar * s1, const CrtChar * s2 ); 

class CrtBase 
{

protected: 
	CrtChar		Id[CRT_MAX_ID_SIZE];
	CrtChar		Sid[CRT_MAX_SID_SIZE];
	CrtChar		Name[CRT_MAX_NAME_SIZE];
	CrtBase *	Next; 
	CrtChar		DocURI[CRT_MAX_URI_SIZE];

public:

	CrtBase()
	{
		Id[0] = 0;
		Sid[0] = 0;
		Name[0] = 0;
		Next = NULL; 
		DocURI[0] = 0;
	}

	virtual ~CrtBase()
	{ 
		Destroy(); 
	};
	
	// for auto deletion of the linked list 
	void	Destroy()
	{
		//CrtDelete( Next ); 		
	}

	inline CrtBase	*GetNext()					{ return Next; }
	inline CrtBase	*SetNext( CrtBase * next )	{ Next = next; return this;}
	inline CrtChar	*GetName()					{ return Name; }
	inline void		SetName( const CrtChar * n ){ CrtCpy(Name, n); }				//TODO: buffer overrun?
	inline void 	SetDocURI(const CrtChar * n) { CrtCpy(DocURI, n); }     //TODO: buffer overrun?
	inline CrtChar	*GetDocURI()				{ return DocURI; }
};

class CrtShader : public CrtBase 
{
	

};

#define CrtDEGREES_TO_RADIANS		0.017453292f


#ifndef M00
#define M00	0
enum
{
    M01=1, M02, M03,
	M10, M11, M12, M13,
	M20, M21, M22, M23,
	M30, M31, M32, M33
};
#endif 

enum CrtTFormType
{
	eCrtRotate,
	eCrtTranslate,
	eCrtLookAt,
	eCrtMatrix, 
	eCrtScale,
	eCrtSkew,
	eCrtTFormUnknown, 
};

enum CrtLookAt
{
	eCrtLook,
	eCrtEye,
	eCrtUp,
};

class CrtAnimation; 
const CrtUInt MAX_TFORM_CHANNEL_IDS = 12;  

class CrtTransform
{
protected:
	CrtTFormType	Type; 
	CrtVec4f		Vector; // this will store axis rotation, translation or scale 
	CrtAnimation*	Animation;
	CrtUInt			ChannelIds[MAX_TFORM_CHANNEL_IDS]; 
	CrtUInt			NumChannels; 
	CrtVec3f		LookAt[3]; // this will store lookAt, eyePoint and up 
	CrtMatrix		Matrix;  
	CrtChar			Sid[MAX_STRING_SIZE];

public:
	
	CrtTransform():
	  Type(eCrtTFormUnknown),
	  Animation(NULL),
	  NumChannels(0)
	{
		memset(ChannelIds, 0, sizeof( ChannelIds ));
		memset(Sid, 0, sizeof( Sid ));
	}

	~CrtTransform(){}; 
	
	CrtVoid 		SetTranslate( CrtVec4f ); 
	CrtVoid			SetRotate( CrtVec4f );  
	CrtVoid			SetScale( CrtVec4f );  
	CrtVoid			SetLookAt( CrtVec3f v[] ); 
	CrtVoid			SetMatrix( CrtMatrix m ); 

	inline			CrtVoid			SetAnimation( CrtAnimation * Anim ) { Animation = Anim; }; 
	inline			CrtVoid			AddChannelId( CrtUInt id )
	{
		if ( NumChannels < MAX_TFORM_CHANNEL_IDS ) 
			ChannelIds[NumChannels++] = id; 		
	}

	CrtVoid					SetSid( CrtChar * Sid ); 
	inline	CrtChar			*GetSid(){ return Sid; }
	inline	CrtVoid			SetType( CrtTFormType type ){ Type = type; };
	inline	CrtTFormType	GetType(){ return Type; }	
	static	CrtTFormType	GetType( CrtChar * nameType ); 
	inline	CrtVec4f		&GetVecTrans(){ return Vector;} // this will be rot,trans or scale depending on type	
	inline	CrtVec3f		*GetLookAt(){ return LookAt;} 
	inline	CrtAnimation	*GetAnimation(){return Animation; }
	inline	CrtUInt			GetChannelId(CrtUInt id){return ChannelIds[id]; }
	inline	CrtUInt			GetNumChannels(){return NumChannels; }
	inline  CrtMatrix		&GetMatrix(){return Matrix; }
};



class CrtOrient : public CrtBase
{
protected:	
	std::vector<CrtTransform *>	Transforms;

	CrtBool			UpdateLocalMatrix;					/**< LocalMatrix needs updating (initialize or update due to animations */
	CrtMatrix		LocalMatrix;						/**< Matrix version of just the transforms in this node */
	CrtMatrix		LocalToWorldMatrix;					/**< Full local to world matrix for this node */
	CrtMatrix		InverseLocalToWorldMatrix;
	CrtMatrix		InverseTransposeLocalToWorldMatrix;
	CrtMatrix		InverseBindMatrix; 
	CrtMatrix		CombinedSkinMatrix; 
	CrtFloat		*GetSkinMatrix()			{return (CrtFloat *)CombinedSkinMatrix;} 	
	CrtBool			UpdateLocalToWorldMatrix; 
	void	LoadOrientScale( const CrtVec3f * data, const CrtChar * sid );
	void	LoadOrientMatrix( const CrtChar * data ); 
	void	LoadIOrientMatrix( const CrtChar * data ); 

public:
	CrtOrient();
	~CrtOrient()
	{
		for (size_t i=0; i<Transforms.size(); i++)
		{
			CrtDelete(Transforms[i]);
		}
		Transforms.clear();
	}
	
	//---------------------- External Interfaces ---------------------------------//	
	
	// if UsingLookAtook() 
	// you should use the GetEyepoint, Center and Up with GLU as I have in my code
	// check line 156 RenderPC.cpp
	
	
	// else use get the axis rotations and build your local matrix from those 

	CrtMatrix *	SetLocalToWorldMatrix(CrtMatrix * m) 
	{
		memcpy(&LocalToWorldMatrix, m, sizeof(CrtMatrix));
		UpdateLocalMatrix = CrtFalse;
		UpdateLocalToWorldMatrix = CrtFalse;
		return &LocalToWorldMatrix;
	};
	inline	CrtMatrix *	GetLocalToWorldMatrix(){ return &LocalToWorldMatrix; } 
	inline	CrtMatrix *	GetInverseLocalToWorldMatrix(){ return &InverseLocalToWorldMatrix; } 
	inline	CrtMatrix *	GetInverseTransposeLocalToWorldMatrix(){ return &InverseTransposeLocalToWorldMatrix; } 
	inline  CrtMatrix * GetInverseBindMatrix(){ return &InverseBindMatrix; }


	//talk to greg about these set flags TODO
	inline  void SetUpdateLocalMatrix(bool b) { UpdateLocalMatrix = (CrtBool) b;};
	inline  void SetUpdateLocalToWorldMatrix(bool b) { UpdateLocalToWorldMatrix = (CrtBool) b;};

	//---------------------- External Interfaces ---------------------------------//	
	

};

class CrtNode;
class CrtInstanceMaterial;
class CrtInstance
{
public:
	CrtInstance() :  Parent(0) {};
	~CrtInstance();
	CrtNode		*Parent;			/**< Node where this instance was instantiated */
	std::vector <CrtInstanceMaterial *> MaterialInstances; /**< list of material instances in the <technique_common> */
};

#endif // _CRT_DATA_H_


