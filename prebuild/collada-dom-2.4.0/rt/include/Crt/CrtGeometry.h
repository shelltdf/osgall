/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef _CRT_GEOMETRY_H_
#define _CRT_GEOMETRY_H_  

#include "Crt/CrtSkin.h"
#include <string>
#include <vector>

class CrtMaterial;
#include "cfxLoader.h"
#include "cfxEffect.h"
#include "cfxSurface.h"
#include "cfxPlatform.h"

class CrtGeometry;
class CrtInstanceMaterial;

/** The CrtInstanceMaterial class holds the material bindings found inside <bind_material>s 
 *  <technique_common> in <instance_material>
 */
class CrtInstanceMaterial
{
public:
	CrtInstanceMaterial() 
	{
		targetMaterial=0;
		targetcfxMaterial=0;
		symbol="";
		target=""; 
	};
	~CrtInstanceMaterial() {};
	CrtNode		*Parent;			/**< Node where this instance was instantiated */
	std::string	symbol;
	std::string	target;
	CrtMaterial * targetMaterial;
	cfxMaterial * targetcfxMaterial;
	// Need a list of "bind" for later
};

/** The CrtInstanceGeometry class holds the information needed to make an instance of a piece of geometry
 *  This maps to the <instance_geometry> tag in COLLADA
 *  NOTE: All the instances share the same original geometry, copies aren't made so changing the parent
 *  will change all the instances.
 */
class CrtInstance;

class CrtInstanceGeometry : public CrtInstance
{
public:
	CrtInstanceGeometry() : AbstractGeometry(0) {};
	~CrtInstanceGeometry() {};
	CrtGeometry	*AbstractGeometry;	/**< The abstract geometry where the original data is stored */
	// Need a list of "param" for later
};

class CrtPoly 
{
public:
	CrtInt32			* Points;
//	CrtVertexAllPtr  * PointData;
	
	CrtInt32		 NumIndices; 
	CrtInt32		 NumPoints;

	CrtPoly():
		Points(NULL),
//		PointData(NULL),
		NumIndices(0),
		NumPoints(0)
	{
	}

	~CrtPoly()
	{
		CrtDelete( Points );
//		CrtDelete( PointData );  
	}
};

class CrtTri
{
public:
	CrtInt32	 Points[3];
	
	CrtTri()
	{
		memset( Points, 0, sizeof ( Points )); 
	}
};

enum CrtInputType 
{
	eUNKNOWN, 
	ePOINT_SOURCE = 1,
	eNORMAL_SOURCE,
	eTANGENT_SOURCE,
	eTEXCOORD0_SOURCE,
	eTEXCOORD1_SOURCE,
	eTEXCOORD2_SOURCE,
	eTEXCOORD3_SOURCE,
	eTEXCOORD4_SOURCE,
	eTEXCOORD5_SOURCE,
	eTEXCOORD6_SOURCE,
	eTEXCOORD7_SOURCE,
	eTEXCOORD8_SOURCE,
	
};

class CrtSource : public CrtBase
{
public:
	CrtFloat	*data;
	CrtInt32	nbr; 
	CrtChar		semantic[CRT_MAX_NAME_SIZE]; 
	CrtChar		vertexSourceId[CRT_MAX_NAME_SIZE];
	CrtBool		hasVertexBinding; 

	CrtSource():
		data(NULL),
		nbr(0),
		hasVertexBinding(CrtFalse)
	{
		memset( semantic,		0, sizeof ( semantic )); 
		memset( vertexSourceId, 0, sizeof ( vertexSourceId )); 
	}

	~CrtSource()
	{
		DeleteData(); 
	}

	CrtVoid DeleteData()
	{
		CrtDeleteData( data ); 			
	}
};

class CrtInput 
{
public:
	CrtChar			semantic[CRT_MAX_NAME_SIZE];
	CrtChar			sourceName[CRT_MAX_NAME_SIZE]; 
	CrtSource		*source; 
	CrtInputType	type; 

	CrtInput():
		source(NULL),
		type(eUNKNOWN)

	{
		memset( semantic,	0, sizeof ( semantic )); 
		memset( sourceName, 0, sizeof ( sourceName )); 
	}
};

struct CrtVertexArray
{
	int								primitiveType;
	int								count;
	int								vertexFormat;
	CrtVoid*							i;
	CrtVoid*							p;
};

struct CrtMultiArray
{
	CrtVertexArray*		pVertexArray;
	CrtInt32				nbVertexArray;
};


enum CrtGeoDataType 
{
	eGeoPoints,
	eGeoNormals,
	eGeoBiNormals,
	eGeoTangents,
	eGeoSkinBindPoints,
	eGeoSkinBindNormals,
	eGeoSkinIndices,
	eGeoSkinWeights,
    eGeoIndices,
	eGeoTexCoord0,
	eGeoTexCoord1,
	eGeoTexCoord2,
	eGeoTexCoord3,
	eGeoTexCoord4,
	eGeoTexCoord5,
	eGeoTexCoord6,
	eGeoTexCoord7,
	eGeoDataTypeMax
};


CrtChar * CrtCpy( CrtChar * s1, const CrtChar * s2 );
class CrtSkin; 
class CrtJoint; 
class CrtPolyGroup;


class CrtPolyGroup : public  CrtBase
{

public:
	cfxMaterial		*FXMaterial;	/**< COLLADA FX material for this polygon group */
	CrtMaterial *	Material;		// !!!GAC OBSOLETE, will be removed by refactor later
	CrtChar			MaterialName[CRT_MAX_NAME_SIZE];  // !!!GAC could be obsolete or kept for debugging
	CrtBool			HasSkin;  
	CrtGeometry		*	geometry;	
	CrtPoly			*	Polys; 
	CrtTri			*	Tris; 

	CrtMultiArray	MultiArray; 
	CrtVertexArray	BoundingBoxArray;
	CrtVec3f		minBound;
	CrtVec3f		maxBound;

	CrtUInt32		count;
	CrtInt32		NumTextures;
	CrtInt32		NumSharedVerts; 

	CrtBool			VertsShared; 
	CrtBool			DataReadyForCg; 
	CrtVec4f	*	SkinIndicesCg;
	CrtVec4f	*	SkinWeightsCg; 

	CrtUInt     *   indexes;

public:

	CrtPolyGroup()
	{
		Material = NULL;
		FXMaterial = NULL;
		NumTextures	=	0;
		Polys = NULL;
		Tris = NULL; 
		MultiArray.pVertexArray = 0;
		NumSharedVerts = 0; 
		HasSkin = CrtFalse; 
		VertsShared = CrtFalse; 
		DataReadyForCg = CrtFalse; 
		SkinIndicesCg = NULL;
		SkinWeightsCg = NULL; 

		indexes = 0;
	}
	~CrtPolyGroup()
	{
		Destroy(); 
		DeleteVertexData(); 
		DeletePolygonData(); 

	}

protected:
	friend class CrtScene; 
	friend class CrtGeometry; 

	CrtVoid	DeleteVertexData()
	{
		if (indexes)
		{
			CrtDeleteData( indexes ); 
			indexes = NULL;			
		}
	}

	CrtVoid	DeleteData()
	{
	}

	CrtVoid	DeletePolygonData()
	{
		CrtDeleteData( Polys ); 
		CrtDeleteData( Tris ); 		
	}

	CrtVoid	DeleteAll()
	{
		DeleteVertexData();
		DeleteData(); 	
		DeletePolygonData(); 		
	}

	CrtVoid	SetMaterial( CrtMaterial * mat, CrtChar * matName )
	{
		Material = mat; 
		CrtCpy( MaterialName, matName ); 
	}

	CrtInput * GetPointInput();  
	CrtVoid	CopyPointInput( CrtPolyGroup * group );

	CrtVoid	ParseSource( const CrtChar * data, const CrtChar * name, const CrtChar * count );
    CrtInputType	SetType( CrtChar * s );
	CrtVoid	Draw(CrtNode *parentNode, CrtInstance * instance);
	CrtVoid	SetHasSkin(){ HasSkin = CrtTrue; }
	CrtVoid	RenderSkinnedCg( CrtVec3f * bindPos, CrtVec3f * bindNorms, 
				CrtJoint * joints, CrtInt nbrBindPoints, CrtInt nbrJoints );
	cfxMaterial*	SetupMaterialForDraw(CrtNode *parentNode, CrtInstance * instance);

	CrtVoid	RenderCg(); 
	CrtVoid	SetDataPointers(); 
	CrtVoid	RenderDebug();
public:
	//---------------------- External Interfaces ---------------------------------//
	// Each Geometry represents a Collada Geometry Library.  Each lib can have 
	// may groups.  I these groups you will get the Material and from the Material
	// you can get the texture(s) if there are any.  
	// You will also get the Point,Normal,Tanget, TexCoords[8], UVs and Weights.
	// Currently the Weight data is loaded, but I have added it to the triangulation 
	// so this will return null for now.  But you can build your interface from the 
	// stucts that I have provide already.  

	inline	CrtMaterial *GetMaterial()		{ return Material; }
	inline	CrtTri		*GetTris()			{ return Tris; }
	CrtVoid		SetRender();
	virtual CrtVoid		Render() = 0;
	virtual CrtVoid		SetVBOs() = 0;

	//---------------------- End External Interfaces -----------------------------//

};

class CrtPolygons : public CrtPolyGroup
{
public:
	CrtPolygons ();
	~CrtPolygons ();
	void SetVBOs();
	void Render();
	std::vector<CrtUInt> countvector;	
	std::vector<CrtUInt *> indexvector;
private:
	CrtUInt * VBOID;
};

class CrtTriangles : public CrtPolyGroup
{
public:
	CrtTriangles ();
	~CrtTriangles ();
	void SetVBOs();
	void Render();
private:
	CrtUInt VBOID;
};
class CrtLines : public CrtPolyGroup
{
public:
	CrtLines ();
	~CrtLines ();
	void SetVBOs();
	void Render();
private:
	CrtUInt VBOID;
};
class CrtLinestrips : public CrtPolyGroup
{
public:
	CrtLinestrips ();
	~CrtLinestrips ();
	std::vector<CrtUInt> countvector;	
	std::vector<CrtUInt *> indexvector;
	void SetVBOs();
	void Render();
private:
	CrtUInt * VBOID;
};
class CrtTriStrips : public CrtPolyGroup
{
public:
	CrtTriStrips ();
	~CrtTriStrips ();
	std::vector<CrtUInt> countvector;	
	std::vector<CrtUInt *> indexvector;
	void SetVBOs();
	void Render();
private:
	CrtUInt * VBOID;
};
class CrtTriFans : public CrtPolyGroup
{
public:
	CrtTriFans ();
	~CrtTriFans ();
	std::vector<CrtUInt> countvector;	
	std::vector<CrtUInt *> indexvector;
	void SetVBOs();
	void Render();
private:
	CrtUInt * VBOID;
};

//class CrtWeight;
class CrtGeometry : public CrtBase 
{
 
//	CrtSkin			*	SkinData; 

	CrtBool			VerticesSet; 
	CrtBool			VertexSourceCopied; 

public:
	CrtVec3f		*	Points;
	CrtVec3f		*	Normals;
//	CrtVec3f		*	Tangents;
//	CrtColor4f		*	Colors; 
	CrtVec2f		*	TexCoords[CRT_MAX_TEX_COORDS];
//	CrtWeight		*	Weights; 
	CrtUInt			vertexcount;
	CrtUInt			vertexcountmax;
	CrtUInt			*	SkinIndex;				// we need this because we deindex the vertices. it store the original index.
												// we need the original index to access the right skin index
	CrtUInt			VBOIDs[eGeoDataTypeMax];

	std::vector<CrtPolyGroup *> Groups;

	CrtGeometry()
	{
		VerticesSet			= CrtFalse; 
		VertexSourceCopied  = CrtFalse; 

		vertexcount = 0;
		vertexcountmax = 1 * 1024;

		Points = CrtNewData(CrtVec3f, vertexcountmax); 
		Normals = CrtNewData( CrtVec3f, vertexcountmax);
		TexCoords[0] = CrtNewData( CrtVec2f, vertexcountmax);
		SkinIndex =  CrtNewData( CrtUInt, vertexcountmax);

		memset( VBOIDs, 0, sizeof( VBOIDs )); 
	}

	~CrtGeometry();

protected:
	friend class CrtScene; 
	friend class CrtNode; 
	friend class CrtSkin; 
	friend class CrtMorph; 

	CrtVoid	ParseSource( const CrtChar * data, const CrtChar * name, const CrtChar * count );
	CrtVoid	DeleteSources();
	CrtVoid	AddVertexBinding( const CrtChar * curName, const CrtChar * newName, const CrtChar * sematic ); 
	inline	CrtVoid	SetVerticesReady() { VerticesSet = CrtTrue; } 

	/**
	 * Draw function iterates all the polygroups in the geometry and draws them
	 */
	CrtVoid	Draw(CrtNode *parentNode, CrtInstance * instance)
	{
		SetRender();
		for (CrtUInt i = 0; i < Groups.size(); i++ )
		{
			Groups[i]->Draw(parentNode, instance); 
		}			
	};
	void 	SetRender();
	
	CrtVoid	DrawSkinned(CrtNode *parentNode, CrtInstance * instance);

public:

	//---------------------- External Interfaces ---------------------------------//
	// Again the Geo can contain many polyGroups in collada.  From the  you get the material
	// and geometry data.  To cycle through the groups it's just Groups = (CrtPolyGroup *)Groups->GetNext(); 

};

#endif // _CRT_GEOMETRY_H_


