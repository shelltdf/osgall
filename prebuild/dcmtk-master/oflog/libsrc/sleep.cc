// Module:  Log4CPLUS
// File:    sleep.cxx
// Created: 5/2003
// Author:  Tad E. Smith
//
//
// Copyright 2003-2010 Tad E. Smith
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dcmtk/oflog/config/windowsh.h"
#include "dcmtk/oflog/helpers/sleep.h"
#include "dcmtk/oflog/helpers/timehelp.h"

#include <cerrno>
#if defined (DCMTK_LOG4CPLUS_HAVE_ERRNO_H)
#include <errno.h>
#endif

#if defined (DCMTK_LOG4CPLUS_HAVE_TIME_H)
#include <time.h>
#endif


namespace dcmtk {
namespace log4cplus { namespace helpers {


///////////////////////////////////////////////////////////////////////////////
// public methods
///////////////////////////////////////////////////////////////////////////////

int const MILLIS_TO_NANOS = 1000000;
int const SEC_TO_MILLIS = 1000;


#if ! defined (_WIN32)
static inline
int
unix_nanosleep (const struct timespec *req, struct timespec *rem)
{
#if defined (DCMTK_LOG4CPLUS_HAVE_CLOCK_NANOSLEEP)
    return clock_nanosleep (CLOCK_REALTIME, 0, req, rem);

#elif defined (DCMTK_LOG4CPLUS_HAVE_NANOSLEEP)
    return nanosleep (req, rem);

#else
#  error no nanosleep() or clock_nanosleep()
#endif
}

#endif


void
sleep(unsigned long secs, unsigned long nanosecs)
{
#if defined(_WIN32)
    DWORD const MAX_SLEEP_SECONDS = 4294966;        // (2**32-2)/1000

    DWORD nano_millis = nanosecs / OFstatic_cast(unsigned long, MILLIS_TO_NANOS);
    if (secs <= MAX_SLEEP_SECONDS) {
        Sleep((secs * SEC_TO_MILLIS) + nano_millis);
        return;
    }
        
    DWORD no_of_max_sleeps = secs / MAX_SLEEP_SECONDS;
            
    for(DWORD i = 0; i < no_of_max_sleeps; i++) {
        Sleep(MAX_SLEEP_SECONDS * SEC_TO_MILLIS);
    }
               
    Sleep((secs % MAX_SLEEP_SECONDS) * SEC_TO_MILLIS + nano_millis);
#else
    timespec sleep_time = { OFstatic_cast(STD_NAMESPACE time_t, secs),
                            OFstatic_cast(long, nanosecs) };
    timespec remain;
    while (unix_nanosleep (&sleep_time, &remain)) {
        if (errno == EINTR) {
            sleep_time.tv_sec  = remain.tv_sec;
            sleep_time.tv_nsec = remain.tv_nsec;               
            continue;
        }
        else {
            return;
        }
    }
#endif
}


void
sleepmillis(unsigned long millis)
{
    unsigned long secs = millis / SEC_TO_MILLIS;
    unsigned long nanosecs = (millis % SEC_TO_MILLIS) * MILLIS_TO_NANOS;
    sleep(secs, nanosecs);
}

} } // namespace log4cplus { namespace helpers {
} // end namespace dcmtk
