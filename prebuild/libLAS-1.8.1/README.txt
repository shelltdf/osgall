.. _home:

******************************************************************************
libLAS - LAS 1.0/1.1/1.2 ASPRS LiDAR data translation toolset
******************************************************************************


libLAS is a C/C++ library for reading and writing the very common `LAS`
`LiDAR`_ format. The `ASPRS LAS format`_ is a sequential binary format used to
store data from LiDAR sensors and by LiDAR processing software for data
interchange and archival.  See :ref:`features` for more details on what 
libLAS can provide a LiDAR software developer.  See :ref:`start` for how to 
get started using the library.


libLAS' initial development was supported in 2007-2008 by the `IGSB`_ of the
Iowa DNR for use in its state-wide `LIDAR`_ project.  Ongoing support for 
libLAS is provided by a number of organizations including the 
`U.S. Army Cold Regions Research and Engineering Laboratory`_.

As of 2014, libLAS has been almost entirely superceded by `Martin Isenburg`_'s `LASlib`_ library. The only functionality that libLAS provides over LASlib is the 
ability to work with `GDAL`_-derived coordinate system information and 
reproject into infrequently-used coordinate systems. Additionally, libLAS' 
Python capabilities are much better expressed by the pure-Python laspy 
http://laspy.org library, and if manipulating LAS data in Python is your 
ultimate desire, use that instead of libLAS.




.. _`U.S. Army Cold Regions Research and Engineering Laboratory` : http://www.crrel.usace.army.mil/

.. _`LASlib`: http://www.cs.unc.edu/~isenburg/lastools/download/laslib.zip





.. _`free-as-in-beer`: http://en.wikipedia.org/wiki/Gratis_versus_Libre#.22Free_beer.22_vs_.22free_speech.22_distinction




License
------------------------------------------------------------------------------

libLAS is available under the terms of the `BSD License`_. 
See :ref:`License` for a copy.











.. _`LIDAR`: http://en.wikipedia.org/wiki/LIDAR
.. _`IGSB`: http://www.igsb.uiowa.edu/
.. _`Martin Isenburg`: http://www.cs.unc.edu/~isenburg/
.. _`LAStools`: http://www.cs.unc.edu/~isenburg/lastools/
.. _`Martin Isenburg and Jonathan Shewchuk`: http://www.cs.unc.edu/~isenburg/lastools/
.. _`LAS Format`: http://www.lasformat.org/
.. _`ASPRS Standards Committee`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html
.. _`ASPRS LAS format`: http://www.asprs.org/society/committees/standards/lidar_exchange_format.html

.. _`BSD license`: http://www.opensource.org/licenses/bsd-license.php
.. _`LAS Format Elements`: http://liblas.org/wiki/LASElements
.. _`Tutorials`: http://liblas.org/wiki/Tutorial
.. _`Software`: http://liblas.org/wiki/Software
.. _`Resources`: http://liblas.org/wiki/Resources
.. _`License`: http://liblas.org/wiki/License
.. _`Library Design`: http://liblas.org/wiki/LibraryDesign
.. _`FOSS4G 2008`: http://liblas.org/raw-attachment/wiki/WikiStart/liblas_foss4g_2008.pdf
.. _`LAS C API`: http://liblas.org/doxygen/liblas_8h.html
.. _`LAS Doxygen Docs`: http://liblas.org/doxygen/
.. _`How To Release libLAS`: http://liblas.org/wiki/HowToRelease
.. _`libgeotiff`: http://www.remotesensing.org/geotiff/geotiff.html
.. _`7/28/2008`: http://lists.maptools.org/pipermail/geotiff/2008-July/000501.html
.. _`proj.4`: http://proj.maptools.org/
.. _`GDAL`: http://www.gdal.org
.. _`OSGeo4W`: http://wiki.osgeo.org/wiki/OSGeo_Win32_Installer
.. _`dotnetLibLAS API`: http://liblas.org/wiki/dotnetapi
.. _`FOSS4G 2008`: http://liblas.org/raw-attachment/wiki/WikiStart/liblas_foss4g_2008.pdf
.. _`ASPRS Workshop Demo`: http://liblas.org/raw-attachment/wiki/WikiStart/ASPRS-Workshop-NOAA-Demo.pdf
.. _`vertical coordinate systems`: http://liblas.org/wiki/VerticalCS
.. _`Python howto`: http://liblas.org/wiki/PythonHowTo