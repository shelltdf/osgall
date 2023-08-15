/*
 *
 *  Copyright (C) 1996-2021, OFFIS e.V.
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
 *  Module:  dcmwlm
 *
 *  Author:  Andrew Hewett
 *
 *  Purpose: Worklist Database Test Program
 *
 */

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */


#include "dcmtk/dcmnet/dicom.h"
#include "dcmtk/dcmnet/dul.h"
#include "dcmtk/dcmwlm/wltypdef.h"
#include "dcmtk/dcmdata/dcvrlo.h"
#include "dcmtk/dcmdata/dcvrat.h"
#include "dcmtk/dcmwlm/wldsfs.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/dcmdata/dcuid.h"    /* for dcmtk version name */

static char rcsid[] = "$dcmtk: wltest v"
  OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

char* progname = NULL;
OFBool verbose = OFFalse;

static void
shortusage()
{
    fprintf(stderr,
      "usage: %s [-k key][-v][-d<n>] <worklist-db-dir> <query> [<query> ...]\n",
      progname);
}

static void
fullusage()
{
    fprintf(stderr, "%s\n\n", rcsid);
    shortusage();
    fprintf(stderr, "\
parameters:\n\
    <worklist-db-dir> the worklist database directory\n\
    <query> a dicom file containing a worklist query\n\
options:\n\
    -k key  override query key (gggg,eeee=\"string\")\n\
    -v      verbose mode\n\
    -d      debug mode\n");
    exit(1);
}

static void
usage()
{
    shortusage();
    exit(1);
}

static void errmsg(const char* msg, ...)
{
    va_list args;

    fprintf(stderr, "%s: ", progname);
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
}


static void
addOverrideKey(DcmDataset& overrideKeys, char* s)
{
    unsigned int g = 0xffff;
    unsigned int e = 0xffff;
    int n = 0;

    n = sscanf(s, "%x,%x=", &g, &e);
    OFString toParse = s;
    size_t eqPos = toParse.find('=');
    if (n < 2) {
        errmsg("bad key format: %s", s);
        usage(); /* does not return */
    }

    DcmTag tag(OFstatic_cast(Uint16, g), OFstatic_cast(Uint16, e));
    if (tag.error() != EC_Normal) {
        errmsg("unknown tag: (%04x,%04x)", g, e);
        usage();
    }
    DcmElement *elem = DcmItem::newDicomElement(tag);
    if (elem == NULL) {
        errmsg("cannot create element for tag: (%04x,%04x)", g, e);
        usage();
    }
    if (eqPos != OFString_npos) {
        // Make this point to the first character after the '='
        const char *val = s + eqPos + 1;
        if (elem->putString(val).bad()) {
            errmsg("cannot put tag value: (%04x,%04x)=\"%s\"", g, e, val);
            usage();
        }
    }

   if (overrideKeys.insert(elem, OFTrue).bad()) {
        errmsg("cannot insert tag: (%04x,%04x)", g, e);
        usage();
    }
}

static OFBool
queryWorklistDB(WlmDataSourceFileSystem& wdb,
    const char* queryFilename, DcmDataset& overrideKeys);


int main(int argc, char* argv[])
{
    int i = 0;
    int j = 0;

#ifdef WITH_TCPWRAPPER
    // this code makes sure that the linker cannot optimize away
    // the DUL part of the network module where the external flags
    // for libwrap are defined. Needed on OpenBSD.
    dcmTCPWrapperDaemonName.set(NULL);
#endif

    prepareCmdLineArgs(argc, argv, "wltest");

    /* strip any leading path from program name */
    if ((progname = (char *)strrchr(argv[0], PATH_SEPARATOR)) != NULL) {
        progname++;
    } else {
        progname = argv[0];
    }

    if (argc < 2) {
        fullusage();
    }

    /* make sure data dictionary is loaded */
    if (!dcmDataDict.isDictionaryLoaded()) {
        errmsg("Warning: no data dictionary loaded, check environment variable: %s\n",
              DCM_DICT_ENVIRONMENT_VARIABLE);
    }

    /* a set of query keys to be added/substituted in each query */
    DcmDataset overrideKeys;

    /* parse program arguments */
    for (i = 1; i < argc && argv[i][0] == '-'; i++) {
        switch (argv[i][1]) {
            case 'v':
                verbose = OFTrue;
                break;
            case 'd':
                OFLog::configure(OFLogger::DEBUG_LOG_LEVEL);
                verbose = OFTrue;
                break;
            case 'k':
                if ((i + 1) < argc) {
                    addOverrideKey(overrideKeys, argv[++i]);
                } else {
                    usage();
                }
                break;
            default:
                fullusage();
        }
    }

#ifdef USE_MALLOC_DEBUG
#ifdef HAVE_MALLOC_DEBUG
#define MALLOCDEBUGLEVEL 16
    if (verbose) {
        printf("malloc_debug on (%d)\n", MALLOCDEBUGLEVEL);
    }
    malloc_debug(MALLOCDEBUGLEVEL);
#endif
#endif

    for (j=i; j<argc; j++) {
        if (!OFStandard::pathExists(argv[j])) {
            OFString buffer = OFStandard::getLastSystemErrorCode().message();
            errmsg("cannot access %s: %s", argv[j], buffer.c_str());
            usage();
        }
    }

    char* dbPath = argv[i];
    i++;

    WlmDataSourceFileSystem wdb;
    wdb.SetDfPath(dbPath);
    wdb.ConnectToDataSource();

    for (j=i; j<argc; j++) {
        char* queryFilename = argv[j];

        queryWorklistDB(wdb, queryFilename, overrideKeys);
    }

    wdb.DisconnectFromDataSource();

    return 0;
}

static void
substituteOverrideKeys(DcmDataset* query, DcmDataset& overrideKeys)
{
    if (overrideKeys.card() == 0) {
        return; /* nothing to do */
    }

    /* copy the override keys */
    DcmDataset keys(overrideKeys);

    /* put the override keys into query replacing existing tags */
    int elemCount = (int)(keys.card());
    for (int i=0; i<elemCount; i++) {
        DcmElement *elem = keys.remove((unsigned long)0);

        query->insert(elem, OFTrue);
    }
}

static OFBool
isaPendingStatus(WlmDataSourceStatusType status)
{
    return ((status == WLM_PENDING) || (status == WLM_PENDING_WARNING));
}

static const char*
status2string(WlmDataSourceStatusType status)
{
    const char* s = NULL;
    switch (status) {
        case WLM_REFUSED_OUT_OF_RESOURCES:
            s = "Refused_OutOfResorces"; break;
        case WLM_FAILED_IDENTIFIER_DOES_NOT_MATCH_SOP_CLASS:
            s = "Failed_IdentifierDoesNotMatchSOPClass"; break;
        case WLM_FAILED_UNABLE_TO_PROCESS:
            s = "Failed_UnableToProcess"; break;
        case WLM_CANCEL:
            s = "Cancel"; break;
        case WLM_SUCCESS:
            s = "Success"; break;
        case WLM_PENDING:
            s = "Pending"; break;
        case WLM_PENDING_WARNING:
            s = "Pending_Warning"; break;
        default:
            s = "<UNKNOWN-STATUS>"; break;
    }
    return s;
}

static OFBool
queryWorklistDB(WlmDataSourceFileSystem& wdb,
    const char* queryFilename, DcmDataset& overrideKeys)
{

    DcmFileFormat dcmff;
    if (queryFilename != NULL)
    {
        if (dcmff.loadFile(queryFilename).bad())
        {
            errmsg("Cannot open file: %s", queryFilename);
            return OFFalse;
        }
    }

    /* replace specific keys by those in overrideKeys */
    substituteOverrideKeys(dcmff.getDataset(), overrideKeys);

    if (verbose) {
        printf("------------\n");
        printf("Query:\n");
        dcmff.getDataset()->print(COUT);
        printf("------------\n");
        printf("Starting Query\n");
    }

    /* perform the query */

    WlmDataSourceStatusType status = WLM_PENDING;
    int responseCount = 0;

    /*
    ** Start the find request.
    */
    status = wdb.StartFindRequest(*dcmff.getDataset());

    while (isaPendingStatus(status)) {

        DcmDataset* response=NULL;

        /*
        ** Get the next find response if there is one.
        */
        response = wdb.NextFindResponse(status);
        responseCount++;

        if (isaPendingStatus(status)) {
            printf("------------\n");
            printf("Response %d (%s)\n", responseCount, status2string(status));
            response->print(COUT);
        } else if (response->card() > 0) {
            errmsg("ERROR: non-pending status with a response!");
        }
        delete response;
    }

    if (verbose) {
        printf("------------\n");
        printf("Query Complete (%s)\n", status2string(status));
        if (wdb.GetErrorComments() != NULL) {
            printf("Error Comment:\n");
            DcmLongString errorComment(*wdb.GetErrorComments());
            errorComment.print(COUT);
        }
        if (wdb.GetOffendingElements() != NULL) {
            printf("Offending Elements:\n");
            DcmAttributeTag offendingElements(*wdb.GetOffendingElements());
            offendingElements.print(COUT);
        }
    }

    return OFTrue;
}
