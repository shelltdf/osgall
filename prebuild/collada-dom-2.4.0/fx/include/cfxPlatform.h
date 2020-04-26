/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
#ifndef _CFX_PLATFORM_H_
#define _CFX_PLATFORM_H_

#include <stdio.h>

// The CFX_PLATFORM_INCLUDE define specifies the header file which includes all platform specific information
// For windows projects this is \<cfxWin.h\> which makes this statement include <cfxWin.h>
// For Linux projects this is \<cfxLinux.h\> which makes this statement include <cfxLinux.h>
// The define is defaulted to windows in the Makefile
#include CFX_PLATFORM_INCLUDE

#endif 

