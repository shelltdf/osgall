/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtTargaLoader.h"

//#define CRT_WORDS_BIGENDIAN
#define FORCE_RGB_TO_RGBA	1

CrtUByte CrtGetByte(CrtUChar *& data)
{
    CrtUByte b;

    if ( (data)  == NULL )
        return 0;

    b = (*data);

    (data) ++;
    return b;
};

CrtInt32 CrtSwapWord( CrtUInt32* word ) 
{
    unsigned char*  pVal;
    pVal    =   (unsigned char*)(word);
    	
#ifdef CRT_WORDS_BIGENDIAN
    return( (CrtInt32(pVal[0]) << 24) | (CrtInt32(pVal[1] << 16)) | (CrtInt32(pVal[2]) << 8) | (CrtInt32(pVal[3]) << 0) );	
#else
    return( (CrtInt32(pVal[0]) << 0) | (CrtInt32(pVal[1] << 8)) | (CrtInt32(pVal[2]) << 16) | (CrtInt32(pVal[3]) << 24) );
#endif 

}

CrtInt16 CrtSwapShort( CrtUInt16 * hword )
{
	unsigned char * pVal; 
	pVal = (unsigned char *)(hword);
	return( (CrtInt16(pVal[0]) << 0) | (CrtInt16(pVal[1] << 8)) );
//#ifdef CRT_WORDS_BIGENDIAN	
//	return (pVal[0]<<8) | pVal[1];
//#else
//	return pVal[0] | (pVal[1]<<8);
//#endif 	
	
}

CrtVoid CrtSwapImageData( CrtUInt32* words, CrtInt dataSize, CrtUInt bytesPerPixel  )
{
	CrtUChar * data = (CrtUChar *) words;
	CrtUInt32 newData = 0; 
	CrtUChar * newBytes = NULL; 

	CrtPrint("Data size %d \n", dataSize ); 
	// for swapping the texture image data; 
	for ( CrtUInt i = 0; i < dataSize/bytesPerPixel; i++)
	{	
		// swap and go on to the next 4 byte word 
		newData = CrtSwapWord( (CrtUInt32*)data );
		// new bytes 
		newBytes = (CrtUChar * ) &newData; 
		// now copy the swapped data back 
		switch( bytesPerPixel )
		{
			case 3:
			data[0] = newBytes[0];
			data[1] = newBytes[1];
			data[2] = newBytes[2];
			break;
			case 4:		
			data[0] = newBytes[2];
			data[1] = newBytes[1];
			data[2] = newBytes[0];
			data[3] = newBytes[3];
			break; 
		}
	
	//	if ( i < 5 )
	//	{	
	//		CrtPrint("bytes %d %d %d \n", (int)data[0], (int)data[1], (int)data[2] ); 	
	//		CrtPrint("bytes %d %d %d \n", (int)newBytes[0], (int)newBytes[1], (int)newBytes[2] ); 	
	//	}

		data += bytesPerPixel;   
	}

//	CrtUChar * dat = (CrtUChar * )words; 
//	for ( CrtUInt u = 0; u < 5 * bytesPerPixel; u+= bytesPerPixel)
//	{
//		CrtPrint("image bytes %d %d %d \n", (int)dat[0], (int)dat[1], (int)dat[2] );
//		dat += bytesPerPixel;  	
//	}

	return; 
}

CrtVoid CrtSwapHeader( CrtTargaHeader * header )
{
	// only need to swap the 16 bit values of the targa header 
	header->XOrigin = CrtSwapShort( &header->XOrigin ); 
	header->YOrigin = CrtSwapShort( &header->YOrigin ); 
	header->ImageWidth = CrtSwapShort( &header->ImageWidth ); 
	header->ImageHeight = CrtSwapShort( &header->ImageHeight ); 

	return;
}

// load a targa file from the file preloaded into memory 
CrtTgaFile *CrtLoadTargaFromMemory( CrtUChar * Memory )
{
    CrtUChar * data;
    
    data = (CrtUChar *) Memory; 
	
	if ( !data )
		return NULL;

	CrtTgaFile * tga = CrtNew( CrtTgaFile );
	memcpy( &tga->Header , data, sizeof(CrtTargaHeader));
	data += sizeof(CrtTargaHeader);
	
	// need to swap the data if needed; 
	CrtSwapHeader( &tga->Header );

	CrtPrint("CrtTargaLoader: Texture loaded to memory width %d height %d pixeldepth %d ", 
		(int)tga->Header.ImageWidth, 	(int)tga->Header.ImageHeight, (int)tga->Header.PixelDepth ); 

#ifdef CRT_WORDS_BIGENDIAN	
	CrtPrint("with big endian swapping. ");
#else
	CrtPrint("with no endian swapping. ");
#endif 
	
	CrtSwapImageData( (CrtUInt32 *)data, 
		tga->Header.PixelDepth * tga->Header.ImageWidth * tga->Header.ImageHeight/8,
		tga->Header.PixelDepth/8  ); 
	
//	CrtUChar * dat = (CrtUChar * )data; 
//	for ( CrtUInt u = 0; u < 5 * tga->Header.PixelDepth/8; u+= tga->Header.PixelDepth/8)
//	{
//		CrtPrint("original image bytes %d %d %d \n", (int)dat[0], (int)dat[1], (int)dat[2] );
//		dat += tga->Header.PixelDepth/8;  	
//	}


    // Only true color, non-mapped or compressed images are supported
    if( ( 0 != tga->Header.ColormapType ) || ( tga->Header.ImageType != 10 && tga->Header.ImageType != 2 ) )
    {
		CrtPrint("The Texture format isn't supported by the CrtTarga Loader \n" ); 
		CrtPrint("Please convert to 24 or 32 bit no REL compressed targa \n" ); 
		return NULL;
    }

    // Skip the ID field. The first byte of the header is the length of this field
    if( tga->Header.IDLength )
	data += tga->Header.IDLength;

#ifdef FORCE_RGB_TO_RGBA
		tga->BBP = 32;
#else
		tga->BBP = tga->Header.PixelDepth;
#endif

	// allocate for the data 
    if ( tga->BBP == 32 )
		tga->RGBAData = CrtNewData( CrtColor4i , tga->Header.ImageWidth * tga->Header.ImageHeight);
	else
		tga->RGBData = CrtNewData( CrtColor3i , tga->Header.ImageWidth * tga->Header.ImageHeight);

    if( tga->RGBAData == NULL && tga->RGBData == NULL )
    {
        return NULL;
    } 
	
	int pCount = 0;

    for( CrtInt y=0; y <tga->Header.ImageHeight; y++ )
    {
        CrtInt dwOffset = y*tga->Header.ImageWidth;

        if( 0 == ( tga->Header.ImageDescriptor & 0x0010 ) )
            dwOffset = (tga->Header.ImageHeight-y-1)*tga->Header.ImageWidth;

        for( CrtInt x=0; x<tga->Header.ImageWidth; x++ )
        {
            {
                CrtUByte b = CrtGetByte( data );
                CrtUByte g = CrtGetByte( data );
                CrtUByte r = CrtGetByte( data );
                CrtUByte a = (CrtUByte)255;
                if( tga->BBP == 32 )
				{
					if (tga->Header.PixelDepth == 32)
					    a = CrtGetByte( data );
            
                    tga->RGBAData[dwOffset+x].r = r;
                    tga->RGBAData[dwOffset+x].g = g;
                    tga->RGBAData[dwOffset+x].b = b;
                    tga->RGBAData[dwOffset+x].a = a;
					pCount++;
				}
				else
				{
				
                    tga->RGBData[dwOffset+x].r = r;
                    tga->RGBData[dwOffset+x].g = g;
                    tga->RGBData[dwOffset+x].b = b;
					pCount++;
				}
            }
        }
    }

//	dat = (CrtUChar * )tga->RGBData; 
//	for ( CrtUInt u = 0; u < 5 * tga->Header.PixelDepth/8; u+= tga->Header.PixelDepth/8)
//	{
//		CrtPrint("copied image bytes %d %d %d \n", (int)dat[0], (int)dat[1], (int)dat[2] );
//		dat += tga->Header.PixelDepth/8;  	
//	}


	// Flipping the Tga data before passing to OpenGL 
	{
        CrtInt32 bbp = 0;
		CrtChar * imageData = NULL; 

		switch ( tga->BBP )
        {
        case 24:
            bbp = 3;
			imageData = (CrtChar *)tga->RGBData;
            break;
        case 32:
            bbp = 4; 
			imageData = (CrtChar *)tga->RGBAData;
            break;
		default:
			CrtAssert("COLLADA RT TargaLoader Only supports 32 and 24 bit texture supported \n", CrtFalse ); 
			return NULL; 
        }

        {
			CrtInt32 w = tga->Header.ImageWidth;
			CrtInt32 h = tga->Header.ImageHeight; 
            CrtInt32 lineSize = bbp*w;

            CrtChar * newImage = CrtNewData( CrtChar , bbp*w*h );
			CrtChar * imageCurPos = (CrtChar *)imageData; 
            CrtChar * imageNewPos = newImage + (bbp * w * (h-1));// start at bottom line 

            CrtInt32 i = 0;
			
            for ( i = 0 ; i < h; i++ )
            {
                memcpy( imageNewPos, imageCurPos, lineSize );
                imageNewPos -= lineSize;
                imageCurPos += lineSize; 
            }

            memcpy( imageData, newImage, bbp * w * h ); 

            CrtDeleteData ( newImage ); 
        }
    }

	if (tga->Header.PixelDepth == 32 )
	{
		tga->HasAlpha = CrtTrue;
	}
	else
	{
		tga->HasAlpha = CrtFalse;
	}
    
    return tga;
}

// load a targa file from the file  
CrtTgaFile *CrtLoadTargaFromFile( CrtChar * fileName )
{
	CrtFile file;
	file.LoadFile( fileName );	 
    
	CrtUChar * data;
    
    data = (CrtUChar *) file.GetData(); 
	
	if ( !data )
	{
		CrtPrint( "Failed to Load File %s \n", fileName  ); 
		return NULL;
	}

	return CrtLoadTargaFromMemory( (CrtUChar *)file.GetData() );         
}

