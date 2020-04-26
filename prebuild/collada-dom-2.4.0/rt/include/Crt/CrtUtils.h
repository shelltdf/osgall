/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_UTILS_H_
#define _CRT_UTILS_H_

#include "Crt/CrtData.h"
#include "Crt/CrtMemory.h"
#include "Crt/CrtTypes.h"

#include <stdlib.h>

// --------------------------------------------------
//		Utility function protos 
// --------------------------------------------------

// utility parsing functions 
inline CrtInt32			CrtGetValueLenth(	const CrtChar*	str );
inline CrtInt32			CrtEatWhite(			const CrtChar**	data );
inline const CrtChar*	CrtGetValueString(	const CrtChar**	data );

const CrtChar*			CrtGetString(		const CrtChar**	data );
CrtInt32					CrtGetIntString(		const CrtChar**	data );
CrtFloat					CrtGetFloatString(	const CrtChar**	data );

// slightly more advance utilites that parse the node trees as well as set data 
void					CrtGetColor3Data(	CrtColor3f& color, const CrtChar* data );
void					CrtGetColor4Data(	CrtColor4f& color, const CrtChar* data );
void					CrtGetVector3Data(	CrtVec3f& vec, const CrtChar* data );
void					CrtGetVector4Data(	CrtVec4f& vec, const CrtChar*	data );

class TiXmlNode; 
// simple node search utility functions for locate a specific child node 
//TiXmlNode * CrtFindChild( TiXmlNode * , CrtChar * named );
TiXmlNode * CrtFindChild( TiXmlNode * node, CrtChar * named );
TiXmlNode * CrtFindChildOrSibling( TiXmlNode * , CrtChar * named ); 
TiXmlNode * CrtFindSibling( TiXmlNode * , CrtChar * named ); 




// --------------------------------------------------
//		Inlined parser function bodies 
// --------------------------------------------------

inline	bool CrtCmp( const CrtChar * s1, const CrtChar * s2 )
{	
	if ( strcmp( s1, s2 )  == 0 )
		return true;
	return false;
}

#ifndef _stricmp
inline int _stricmp(const char *s1, const char *s2)
{
  char f, l;

  do 
  {
    f = ((*s1 <= 'Z') && (*s1 >= 'A')) ? *s1 + 'a' - 'A' : *s1;
    l = ((*s2 <= 'Z') && (*s2 >= 'A')) ? *s2 + 'a' - 'A' : *s2;
    s1++;
    s2++;
  } while ((f) && (f == l));

  return (int) (f - l);
}
#endif 

inline	bool CrtICmp( const CrtChar * s1, const CrtChar * s2 )
{	
	if ( _stricmp( s1, s2 )  == 0 )
		return true;
	return false;
}

inline	bool CrtCmpIn( const CrtChar * s1, const CrtChar * s2 )
{	
	if ( strstr( s1, s2 ) != NULL )
		return true;
	return false;
}

inline	CrtChar * CrtCpy( CrtChar * dest, const CrtChar * src )
{	
	return strcpy( dest, src );	
}

inline	CrtChar * CrtNCpy( CrtChar * dest, const CrtChar * src , const CrtInt n)
{	
	return strncpy( dest, src, n );	
}

inline	CrtChar * CrtCat( CrtChar * dest, const CrtChar * src )
{
	return strcat( dest, src ); 
}

inline	void CrtCpy( CrtChar * dest, const CrtChar * src, const CrtChar endChar )
{
	CrtUInt len = (CrtUInt)strlen( src );
	CrtUInt i = 0; 

	while ( i < len )
	{
		// need to stop at a certain at a given character passed in. 
		if ( src[i] == endChar )
		{
			dest[i] = '\0';
			break; 
		}

		dest[i] = src[i]; 
		i++;		
	}
}

inline	CrtChar * CrtFindStr( const CrtChar * str, const CrtChar * c )
{
	return strstr((CrtChar *)str,(CrtChar *)c); 
}

inline	CrtInt	CrtLen( const CrtChar * s1 )
{
	return (CrtUInt)strlen( s1 ); 
}

inline CrtInt	CrtEatWhite(	const CrtChar**	data )
{
	const CrtChar*	c	=	*data;
	CrtInt32			w	=	0;

	while	(	*c==' '
			||	*c=='\t'
			||	*c=='\r'
			||	*c=='\n' )
	{
		w++;
		c++;
	}
	*data	=	c;
	return	(CrtInt)w; 
}

inline CrtInt	CrtGetValueLenth(	const CrtChar*	str )
{
	CrtInt32			len	=	0;
	const CrtChar*	c	=	str;

	while	(	c[0] != '\0'
			&&	c[0] != ' '
			&&	c[0] != '\t'
			&&	c[0] != '\r'
			&&	c[0] != '\n'
			&&	c[0] != '@' )
	{
		len++;
		c++;
	}

	return	(CrtInt)len; 
}

inline const CrtChar*	CrtGetValueString(	const CrtChar**	data )
{
	const CrtChar*	d	=	*data;
	CrtInt32			w	=	CrtEatWhite(&d);
	CrtInt32			len	=	CrtGetValueLenth(d);
	*data	+=	len + w;
	return	(CrtChar *)d; 
}

inline CrtFloat			CrtGetFloatString(	const CrtChar**	data )
{
	return	(CrtFloat)atof(CrtGetValueString(data));
}

inline CrtInt32			CrtGetIntString(		const CrtChar**	data )
{
	return	atoi(CrtGetValueString(data));
}

inline const CrtChar*	CrtGetString(		const CrtChar**	data )
{
	return	CrtGetValueString(data); 
}

inline CrtChar * CrtGetExtention( CrtChar * name )
{
	CrtUInt len = (CrtUInt)strlen( name );
	CrtChar * tName = name + len; 

	// from the end go backward to find the first forward or backslash
    while ( tName[0] != '.' && len > 0 )
    {
        tName--;
        len--; 
    }

	return tName; 
}


inline CrtChar * CrtGetAfterPath( CrtChar * name )
{
	CrtUInt len = (CrtUInt)strlen( name ); 
	CrtChar * tName = name + strlen( name ); 
	
	// from the end go backward to find the first forward or backslash
	while ( tName[0] != '/' && tName[0] != '\\' && len > 0 )
	{
		tName--;
		len--; 
	}	

	if ( len != 0 )
        tName ++;

	return tName; 
}

//-------------------------------------------------
//-------------------------------------------------
inline CrtUInt32	Convert( CrtColor3f col )
{
	return	(	 ((CrtUInt32)CrtClampf( col.r*255.0f + 0.5f, 0.0f, 255.0f )) 
			|	(((CrtUInt32)CrtClampf( col.g*255.0f + 0.5f, 0.0f, 255.0f )) << 8)
			|	(((CrtUInt32)CrtClampf( col.b*255.0f + 0.5f, 0.0f, 255.0f )) << 16) );
}

inline CrtUInt32	Convert( CrtColor3f col, CrtFloat alpha )
{
	return	(	 ((CrtUInt32)CrtClampf( col.r*255.0f + 0.5f, 0.0f, 255.0f )) 
			|	(((CrtUInt32)CrtClampf( col.g*255.0f + 0.5f, 0.0f, 255.0f )) << 8)
			|	(((CrtUInt32)CrtClampf( col.b*255.0f + 0.5f, 0.0f, 255.0f )) << 16)
			|	(((CrtUInt32)CrtClampf( alpha*255.0f + 0.5f, 0.0f, 255.0f )) << 24) );
}

inline CrtUInt32	Convert( CrtColor4f col )
{
	return	(	 ((CrtUInt32)CrtClampf( col.r*255.0f + 0.5f, 0.0f, 255.0f )) 
			|	(((CrtUInt32)CrtClampf( col.g*255.0f + 0.5f, 0.0f, 255.0f )) << 8)
			|	(((CrtUInt32)CrtClampf( col.b*255.0f + 0.5f, 0.0f, 255.0f )) << 16)
			|	(((CrtUInt32)CrtClampf( col.a*255.0f + 0.5f, 0.0f, 255.0f )) << 24) );
}

//-------------------------------------------------
// simple File IO class for encapsulating the io 
//-------------------------------------------------

class CrtFile 
{
	CrtChar		*data;
	CrtInt32	size; 

public:
	CrtFile()
	{
		data = NULL;
		size = 0;
	}

	~CrtFile()
	{
		CrtDeleteData( data );
	}

	CrtBool 	LoadFile( CrtChar * fName ); 
	inline 	CrtChar *	GetData(){ return data; }
	inline	CrtInt32 	GetSize(){ return size; } 
	inline	void	DeleteData(){ CrtDelete(data ); } //{ CrtDeleteData( data ); } 

};


#endif // _CRT_UTILS_H_

