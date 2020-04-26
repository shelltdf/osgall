/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
/*
 * Binary file importer and exporter for the COLLADA FX loader library.
 * Summary of format:
 * newlines added for comment readability only
 * all string are NULL terminated
 * '...' means that the structure before may be repeated as many times as needed
 * All *_TOKEN are optional structures, but if they appear they must include data specified after
 * 
 * COLLADA_FX_BIN_ID
 * COLLADA_FX_BIN_VER
 * platform identifier
 * EFFECT_TOKEN effect-name chunk-size
 *	ANNOTATE_TOKEN annotate-name
 *		Type_ID data
 *	...
 *	NEWPARAM_TOKEN param-name semantic
 *		Type_ID data
 *	...
 *  SURFACE_TAG surface-name image-name format
 *  ...
 *	TECHNIQUE_TOKEN technique-name
 *		ANNOTATE 
 *		...
 *		INCLUDE_TAG include-name path
 *		... 
 *		SURFACE_TAG image-name format
 *		... 
 *		NEWPARAM 
 *		...
 *		SETPARAM_TOKEN param-name
 *			Type_ID data
 *		... 
 *		PASS_TOKEN pass-name
 *			ANNOTATE 
 *			...
 *			NEWPARAM 
 *			...
 *			SETPARAM 
 *			...
 *			GL_SETTING_TOKEN setting-Identifier setting-values
 *			...
 *			SHADER_TOKEN vertex-or-fragment-identifier source entry-name
 *				ANNOTATE_TOKEN
 *				...
 *				CONNECT_PARAM_TOKEN shader-symbol param-name
 *				...
 *				BIND_PARAM_TOKEN shader-symbol Type_ID data
 *				...
 *			...
 *		...
 *	...
 * ...
 * MATERIAL_TOKEN name effect-name size
 *   SETPARAM_TOKEN param-name
 *	   Type_ID data
 *   ...
 *   SURFACE_TAG surface-name image-name format
 *   ...
 * ...
 * EOF
 */

// System includes
#include <stdio.h>
#include <cstdlib>
#include <iostream>

//#include <Cg/cg.h>
#include <cfxPlatform.h>

// User includes

#include <cfxBinaryUtil.h>
#include <cfxAnnotatable.h>
#include <cfxAnnotate.h>
#include <cfxBindParam.h>
#include <cfxCode.h>
#include <cfxConnectParam.h>
#include <cfxData.h>
#include <cfxDataMaker.h>
#include <cfxEffect.h>
#include <cfxGlPipelineSetting.h>
#include <cfxGlPipelineSettingMaker.h>
#include <cfxGlSamplerSetting.h>
#include <cfxMaterial.h>
#include <cfxNewParam.h>
#include <cfxParamable.h>
#include <cfxPass.h>
#include <cfxSampler.h>
#include <cfxSetParam.h>
#include <cfxShader.h>
#include <cfxSurface.h>
#include <cfxTechnique.h>
#include <cfxError.h>
#include <cfxLoader.h>

#include <dae.h>
#include <dae/daeURI.h>

//File Header identifiers
#define COLLADA_FX_BIN_ID		0xDAEFEC
#define COLLADA_FX_BIN_VER		0x14

#define CFX_BIN_TOKEN_SIZE		2

//Chunk Header Tags
#define EFFECT_TOKEN			0xEFEC
#define ANNOTATE_TOKEN			0xA77E
#define NEWPARAM_TOKEN			0x2E39
#define TECHNIQUE_TOKEN			0x7C2C
#define INCLUDE_TOKEN			0x12CD
#define SURFACE_TOKEN			0x5E2F
#define SETPARAM_TOKEN			0x5E79
#define PASS_TOKEN				0x9A55
#define GL_SETTING_TOKEN		0x5E72
#define SHADER_TOKEN			0x5AD2
#define CONNECTPARAM_TOKEN		0xC2C7
#define BINDPARAM_TOKEN			0xB12D
#define MATERIAL_TOKEN			0x3A71

//Type Tags
#define CFXBOOL0				0xB000
#define CFXBOOL1				0xB001
#define CFXBOOL2				0xB002
#define CFXBOOL3				0xB003
#define CFXBOOL4				0xB004
#define CFXINT0					0x1200
#define CFXINT1					0x1201
#define CFXINT2					0x1202
#define CFXINT3					0x1203
#define CFXINT4					0x1204
#define CFXFLOAT0				0xF100
#define CFXFLOAT1				0xF101
#define CFXFLOAT2				0xF102
#define CFXFLOAT3				0xF103
#define CFXFLOAT4				0xF104
#define CFXFLOAT2X2				0xF122
#define CFXFLOAT3X3				0xF133
#define CFXFLOAT4X4				0xF144
#define CFXSAMPLER1D			0x5A31
#define CFXSAMPLER2D			0x5A32
#define CFXSAMPLER3D			0x5A33
#define CFXSAMPLERCUBE			0x5A3C
#define CFXSTRING				0x5712

std::string cfxBinaryUtil::fileName;
std::string cfxBinaryUtil::remotePath = "";

void cfxBinaryUtil::setRemotePath( const std::string &path )
{
	remotePath = path;
}

cfxBool cfxBinaryUtil::load(const std::string& file, std::map<std::string, cfxMaterial*>& materials, 
					      std::map<std::string, cfxEffect*>& effects, CGcontext _context)
{
	// Open file for loading
    FILE *input = NULL;

    // Load the COLLADA file
	std::string fn = remotePath + file;
	printf("loading %s\n", fn.c_str());
	input = fopen( fn.c_str(), "rb" );
    if ( input == NULL )
	{
		printf( "ERROR: Unable to open file %s!\n", file.c_str() );
	    return false;
	}
	// Load file header
	if ( !loadFileHeader( input ) ) 
	{
		printf( "ERROR: Improper file header!\n" );
		return false;
	}

	cgSetErrorCallback(cfxCgErrorCallback);

	if (_context == NULL)
	{
		_context = cgCreateContext(); 
	    cgGLRegisterStates(_context);
	}
	
	short token = 0;
	cfxBool trew = true;
	while( trew ) {
		readToken( token, input );
		if ( feof( input ) ) {
			//eof only happens after you try to read past it so try one more token.
			break;
		}
		cfxEffect* effect = NULL;
		cfxMaterial *material = NULL;
		switch( token ) 
		{
		case (short)EFFECT_TOKEN:
			// currently using managed textures to ensure that the texture objects are 
			// on the correct texture unit where the program expects them to be.
			cgGLSetManageTextureParameters(_context, true);

			effect = readEffect( input, _context ); 
			if ( effect == NULL )
			{
				return false;
			}
			effects.insert(std::make_pair(effect->getName(), effect));
#ifndef _LIB
			effect->apply();
#endif
			break;
		case (short)MATERIAL_TOKEN:
			material = readMaterial( input, _context, &effects );
			materials.insert(std::make_pair(material->getName(), material));
			break;
		default:
			printf( "Unexpected Token!" );
			return false;
		}
	}
	fclose( input );
    return true;
}

size_t cfxBinaryUtil::readString( std::string &str, FILE *input ) 
{
	str.clear();
	char currChar[2];
	currChar[1] = '\0';
	size_t count = 0;
	do
	{
		currChar[0] = fgetc( input );
		str.append( currChar );
		++count;
	}while( currChar[0] != '\0' );
	return count;
}

void cfxBinaryUtil::endianSwap( short *s )
{
	char temp;
	char *bytes = (char *)s;
	temp = bytes[0];
	bytes[0] = bytes[1];
	bytes[1] = temp;
}

void cfxBinaryUtil::endianSwap( cfxInt *i )
{
	size_t sz = sizeof( cfxInt );
	char temp;
	char *bytes = (char *)i;
	for ( size_t x = 0; x < sz/2; ++x )
	{
		temp = bytes[x];
		bytes[x] = bytes[sz-x-1];
		bytes[sz-x-1] = temp;
	}
}

void cfxBinaryUtil::endianSwap( cfxFloat *f )
{
	size_t sz = sizeof( cfxFloat );
	char temp;
	char *bytes = (char *)f;
	for ( size_t x = 0; x < sz/2; ++x )
	{
		temp = bytes[x];
		bytes[x] = bytes[sz-x-1];
		bytes[sz-x-1] = temp;
	}
}

size_t cfxBinaryUtil::readBool( cfxBool *buf, size_t count, FILE *input )
{
	//cfxBool has no endian issues
	return fread( buf, sizeof(cfxBool), count, input) * sizeof(cfxBool);
}

size_t cfxBinaryUtil::readInt( cfxInt *buf, size_t count, FILE *input )
{
#ifdef WIN32
	//win32 requires endian swap
	size_t sz = fread( buf, sizeof(cfxInt), count, input) * sizeof(cfxInt);
	for ( size_t i = 0; i < count; ++i ) 
	{
		endianSwap( buf+i );
	}
	return sz;
#else
	return fread( buf, sizeof(cfxInt), count, input) * sizeof(cfxInt);
#endif
}

size_t cfxBinaryUtil::readFloat( cfxFloat *buf, size_t count, FILE *input )
{
#ifdef WIN32
//win32 requires endian swap
	size_t sz = fread( buf, sizeof(cfxFloat), count, input) * sizeof(cfxFloat);
	for ( size_t i = 0; i < count; ++i ) 
	{
		endianSwap( buf+i );
	}
	return sz;
#else
	return fread( buf, sizeof(cfxFloat), count, input) * sizeof(cfxFloat);
#endif
}

size_t cfxBinaryUtil::readToken( short &token, FILE *input)
{
	fread( &token, CFX_BIN_TOKEN_SIZE, 1, input );
#ifdef WIN32
	endianSwap( &token );
#endif
	return CFX_BIN_TOKEN_SIZE;
}


cfxBool cfxBinaryUtil::loadFileHeader( FILE *input )
{
	if ( input == NULL ) 
	{
		return false;
	}
	//check type and version
	size_t byteCount = 0;
	int typenver;
	fread( &typenver, 4, 1, input );
#ifdef WIN32
	endianSwap( &typenver );
#endif
	if ( typenver != (COLLADA_FX_BIN_ID << 8 )+ COLLADA_FX_BIN_VER ) {
		printf( "ERROR: Wrong file type or COLLADA_FX Binary version!\n" );
		return false;
	}
	int offset;
	byteCount = 4 + readInt( &offset, 1, input );
	if ( (int)byteCount != offset )
	{
		//just have this here because you should be at the start of data now. if not I messed 
		//something up somewhere
		return false;
	}
	return true;
}

cfxAnnotate *cfxBinaryUtil::readAnnotate( FILE *input, size_t &size, cfxEffect *effect )
{
	std::string name;
	cfxData *data;
	size = readString(name, input);
	size += readTypedData( &data, input, effect );
	return new cfxAnnotate( data, name );
}

cfxParam *cfxBinaryUtil::readParam( short token, FILE *input, size_t &size, void *other )
{
	std::string name;
	std::string fromName;
	std::string semantic;
	cfxData *data;
	switch ( token )
	{
	case (short)BINDPARAM_TOKEN:
		size = readString( name, input );
		size += readTypedData( &data, input, ((cfxShader*)other)->getPass()->getTechnique()->getEffect() );
		return new cfxBindParam( name, (cfxShader*)other, data );
	case (short)CONNECTPARAM_TOKEN:
		size = readString( name, input );
		size += readString( fromName, input );
		return new cfxConnectParam( name, (cfxShader*)other, fromName );
	case (short)NEWPARAM_TOKEN:
		size = readString( name, input );
		size += readString( semantic, input );
        size += readTypedData( &data, input, (cfxEffect*)other );
		return new cfxNewParam( data, name, (cfxEffect*)other, semantic );
	case (short)SETPARAM_TOKEN:
		size = readString( name, input );
		size += readTypedData( &data, input, (cfxEffect*)other );
		return new cfxSetParam( data, name, (cfxEffect*)other );
	default:
		printf( "ERROR: Unknown param type or bad token!\n" );
		break;
	}
	size = 0;
	return NULL;
}

size_t cfxBinaryUtil::readTypedData( cfxData **data, FILE *input, cfxEffect *effect )
{
	short token;
	size_t size = readToken( token, input );
	if ( token == (short)CFXBOOL0 )
	{
		cfxBool b;
		size += readBool( &b, 1, input );
		*data = new cfxDataBool( b );
	}
	else if ( token == (short)CFXBOOL1 ) 
	{
		cfxBool1 b;
		size += readBool( (cfxBool*)&b, 1, input );
		*data = new cfxDataBool1( b );
	}
	else if ( token == (short)CFXBOOL2 ) 
	{
		cfxBool2 b;
		size += readBool( (cfxBool*)&b, 2, input );
		*data = new cfxDataBool2( b );
	}
	else if ( token == (short)CFXBOOL3 ) 
	{
		cfxBool3 b;
		size += readBool( (cfxBool*)&b, 3, input );
		*data = new cfxDataBool3( b );
	}
	else if ( token == (short)CFXBOOL4 ) 
	{
		cfxBool4 b;
		size += readBool( (cfxBool*)&b, 4, input );
		*data = new cfxDataBool4( b );
	}
	else if ( token == (short)CFXINT0 ) 
	{
		cfxInt i;
		size += readInt( &i, 1, input );
		*data = new cfxDataInt( i );
	}
	else if ( token == (short)CFXINT1 ) 
	{
		cfxInt1 i;
		size += readInt( (cfxInt*)&i, 1, input );
		*data = new cfxDataInt1( i );
	}
	else if ( token == (short)CFXINT2 ) 
	{
		cfxInt2 i;
		size += readInt( (cfxInt*)&i, 2, input );
		*data = new cfxDataInt2( i );
	}
	else if ( token == (short)CFXINT3 ) 
	{
		cfxInt3 i;
		size += readInt( (cfxInt*)&i, 3, input );
		*data = new cfxDataInt3( i );
	}
	else if ( token == (short)CFXINT4 ) 
	{
		cfxInt4 i;
		size += readInt( (cfxInt*)&i, 4, input );
		*data = new cfxDataInt4( i );
	}
	else if ( token == (short)CFXFLOAT0 ) 
	{
		cfxFloat f;
		size += readFloat( &f, 1, input );
		*data = new cfxDataFloat( f );
	}
	else if ( token == (short)CFXFLOAT1 ) 
	{
		cfxFloat1 f;
		size += readFloat( (cfxFloat*)&f, 1, input );
		*data = new cfxDataFloat1( f );
	}
	else if ( token == (short)CFXFLOAT2 ) 
	{
		cfxFloat2 f;
		size += readFloat( (cfxFloat*)&f, 2, input );
		*data = new cfxDataFloat2( f );
	}
	else if ( token == (short)CFXFLOAT3 ) 
	{
		cfxFloat3 f;
		size += readFloat( (cfxFloat*)&f, 3, input );
		*data = new cfxDataFloat3( f );
	}
	else if ( token == (short)CFXFLOAT4 ) 
	{
		cfxFloat4 f;
		size += readFloat( (cfxFloat*)&f, 4, input );
		*data = new cfxDataFloat4( f );
	}
	else if ( token == (short)CFXFLOAT2X2 ) 
	{
		cfxFloat2x2 f;
		size += readFloat( (cfxFloat*)&f, 4, input );
		*data = new cfxDataFloat2x2( f );
	}
	else if ( token == (short)CFXFLOAT3X3 ) 
	{
		cfxFloat3x3 f;
		size += readFloat( (cfxFloat*)&f, 9, input );
		*data = new cfxDataFloat3x3( f );
	}
	else if ( token == (short)CFXFLOAT4X4 ) 
	{
		cfxFloat4x4 f;
		size += readFloat( (cfxFloat*)&f, 16, input );
		*data = new cfxDataFloat4x4( f );
	}
	else if ( token == (short)CFXSAMPLER1D )
	{
		cfxSampler *sampler;
		std::string source;
		int count;
		size += readString( source, input );
		size += readInt( &count, 1, input );
		sampler = new cfxSampler( source, effect );
		size_t sz;
		for ( int x = 0; x < count; ++x )
		{
			sampler->pushSetting( readSamplerSetting( effect, input, sz ) );
			size += sz;
		}
		*data = new cfxDataSampler1D( sampler );
	}
	else if ( token == (short)CFXSAMPLER2D )
	{
		cfxSampler *sampler;
		std::string source;
		int count;
		size += readString( source, input );
		size += readInt( &count, 1, input );
		sampler = new cfxSampler( source, effect );
		size_t sz;
		for ( int x = 0; x < count; ++x )
		{
			sampler->pushSetting( readSamplerSetting( effect, input, sz ) );
			size += sz;
		}
		*data = new cfxDataSampler2D( sampler );
	}
	else if ( token == (short)CFXSAMPLER3D )
	{
		cfxSampler *sampler;
		std::string source;
		int count;
		size += readString( source, input );
		size += readInt( &count, 1, input );
		sampler = new cfxSampler( source, effect );
		size_t sz;
		for ( int x = 0; x < count; ++x )
		{
			sampler->pushSetting( readSamplerSetting( effect, input, sz ) );
			size += sz;
		}
		*data = new cfxDataSampler3D( sampler );
	}
	else if ( token == (short)CFXSAMPLERCUBE )
	{
		cfxSampler *sampler;
		std::string source;
		int count;
		size += readString( source, input );
		size += readInt( &count, 1, input );
		sampler = new cfxSampler( source, effect );
		size_t sz;
		for ( int x = 0; x < count; ++x )
		{
			sampler->pushSetting( readSamplerSetting( effect, input, sz ) );
			size += sz;
		}
		*data = new cfxDataSamplerCUBE( sampler );
	}
	else if ( token == (short)CFXSTRING )
	{
		std::string str;
		size += readString( str, input );
		*data = new cfxDataString( str );
	}
	else 
	{
		*data = NULL;
		printf( "ERROR: Unknown data type of invalid token!\n" );
	}
	return size;
}

cfxEffect *cfxBinaryUtil::readEffect( FILE *input, CGcontext _context )
{
	//read the name
	std::string name;
	readString( name, input );	
	//read size. Its used if you want to skip effects. for now it is unused.
	int size = 0;
	int currSize = 0;
	readInt( &size, 1, input );

	cfxEffect* effect = new cfxEffect(name, _context);
	if ( effect == NULL ) {
		printf( "ERROR: Failed to create effect %s!\n", name.c_str() );
		fseek( input, (long)size, SEEK_CUR ); //move to the end
		return NULL;
	}
	if ( size == 0 ) 
	{
		//this is an empty effect. It will probably break when running but return it now to load properly
		return effect;
	}
	short token;
	size_t sz;
	std::string surfName, surfInit, surfFormat;
	do {
		currSize += (int)readToken( token, input );
		switch ( token ) {
			case (short)ANNOTATE_TOKEN:
				effect->pushAnnotate( readAnnotate( input, sz, effect ) );
				currSize += (int)sz;
				break;
			case (short)NEWPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				effect->pushParam( readParam( token, input, sz, effect ) );
				currSize += (int)sz;
				break;
			case (short)SURFACE_TOKEN:
				currSize += (int)readString( surfName, input );
				currSize += (int)readString( surfInit, input );
				currSize +=	(int)readString( surfFormat, input );
				effect->addNamedSurface( surfName, new cfxSurface( surfInit, surfFormat ) );
				break;
			case (short)TECHNIQUE_TOKEN:
				effect->pushTechnique( readTechnique( effect, input, sz ) );
				currSize += (int)sz;
				break;
			default:
				printf( "ERROR: Unexpected Token reading effect %s!\n", name.c_str() );
				fseek( input, (long)size-currSize, SEEK_CUR ); //move to the end
				return NULL;
		}
	}while( currSize < size );

	return effect;
}

cfxTechnique *cfxBinaryUtil::readTechnique( cfxEffect *effect, FILE *input, size_t &size )
{
	std::string name;
	size = readString( name, input );
	cfxTechnique *teq = new cfxTechnique( effect, name );
    int m_size = 0; //the size of this technique block after the name
	int count = 0; //the amount read so far
	size += readInt( &m_size, 1, input );
	if ( teq == NULL ) {
		printf( "ERROR: Failed to create technique %s!\n", name.c_str() );
		fseek( input, (long)m_size, SEEK_CUR ); //move to the end
		size += m_size;
		return NULL;
	}
	if ( m_size == 0 )
	{
		//empty Technique probablt makes everything break. but return it here to continue loading
		return teq;
	}
	short token;
	size_t sz; //variable to pass in for return from functions
	std::string s1, s2; //temp strings used for include and surface constructors 
	do {
		count += (int)readToken( token, input );
		switch ( token ) {
			case (short)ANNOTATE_TOKEN:
				teq->pushAnnotate( readAnnotate( input, sz, effect ) );
				count += (int)sz;
				break;
			case (short)INCLUDE_TOKEN:
				count += (int)readString( s1, input );
				count += (int)readString( s2, input );
				s2.insert( 0, remotePath );
				teq->pushInclude( s1, s2 );
				break;
			case (short)SURFACE_TOKEN:
				count += (int)readString( s1, input );
				count += (int)readString( s2, input );
				teq->pushSurface( new cfxSurface( s1, s2 ) );
				break;
			case (short)NEWPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				teq->pushParam( readParam( token, input, sz, effect ) );
				count += (int)sz;
				break;
			case (short)SETPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				teq->pushParam( readParam( token, input, sz, effect ) );
				count += (int)sz;
				break;
			case (short)PASS_TOKEN:
				teq->pushPass( readPass( teq, input, sz ) );
				count += (int)sz;
				break;
			default:
				printf( "ERROR: Unexpected Token reading teqhnique %s!", name.c_str() );
				fseek( input, (long)m_size-count, SEEK_CUR ); //move to the end
				size += m_size;
				return NULL;
		}
	}while( count < m_size );
	size += count;
	return teq;
}

cfxPass *cfxBinaryUtil::readPass( cfxTechnique *teq, FILE *input, size_t &size )
{
	std::string name;
	size = readString( name, input );
	cfxPass *pass = new cfxPass( teq, name );
    int m_size = 0; //the size of this technique block after the name
	int count = 0; //the amount read so far
	size += readInt( &m_size, 1, input );
	if ( pass == NULL ) {
		printf( "ERROR: Failed to create pass %s!\n", name.c_str() );
		fseek( input, (long)m_size, SEEK_CUR ); //move to the end
		size += m_size;
		return NULL;
	}
	if ( m_size == 0 )
	{
		//empty Pass probablt makes everything break. but return it here to continue loading
		return pass;
	}
	short token;
	size_t sz; //variable to pass in for return from functions
	do {
		count += (int)readToken( token, input );
		switch ( token ) {
			case (short)ANNOTATE_TOKEN:
				pass->pushAnnotate( readAnnotate( input, sz, teq->getEffect() ) );
				count += (int)sz;
				break;
			/*case (short)NEWPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				teq->pushParam( readParam( token, input, sz, effect ) );
				count += (int)sz;
				break;
			case (short)SETPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				teq->pushParam( readParam( token, input, sz, effect ) );
				count += (int)sz;
				break;*/
			case (short)GL_SETTING_TOKEN:
				pass->pushSetting( readGlSetting( pass, input, sz ) );
				count += (int)sz;
				break;
			case (short)SHADER_TOKEN:
				pass->pushShader( readShader( pass, input, sz ) );
				count += (int)sz;
				break;
			default:
				printf( "ERROR: Unexpected Token reading pass %s!", name.c_str() );
				fseek( input, (long)m_size-count, SEEK_CUR ); //move to the end
				size += m_size;
				return NULL;
		}
	}while( count < m_size );
	size += count;
	return pass;
}

cfxShader *cfxBinaryUtil::readShader( cfxPass *pass, FILE *input, size_t &size )
{
	std::string name, source;
	cfxShader::type_enum type;
	size = readInt( (cfxInt*) (void*) &type, 1, input );
	size += readString( source, input );
	size += readString( name, input );
    cfxShader *s;
	if ( type == cfxShader::VERTEX ) 
	{
		s = new cfxShader( pass, source, name, cgGLGetLatestProfile(CG_GL_VERTEX), type );
	}
	else 
	{
		s = new cfxShader( pass, source, name, cgGLGetLatestProfile(CG_GL_FRAGMENT), type );
	}
    int m_size = 0; //the size of this technique block after the name
	int count = 0; //the amount read so far
	size += readInt( &m_size, 1, input );
	if ( s == NULL ) {
		printf( "ERROR: Failed to create shader %s - %s!\n", source.c_str(), name.c_str() );
		fseek( input, (long)m_size, SEEK_CUR ); //move to the end
		size += m_size;
		return NULL;
	}
	if ( m_size == 0 )
	{
		//empty shaders are ok since it can be simple and need not parameters bound
		return s;
	}
	short token;
	size_t sz; //variable to pass in for return from functions
	do {
		count += (int)readToken( token, input );
		switch ( token ) {
			case (short)ANNOTATE_TOKEN:
				s->pushAnnotate( readAnnotate( input, sz, pass->getTechnique()->getEffect() ) );
				count += (int)sz;
				break;
			case (short)BINDPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				s->pushParam( readParam( token, input, sz, s ) );
				count += (int)sz;
				break;
			case (short)CONNECTPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				s->pushParam( readParam( token, input, sz, s ) );
				count += (int)sz;
				break;
			default:
				printf( "ERROR: Unexpected Token reading shader %s - %s!", source.c_str(), name.c_str() );
				fseek( input, (long)m_size-count, SEEK_CUR ); //move to the end
				size += m_size;
				return NULL;
		}
	}while( count < m_size );
	size += count;
	return s;
}

cfxGlPipelineSetting *cfxBinaryUtil::readGlSetting( cfxPass *pass, FILE *input, size_t &size )
{
	cfxGlPipelineSetting::type_enum type;
	int i[4];
	float f[4];
	cfxFloat4x4 f4x4;
	cfxBool4 b;
	cfxSampler *sampler;
	std::string source;
	size_t sz;

	size = readInt( (cfxInt*)(void*)&type, 1, input );
	switch (type) {
	case cfxGlPipelineSetting::ALPHA_FUNC:
		size += readInt( i, 1, input );
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingAlphaFunc(pass, i[0], f[0] );

	case cfxGlPipelineSetting::BLEND_FUNC:
		size += readInt( i, 2, input );
		return new cfxGlPipelineSettingBlendFunc(pass, i[0], i[1] );

	case cfxGlPipelineSetting::BLEND_FUNC_SEPARATE:
		size += readInt( i, 4, input );
		return new cfxGlPipelineSettingBlendFuncSeparate(pass, i[0], i[1], i[2], i[3] );

	case cfxGlPipelineSetting::BLEND_EQUATION:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingBlendEquation(pass, i[0] );

	case cfxGlPipelineSetting::BLEND_EQUATION_SEPARATE:
		size += readInt( i, 2, input );
		return new cfxGlPipelineSettingBlendEquationSeparate(pass, i[0], i[1] );

	case cfxGlPipelineSetting::BLEND_COLOR:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingBlendColor(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::CLEAR_COLOR:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingClearColor(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::CLEAR_STENCIL:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingClearStencil(pass, i[0]);

	case cfxGlPipelineSetting::CLEAR_DEPTH:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingClearDepth(pass, f[0] );

	case cfxGlPipelineSetting::CLIP_PLANE:
		size += readInt( i, 1, input );
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingClipPlane(pass, i[0], *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::COLOR_MASK:
		size += readBool( (cfxBool*)&b, 4, input );
		return new cfxGlPipelineSettingColorMask(pass, b );

	case cfxGlPipelineSetting::COLOR_MATERIAL:
		size += readInt( i, 2, input );
		return new cfxGlPipelineSettingColorMaterial(pass, i[0], i[1] );

	case cfxGlPipelineSetting::CULL_FACE:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingCullFace(pass, i[0] );

	case cfxGlPipelineSetting::DEPTH_FUNC:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingDepthFunc(pass, i[0] );

	case cfxGlPipelineSetting::DEPTH_MASK:
		size += readBool( (cfxBool*)&b, 1, input );
		return new cfxGlPipelineSettingDepthMask(pass, b.b0 );

	case cfxGlPipelineSetting::DEPTH_RANGE:
		size += readFloat( f, 2, input );
		return new cfxGlPipelineSettingDepthRange(pass, *(cfxFloat2*)(void*)f );

	case cfxGlPipelineSetting::FOG_MODE:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingFogMode(pass, i[0] );

	case cfxGlPipelineSetting::FOG_DENSITY:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingFogDensity(pass, f[0] );

	case cfxGlPipelineSetting::FOG_START:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingFogStart(pass, f[0] );

	case cfxGlPipelineSetting::FOG_END:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingFogEnd(pass, f[0] );

	case cfxGlPipelineSetting::FOG_COLOR:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingFogColor(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::FRONT_FACE:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingFrontFace(pass, i[0] );

	case cfxGlPipelineSetting::LIGHT_MODEL_AMBEINT:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingLightModelAmbient(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::LIGHT_AMBIENT:
		size += readInt( i, 1, input );
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingLightAmbient(pass, i[0], *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::LIGHT_CONSTANT_ATTENUATION:
		size += readInt( i, 1, input );
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingLightConstantAttenuation(pass, i[0], f[0] );

	case cfxGlPipelineSetting::LIGHT_DIFFUSE:
		size += readInt( i, 1, input );
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingLightDiffuse(pass, i[0], *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::LIGHT_LINEAR_ATTENUATION:
		size += readInt( i, 1, input );
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingLightLinearAttenuation(pass, i[0], f[0] );

	case cfxGlPipelineSetting::LIGHT_POSITION:
		size += readInt( i, 1, input );
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingLightPosition(pass, i[0], *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::LIGHT_QUADRATIC_ATTENUATION:
		size += readInt( i, 1, input );
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingLightQuadraticAttenuation(pass, i[0], f[0] );

	case cfxGlPipelineSetting::LIGHT_SPECULAR:
		size += readInt( i, 1, input );
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingLightSpecular(pass, i[0], *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::LIGHT_SPOT_CUTOFF:
		size += readInt( i, 1, input );
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingLightSpotCutoff(pass, i[0], f[0] );

	case cfxGlPipelineSetting::LIGHT_SPOT_DIRECTION:
		size += readInt( i, 1, input );
		size += readFloat( f, 3, input );
		return new cfxGlPipelineSettingLightSpotDirection(pass, i[0], *(cfxFloat3*)(void*)f );

	case cfxGlPipelineSetting::LIGHT_SPOT_EXPONENT:
		size += readInt( i, 1, input );
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingLightSpotCutoff(pass, i[0], f[0] );

	case cfxGlPipelineSetting::LINE_WIDTH:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingLineWidth(pass, f[0] );

	case cfxGlPipelineSetting::LOGIC_OP:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingLogicOp(pass, i[0] );

	case cfxGlPipelineSetting::MATERIAL_AMBIENT:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingMaterialAmbient(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::MATERIAL_DIFFUSE:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingMaterialDiffuse(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::MATERIAL_EMISSION:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingMaterialEmission(pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::MATERIAL_SHININESS:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingMaterialShininess(pass, f[0] );

	case cfxGlPipelineSetting::MATERIAL_SPECULAR:
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingMaterialSpecular( pass, *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::MODEL_VIEW_MATRIX:
		size += readFloat( (cfxFloat*)&f4x4, 16, input );
		return new cfxGlPipelineSettingModelViewMatrix(pass, f4x4 );

	case cfxGlPipelineSetting::POINT_SIZE:
		size += readFloat( f, 1, input );
		return new cfxGlPipelineSettingPointSize(pass, f[0] );

	case cfxGlPipelineSetting::POINT_SPRITE_COORD_REPLACE:
		break;

	case cfxGlPipelineSetting::POINT_SPRITE_R_MODE:
		break;

	case cfxGlPipelineSetting::POLYGON_MODE:
		size += readInt( i, 2, input );
		return new cfxGlPipelineSettingPolygonMode(pass, i[0], i[1] );

	case cfxGlPipelineSetting::POLYGON_OFFSET:
		size += readFloat( f, 2, input );
		return new cfxGlPipelineSettingPolygonOffset(pass, *(cfxFloat2*)(void*)f );

	case cfxGlPipelineSetting::PROJECTION_MATRIX:
		size += readFloat( (cfxFloat*)&f4x4, 16, input );
		return new cfxGlPipelineSettingProjectionMatrix(pass, f4x4 );

	case cfxGlPipelineSetting::SCISSOR:
		size += readInt( i, 4, input );
		return new cfxGlPipelineSettingScissor(pass, *(cfxInt4*)(void*)i );

	case cfxGlPipelineSetting::SHADE_MODEL:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingShadeModel(pass, i[0] );

	case cfxGlPipelineSetting::STENCIL_FUNC:
		size += readInt( i, 3, input );
		return new cfxGlPipelineSettingStencilFunc(pass, i[0], i[1], i[2] );

	case cfxGlPipelineSetting::STENCIL_MASK:
		size += readInt( i, 1, input );
		return new cfxGlPipelineSettingStencilMask(pass, i[0] );

	case cfxGlPipelineSetting::STENCIL_OP:
		size += readInt( i, 3, input );
		return new cfxGlPipelineSettingStencilOp(pass, i[0], i[1], i[2] );

	case cfxGlPipelineSetting::STENCIL_FUNC_SEPARATE:
		size += readInt( i, 4, input );
		return new cfxGlPipelineSettingStencilFuncSeparate(pass, i[0], i[1], i[2], i[3] );

	case cfxGlPipelineSetting::STENCIL_MASK_SEPARATE:
		size += readInt( i, 2, input );
		return new cfxGlPipelineSettingStencilMaskSeparate(pass, i[0], i[1] );

	case cfxGlPipelineSetting::STENCIL_OP_SEPAREATE:
		size += readInt( i, 4, input );
		return new cfxGlPipelineSettingStencilOpSeparate(pass, i[0], i[1], i[2], i[3] );

	case cfxGlPipelineSetting::TEX_GEN_S_MODE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_T_MODE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_R_MODE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_Q_MODE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_S_EYE_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_T_EYE_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_R_EYE_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_Q_EYE_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_S_OBJECT_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_T_OBJECT_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_R_OBJECT_PLANE:
		break;

	case cfxGlPipelineSetting::TEX_GEN_Q_OBJECT_PLANE:
		break;

	case cfxGlPipelineSetting::TEXTURE_2D:
		size += readInt( i, 1, input );
		size += readString( source, input );
		size += readInt( (i+1), 1, input );
		sampler = new cfxSampler( source, pass->getTechnique()->getEffect() );
		for ( int x = 0; x < i[1]; ++x )
		{
			sampler->pushSetting( readSamplerSetting( pass->getTechnique()->getEffect(), input, sz ) );
			size += sz;
		}
		return new cfxGlPipelineSettingTexture2D( pass, i[0], sampler );

	case cfxGlPipelineSetting::TEXTURE_3D:
		size += readInt( i, 1, input );
		size += readString( source, input );
		size += readInt( (i+1), 1, input );
		sampler = new cfxSampler( source, pass->getTechnique()->getEffect() );
		for ( int x = 0; x < i[1]; ++x )
		{
			sampler->pushSetting( readSamplerSetting( pass->getTechnique()->getEffect(), input, sz ) );
			size += sz;
		}
		return new cfxGlPipelineSettingTexture3D( pass, i[0], sampler );

	case cfxGlPipelineSetting::TEXTURE_CUBE_MAP:
		size += readInt( i, 1, input );
		size += readString( source, input );
		size += readInt( (i+1), 1, input );
		sampler = new cfxSampler( source, pass->getTechnique()->getEffect() );
		for ( int x = 0; x < i[1]; ++x )
		{
			sampler->pushSetting( readSamplerSetting( pass->getTechnique()->getEffect(), input, sz ) );
			size += sz;
		}
		return new cfxGlPipelineSettingTextureCubeMap( pass, i[0], sampler );

	case cfxGlPipelineSetting::TEXTURE_ENV_COLOR:
		size += readInt( i, 1, input );
		size += readFloat( f, 4, input );
		return new cfxGlPipelineSettingTextureEnvColor(pass, i[0], *(cfxFloat4*)(void*)f );

	case cfxGlPipelineSetting::TEXTURE_ENV_MODE:
		size += readInt( i, 2, input );
		return new cfxGlPipelineSettingTextureEnvMode(pass, i[0], i[1] );

	case cfxGlPipelineSetting::ALPHA_TEST_ENABLE:
		size += readBool( (cfxBool*)&b, 1, input);
		return new cfxGlPipelineSettingAlphaTestEnable(pass, b.b0 );

	case cfxGlPipelineSetting::BLEND_ENABLE:
		size += readBool( (cfxBool*)&b, 1, input);
		return new cfxGlPipelineSettingBlendEnable(pass, b.b0 );

	case cfxGlPipelineSetting::CULL_FACE_ENABLE:
		size += readBool( (cfxBool*)&b, 1, input);
		return new cfxGlPipelineSettingCullFaceEnable(pass, b.b0 );

	case cfxGlPipelineSetting::DEPTH_TEST_ENABLE:
		size += readBool( (cfxBool*)&b, 1, input);
		return new cfxGlPipelineSettingDepthTestEnable(pass, b.b0 );

	default:
		printf( "ERROR: Unknown GL Pipeline Setting or invalid token!\n" );
		break;
	}
	return NULL;
}

cfxGlSamplerSetting *cfxBinaryUtil::readSamplerSetting( cfxEffect *effect, FILE *input, size_t &size )
{
	int i;
	float f;
	cfxBool b;
	cfxFloat4 f4;

	cfxGlSamplerSetting::type_enum type;
	size = readInt( (cfxInt*)(void*)&type, 1, input );
	switch ( type )
	{
	case cfxGlSamplerSetting::WRAP_S:
		size += readInt( &i, 1, input );
		return new cfxGlSamplerSettingWrapS( effect, "WrapS", i );
	case cfxGlSamplerSetting::WRAP_T:
		size += readInt( &i, 1, input );
		return new cfxGlSamplerSettingWrapT( effect, "WrapT", i );
	case cfxGlSamplerSetting::MINFILTER:
		size += readInt( &i, 1, input );
        return new cfxGlSamplerSettingMinFilter( effect, "MinFilter", i );		
	case cfxGlSamplerSetting::MAGFILTER:
		size += readInt( &i, 1, input );
        return new cfxGlSamplerSettingMagFilter( effect, "MagFilter", i );		
	case cfxGlSamplerSetting::BORDER_COLOR:
		size += readFloat( (cfxFloat*)&f4, 4, input );
        return new cfxGlSamplerSettingBorderColor( effect, "BorderColor", f4 );		
	case cfxGlSamplerSetting::GENERATE_MIPMAP:
		size += readBool( &b, 1, input );
        return new cfxGlSamplerSettingGenerateMipMap( effect, "GenerateMipMap", b );		
	case cfxGlSamplerSetting::MAX_MIP_LEVEL:
		size += readFloat( &f, 1, input );
        return new cfxGlSamplerSettingMaxMipLevel( effect, "MaxMipLevel", f );		
	case cfxGlSamplerSetting::LOD_BIAS:
		size += readFloat( &f, 1, input );
        return new cfxGlSamplerSettingLodBias( effect, "LodBias", f );		
	default:
		printf( "ERROR: Unknown sampler setting type or invalid token!\n" );
		break;
	}
	return NULL;
}

cfxMaterial *cfxBinaryUtil::readMaterial( FILE *input, CGcontext _context, const std::map<std::string, cfxEffect*> *effects )
{
	(void)_context;
	//read the name
	std::string name, effectName;
	readString( name, input );
	readString( effectName, input );
	//read size. Its used if you want to skip effects. for now it is unused.
	int size = 0;
	int currSize = 0;
	readInt( &size, 1, input );

	cfxEffect* effect = NULL;

	if (effects)
	{
		std::map<std::string, cfxEffect*>::const_iterator effectFinder = effects->find(effectName);
		if (effectFinder != effects->end())
		{
			effect = effectFinder->second;
		}
	}
	if ( effect == NULL )
	{
		printf( "Could not find effect %s associated with material %s\n", effectName.c_str(), name.c_str() ); 
	}

	cfxMaterial* material = new cfxMaterial( name, effectName, effect );
	if ( material == NULL ) {
		printf( "ERROR: Failed to create material %s!\n", name.c_str() );
		fseek( input, (long)size, SEEK_CUR ); //move to the end
		return NULL;
	}
	if ( size == 0 )
	{
		//empty material
		return material;
	}
	std::string surfName, surfInit, surfFormat;
	cfxSurface *surface = NULL, *newSurface = NULL;
	short token;
	size_t sz;
	do {
		currSize += (int)readToken( token, input );
		switch ( token ) {
			case (short)SETPARAM_TOKEN:
				//at this time you can only have newparams so pass in effect as other
				material->pushParam( readParam( token, input, sz, effect ) );
				currSize += (int)sz;
				break;
			case (short)SURFACE_TOKEN:
				currSize += (int)readString( surfName, input );
				currSize += (int)readString( surfInit, input );
				currSize +=	(int)readString( surfFormat, input );
				surface = effect->getSurfaceByName(surfName);
				newSurface = new cfxSurface( surfInit, surfFormat, surface );
				material->addSurface(newSurface);
				break;
			default:
				printf( "ERROR: Unexpected Token reading material %s!", name.c_str() );
				fseek( input, (long)size-currSize, SEEK_CUR ); //move to the end
				return NULL;
		}
	}while( currSize < size );

	return material;
}




cfxBool cfxBinaryUtil::save(const std::string& file, const std::map<std::string, cfxMaterial*>& materials, 
					      const std::map<std::string, cfxEffect*>& effects, cfxBool replace ) 
{
	FILE *out = NULL;
	if(!replace)
	{
		FILE *tempfd = fopen(file.c_str(),"r");
		if(tempfd != NULL)
		{
			// File exists, return error
			fclose(tempfd);
			printf( "File %s already exists and replace is false.\n", file.c_str() );
			return false;
		}
	}
	out = fopen( file.c_str(), "wb" );
	if ( out == NULL ) 
	{
		printf( "Failed to open %s for writing.\n", file.c_str() );
		return false;
	}
	// It'd be better if this code just did a getcwd call instead of relying on the DOM for this
	DAE dae;
	fileName = file;
	daeURI tmp( dae, fileName.c_str() );
	tmp.validate();
	fileName = tmp.getURI();
	fileName = fileName.substr( 7 ); //remove the file://
	writeFileHeader( out );
	std::map<std::string, cfxEffect *>::const_iterator effectIter = effects.begin();
	while ( effectIter != effects.end() )
	{
		writeEffect( effectIter->second, out );
		effectIter++;
	}

	std::map<std::string, cfxMaterial *>::const_iterator matIter = materials.begin();
	while ( matIter != materials.end() )
	{
		writeMaterial( matIter->second, out );
		matIter++;
	}

	fclose( out );
	return true;

}

size_t cfxBinaryUtil::writeString( const std::string &str, FILE *out )
{
	fputs( str.c_str(), out );
	fputc( '\0', out );
	return str.length() +1;
}

size_t cfxBinaryUtil::writeBool( const cfxBool *buf, size_t count, FILE *out )
{
	return fwrite( buf, sizeof(cfxBool), count, out ) * sizeof(cfxBool);
}

size_t cfxBinaryUtil::writeInt( const cfxInt *buf, size_t count, FILE *out )
{
#ifdef WIN32
	//win32 requires endian swap
	cfxInt *tempBuf = new cfxInt[count]; //create a temp buffer because of const parameter
	memcpy( tempBuf, buf, count * sizeof( cfxInt ) );
	for ( size_t i = 0; i < count; ++i ) 
	{
		endianSwap( tempBuf+i );
	}
	size_t sz = fwrite( tempBuf, sizeof(cfxInt), count, out ) * sizeof(cfxInt);
	delete[] tempBuf;
	return sz;
#else
	return fwrite( buf, sizeof(cfxInt), count, out ) * sizeof(cfxInt);
#endif	
}

size_t cfxBinaryUtil::writeFloat( const cfxFloat *buf, size_t count, FILE *out )
{
#ifdef WIN32
	//win32 requires endian swap
	cfxFloat *tempBuf = new cfxFloat[count]; //create a temp buffer because of const parameter
	memcpy( tempBuf, buf, count * sizeof( cfxFloat ) );
	for ( size_t i = 0; i < count; ++i ) 
	{
		endianSwap( tempBuf+i );
	}
	size_t sz = fwrite( tempBuf, sizeof(cfxFloat), count, out ) * sizeof(cfxFloat);
	delete[] tempBuf;
	return sz;
#else
	return fwrite( buf, sizeof(cfxFloat), count, out ) * sizeof(cfxFloat);
#endif
}

size_t cfxBinaryUtil::writeToken( short token, FILE *out )
{
#ifdef WIN32
	//win32 requires endian swap
	endianSwap( &token );
#endif
	fwrite( &token, CFX_BIN_TOKEN_SIZE, 1, out );
	return CFX_BIN_TOKEN_SIZE;
}

void cfxBinaryUtil::writeFileHeader( FILE *out )
{
	size_t byteCount = 4 + sizeof( cfxInt ); // add the space needed for the offset itself
	int idnver = (COLLADA_FX_BIN_ID << 8 )+ COLLADA_FX_BIN_VER;
#ifdef WIN32
	endianSwap( &idnver );
#endif
    fwrite(  &idnver, 4, 1, out ); //write out the id and version
	writeInt( (cfxInt*)(&byteCount), 1, out ); //write the offset

}

void cfxBinaryUtil::writeEffect( const cfxEffect *effect, FILE *out )
{
	size_t size = 0;
	writeToken( (short)EFFECT_TOKEN, out );
	writeString( effect->getName(), out );
	fpos_t offsetPos; 
	fgetpos(out, &offsetPos); //Save the position so we can come back and write the size in later
	writeInt( (cfxInt*)(&size), 1, out ); //Write dummy int as temporary size
	for ( unsigned int i = 0; i < effect->getAnnotateArray().size(); ++i ) {
		size += writeAnnotate( effect->getAnnotateArray()[i], out );
	}
	for ( unsigned int i = 0; i < effect->getParamArray().size(); ++i ) {
		size += writeParam( effect->getParamArray()[i], out );
	}
	std::map<std::string, cfxSurface*>::const_iterator iter = effect->getSurfaceMap().begin();
	while ( iter != effect->getSurfaceMap().end() ) {
		const cfxSurface *s = iter->second; 
		size += writeToken( (short)SURFACE_TOKEN, out );
		size += writeString( iter->first, out );
		size += writeString( s->getInitFrom(), out );
		size += writeString( s->getFormat(), out );
		++iter;
	}
	for ( unsigned int i = 0; i < effect->getTechniqueArray().size(); ++i ) {
		size += writeTechnique( effect->getTechniqueArray()[i], out );
	}

	fsetpos( out, &offsetPos ); //move back to the offset position
	writeInt( (cfxInt*)(&size), 1, out ); //Write out the real size
	fseek( out, (long)size, SEEK_CUR ); //move to the end to write more data
}

size_t cfxBinaryUtil::writeAnnotate( const cfxAnnotate *an, FILE *out )
{
	size_t sz = writeToken( (short)ANNOTATE_TOKEN, out);
	sz += writeString( an->getName(), out );
    sz += writeTypedData( an->getData(), out );
	return sz;
}

size_t cfxBinaryUtil::writeParam( const cfxParam *p, FILE *out )
{
	cfxParam::cfxParamTypeEnum type = p->getType();
	const cfxBindParam *bp;
	const cfxConnectParam *cp;
	const cfxNewParam *np;
	const cfxSetParam *sp;
	size_t sz = 0;
	switch ( type ) 
	{
	case cfxParam::CFXBINDPARAM:
		bp = (cfxBindParam*)p;
		sz += writeToken( (short)BINDPARAM_TOKEN, out );
		sz += writeString( bp->getName(), out );
		sz += writeTypedData( bp->getData(), out );
		return sz;

	case cfxParam::CFXCONNECTPARAM:
		cp = (cfxConnectParam*)p;
		sz += writeToken( (short)CONNECTPARAM_TOKEN, out );
		sz += writeString( cp->getName(), out );
		sz += writeString( cp->getFromName(), out );
		return sz;

	case cfxParam::CFXNEWPARAM:
		np = (cfxNewParam*)p;
		sz += writeToken( (short)NEWPARAM_TOKEN, out );
		sz += writeString( np->getName(), out );
		sz += writeString( np->getSemantic(), out );
        sz += writeTypedData( np->getData(), out );
		return sz;

	case cfxParam::CFXSETPARAM:
		sp = (cfxSetParam*)p;
		sz += writeToken( (short)SETPARAM_TOKEN, out );
		sz += writeString( sp->getName(), out );
		sz += writeTypedData( sp->getData(), out );
		return sz;

	default:
		printf( "Unknown param type!\n");
		break;
	}
	return 0;
}

size_t cfxBinaryUtil::writeTypedData( const cfxData *data, FILE *out )
{
	const cfxSampler *sampler;
	size_t count;
	size_t size = 0;

	CGtype t = data->getType();
	switch ( t ) 
	{
	case CG_BOOL:
		size += writeToken( (short)CFXBOOL0, out );
		size += writeBool( &(((cfxDataBool*)data)->getData()), 1, out );
		return size;
		
	case CG_BOOL1:
		size += writeToken( (short)CFXBOOL1, out );
		size += writeBool( (cfxBool*)&(((cfxDataBool1*)data)->getData()), 1, out );
		return size;
		
	case CG_BOOL2:
		size += writeToken( (short)CFXBOOL2, out );
		size += writeBool( (cfxBool*)&(((cfxDataBool2*)data)->getData()), 2, out );
		return size;
		
	case CG_BOOL3:
		size += writeToken( (short)CFXBOOL3, out );
		size += writeBool( (cfxBool*)&(((cfxDataBool3*)data)->getData()), 3, out );
		return size;
		
	case CG_BOOL4:
		size += writeToken( (short)CFXBOOL4, out );
		size += writeBool( (cfxBool*)&(((cfxDataBool4*)data)->getData()), 4, out );
		return size;
		
	case CG_INT:
		size += writeToken( (short)CFXINT0, out );
		size += writeInt( (cfxInt*)&(((cfxDataInt*)data)->getData()), 1, out );
		return size;
		
	case CG_INT1:
		size += writeToken( (short)CFXINT1, out );
		size += writeInt( (cfxInt*)&(((cfxDataInt1*)data)->getData()), 1, out );
		return size;
		
	case CG_INT2:
		size += writeToken( (short)CFXINT2, out );
		size += writeInt( (cfxInt*)&(((cfxDataInt2*)data)->getData()), 2, out );
		return size;
		
	case CG_INT3:
		size += writeToken( (short)CFXINT3, out );
		size += writeInt( (cfxInt*)&(((cfxDataInt3*)data)->getData()), 3, out );
		return size;
		
	case CG_INT4:
		size += writeToken( (short)CFXINT4, out );
		size += writeInt( (cfxInt*)&(((cfxDataInt4*)data)->getData()), 4, out );
		return size;
		
	case CG_FLOAT:
		size += writeToken( (short)CFXFLOAT0, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat*)data)->getData()), 1, out );
		return size;
		
	case CG_FLOAT1:
		size += writeToken( (short)CFXFLOAT1, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat1*)data)->getData()), 1, out );
		return size;
		
	case CG_FLOAT2:
		size += writeToken( (short)CFXFLOAT2, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat2*)data)->getData()), 2, out );
		return size;
		
	case CG_FLOAT3:
		size += writeToken( (short)CFXFLOAT3, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat3*)data)->getData()), 3, out );
		return size;
		
	case CG_FLOAT4:
		size += writeToken( (short)CFXFLOAT4, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat4*)data)->getData()), 4, out );
		return size;
		
	case CG_FLOAT2x2:
		size += writeToken( (short)CFXFLOAT2X2, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat2x2*)data)->getData()), 4, out );
		return size;
		
	case CG_FLOAT3x3:
		size += writeToken( (short)CFXFLOAT3X3, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat3x3*)data)->getData()), 9, out );
		return size;
			
	case CG_FLOAT4x4:
		size += writeToken( (short)CFXFLOAT4X4, out );
		size += writeFloat( (cfxFloat*)&(((cfxDataFloat4x4*)data)->getData()), 16, out );
		return size;
		
	case CG_SAMPLER1D:
		sampler = ((cfxDataSampler1D*)data)->getData();
		size = writeToken( (short)CFXSAMPLER1D, out );
		size += writeString( sampler->getSource(), out );
		count = sampler->getSettingArray().size();
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
		return size;

	case CG_SAMPLER2D:
		sampler = ((cfxDataSampler2D*)data)->getData();
		size = writeToken( (short)CFXSAMPLER2D, out );
		size += writeString( sampler->getSource(), out );
		count = sampler->getSettingArray().size();
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
		return size;

	case CG_SAMPLER3D:
		sampler = ((cfxDataSampler3D*)data)->getData();
		size = writeToken( (short)CFXSAMPLER3D, out );
		size += writeString( sampler->getSource(), out );
		count = sampler->getSettingArray().size();
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
		return size;

	case CG_SAMPLERCUBE:
		sampler = ((cfxDataSamplerCUBE*)data)->getData();
		size = writeToken( (short)CFXSAMPLERCUBE, out );
		size += writeString( sampler->getSource(), out );
		count = sampler->getSettingArray().size();
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
		return size;

	case CG_STRING:
		size += writeToken( (short)CFXSTRING, out );
		size += writeString( ((cfxDataString*)data)->getData(), out );
		return size;
		
	default:
		printf( "Unknown data type!\n");
		break;
	}

	return 0;
}
	
size_t cfxBinaryUtil::writeTechnique( const cfxTechnique *teq, FILE *out )
{
	size_t size = writeToken( (short)TECHNIQUE_TOKEN, out );
	size += writeString( teq->getName(), out );
	fpos_t offsetPos; 
	fgetpos(out, &offsetPos);
	size_t byteCount = 0; //used to store the size of the technique body
	size += writeInt( (cfxInt*)&byteCount, 1, out );
	for ( unsigned int i = 0; i < teq->getAnnotateArray().size(); ++i ) {
		byteCount += writeAnnotate( teq->getAnnotateArray()[i], out );
	}
	const std::map<std::string, std::string> &inc = teq->getIncludeMap();
	std::map<std::string, std::string>::const_iterator iter = inc.begin();
	while ( iter != inc.end() ) {
		byteCount += writeToken( (short)INCLUDE_TOKEN, out );
		byteCount += writeString( iter->first, out );
		byteCount += writeString( makeRelativePath(iter->second, fileName ), out );
		++iter;
	}
	for ( unsigned int i = 0; i < teq->getSurfaceArray().size(); ++i ) {
		const cfxSurface *s = teq->getSurfaceArray()[i]; 
		byteCount += writeToken( (short)SURFACE_TOKEN, out );
		byteCount += writeString( s->getInitFrom(), out );
		byteCount += writeString( s->getFormat(), out );
	}
	for ( unsigned int i = 0; i < teq->getParamArray().size(); ++i ) {
		byteCount += writeParam( teq->getParamArray()[i], out );
	}
	for ( unsigned int i = 0; i < teq->getPassArray().size(); ++i ) {
		byteCount += writePass( teq->getPassArray()[i], out );
	}
	fsetpos( out, &offsetPos ); //move back to the offset position
	writeInt( (cfxInt*)(&byteCount), 1, out ); //Write out the real size
	fseek( out, (long)byteCount, SEEK_CUR ); //move to the end to write more data
	size += byteCount;
	return size;
}

size_t cfxBinaryUtil::writePass( const cfxPass *pass, FILE *out )
{
	size_t size = writeToken( (short)PASS_TOKEN, out );
	size += writeString( pass->getName(), out );
	fpos_t offsetPos; 
	fgetpos(out, &offsetPos);
	size_t byteCount = 0; //used to store the size of the pass body
	size += writeInt( (cfxInt*)&byteCount, 1, out );
	for ( unsigned int i = 0; i < pass->getAnnotateArray().size(); ++i ) {
		byteCount += writeAnnotate( pass->getAnnotateArray()[i], out );
	}
	for ( unsigned int i = 0; i < pass->getParamArray().size(); ++i ) {
		byteCount += writeParam( pass->getParamArray()[i], out );
	}
	for ( unsigned int i = 0; i < pass->getSettingArray().size(); ++i ) {
		byteCount += writeGLSetting( pass->getSettingArray()[i], out );		
	}
	for ( unsigned int i = 0; i < pass->getShaderArray().size(); ++i ) {
		byteCount += writeShader( pass->getShaderArray()[i], out );
	}
	fsetpos( out, &offsetPos ); //move back to the offset position
	writeInt( (cfxInt*)(&byteCount), 1, out ); //Write out the real size
	fseek( out, (long)byteCount, SEEK_CUR ); //move to the end to write more data
	size += byteCount;
	return size;
}

size_t cfxBinaryUtil::writeShader( const cfxShader *shader, FILE *out )
{
	size_t size = writeToken( (short)SHADER_TOKEN, out );
	cfxShader::type_enum type = shader->getOriginalType();
	size += writeInt( (cfxInt*)(void*)&type, 1, out );
	size += writeString( shader->getSource(), out );
	size += writeString( shader->getName(), out );
	fpos_t offsetPos; 
	fgetpos(out, &offsetPos);
	size_t byteCount = 0; //used to store the size of the pass body
	size += writeInt( (cfxInt*)&byteCount, 1, out );
	for ( unsigned int i = 0; i < shader->getAnnotateArray().size(); ++i ) {
		byteCount += writeAnnotate( shader->getAnnotateArray()[i], out );
	}
	for ( unsigned int i = 0; i < shader->getParamArray().size(); ++i ) {
		byteCount += writeParam( shader->getParamArray()[i], out );
	}
	fsetpos( out, &offsetPos ); //move back to the offset position
	writeInt( (cfxInt*)(&byteCount), 1, out ); //Write out the real size
	fseek( out, (long)byteCount, SEEK_CUR ); //move to the end to write more data
	size += byteCount;
	return size;
}

size_t cfxBinaryUtil::writeGLSetting( const cfxGlPipelineSetting *s, FILE *out )
{
	cfxGlPipelineSetting::type_enum type = s->getType();
	size_t size = writeToken( (short)GL_SETTING_TOKEN, out );
	size += writeInt( (cfxInt*)(void*)&type, 1, out );
	
	if ( type == cfxGlPipelineSetting::ALPHA_FUNC ) 
	{
		cfxGlPipelineSettingAlphaFunc *gls = (cfxGlPipelineSettingAlphaFunc*)s;
		int i = gls->getFunc();
		float f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::BLEND_FUNC ) 
	{
		cfxGlPipelineSettingBlendFunc *gls = (cfxGlPipelineSettingBlendFunc*)s;
		int i[2] = { gls->getSrc(), gls->getDst() };
		size += writeInt( i, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::BLEND_FUNC_SEPARATE ) 
	{
		cfxGlPipelineSettingBlendFuncSeparate *gls = (cfxGlPipelineSettingBlendFuncSeparate*)s;
		int i[4] = { gls->getSrcRGB(), gls->getSrcRGB(), gls->getSrcAlpha(), gls->getDstAlpha() };
		size += writeInt( i, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::BLEND_EQUATION ) 
	{
		cfxGlPipelineSettingBlendEquation *gls = (cfxGlPipelineSettingBlendEquation*)s;
		int i = gls->getEquation();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::BLEND_EQUATION_SEPARATE ) 
	{
		cfxGlPipelineSettingBlendEquationSeparate *gls = (cfxGlPipelineSettingBlendEquationSeparate*)s;
		int i[2] = { gls->getRgb(), gls->getAlpha() };
		size += writeInt( i, 2, out );
	}	
	else if ( type == cfxGlPipelineSetting::BLEND_COLOR ) 
	{	
		cfxGlPipelineSettingBlendColor *gls = (cfxGlPipelineSettingBlendColor*)s;
		cfxFloat4 f = gls->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::CLEAR_COLOR ) 
	{	
		cfxGlPipelineSettingClearColor *gls = (cfxGlPipelineSettingClearColor*)s;
		cfxFloat4 f = gls->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::CLEAR_STENCIL ) 
	{	
		cfxGlPipelineSettingClearStencil *gls = (cfxGlPipelineSettingClearStencil*)s;
		int i = gls->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::CLEAR_DEPTH ) 
	{	
		cfxGlPipelineSettingClearDepth *gls = (cfxGlPipelineSettingClearDepth*)s;
		float f = gls->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::CLIP_PLANE ) 
	{	
		cfxGlPipelineSettingClipPlane *gls = (cfxGlPipelineSettingClipPlane*)s;
		int i = gls->getIndex();
		cfxFloat4 f = gls->getValue();
		size += writeInt( &i, 1, out) ;
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::COLOR_MASK )
	{	
		cfxGlPipelineSettingColorMask *gls = (cfxGlPipelineSettingColorMask*)s;
		cfxBool4 b = gls->getValue();
		size += writeBool( (cfxBool*)&b, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::COLOR_MATERIAL ) 
	{	
		cfxGlPipelineSettingColorMaterial *gls = (cfxGlPipelineSettingColorMaterial*)s;
		int i[2] = { gls->getFace(), gls->getMode() };
		size += writeInt( i, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::CULL_FACE ) 
	{	
		int i = ((cfxGlPipelineSettingCullFace*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::DEPTH_FUNC )
	{	
		int i = ((cfxGlPipelineSettingDepthFunc*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::DEPTH_MASK ) 
	{	
		cfxBool b = ((cfxGlPipelineSettingDepthMask*)s)->getValue();
		size += writeBool( &b, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::DEPTH_RANGE )
	{	
		cfxFloat2 f = ((cfxGlPipelineSettingDepthRange*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::FOG_MODE ) 
	{	
		int i = ((cfxGlPipelineSettingFogMode*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::FOG_DENSITY ) 
	{	
		float f = ((cfxGlPipelineSettingFogDensity*)s)->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::FOG_START )
	{	
		float f = ((cfxGlPipelineSettingFogStart*)s)->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::FOG_END ) 
	{	
		float f = ((cfxGlPipelineSettingFogEnd*)s)->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::FOG_COLOR ) 
	{	
		cfxFloat4 f = ((cfxGlPipelineSettingFogColor*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::FRONT_FACE ) 
	{	
		int i = ((cfxGlPipelineSettingFrontFace*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_MODEL_AMBEINT ) 
	{	
		cfxFloat4 f = ((cfxGlPipelineSettingLightModelAmbient*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_AMBIENT )
	{	
		cfxGlPipelineSettingLightAmbient *gls = (cfxGlPipelineSettingLightAmbient*)s;
		int i = gls->getIndex();
		cfxFloat4 f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_CONSTANT_ATTENUATION ) 
	{	
		cfxGlPipelineSettingLightConstantAttenuation *gls = (cfxGlPipelineSettingLightConstantAttenuation*)s;
		int i = gls->getIndex();
		float f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_DIFFUSE ) 
	{	
		cfxGlPipelineSettingLightDiffuse *gls = (cfxGlPipelineSettingLightDiffuse*)s;
		int i = gls->getIndex();
		cfxFloat4 f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_LINEAR_ATTENUATION ) 
	{	
		cfxGlPipelineSettingLightLinearAttenuation *gls = (cfxGlPipelineSettingLightLinearAttenuation*)s;
		int i = gls->getIndex();
		float f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_POSITION ) 
	{	
		cfxGlPipelineSettingLightPosition *gls = (cfxGlPipelineSettingLightPosition*)s;
		int i = gls->getIndex();
		cfxFloat4 f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_QUADRATIC_ATTENUATION ) 
	{	
		cfxGlPipelineSettingLightQuadraticAttenuation *gls = (cfxGlPipelineSettingLightQuadraticAttenuation*)s;
		int i = gls->getIndex();
		float f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_SPECULAR ) 
	{	
		cfxGlPipelineSettingLightSpecular *gls = (cfxGlPipelineSettingLightSpecular*)s;
		int i = gls->getIndex();
		cfxFloat4 f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_SPOT_CUTOFF ) 
	{	
		cfxGlPipelineSettingLightSpotCutoff *gls = (cfxGlPipelineSettingLightSpotCutoff*)s;
		int i = gls->getIndex();
		float f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_SPOT_DIRECTION )
	{	
		cfxGlPipelineSettingLightSpotDirection *gls = (cfxGlPipelineSettingLightSpotDirection*)s;
		int i = gls->getIndex();
		cfxFloat3 f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( (cfxFloat*)&f, 3, out );
	}
	else if ( type == cfxGlPipelineSetting::LIGHT_SPOT_EXPONENT ) 
	{	
		cfxGlPipelineSettingLightSpotExponent *gls = (cfxGlPipelineSettingLightSpotExponent*)s;
		int i = gls->getIndex();
		float f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LINE_WIDTH ) 
	{	
		float f = ((cfxGlPipelineSettingLineWidth*)s)->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::LOGIC_OP ) 
	{	
		int i = ((cfxGlPipelineSettingLogicOp*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::MATERIAL_AMBIENT ) 
	{	
		cfxFloat4 f = ((cfxGlPipelineSettingMaterialAmbient*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::MATERIAL_DIFFUSE ) 
	{	
		cfxFloat4 f = ((cfxGlPipelineSettingMaterialDiffuse*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::MATERIAL_EMISSION ) 
	{	
		cfxFloat4 f = ((cfxGlPipelineSettingMaterialEmission*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::MATERIAL_SHININESS ) 
	{	
		float f = ((cfxGlPipelineSettingMaterialShininess*)s)->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::MATERIAL_SPECULAR ) 
	{	
		cfxFloat4 f = ((cfxGlPipelineSettingMaterialSpecular*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::MODEL_VIEW_MATRIX ) 
	{	
		cfxFloat4x4 f = ((cfxGlPipelineSettingModelViewMatrix*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 16, out );
	}
	else if ( type == cfxGlPipelineSetting::POINT_SIZE ) 
	{	
		float f = ((cfxGlPipelineSettingPointSize*)s)->getValue();
		size += writeFloat( &f, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::POINT_SPRITE_COORD_REPLACE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::POINT_SPRITE_R_MODE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::POLYGON_MODE ) 
	{	
		cfxGlPipelineSettingPolygonMode *gls = (cfxGlPipelineSettingPolygonMode*)s;
		int i[2] = { gls->getFace(), gls->getMode() };
		size += writeInt( i, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::POLYGON_OFFSET ) 
	{	
		cfxFloat2 f = ((cfxGlPipelineSettingPolygonOffset*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::PROJECTION_MATRIX ) 
	{	
		cfxFloat4x4 f = ((cfxGlPipelineSettingProjectionMatrix*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f, 16, out );
	}
	else if ( type == cfxGlPipelineSetting::SCISSOR ) 
	{	
		cfxInt4 i = ((cfxGlPipelineSettingScissor*)s)->getValue();
		size += writeInt( (cfxInt*)&i, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::SHADE_MODEL ) 
	{	
		int i = ((cfxGlPipelineSettingShadeModel*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::STENCIL_FUNC ) 
	{	
		cfxGlPipelineSettingStencilFunc *gls = (cfxGlPipelineSettingStencilFunc*)s;
		int i[3] = { gls->getFunc(), gls->getRef(), gls->getMask() };
		size += writeInt( i, 3, out );
	}
	else if ( type == cfxGlPipelineSetting::STENCIL_MASK ) 
	{	
		int i = ((cfxGlPipelineSettingStencilMask*)s)->getValue();
		size += writeInt( &i, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::STENCIL_OP ) 
	{	
		cfxGlPipelineSettingStencilOp *gls = (cfxGlPipelineSettingStencilOp*)s;
		int i[3] = { gls->getFail(), gls->getZFail(), gls->getZPass() };
		size += writeInt( i, 3, out );
	}
	else if ( type == cfxGlPipelineSetting::STENCIL_FUNC_SEPARATE ) 
	{	
		cfxGlPipelineSettingStencilFuncSeparate *gls = (cfxGlPipelineSettingStencilFuncSeparate*)s;
		int i[4] = { gls->getFront(), gls->getBack(), gls->getRef(), gls->getMask() };
		size += writeInt( i, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::STENCIL_MASK_SEPARATE ) 
	{	
		cfxGlPipelineSettingStencilMaskSeparate *gls = (cfxGlPipelineSettingStencilMaskSeparate*)s;
		int i[2] = { gls->getFace(), gls->getMask() };
		size += writeInt( i, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::STENCIL_OP_SEPAREATE ) 
	{	
		cfxGlPipelineSettingStencilOpSeparate *gls = (cfxGlPipelineSettingStencilOpSeparate*)s;
		int i[4] = { gls->getFace(), gls->getFail(), gls->getZFail(), gls->getZPass() };
		size += writeInt( i, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_S_MODE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_T_MODE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_R_MODE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_Q_MODE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_S_EYE_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_T_EYE_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_R_EYE_PLANE )
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_Q_EYE_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_S_OBJECT_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_T_OBJECT_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_R_OBJECT_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEX_GEN_Q_OBJECT_PLANE ) 
	{	
	}
	else if ( type == cfxGlPipelineSetting::TEXTURE_2D ) 
	{	
		cfxGlPipelineSettingTexture2D *gls = (cfxGlPipelineSettingTexture2D*)s;
		int i = gls->getIndex();
		const cfxSampler *sampler = gls->getSampler();
		size_t count = sampler->getSettingArray().size();
		size += writeInt( &i, 1, out );
		size += writeString( sampler->getSource(), out );
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
	}
	else if ( type == cfxGlPipelineSetting::TEXTURE_3D ) 
	{	
		cfxGlPipelineSettingTexture3D *gls = (cfxGlPipelineSettingTexture3D*)s;
		int i = gls->getIndex();
		const cfxSampler *sampler = gls->getSampler();
		size_t count = sampler->getSettingArray().size();
		size += writeInt( &i, 1, out );
		size += writeString( sampler->getSource(), out );
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
	}
	else if ( type == cfxGlPipelineSetting::TEXTURE_CUBE_MAP ) 
	{	
		cfxGlPipelineSettingTextureCubeMap *gls = (cfxGlPipelineSettingTextureCubeMap*)s;
		int i = gls->getIndex();
		const cfxSampler *sampler = gls->getSampler();
		size_t count = sampler->getSettingArray().size();
		size += writeInt( &i, 1, out );
		size += writeString( sampler->getSource(), out );
		size += writeInt( (cfxInt*)&count, 1, out );
		for ( size_t x = 0; x < count; ++x ) {
			size += writeSamplerSetting( sampler->getSettingArray()[x], out );
		}
	}
	else if ( type == cfxGlPipelineSetting::TEXTURE_ENV_COLOR )
	{	
		cfxGlPipelineSettingTextureEnvColor *gls = (cfxGlPipelineSettingTextureEnvColor*)s;
		int i = gls->getIndex();
		cfxFloat4 f = gls->getValue();
		size += writeInt( &i, 1, out );
		size += writeFloat( (cfxFloat*)&f, 4, out );
	}
	else if ( type == cfxGlPipelineSetting::TEXTURE_ENV_MODE ) 
	{	
		cfxGlPipelineSettingTextureEnvMode *gls = (cfxGlPipelineSettingTextureEnvMode*)s;
		int i[2] = { gls->getIndex(), gls->getValue() };
		size += writeInt( i, 2, out );
	}
	else if ( type == cfxGlPipelineSetting::ALPHA_TEST_ENABLE ) 
	{	
		cfxBool b = ((cfxGlPipelineSettingAlphaTestEnable*)s)->getValue();
		size += writeBool( &b, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::BLEND_ENABLE ) 
	{	
		cfxBool b = ((cfxGlPipelineSettingBlendEnable*)s)->getValue();
		size += writeBool( &b, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::CULL_FACE_ENABLE ) 
	{	
		cfxBool b = ((cfxGlPipelineSettingCullFaceEnable*)s)->getValue();
		size += writeBool( &b, 1, out );
	}
	else if ( type == cfxGlPipelineSetting::DEPTH_TEST_ENABLE )
	{	
		cfxBool b = ((cfxGlPipelineSettingDepthTestEnable*)s)->getValue();
		size += writeBool( &b, 1, out );
	}
	return size;
}

size_t cfxBinaryUtil::writeSamplerSetting( const cfxGlSamplerSetting *s, FILE *out )
{
	cfxGlSamplerSetting::type_enum type = s->getType();
	size_t size = writeInt( (cfxInt*)(void*)&type, 1, out );
	int i;
	float f;
	cfxFloat4 f4;
	cfxBool b;

	switch ( type )
	{
	case cfxGlSamplerSetting::WRAP_S:
		i = ((cfxGlSamplerSettingWrapS*)s)->getValue();
		size += writeInt( &i, 1, out );
		break;
	case cfxGlSamplerSetting::WRAP_T:
		i = ((cfxGlSamplerSettingWrapT*)s)->getValue();
		size += writeInt( &i, 1, out );
		break;
	case cfxGlSamplerSetting::MINFILTER:
		i = ((cfxGlSamplerSettingMinFilter*)s)->getValue();
		size += writeInt( &i, 1, out );
		break;
	case cfxGlSamplerSetting::MAGFILTER:
		i = ((cfxGlSamplerSettingMagFilter*)s)->getValue();
		size += writeInt( &i, 1, out );
		break;
	case cfxGlSamplerSetting::BORDER_COLOR:
		f4 = ((cfxGlSamplerSettingBorderColor*)s)->getValue();
		size += writeFloat( (cfxFloat*)&f4, 4, out );
		break;
	case cfxGlSamplerSetting::GENERATE_MIPMAP:
		b = ((cfxGlSamplerSettingGenerateMipMap*)s)->getValue();
		size += writeBool( &b, 1, out );
		break;
	case cfxGlSamplerSetting::MAX_MIP_LEVEL:
		f = ((cfxGlSamplerSettingMaxMipLevel*)s)->getValue();
		size += writeFloat( &f, 1, out );
		break;
	case cfxGlSamplerSetting::LOD_BIAS:
		f = ((cfxGlSamplerSettingLodBias*)s)->getValue();
		size += writeFloat( &f, 1, out ); 
		break;
	default: 
		printf( "Unknown sampler setting type or bad token!\n" );
		break;
	}
	return size;
}

void cfxBinaryUtil::writeMaterial( const cfxMaterial *material, FILE *out )
{
	size_t size = 0;
	writeToken( (short)MATERIAL_TOKEN, out );
	writeString( material->getName(), out );
	writeString( material->getEffectName(), out );
	fpos_t offsetPos; 
	fgetpos(out, &offsetPos); //Save the position so we can come back and write the size in later
	writeInt( (cfxInt*)(&size), 1, out ); //Write dummy int as temporary size
	for ( unsigned int i = 0; i < material->getParamArray().size(); ++i ) 
	{
		size += writeParam( material->getParamArray()[i], out );
	}
	for ( unsigned int i = 0; i < material->getSurfaces().size(); ++i )
	{
		cfxSurface *s = material->getSurfaces()[i];
		std::string name = material->getEffect()->getSurfaceName( s->getParentSurface() );
		size += writeToken( (short)SURFACE_TOKEN, out );
		size += writeString( name, out );
		size +=	writeString( s->getInitFrom(), out );
		size += writeString( s->getFormat(), out );
	}

	fsetpos( out, &offsetPos ); //move back to the offset position
	writeInt( (cfxInt*)(&size), 1, out ); //Write out the real size
	fseek( out, (long)size, SEEK_CUR ); //move to the end to write more data
}

std::string cfxBinaryUtil::makeRelativePath( const std::string &path, const std::string &relTo )
{
	const char *this_filepath		= path.c_str();
	const char *relativeTo_filepath = relTo.c_str();
	const char *this_slash			= this_filepath;
	const char *relativeTo_slash	= relativeTo_filepath;

	while(*this_filepath == *relativeTo_filepath)
	{
		if(*this_filepath == '/')
		{
			this_slash = this_filepath;
			relativeTo_slash = relativeTo_filepath;
		}
		this_filepath++;
		relativeTo_filepath++;
	}

	// Decide how many ../ segments are needed (Filepath should always end in a /)
	int segment_count = 0;
	relativeTo_slash++;
	while(*relativeTo_slash != 0)
	{
		if(*relativeTo_slash == '/')
			segment_count ++;
		relativeTo_slash++;
	}
	this_slash++;
	
	std::string newPath;
	for(int i = 0; i < segment_count; i++)
	{
		newPath.append( "../" );
	}
	//add rest of original path
	if ( cfxLoader::getPlatformString() == "PC-OGL" ) {
		newPath.append( this_filepath );
		return newPath;
	}
	return this_filepath;
}

