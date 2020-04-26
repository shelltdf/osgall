/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtPlatform.h"
#include "Crt/CrtRender.h"
#include "Crt/CrtTexture.h"
#include "Crt/CrtTargaLoader.h"
#include "Crt/CrtImageLoader.h"

#include <stdio.h>
extern CrtChar TextureFilePrefix[];

enum CrtTextureType 
{
    eCrtTGA,
    eCrtPNG,
    eCrtUNKNOWN,
};

CrtInt32     CrtGetTextureType( CrtChar * name )
{
    if ( CrtCmpIn( name, ".tga" ) || CrtCmpIn( name, ".TGA" ) )
        return eCrtTGA;
    if ( CrtCmpIn( name, ".png" ) || CrtCmpIn( name, ".PNG" ) )
        return eCrtPNG;
    return eCrtUNKNOWN;
}

CrtBool  CrtDeleteTexture( CrtImage * texObj)
{
//	CrtDelete( texObj->tga );
	return _CrtRender.DeleteTexture(texObj);
}

CrtBool  CrtLoadTexture( CrtImage * texObj, CrtChar * name, CrtChar * sceneFileName  )

{
    //try to load the file from the path given. If it fails go to the backup
//	CrtTgaFile * tga = NULL;
	CrtFile f;

	CrtChar * ext = CrtGetExtention( name );
	if ( !CrtICmp( ext, ".tga" ) )
	{
#ifdef _WIN32
		if ( CrtLoadImage( name, texObj ) )
		{
			_CrtRender.CreateTexture( texObj );
			return CrtTrue; 
		}
#endif //_WIN32
	}
	if ( !CrtICmp( ext, ".tga" ) || !f.LoadFile(name) )
	{
		CrtChar filename[CRT_MAX_NAME_SIZE];
		strncpy(filename, name, ext - name);
		filename[ext - name] = 0;

		// add the path location to the texture name 
		CrtChar pathName[CRT_MAX_NAME_SIZE];
		CrtChar sceneName[CRT_MAX_NAME_SIZE];

		// need to remove the path if there is one 
		CrtChar * texName = CrtGetAfterPath( name ); 

		// remove the ending from the scene file name 
		strcpy( sceneName , CrtGetAfterPath(sceneFileName) ); 
		
		// first find the dot 
		ext = CrtGetExtention( sceneName ); 
			
		// remove it 
		if ( ext )
			ext[0] = 0;

		// Add the _Textures prefix only if the scene name wasn't empty
		if(*sceneFileName == 0)
			strcpy(pathName, texName);
		else
			sprintf( pathName, "%s_Textures/%s", sceneName, texName );
		
		ext = CrtGetExtention( pathName ); 

		// COLLADA_RT only support targa so that is what we are going to look for  
		if ( !CrtICmp( ext, ".tga" ) )
			CrtCpy( ext, ".tga" ); 

		// Build a path using the scene name ie: images/scene_Textures/boy.tga
		CrtChar newTexName[CRT_MAX_NAME_SIZE]; 	
		sprintf(newTexName, "%s%s", TextureFilePrefix, pathName ); 
		
		// Build a path for the Shared texture directory ie: images/Shared/boy.tga
		CrtChar sharedTexName[CRT_MAX_NAME_SIZE];
		sprintf(sharedTexName, "%sShared/%s",TextureFilePrefix, texName);

		// Build a path for the Shared texture directory ie: images/Shared/boy.tga
		CrtChar tgaName[CRT_MAX_NAME_SIZE];
		sprintf(tgaName, "%s.tga",filename);

		// Try to load the file from the scene-name path, if that fails tried the shared path
		if ( !f.LoadFile(newTexName) )
		{
			if(!f.LoadFile(sharedTexName))
			{
				if(!f.LoadFile(tgaName))
				{
					CrtPrint("*** Texture file not found in either %s or %s\n", newTexName, sharedTexName );       
					return CrtFalse;
				}
			}
		}
	}
	// Create a targa from the file image in memory, assume DeviL won't handle the image.

	CrtTgaFile * tga = CrtLoadTargaFromMemory( (CrtUChar *)f.GetData() );	
	
	if ( tga )
	{
		texObj->SetHeight( tga->Header.ImageHeight ); 
		texObj->SetWidth( tga->Header.ImageWidth ); 
		
		if ( tga->BBP == 32 )
		{
			texObj->SetImageData( (CrtUChar *)tga->RGBAData ); 
			texObj->SetFormat( TGA_TRUECOLOR_32 );
			CrtPrint("TexFormat 32 "); 
		}
		else
			if ( tga->BBP == 24 )
			{
				texObj->SetImageData( (CrtUChar *) tga->RGBData ); 
				texObj->SetFormat( TGA_TRUECOLOR_24 ); 
				CrtPrint("TexFormat 24 "); 
			}
			else
			{
				CrtPrint("Unsupported Targa format!!!\n" ); 
				CrtPrint("Make sure you are using 32 or 24 bit textures\n" ); 
			}

		_CrtRender.CreateTexture( texObj ); 
	}

	CrtDelete(tga);

    return CrtTrue;  
}; 

