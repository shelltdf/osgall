/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/
// System includes

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <cfxError.h>


// this is a handy callback for the cg errors
void cfxCgErrorCallback()
{
		CGerror error = cgGetError();
			
#ifndef _LIB
		const char* report = cgGetErrorString(error);
		printf("CG Error Detected:  %s\n", report);

		assert(0 && report);
#endif
}


CGerror cfxCheckCgError()
{
		CGerror error = cgGetError();
			
		if (error != CG_NO_ERROR)
		{
			const char* report = cgGetErrorString(error);
			printf("CG Error Detected:  %s\n", report);
			assert(0 && report);
		}

		return error;
}




