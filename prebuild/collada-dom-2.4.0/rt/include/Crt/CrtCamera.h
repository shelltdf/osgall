/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_CAMERA_H_
#define _CRT_CAMERA_H_

#include "Crt/CrtData.h"
#include "Crt/CrtMatrix.h"

class CrtCamera;
class CrtNode;

/** The CrtInstanceCamera class holds the information needed to make an instance of an abstract camera.
 *  This maps to the <instance_camera> tag in COLLADA
 *  NOTE: This assumes that all camera instances that reference the same CrtCamera are sharing the info
 *  in that abstract camera, ie: if someone changes the stuff in the CrtCamera all the instances derived
 *  from it will change.  We could avoid this by keeping a copy of CrtCamera in the instance.
 */
class CrtInstanceCamera
{
public:
	CrtInstanceCamera()
	{
		x=0; y=0; z=0;
		pan=0; tilt=0; zoom=0;
		CrtMatrixLoadIdentity(transform);
	};
	CrtNode		*Parent;			/**<< Node where this instance was instantiated */
	CrtCamera	*AbstractCamera;	/**<< The abstract camera where the cam parameters are stored*/
	CrtMatrix   transform;
	void		SetPanAndTilt(CrtFloat setpan, CrtFloat settilt)
	{
		pan+=setpan;
		tilt+=settilt;
		SetTransform();
	}
	void        MoveOrbit(CrtFloat xx, CrtFloat yy)
	{
		CrtMatrixTranslate(transform, 0, 0, -zoom);
		CrtMatrixTranslate(transform, xx, yy, 0);
		x = transform[M30];
		y = transform[M31];
		z = transform[M32];
		SetTransform();
	}
	void        MoveTransform(CrtFloat forward, CrtFloat strafe, CrtFloat jump)
	{   
		CrtFloat COSPAN = cos(pan * CrtDEGREES_TO_RADIANS);
		CrtFloat SINPAN = sin(pan * CrtDEGREES_TO_RADIANS);

		x += - COSPAN * strafe + SINPAN * forward;
		y += jump;
		z += SINPAN * strafe + COSPAN * forward;
		SetTransform();
	}
	void		ZoomTransform(CrtFloat zoomz) 
	{
		zoom+=zoomz;
		SetTransform();
	}
private:
	void		SetTransform() 
	{
		CrtMatrixLoadIdentity(transform);
		CrtMatrixTranslate(transform, x, y, z);
		CrtMatrix4x4RotateAngleAxis(transform, 0.0f, 1.0f, 0.0f, pan);
		CrtMatrix4x4RotateAngleAxis(transform, 1.0f, 0.0f, 0.0f, tilt);
		CrtMatrixTranslate(transform, 0, 0, zoom);
	}
	CrtFloat	pan;
	CrtFloat	tilt;
	CrtFloat    x,y,z;
	CrtFloat	zoom;
};

/**
 * The CrtCamera class holds the parameters for a camera taken from a <camera> inside a <library_cameras>.  
 * These parameters are then used to instantiate an <instance_camera> in a node.  Currently, the
 * instance_cameras reference this data so if you change it here all the related instance_cameras will
 * change too.  This should probably be changed so each <instance_camera> has a copy of the <camera> data,
 * this would allow the parameters of individual <instance_camera> to change.
 *
 * NOTE: Should probably have a CrtCamera with subclasses for perspective and orthographic, but for now
 * this one class holds parameters for both.
 */
class	CrtCamera : public CrtBase 
{
private:
	CrtFloat		Xfov;		/**< X field of view for perspective camera */
	CrtFloat		Yfov;		/**< Y field of view for perspective camera */ 
	CrtFloat		Aspect;		/**< Aspect Ratio for perspective camera */
	CrtFloat		ZNear;		/**< Z clip near for perspective camera */
	CrtFloat		ZFar;		/**< Z clip far for perspective camera */

	CrtFloat		Xmag;		/**< X magnification for an orthographic camera */
	CrtFloat		Ymag;		/**< Y magnification for an orthographic camera */
		
public:
	CrtCamera()
	{
		Yfov = 36.0f;
		Aspect = 16.0f / 9.0f;
		ZNear = 10.0f;
		ZFar = 15000.0f;		
	}

	~CrtCamera()
	{
        Destroy(); 
	}

protected:

	void	Update( CrtInt32 sWidth, CrtInt32 sHeight );

public:

	inline CrtFloat	GetXFov()	{ return Xfov;	 }
	inline CrtFloat	GetYFov()	{ return Yfov;	 }
	inline CrtFloat	GetZNear()	{ return ZNear;  }
	inline CrtFloat	GetZFar()	{ return ZFar;	 }
	inline CrtFloat	GetAspect()	{ return Aspect; }
	inline CrtFloat	GetXMag()	{ return Xmag;	 }
	inline CrtFloat	GetYMag()	{ return Ymag;	 }

	inline CrtVoid	SetXFov(CrtFloat fovx)		{ Xfov   = fovx;  }
	inline CrtVoid 	SetYFov(CrtFloat fovf)		{ Yfov   = fovf;  }
	inline CrtVoid 	SetZNear(CrtFloat nearf)	{ ZNear  = nearf; }
	inline CrtVoid	SetZFar(CrtFloat farf)		{ ZFar	 = farf;  }
	inline CrtVoid	SetAspect(CrtFloat aspect)	{ Aspect = aspect;}
	inline CrtVoid	SetXMag(CrtFloat magx)		{ Xmag   = magx;  }
	inline CrtVoid 	SetYMag(CrtFloat magy)		{ Ymag   = magy;  }
};

#endif // CRT_CAMERA_H


