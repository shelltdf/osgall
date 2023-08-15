/*
 *
 *  Copyright (C) 2003-2022, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmimage
 *
 *  Author:  Alexander Haderer
 *
 *  Purpose: Implements PNG interface for pluggable image formats
 *
 */


#ifndef DIPIPNG_H
#define DIPIPNG_H

#include "dcmtk/config/osconfig.h"

#ifdef WITH_LIBPNG

#include "dcmtk/ofstd/ofstring.h"
#include "dcmtk/dcmimgle/diplugin.h"
#include "dcmtk/dcmimage/dicdefin.h"


/*------------------------*
 *  forward declarations  *
 *------------------------*/

class DiImage;


/*--------------------*
 *  type definitions  *
 *--------------------*/

/** describes the different types of interlace supported by
 *  the PNG plugin.  Enumeration depends on capabilities
 *  of underlying PNG library (libpng).
 *  @remark this enum is only available if DCMTK is compiled with
 *  PNG (libpng) support enabled.
 */
enum DiPNGInterlace
{
  /// Adam7
  E_pngInterlaceAdam7,

  /// no interlace
  E_pngInterlaceNone
};

/** describes the different types of metainfo written to
 *  the png file.
 *  @remark this enum is only available if DCMTK is compiled with
 *  PNG (libpng) support enabled.
 */
enum DiPNGMetainfo
{
  /// no meta information
  E_pngNoMetainfo,

  /// file time, creator
  E_pngFileMetainfo
};


/*---------------------*
 *  class declaration  *
 *---------------------*/

/** Implementation of a PNG plugin for the dcmimgle/dcmimage library
 *  @remark This class is only available if DCMTK is compiled with
 *  PNG (libpng) support enabled.
 */
class DCMTK_DCMIMAGE_EXPORT DiPNGPlugin
  : public DiPluginFormat
{

  public:

    /** constructor
     */
    DiPNGPlugin();

    /** destructor
     */
    virtual ~DiPNGPlugin();

    /** write given image to a file stream (PNG format)
     *  @param image pointer to DICOM image object to be written
     *  @param stream stream to which the image is written (open in binary mode!)
     *  @param frame index of frame used for output (default: first frame = 0)
     *  @return true if successful, false otherwise
     */
    virtual int write(DiImage *image,
                      FILE *stream,
                      const unsigned long frame = 0) const;

    /** set interlace type for PNG creation
     *  @param inter interlace type
     */
    void setInterlaceType(DiPNGInterlace inter);

    /** set metainfo type for PNG creation
     *  @param minfo metainfo type
     */
    void setMetainfoType(DiPNGMetainfo minfo);

    /* set bits per sample for PNG creation
     * @param bpp bits per sample (8 or 16)
     */
    void setBitsPerSample(const int bpp);

    /** get version information of the PNG library.
     *  Typical output format: "LIBPNG, Version 3.5.7"
     *  @return name and version number of the PNG library
     */
    static OFString getLibraryVersionString();


 private:

    /// PNG interlace type
    DiPNGInterlace interlaceType;

    /// PNG metainfo type
    DiPNGMetainfo metainfoType;

    /// bits per sample (8 or 16, default: 8)
    int bitsPerSample;
};

#endif
#endif
