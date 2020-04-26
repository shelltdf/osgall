/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#include "Crt/CrtUtils.h"

//========================================

#ifdef _WIN32 

#include <windows.h>
#include <stdio.h>

inline void CrtPrintConsole(const CrtChar * str)
{
	OutputDebugString( str );
}

inline void CrtPrint(const CrtChar* str, ...)
{
	va_list	valist;
	CrtChar		tempStr[4096]; 

	va_start(valist, str);
	vsprintf(tempStr, str, valist);
	va_end(valist);

	CrtPrintConsole( tempStr ); 	
}

#else

#include <stdarg.h>
#include <stdio.h>

#ifdef LINUX
void CrtPrint(const CrtChar* str, ...)
{
	CrtChar		tempStr[4096]; 
	va_list		valist;
    
	va_start(valist, str);
	vsprintf(tempStr, str, valist);
	va_end(valist);

    printf( tempStr );
}
#else
void CrtPrint(const CrtChar* str, ...)
{
//	CrtChar	tempStr[4096]; 
	/*va_list *	valist;
    
	va_start(valist, &str);
	vsprintf(tempStr, str, valist);
	va_end(valist);

	*/

// FIXME:
// I didn't want to interfere, so I didn't change this, but this will
// crash if str has formatting characters that expect parameters!
// - G
//
    printf( str );
}
#endif

inline void CrtPrintConsole(const CrtChar * str)
{	
	CrtPrint("%s", str ); 
}
#endif 



void CrtPrintString( CrtChar * str, CrtChar * label )
{
	CrtChar s[200];
	CrtInt32 c = 0; 
	while ( c < 10)
	{
		s[c] = str[c]; 
		c++;
	}

	s[c] = 0;

	CrtPrint( "%s %s \n",label, s ); 
	
}
//---------------------------------------------------------------------------------------------------------
// Open file, allocate RAM and read file entirely into RAM buffer
CrtBool	CrtFile::LoadFile( CrtChar *fName)
{
	FILE		*fp;

	// Error check the file name
	if ( !fName || strlen(fName) == 0 )
		return CrtFalse; 

	// Open the file, complain and exit if we can't
	fp= fopen( fName, "rb");
	if ( !fp ) 
	{
		CrtPrint("CrtFile: Failed to open file = %s fp = %d \n", fName, fp ); 
		return CrtFalse;
	}

	// Find the size of the file

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp); 

	if(!size)
	{
		CrtPrint("CrtFile Seek Failed for file %s \n", fName ); 
		return CrtFalse; 
	}

	// Allocate RAM and load the file into it

	data = CrtNewData( CrtChar, size );

	if(data == NULL)
	{
		CrtPrint("CrtFile Could not allocate %d bytes of RAM for file %s\n",size,fName);
		return CrtFalse;
	}

	fread(data, 1, size, fp);
	CrtPrint("CrtFile: Read file %s size %d\n", fName, size ); 
	fclose(fp);	
	return CrtTrue;
};



void	CrtGetColor3Data(	CrtColor3f&		color,
							const CrtChar*	data )
{
	color.r = CrtGetFloatString(&data);
	color.g = CrtGetFloatString(&data);
	color.b = CrtGetFloatString(&data);
	color.Clamp();
}

void	CrtGetColor4Data(	CrtColor4f&		color,
							const CrtChar*	data )
{
	color.r = CrtGetFloatString(&data);
	color.g = CrtGetFloatString(&data);
	color.b = CrtGetFloatString(&data);
	color.a = CrtGetFloatString(&data);
	color.Clamp();
}

void	CrtGetVector3Data(	CrtVec3f&		vec,
							const CrtChar*	data )
{
	vec.x = CrtGetFloatString(&data);
	vec.y = CrtGetFloatString(&data);
	vec.z = CrtGetFloatString(&data);
}

void	CrtGetVector4Data(	CrtVec4f&		vec,
							const CrtChar*	data )
{
	vec.x = CrtGetFloatString(&data);
	vec.y = CrtGetFloatString(&data);
	vec.z = CrtGetFloatString(&data);
	vec.w = CrtGetFloatString(&data);
}

/*
CrtBool CrtSearchForChild( TiXmlNode * node, CrtChar * named, TiXmlNode ** foundNode )
{
	TiXmlNode * n = node;
	TiXmlElement * e;

	TiXmlNode * c = n->FirstChild(); 
	
	while ( c )
	{			
		if ( CrtSearchForChild( c, named, foundNode ) )
			return CrtTrue;

		e = c->ToElement();
		
		if( e )
		{
			const CrtChar * name = e->Value();
			if ( e && CrtCmp( name, named) )
			{	
				*foundNode = c; 
				return CrtTrue; 
			}
		}
		
		c = c->NextSibling(); 
	}	

	return CrtFalse; 
};


TiXmlNode * CrtFindChild( TiXmlNode * node, CrtChar * named )
{
    TiXmlNode * FoundNode;

 	if ( CrtSearchForChild( node, named, &FoundNode ))
		return FoundNode;
	return NULL; 
};


TiXmlNode * CrtFindChildOrSibling( TiXmlNode * node, const CrtChar * named )
{
	
	
	
	return NULL;					
}; 


TiXmlNode * CrtFindSibling( TiXmlNode * node, const CrtChar * named )
{
	
	
	
	return NULL; 
}; */

