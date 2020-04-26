
#ifndef USE_OSG_STATIC_LIBRARY_H
#define USE_OSG_STATIC_LIBRARY_H

#include <osgDB/Registry>
#include <osgViewer/GraphicsWindow>


#if OSG_LIBRARY_STATIC

#if 1
//image
USE_OSGPLUGIN(png)
USE_OSGPLUGIN(rgb)
USE_OSGPLUGIN(jpeg)
USE_OSGPLUGIN(tiff)
USE_OSGPLUGIN(dds)
USE_OSGPLUGIN(tga)
USE_OSGPLUGIN(bmp)

//model and scene
USE_OSGPLUGIN(osg)
USE_OSGPLUGIN(osg2)
USE_OSGPLUGIN(ive)
USE_OSGPLUGIN(obj)

//other
USE_OSGPLUGIN(freetype)
USE_OSGPLUGIN(glsl)
//USE_OSGPLUGIN(nvtt) //这个插件只能压缩 所有不能直接用于双向操作
USE_OSGPLUGIN(curl)


//
USE_OSGPLUGIN(3dc)
USE_OSGPLUGIN(ac)
USE_OSGPLUGIN(bsp)
USE_OSGPLUGIN(bvh)
USE_OSGPLUGIN(cfg)
USE_OSGPLUGIN(dot)
USE_OSGPLUGIN(dw)
USE_OSGPLUGIN(dxf)
//USE_OSGPLUGIN(exr)
//USE_OSGPLUGIN(gif)
USE_OSGPLUGIN(gles)
//USE_OSGPLUGIN(gz)
USE_OSGPLUGIN(hdr)
USE_OSGPLUGIN(ktx)
USE_OSGPLUGIN(logo)
USE_OSGPLUGIN(lua)
USE_OSGPLUGIN(lwo)
USE_OSGPLUGIN(lws)
USE_OSGPLUGIN(md2)
USE_OSGPLUGIN(mdl)
USE_OSGPLUGIN(normals)
//USE_OSGPLUGIN(nvtt)
USE_OSGPLUGIN(ogr)
//USE_OSGPLUGIN(openflight)
//USE_OSGPLUGIN(osc)
USE_OSGPLUGIN(p3d)
USE_OSGPLUGIN(pic)
USE_OSGPLUGIN(ply)
USE_OSGPLUGIN(pnm)
//USE_OSGPLUGIN(pov)
USE_OSGPLUGIN(pvr)
USE_OSGPLUGIN(revisions)
USE_OSGPLUGIN(rot)
USE_OSGPLUGIN(scale)
USE_OSGPLUGIN(shp)
USE_OSGPLUGIN(stl)
USE_OSGPLUGIN(tf)
USE_OSGPLUGIN(trans)
USE_OSGPLUGIN(trk)
USE_OSGPLUGIN(txf)
USE_OSGPLUGIN(txp)
USE_OSGPLUGIN(vtf)
USE_OSGPLUGIN(x)
USE_OSGPLUGIN(zip)




// .osg
USE_DOTOSGWRAPPER_LIBRARY(osg)
USE_DOTOSGWRAPPER_LIBRARY(osgFX)
USE_DOTOSGWRAPPER_LIBRARY(osgParticle)
USE_DOTOSGWRAPPER_LIBRARY(osgShadow)
USE_DOTOSGWRAPPER_LIBRARY(osgSim)
USE_DOTOSGWRAPPER_LIBRARY(osgTerrain)
USE_DOTOSGWRAPPER_LIBRARY(osgText)
USE_DOTOSGWRAPPER_LIBRARY(osgViewer)
USE_DOTOSGWRAPPER_LIBRARY(osgVolume)
USE_DOTOSGWRAPPER_LIBRARY(osgWidget)

// .osgb .osgx .osgt
USE_SERIALIZER_WRAPPER_LIBRARY(osg)
USE_SERIALIZER_WRAPPER_LIBRARY(osgAnimation)
USE_SERIALIZER_WRAPPER_LIBRARY(osgFX)
USE_SERIALIZER_WRAPPER_LIBRARY(osgManipulator)
USE_SERIALIZER_WRAPPER_LIBRARY(osgParticle)
USE_SERIALIZER_WRAPPER_LIBRARY(osgShadow)
USE_SERIALIZER_WRAPPER_LIBRARY(osgSim)
USE_SERIALIZER_WRAPPER_LIBRARY(osgTerrain)
USE_SERIALIZER_WRAPPER_LIBRARY(osgText)
USE_SERIALIZER_WRAPPER_LIBRARY(osgVolume)
#endif //0

//window

#ifndef NACL
#ifndef OSG_GLES1_AVAILABLE
#ifndef OSG_GLES2_AVAILABLE
#ifndef OSG_GLES3_AVAILABLE
USE_GRAPHICSWINDOW()
#endif
#endif
#endif
#endif

#endif //OSG_LIBRARY_STATIC


#ifdef OSGEARTH_LIBRARY_STATIC

USE_OSGPLUGIN(earth)
USE_OSGPLUGIN(kml)

USE_OSGPLUGIN(osgearth_agglite)
USE_OSGPLUGIN(osgearth_arcgis_map_cache)
USE_OSGPLUGIN(osgearth_arcgis)
USE_OSGPLUGIN(osgearth_billboard)
USE_OSGPLUGIN(osgearth_bing)
USE_OSGPLUGIN(osgearth_bumpmap)
USE_OSGPLUGIN(osgearth_cache_filesystem)
//USE_OSGPLUGIN(osgearth_cache_leveldb)
USE_OSGPLUGIN(osgearth_colorramp)
USE_OSGPLUGIN(osgearth_debug)
USE_OSGPLUGIN(osgearth_engine_byo)
USE_OSGPLUGIN(osgearth_engine_mp)
USE_OSGPLUGIN(osgearth_feature_elevation)
USE_OSGPLUGIN(osgearth_feature_ogr)
USE_OSGPLUGIN(osgearth_feature_raster)
USE_OSGPLUGIN(osgearth_feature_tfs)
USE_OSGPLUGIN(osgearth_feature_wfs)
USE_OSGPLUGIN(osgearth_gdal)
USE_OSGPLUGIN(osgearth_label_annotation)
USE_OSGPLUGIN(osgearth_mapinspector)
USE_OSGPLUGIN(osgearth_mask_feature)
USE_OSGPLUGIN(osgearth_mbtiles)
USE_OSGPLUGIN(osgearth_model_feature_geom)
USE_OSGPLUGIN(osgearth_model_feature_stencil)
USE_OSGPLUGIN(osgearth_model_simple)
USE_OSGPLUGIN(osgearth_noise)
USE_OSGPLUGIN(osgearth_normalmap)
USE_OSGPLUGIN(osgearth_ocean_simple)
USE_OSGPLUGIN(osgearth_osg)
USE_OSGPLUGIN(osgearth_quadkey)
USE_OSGPLUGIN(osgearth_refresh)
USE_OSGPLUGIN(osgearth_scriptengine_javascript)
USE_OSGPLUGIN(osgearth_sky_gl)
USE_OSGPLUGIN(osgearth_sky_simple)
USE_OSGPLUGIN(osgearth_splat_mask)
USE_OSGPLUGIN(osgearth_splat)
USE_OSGPLUGIN(osgearth_template_matclass)
USE_OSGPLUGIN(osgearth_terrainshader)
USE_OSGPLUGIN(osgearth_tilecache)
USE_OSGPLUGIN(osgearth_tileindex)
USE_OSGPLUGIN(osgearth_tileservice)
USE_OSGPLUGIN(osgearth_tms)
USE_OSGPLUGIN(osgearth_vdatum_egm84)
USE_OSGPLUGIN(osgearth_vdatum_egm96)
USE_OSGPLUGIN(osgearth_vdatum_egm2008)
USE_OSGPLUGIN(osgearth_viewpoints)
USE_OSGPLUGIN(osgearth_vpb)
USE_OSGPLUGIN(osgearth_wcs)
USE_OSGPLUGIN(osgearth_wms)
USE_OSGPLUGIN(osgearth_xyz)
USE_OSGPLUGIN(osgearth_yahoo)



//USE_OSGPLUGIN(osgearth_feature_wfs)
//USE_OSGPLUGIN(osgearth_feature_tfs)
//USE_OSGPLUGIN(osgearth_tms)
//USE_OSGPLUGIN(osgearth_wms)
////USE_OSGPLUGIN(osgearth_label_overlay)
//USE_OSGPLUGIN(osgearth_xyz)
//USE_OSGPLUGIN(osgearth_label_annotation)
//USE_OSGPLUGIN(osgearth_mask_feature)
//USE_OSGPLUGIN(osgearth_model_feature_geom)
//USE_OSGPLUGIN(osgearth_agglite)
//USE_OSGPLUGIN(osgearth_feature_ogr)
////USE_OSGPLUGIN(osgearth_model_feature_stencil)
//USE_OSGPLUGIN(osgearth_vdatum_egm2008)
//USE_OSGPLUGIN(osgearth_model_simple)
////USE_OSGPLUGIN(osgearth_engine_osgterrain)
////USE_OSGPLUGIN(osgearth_engine_quadtree)
//USE_OSGPLUGIN(osgearth_engine_mp)
//USE_OSGPLUGIN(osgearth_vdatum_egm96)
////USE_OSGPLUGIN(osgearth_ocean_surface)
//USE_OSGPLUGIN(osgearth_debug)
//USE_OSGPLUGIN(osgearth_mbtiles)
//USE_OSGPLUGIN(osgearth_vdatum_egm84)
//USE_OSGPLUGIN(osgearth_tileservice)
//USE_OSGPLUGIN(osgearth_yahoo)
//USE_OSGPLUGIN(osgearth_arcgis_map_cache)
//USE_OSGPLUGIN(osgearth_tilecache)
//USE_OSGPLUGIN(osgearth_wcs)
//USE_OSGPLUGIN(osgearth_gdal)
//
//USE_OSGPLUGIN(osgearth_cache_filesystem)
//USE_OSGPLUGIN(osgearth_arcgis)
//USE_OSGPLUGIN(osgearth_osg)
////USE_OSGPLUGIN(osgearth_scriptengine_javascriptcore)
//
//USE_OSGPLUGIN(osgearth_sky_gl)
//USE_OSGPLUGIN(osgearth_sky_simple)






#endif




#endif //USE_OSG_STATIC_LIBRARY_H

