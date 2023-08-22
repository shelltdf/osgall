/* Temporary files with automatic cleanup.
   Copyright (C) 2006-2021 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2006.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#ifndef _CLEAN_TEMP_SIMPLE_H
#define _CLEAN_TEMP_SIMPLE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/* See clean-temp.h for a general discussion of this module.  */

/* Register the given ABSOLUTE_FILE_NAME as being a file that needs to be
   removed.
   Should be called before the file ABSOLUTE_FILE_NAME is created.
   Return 0 upon success, or -1 if there was a memory allocation problem.  */
extern int register_temporary_file (const char *absolute_file_name);

/* Unregister the given ABSOLUTE_FILE_NAME as being a file that needs to be
   removed.
   Should be called when the file ABSOLUTE_FILE_NAME could not be created.  */
extern void unregister_temporary_file (const char *absolute_file_name);

/* Remove the given ABSOLUTE_FILE_NAME and unregister it.
   CLEANUP_VERBOSE determines whether errors are reported to standard error.
   Return 0 upon success, or -1 if there was some problem.  */
extern int cleanup_temporary_file (const char *absolute_file_name,
                                   bool cleanup_verbose);


#ifdef __cplusplus
}
#endif

#endif /* _CLEAN_TEMP_SIMPLE_H */
