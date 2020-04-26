/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_ERROR_H
#define _CFX_ERROR_H

//#ifndef _LIB
#include <Cg/cg.h>
//#else
//#include <cfxNoCg.h>
//#endif

// this is used to set the cg error callback
// it will print the string if there is an error
// it will assert if there is an error in a non release build
void cfxCgErrorCallback();

// this is not needed (or useful in any way) when using the error callback
CGerror cfxCheckCgError();


#endif // _CFX_ERROR_H

