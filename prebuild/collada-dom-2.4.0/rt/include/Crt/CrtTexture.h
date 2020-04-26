/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_TEXTURE_H_
#define _CRT_TEXTURE_H_

//#include "Crt/CrtCg.h"
//#include "Crt/CrtNode.h"
//#include "Crt/CrtScene.h"
#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtTypes.h"
#include "Crt/CrtUtils.h"
#include "Crt/CrtRender.h"

class CrtImage; 
CrtBool	CrtLoadTexture( CrtImage * texObj, CrtChar * name, CrtChar * sceneFileName ); 
CrtBool  CrtDeleteTexture( CrtImage * texObj);

class CrtImage: public CrtBase 
{
protected:

	CrtInt32 		Id; 
	CrtInt32			Width;
	CrtInt32			Height;
	CrtInt32			Format;
	CrtUChar	*		Data; 
	CrtChar			FileName[CRT_MAX_URI_SIZE];

public:
	CrtImage()
	{
		Id = 0; 
		FileName[0] = 0; 
		Width = 0;
		Height = 0; 
		Format = -1;
		Data = NULL; 
	}

	~CrtImage()
	{
		CrtDeleteTexture (this);
	}
	inline	void SetFileName( const CrtChar * name )
	{
		CrtCpy( FileName, name ); 
	}

	inline  CrtChar * GetFileName()
	{
		return FileName; 	
	}
	
	inline	CrtBool	LoadImageFile( CrtChar * sceneFileName )
	{
		return CrtLoadTexture( this, FileName, sceneFileName ); 		
	};

	inline	void	SetId( unsigned CrtInt32 i ){ Id = i; };
	inline	void	SetWidth( CrtInt32 w) { Width = w; }
	inline	void	SetHeight( CrtInt32 h ) { Height = h; } 
	inline	void	SetImageData( CrtUChar * d ) { Data = d; }
	inline	void	SetFormat( CrtInt32 f ) { Format = f; }

	inline	CrtUInt32	GetId(){ return Id; };
	inline	CrtInt32	GetWidth() { return Width; }
	inline	CrtInt32	GetHeight() { return Height; }
	inline	CrtInt32	GetFormat() { return Format; }
	inline	void *		GetImageData() { return Data;  }

};


class CrtTexture : public CrtBase 
{
	CrtChar			ImageFileName[CRT_MAX_URI_SIZE];
	
	CrtInt32 		Id; 
	CrtInt32			Width;
	CrtInt32			Height;
	CrtInt32			Format;
	CrtUChar	*		Data; 	
	CrtImage *		Image; 


public:
	
	CrtTexture()
	{
		Id = 0; 
		ImageFileName[0] = 0; 
		Width = 0;
		Height = 0; 
		Format = -1;
		Data = NULL; 
		Image = NULL; 
	}

	~CrtTexture()
	{
		CrtDeleteData( Data ); 
/*		if (Id!=0) {
			glDeleteTextures(1, (unsigned int *)&Id);
			Id = 0;
		}
*/	}

protected:
	friend class CrtScene;
	
	inline	void SetImageName( const CrtChar * name )
	{
		CrtCpy( Name, name ); 
	}

	inline	void SetImageFileName( const CrtChar * name )
	{
		CrtCpy( ImageFileName, name ); 
	}

	inline  CrtChar * GetImageName()
	{
		if ( Image )
			return Image->GetName(); 
		return NULL; 	
	}
	

public:
	//---------------------- External Interfaces ---------------------------------//
	// currently the only file supported are targa's and that is probably will all 
	// that will be supported for the demo as well.  If we get texture data in another
	// format I will handle converting them to tga 24bit rgb and 32bit rgba.  
	
	inline	CrtChar * GetImageFileName()
	{
		return ImageFileName;
	}
    
	inline	void	SetId( unsigned CrtInt32 i ){ Id = i; };
	inline	void	SetWidth( CrtInt32 w) { Width = w; }
	inline	void	SetHeight( CrtInt32 h ) { Height = h; } 
	inline	void	SetImageData( CrtUChar * d ) { Data = d; }
	inline	void	SetFormat( CrtInt32 f ) { Format = f; }

	inline	void	SetImage( CrtImage * image ){ Image = image; }

	inline	CrtUInt32	GetId(){ return Id; };
	inline	CrtInt32	GetWidth() { return Width; }
	inline	CrtInt32	GetHeight() { return Height; }
	inline	CrtInt32	GetFormat() { return Format; }
	inline	void *		GetImageData() { return Data;  }

	inline	CrtImage	*	GetImage(){ return Image; }

	//---------------------- External Interfaces ---------------------------------//
	
};

#endif // _CRT_TEXTURE_H_

