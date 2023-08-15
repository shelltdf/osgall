// Module:  Log4CPLUS
// File:    hierarchylocker.cxx
// Created: 8/2003
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

#include "dcmtk/oflog/hierlock.h"
#include "dcmtk/oflog/hierarchy.h"
#include "dcmtk/oflog/helpers/loglog.h"
#include "dcmtk/oflog/spi/logimpl.h"
#include "dcmtk/oflog/thread/syncpub.h"


namespace dcmtk
{
namespace log4cplus
{


//////////////////////////////////////////////////////////////////////////////
// HierarchyLocker ctor and dtor
//////////////////////////////////////////////////////////////////////////////

HierarchyLocker::HierarchyLocker(Hierarchy& _h)
: h(_h),
  hierarchyLocker(h.hashtable_mutex),
  loggerList()
{
    // Get a copy of all of the Hierarchy's Loggers (except the Root Logger)
    h.initializeLoggerList(loggerList);

    // Lock all of the Hierarchy's Loggers' mutexes
    LoggerList::iterator it = loggerList.begin();;
    try
    {
        for (it = loggerList.begin(); it != loggerList.end(); ++it)
            it->value->appender_list_mutex.lock ();
    }
    catch (...)
    {
        helpers::getLogLog().error(
            DCMTK_LOG4CPLUS_TEXT("HierarchyLocker::ctor()")
            DCMTK_LOG4CPLUS_TEXT("- An error occurred while locking"));
        LoggerList::iterator range_end = it;
        for (it = loggerList.begin (); it != range_end; ++it)
            it->value->appender_list_mutex.unlock ();
        throw;
    }
}


HierarchyLocker::~HierarchyLocker()
{
    try {
        for(LoggerList::iterator it=loggerList.begin(); it!=loggerList.end(); ++it) {
            it->value->appender_list_mutex.unlock ();
        }
    }
    catch(...) {
        helpers::getLogLog().error(DCMTK_LOG4CPLUS_TEXT("HierarchyLocker::dtor()- An error occurred while unlocking"));
        // Disabled the following line: Don't throw an exception from a destructor!
        // throw;
    }
}

void
HierarchyLocker::resetConfiguration()
{
    Logger root = h.getRoot();
    h.disable(Hierarchy::DISABLE_OFF);

    // begin by closing nested appenders
    // then, remove all appenders
    root.setLogLevel(DEBUG_LOG_LEVEL);
    root.closeNestedAppenders();
    root.removeAllAppenders();

    // repeat
    for(LoggerList::iterator it=loggerList.begin(); it!=loggerList.end(); ++it)
    {
        Logger & logger = *it;

        logger.closeNestedAppenders();
        logger.removeAllAppenders();

        logger.setLogLevel(NOT_SET_LOG_LEVEL);
        logger.setAdditivity(true);
    }
}


Logger
HierarchyLocker::getInstance(const tstring& name)
{
    return h.getInstanceImpl(name, *h.getLoggerFactory());
}


Logger
HierarchyLocker::getInstance(const tstring& name, spi::LoggerFactory& factory)
{
    return h.getInstanceImpl(name, factory);
}


void
HierarchyLocker::addAppender(Logger& logger, SharedAppenderPtr& appender)
{
    for(LoggerList::iterator it=loggerList.begin(); it!=loggerList.end(); ++it) {
        if((*it).value == logger.value) {
            logger.value->appender_list_mutex.unlock ();
            logger.addAppender(appender);
            logger.value->appender_list_mutex.lock ();
            return;
        }
    }

    // I don't have this Logger locked
    logger.addAppender(appender);
}


} // namespace log4cplus
} // end namespace dcmtk
