// Module:  Log4CPLUS
// File:    clogger.cxx
// Created: 01/2011
// Author:  Jens Rehsack
//
//
// Copyright 2011 Jens Rehsack & Tad E. Smith
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

#include "dcmtk/oflog/logger.h"
#include "dcmtk/oflog/clogger.h"
#include "dcmtk/oflog/appender.h"
#include "dcmtk/oflog/hierarchy.h"
#include "dcmtk/oflog/helpers/loglog.h"
#include "dcmtk/oflog/spi/logimpl.h"
#include "dcmtk/oflog/configrt.h"
#include "dcmtk/oflog/streams.h"
#include "dcmtk/oflog/helpers/snprintf.h"

#include <cerrno>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#include <sstream>

using namespace dcmtk::log4cplus;
using namespace dcmtk::log4cplus::helpers;

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_file_configure(const log4cplus_char_t *pathname)
{
    if( !pathname )
        return EINVAL;

    try
    {
        PropertyConfigurator::doConfigure( pathname );
    }
    catch(STD_NAMESPACE exception const &)
    {
        return -1;
    }

    return 0;
}

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_str_configure(const log4cplus_char_t *config)
{
    if( !config )
        return EINVAL;

    try
    {
        tstring s(config);
        PropertyConfigurator pc(s);
        pc.configure();
    }
    catch(STD_NAMESPACE exception const &)
    {
        return -1;
    }

    return 0;
}

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_basic_configure(void)
{
    try
    {
        BasicConfigurator::doConfigure();
    }
    catch(STD_NAMESPACE exception const &)
    {
        return -1;
    }

    return 0;
}

DCMTK_LOG4CPLUS_EXPORT void
log4cplus_shutdown(void)
{
    Logger::shutdown();
}

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_logger_exists(const log4cplus_char_t *name)
{
    int retval = false;

    try
    {
        retval = Logger::exists(name);
    }
    catch(STD_NAMESPACE exception const &)
    {
        // Fall through.
    }

    return retval;
}

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_logger_is_enabled_for(const log4cplus_char_t *name, loglevel_t ll)
{
    int retval = false;

    try
    {
        Logger logger = name ? Logger::getInstance(name) : Logger::getRoot();
        retval = logger.isEnabledFor(ll);
    }
    catch(STD_NAMESPACE exception const &)
    {
        // Fall through.
    }

    return retval;
}

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_logger_log(const log4cplus_char_t *name, loglevel_t ll,
    const log4cplus_char_t *msgfmt, ...)
{
    int retval = -1;

    try
    {
        Logger logger = name ? Logger::getInstance(name) : Logger::getRoot();

        if( logger.isEnabledFor(ll) )
        {
            va_list ap;
            va_start(ap, msgfmt);
            snprintf_buf buf;
            const tchar * msg = buf.print_va_list(msgfmt, ap);
            va_end(ap);
            logger.forcedLog(ll, msg);
        }

        retval = 0;
    }
    catch(STD_NAMESPACE exception const &)
    {
        // Fall through.
    }

    return retval;
}

DCMTK_LOG4CPLUS_EXPORT int
log4cplus_logger_force_log(const log4cplus_char_t *name, loglevel_t ll, const log4cplus_char_t *msgfmt, ...)
{
    int retval = -1;

    try
    {
        Logger logger = name ? Logger::getInstance(name) : Logger::getRoot();

        va_list ap;
        va_start(ap, msgfmt);
        snprintf_buf buf;
        const tchar * msg = buf.print_va_list(msgfmt, ap);
        va_end(ap);
        logger.forcedLog(ll, msg);

        retval = 0;
    }
    catch(STD_NAMESPACE exception const &)
    {
        // Fall through.
    }

    return retval;
}
