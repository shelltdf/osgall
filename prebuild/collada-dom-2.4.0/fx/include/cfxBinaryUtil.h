/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_BINARYUTIL_H
#define _CFX_BINARYUTIL_H

#include <vector>
#include <string>
#include <map>

#include <cfxTypes.h>

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

class cfxEffect;
class cfxAnnotate;
class cfxParam;
class cfxTechnique;
class cfxData;
class cfxPass;
class cfxShader;
class cfxGlPipelineSetting;
class cfxGlSamplerSetting;
class cfxMaterial;

class cfxBinaryUtil {

public:
	static cfxBool load(const std::string& file, std::map<std::string, cfxMaterial*>& materials, 
					      std::map<std::string, cfxEffect*>& effects, CGcontext _context = NULL );
	static cfxBool save(const std::string& file, const std::map<std::string, cfxMaterial*>& materials, 
					      const std::map<std::string, cfxEffect*>& effects, cfxBool replace = false );

	static void setRemotePath( const std::string &path );

private: // IO utility functions
	// reads character by character into str until a null character is reached.
	static size_t readString( std::string &str, FILE *input );
	// reads bools and does endian-ness conversion if needed
	static size_t readBool( cfxBool *buf, size_t count, FILE *input );
	// reads ints and does endian-ness conversion if needed
	static size_t readInt( cfxInt *buf, size_t count, FILE *input );
	// reads floats and does endian-ness conversion if needed
	static size_t readFloat( cfxFloat *buf, size_t count, FILE *input );
	static size_t readToken( short &token, FILE *input);

	static size_t writeString( const std::string &str, FILE *out );
	// writes ints and does endian-ness conversion if needed
	static size_t writeBool( const cfxBool *buf, size_t count, FILE *out );
	// writes ints and does endian-ness conversion if needed
	static size_t writeInt( const cfxInt *buf, size_t count, FILE *out );
	// writes floats and does endian-ness conversion if needed
	static size_t writeFloat( const cfxFloat *buf, size_t count, FILE *out );
	static size_t writeToken( short token, FILE *out );

	static void endianSwap( short *s );
	static void endianSwap( cfxInt *i );
	static void endianSwap( cfxFloat *f );

	static std::string makeRelativePath( const std::string &path, const std::string &relTo );

private: //read functionality
	static cfxBool loadFileHeader( FILE *input );
	static cfxEffect *readEffect( FILE *input, CGcontext _context );
	static cfxAnnotate *readAnnotate( FILE *input, size_t &size, cfxEffect *effect );
	static cfxParam *readParam( short token, FILE *input, size_t &size, void *other = NULL );
	//data should be NULL pointer
	static size_t readTypedData( cfxData **data, FILE *input, cfxEffect *effect ); 
	static cfxTechnique *readTechnique( cfxEffect *effect, FILE *input, size_t &size );
	static cfxPass *readPass( cfxTechnique *teq, FILE *input, size_t &size );
	static cfxShader *readShader( cfxPass *pass, FILE *input, size_t &size );
	static cfxGlPipelineSetting *readGlSetting( cfxPass *pass, FILE *input, size_t &size );
	static cfxGlSamplerSetting *readSamplerSetting( cfxEffect *effect, FILE *input, size_t &size );

	static cfxMaterial *readMaterial( FILE *input, CGcontext _context, const std::map<std::string, cfxEffect*> *effects = NULL );

private: //write functionality
	static void writeFileHeader( FILE *out );
	static void writeEffect( const cfxEffect *effect, FILE *out );
	static size_t writeAnnotate( const cfxAnnotate *an, FILE *out );
	static size_t writeParam( const cfxParam *p, FILE *out );
	static size_t writeTypedData( const cfxData *data, FILE *out );
	static size_t writeTechnique( const cfxTechnique *teq, FILE *out );
	static size_t writePass( const cfxPass *pass, FILE *out );
	static size_t writeShader( const cfxShader *shader, FILE *out );
	static size_t writeGLSetting( const cfxGlPipelineSetting *s, FILE *out );
	static size_t writeSamplerSetting( const cfxGlSamplerSetting *s, FILE *out );

	static void writeMaterial( const cfxMaterial *material, FILE *out );

private: //variables
	static std::string fileName; //Used upon save for creating a relative path for the includes.
	static std::string remotePath; //The path to prepend for the shaders paths
};

#endif
