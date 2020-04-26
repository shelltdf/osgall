# ******************************************************************************
# * Project:  CMake4GDAL
# * Purpose:  CMake build scripts
# * Author: Dmitriy Baryshnikov (aka Bishop), polimax@mail.ru
# ******************************************************************************
# * Copyright (C) 2012,2013 Bishop
# * 
# * Permission is hereby granted, free of charge, to any person obtaining a
# * copy of this software and associated documentation files (the "Software"),
# * to deal in the Software without restriction, including without limitation
# * the rights to use, copy, modify, merge, publish, distribute, sublicense,
# * and/or sell copies of the Software, and to permit persons to whom the
# * Software is furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included
# * in all copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# * DEALINGS IN THE SOFTWARE.
# ******************************************************************************

cmake_minimum_required (VERSION 2.8.10)

set(LIB_NAME "ogrsf_frmts")
project ("lib${LIB_NAME}")

include_directories(${GDAL_ROOT_SOURCE_DIR}/gcore)
include_directories(${GDAL_ROOT_SOURCE_DIR}/ogr/ogrsf_frmts)  

set(LIB_HHEADERS
	ogr_attrind.h
	ogrsf_frmts.h
    
    generic/ogreditablelayer.h
    generic/ogremulatedtransaction.h
    generic/ogrlayerdecorator.h
    generic/ogrlayerpool.h
    generic/ogrmutexeddatasource.h
    generic/ogrmutexedlayer.h
    generic/ogrunionlayer.h
    generic/ogrwarpedlayer.h
    generic/ogr_gensql.h
)

set(LIB_CSOURCES
    generic/ogrdatasource.cpp
    generic/ogreditablelayer.cpp
    generic/ogremulatedtransaction.cpp
    generic/ogrlayer.cpp
    generic/ogrlayerdecorator.cpp
    generic/ogrlayerpool.cpp
    generic/ogrmutexeddatasource.cpp
    generic/ogrmutexedlayer.cpp
    generic/ogrregisterall.cpp
    generic/ogrsfdriver.cpp
    generic/ogrsfdriverregistrar.cpp
    generic/ogrunionlayer.cpp
    generic/ogrwarpedlayer.cpp
    generic/ogr_attrind.cpp
    generic/ogr_gensql.cpp
    generic/ogr_miattrind.cpp
)
#need to compile
#add_subdirectory(generic)

# add_subdirectory(aeronavfaa)
# add_subdirectory(amigocloud)
# add_subdirectory(arcgen)
# add_subdirectory(arcobjects)
# add_subdirectory(avc)
# add_subdirectory(bna)
# add_subdirectory(cad)
# add_subdirectory(carto)
# add_subdirectory(cloudant)
# add_subdirectory(couchdb)
# add_subdirectory(csw)
# add_subdirectory(db2)
# add_subdirectory(dgn)
# add_subdirectory(dods)
# add_subdirectory(dwg)
# add_subdirectory(edigeo)
# add_subdirectory(elastic)
# add_subdirectory(filegdb)
# add_subdirectory(fme)
# add_subdirectory(geoconcept)
# add_subdirectory(geomedia)
# add_subdirectory(georss)
# add_subdirectory(gft)
# add_subdirectory(gmlas)
# add_subdirectory(gmt)
# add_subdirectory(gpkg)
# add_subdirectory(gpsbabel)
# add_subdirectory(grass)
# add_subdirectory(gtm)
# add_subdirectory(htf)
# add_subdirectory(idb)
# add_subdirectory(idrisi)
# add_subdirectory(ili)
# add_subdirectory(ingres)
# add_subdirectory(jml)
# add_subdirectory(mdb)
# add_subdirectory(mongodb)
# add_subdirectory(mongodbv3)
# add_subdirectory(mssqlspatial)
# add_subdirectory(mvt)
# add_subdirectory(nas)
# add_subdirectory(ngw)
# add_subdirectory(ntf)
# add_subdirectory(oci)
# add_subdirectory(odbc)
# add_subdirectory(ods)
# add_subdirectory(ogdi)
# add_subdirectory(openair)
# add_subdirectory(openfilegdb)
# add_subdirectory(osm)
# add_subdirectory(pds)
# add_subdirectory(pgdump)
# add_subdirectory(pgeo)
# add_subdirectory(plscenes)
# add_subdirectory(rec)
# add_subdirectory(sde)
# add_subdirectory(sdts)
# add_subdirectory(segukooa)
# add_subdirectory(segy)
# add_subdirectory(selafin)
# add_subdirectory(sosi)
# add_subdirectory(sua)
# add_subdirectory(svg)
# add_subdirectory(tiger)
# add_subdirectory(vdv)
# add_subdirectory(vfk)
# add_subdirectory(walk)
# add_subdirectory(wasp)
# add_subdirectory(xls)
# add_subdirectory(xlsx)
# add_subdirectory(xplane)


add_definitions(-DTAB_ENABLED)
add_subdirectory(mitab)

option(OGR_ENABLE_CSV "Set ON to build csv driver" OFF)
if(OGR_ENABLE_CSV)
	add_definitions(-DCSV_ENABLED)
	add_subdirectory(csv)
endif()

option(OGR_ENABLE_DFX "Set ON to build dxf driver" ON)
if(OGR_ENABLE_DFX)
	add_definitions(-DDXF_ENABLED)
	add_subdirectory(dxf)
endif()

# NOTE For now it can not be optional
set(OGR_ENABLE_GEOJSON ON)
if(OGR_ENABLE_GEOJSON)
	add_definitions(-DGEOJSON_ENABLED)
	add_subdirectory(geojson)
endif()

if(GDAL_ENABLE_OGR)
	add_definitions(-DGML_ENABLED)
	add_subdirectory(gml)
	add_definitions(-DKML_ENABLED)
	add_subdirectory(kml)
else()
  option(OGR_ENABLE_GML "Set ON to build gml driver" OFF)
  if(OGR_ENABLE_GML)
		add_definitions(-DGML_ENABLED)
		add_subdirectory(gml)
	endif()

  option(OGR_ENABLE_KML "Set ON to build kml driver" OFF)
  if(OGR_ENABLE_KML)
		add_definitions(-DKML_ENABLED)
		add_subdirectory(kml)
	endif()
endif()

option(OGR_ENABLE_MEM "Set ON to build mem driver" OFF)
if(OGR_ENABLE_MEM)
	add_definitions(-DMEM_ENABLED)
	add_subdirectory(mem)
endif()

option(OGR_ENABLE_VRT "Set ON to build vrt driver" OFF)
if(OGR_ENABLE_VRT)
	add_definitions(-DVRT_ENABLED)
	add_subdirectory(vrt)
endif()

option(OGR_ENABLE_GPX "Set ON to build gpx driver" OFF)
if(OGR_ENABLE_GPX)
	add_definitions(-DGPX_ENABLED)
	add_subdirectory(gpx)
endif()

option(OGR_ENABLE_SHP "Set ON to build ESRI shape-file driver" OFF)
if(OGR_ENABLE_SHP)
	add_definitions(-DSHAPE_ENABLED)
	add_subdirectory(shape)
endif()

option(OGR_ENABLE_WFS "Set ON to build WFS driver" OFF)
if(OGR_ENABLE_WFS)
	add_definitions(-DWFS_ENABLED)
	add_subdirectory(wfs)
endif()

option(OGR_ENABLE_PG "Set ON to build PostGIS driver" OFF)
if(OGR_ENABLE_PG)
	add_definitions(-DPG_ENABLED)
	add_subdirectory(pg)
endif()

option(OGR_ENABLE_SQLITE "Set ON to build SpatialLite driver" OFF)
if(OGR_ENABLE_SQLITE)
	add_definitions(-DSQLITE_ENABLED)
	add_subdirectory(sqlite)
endif()

option(OGR_ENABLE_MYSQL "Set ON to build MySQL driver" OFF)
if(OGR_ENABLE_MYSQL)
	add_definitions(-DMYSQL_ENABLED)
	add_subdirectory(mysql)
endif()

option(OGR_ENABLE_LIBKML "Set ON to build libkml driver" OFF)
if(OGR_ENABLE_LIBKML)
	add_definitions(-DLIBKML_ENABLED)
	add_subdirectory(libkml)
endif()

option(OGR_ENABLE_SXF "Set ON to build sxf driver" OFF)
if(OGR_ENABLE_SXF)
    add_definitions(-DSXF_ENABLED)
    add_subdirectory(sxf)
endif()

option(OGR_ENABLE_S57 "Set ON to build s57 driver" OFF)
if(OGR_ENABLE_S57)
    add_definitions(-DS57_ENABLED)
    add_subdirectory(s57)
endif()

set(GDAL_TARGET_OBJECTS ${GDAL_TARGET_OBJECTS} $<TARGET_OBJECTS:${LIB_NAME}> PARENT_SCOPE)
set(GDAL_TARGET_LINK_LIB ${GDAL_TARGET_LINK_LIB} PARENT_SCOPE)
add_library(${LIB_NAME} OBJECT ${LIB_HHEADERS} ${LIB_CSOURCES})

install(FILES ${LIB_HHEADERS} DESTINATION include/gdal)
