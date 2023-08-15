/*
 *
 *  Copyright (C) 1993-2021, OFFIS e.V.
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
 *  Module:  dcmqrdb
 *
 *  Author:  Marco Eichelberg
 *
 *  Purpose: class DcmQueryRetrieveGetContext
 *
 */

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/dcmqrdb/dcmqrcbg.h"

#include "dcmtk/dcmqrdb/dcmqrcnf.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmqrdb/dcmqropt.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmnet/dimse.h"       /* for DICOM_WARNING_STATUS */
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmqrdb/dcmqrdbs.h"
#include "dcmtk/dcmqrdb/dcmqrdbi.h"
#include "dcmtk/ofstd/ofstd.h"

BEGIN_EXTERN_C
#ifdef HAVE_FCNTL_H
#include <fcntl.h>       /* needed on Solaris for O_RDONLY */
#endif
END_EXTERN_C

static void getSubOpProgressCallback(void * /* callbackData */,
    T_DIMSE_StoreProgress *progress,
    T_DIMSE_C_StoreRQ * /*req*/)
{
  //DcmQueryRetrieveGetContext *context = OFstatic_cast(DcmQueryRetrieveGetContext *, callbackData);
  // We can't use oflog for the pdu output, but we use a special logger for
  // generating this output. If it is set to level "INFO" we generate the
  // output, if it's set to "DEBUG" then we'll assume that there is debug output
  // generated for each PDU elsewhere.
  OFLogger progressLogger = OFLog::getLogger("dcmtk.dcmqrdb.progress");
  if (progressLogger.getChainedLogLevel() == OFLogger::INFO_LOG_LEVEL)
  {
    switch (progress->state)
    {
      case DIMSE_StoreBegin:
        printf("XMIT: ");
        break;
      case DIMSE_StoreEnd:
        printf("\n");
        break;
      default:
        putchar('.');
        break;
    }
    fflush(stdout);
  }
}

void DcmQueryRetrieveGetContext::callbackHandler(
    /* in */
    OFBool cancelled, T_DIMSE_C_GetRQ *request,
    DcmDataset *requestIdentifiers, int responseCount,
    /* out */
    T_DIMSE_C_GetRSP *response, DcmDataset **stDetail,
    DcmDataset **responseIdentifiers)
{
    OFCondition dbcond = EC_Normal;
    DcmQueryRetrieveDatabaseStatus dbStatus(priorStatus);

    if (responseCount == 1) {
        /* start the database search */
        DCMQRDB_INFO("Get SCP Request Identifiers:" << OFendl << DcmObject::PrintHelper(*requestIdentifiers));
        dbcond = dbHandle.startMoveRequest(
            request->AffectedSOPClassUID, requestIdentifiers, &dbStatus);
        if (dbcond.bad()) {
            OFString temp_str;
            DCMQRDB_ERROR("getSCP: Database: startMoveRequest Failed ("
                << DU_cmoveStatusString(dbStatus.status()) << "): "
                << DimseCondition::dump(temp_str, dbcond));
        }
    }

    /* only cancel if we have pending status */
    if (cancelled && dbStatus.status() == STATUS_Pending) {
        dbHandle.cancelMoveRequest(&dbStatus);
    }

    if (dbStatus.status() == STATUS_Pending) {
        getNextImage(&dbStatus);
    }

    if (dbStatus.status() != STATUS_Pending) {

        /*
         * Need to adjust the final status if any sub-operations failed or
         * had warnings
         */
        if (nFailed > 0 || nWarning > 0) {
            dbStatus.setStatus(STATUS_GET_Warning_SubOperationsCompleteOneOrMoreFailures);
        }
        /*
         * if all the sub-operations failed then we need to generate a failed or refused status.
         * cf. DICOM part 4, C.4.3.3.1
         * we choose to generate a "Refused - Out of Resources - Unable to perform suboperations" status.
         */
        if ((nFailed > 0) && ((nCompleted + nWarning) == 0)) {
            dbStatus.setStatus(STATUS_GET_Refused_OutOfResourcesSubOperations);
        }
    }

    DCMQRDB_INFO("Get SCP Response " << responseCount << " [status: "
            << DU_cmoveStatusString(dbStatus.status()) << "]");

    if (dbStatus.status() != STATUS_Success &&
        dbStatus.status() != STATUS_Pending) {
        /*
         * May only include response identifiers if not Success
         * and not Pending
         */
        buildFailedInstanceList(responseIdentifiers);
    }

    /* set response status */
    response->DimseStatus = dbStatus.status();
    response->NumberOfRemainingSubOperations = nRemaining;
    response->NumberOfCompletedSubOperations = nCompleted;
    response->NumberOfFailedSubOperations = nFailed;
    response->NumberOfWarningSubOperations = nWarning;
    *stDetail = dbStatus.extractStatusDetail();

}

void DcmQueryRetrieveGetContext::addFailedUIDInstance(const char *sopInstance)
{
    size_t len;
    size_t buflen = DIC_UI_LEN+1;
    if (failedUIDs == NULL) {
        if ((failedUIDs = (char*)malloc(buflen)) == NULL) {
            DCMQRDB_ERROR("malloc failure: addFailedUIDInstance");
            return;
        }
        OFStandard::strlcpy(failedUIDs, sopInstance, buflen);
    } else {
        len = strlen(failedUIDs);
        buflen = len+strlen(sopInstance)+2;
        if ((failedUIDs = (char*)realloc(failedUIDs, buflen)) == NULL) {
            DCMQRDB_ERROR("realloc failure: addFailedUIDInstance");
            return;
        }
        /* tag sopInstance onto end of old with '\' between */
        OFStandard::strlcat(failedUIDs, "\\", buflen);
        OFStandard::strlcat(failedUIDs, sopInstance, buflen);
    }
}

OFCondition DcmQueryRetrieveGetContext::performGetSubOp(DIC_UI sopClass, DIC_UI sopInstance, char *fname)
{
    OFCondition cond = EC_Normal;
    T_DIMSE_C_StoreRQ req;
    T_DIMSE_C_StoreRSP rsp;
    DIC_US msgId;
    T_ASC_PresentationContextID presId;
    DcmDataset *stDetail = NULL;

#ifdef LOCK_IMAGE_FILES
    /* shared lock image file */
    int lockfd;
#ifdef O_BINARY
    lockfd = open(fname, O_RDONLY | O_BINARY, 0666);
#else
    lockfd = open(fname, O_RDONLY , 0666);
#endif
    if (lockfd < 0) {
        /* due to quota system the file could have been deleted */
        DCMQRDB_ERROR("Get SCP: storeSCU: [file: " << fname << "]: " << OFStandard::getLastSystemErrorCode().message());
        nFailed++;
        addFailedUIDInstance(sopInstance);
        return EC_Normal;
    }
    dcmtk_flock(lockfd, LOCK_SH);
#endif

    msgId = origAssoc->nextMsgID++;

    /* which presentation context should be used */
    presId = ASC_findAcceptedPresentationContextID(origAssoc,
        sopClass);
    if (presId == 0) {
        nFailed++;
        addFailedUIDInstance(sopInstance);
        DCMQRDB_ERROR("Get SCP: storeSCU: [file: " << fname << "] No presentation context for: ("
            << dcmSOPClassUIDToModality(sopClass, "OT") << ") " << sopClass);
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    } else {
        /* make sure that we can send images in this presentation context */
        T_ASC_PresentationContext pc;
        ASC_findAcceptedPresentationContext(origAssoc->params, presId, &pc);
        /* the acceptedRole is the association requestor role */
        if ((pc.acceptedRole != ASC_SC_ROLE_SCP) && (pc.acceptedRole != ASC_SC_ROLE_SCUSCP)) {
            /* the role is not appropriate */
            nFailed++;
            addFailedUIDInstance(sopInstance);
            DCMQRDB_ERROR("Get SCP: storeSCU: [file: " << fname << "] No presentation context with requestor SCP role for: ("
                << dcmSOPClassUIDToModality(sopClass, "OT") << ") " << sopClass);
            return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
        }
    }

    req.MessageID = msgId;
    OFStandard::strlcpy(req.AffectedSOPClassUID, sopClass, DIC_UI_LEN + 1);
    OFStandard::strlcpy(req.AffectedSOPInstanceUID, sopInstance, DIC_UI_LEN + 1);
    req.DataSetType = DIMSE_DATASET_PRESENT;
    req.Priority = priority;
    req.opts = 0;

    DCMQRDB_INFO("Store SCU RQ: MsgID " << msgId << ", ("
        << dcmSOPClassUIDToModality(sopClass, "OT") << ")");

    T_DIMSE_DetectedCancelParameters cancelParameters;

    cond = DIMSE_storeUser(origAssoc, presId, &req,
        fname, NULL, getSubOpProgressCallback, this, options_.blockMode_, options_.dimse_timeout_,
        &rsp, &stDetail, &cancelParameters);

#ifdef LOCK_IMAGE_FILES
    /* unlock image file */
    dcmtk_flock(lockfd, LOCK_UN);
    close(lockfd);
#endif

    if (cond.good()) {
        if (cancelParameters.cancelEncountered) {
            if (origPresId == cancelParameters.presId &&
                origMsgId == cancelParameters.req.MessageIDBeingRespondedTo) {
                getCancelled = OFTrue;
            } else {
                DCMQRDB_ERROR("Get SCP: Unexpected C-Cancel-RQ encountered: pid=" << (int)cancelParameters.presId
                    << ", mid=" << (int)cancelParameters.req.MessageIDBeingRespondedTo);
            }
        }
        DCMQRDB_INFO("Get SCP: Received Store SCU RSP [Status="
            << DU_cstoreStatusString(rsp.DimseStatus) << "]");
        if (rsp.DimseStatus == STATUS_Success) {
            /* everything ok */
            nCompleted++;
        } else if (DICOM_WARNING_STATUS(rsp.DimseStatus)) {
            /* a warning status message */
            nWarning++;
            DCMQRDB_ERROR("Get SCP: Store Warning: Response Status: " <<
                DU_cstoreStatusString(rsp.DimseStatus));
        } else {
            nFailed++;
            addFailedUIDInstance(sopInstance);
            /* print a status message */
            DCMQRDB_ERROR("Get SCP: Store Failed: Response Status: "
                << DU_cstoreStatusString(rsp.DimseStatus));
        }
    } else {
        nFailed++;
        addFailedUIDInstance(sopInstance);
        OFString temp_str;
        DCMQRDB_ERROR("Get SCP: storeSCU: Store Request Failed: " << DimseCondition::dump(temp_str, cond));
    }
    if (stDetail) {
        DCMQRDB_INFO("  Status Detail:" << OFendl << DcmObject::PrintHelper(*stDetail));
        delete stDetail;
    }
    return cond;
}

void DcmQueryRetrieveGetContext::getNextImage(DcmQueryRetrieveDatabaseStatus * dbStatus)
{
    OFCondition cond = EC_Normal;
    OFCondition dbcond = EC_Normal;
    DIC_UI subImgSOPClass;      /* sub-operation image SOP Class */
    DIC_UI subImgSOPInstance;   /* sub-operation image SOP Instance */
    char subImgFileName[MAXPATHLEN + 1];    /* sub-operation image file */

    /* clear out strings */
    memset(subImgFileName, 0, sizeof(subImgFileName));
    memset(subImgSOPClass, 0, sizeof(subImgSOPClass));
    memset(subImgSOPInstance, 0, sizeof(subImgSOPInstance));

    /* get DB response */
    dbcond = dbHandle.nextMoveResponse(
        subImgSOPClass, sizeof(subImgSOPClass), subImgSOPInstance, sizeof(subImgSOPInstance), subImgFileName, sizeof(subImgFileName), &nRemaining, dbStatus);
    if (dbcond.bad()) {
        DCMQRDB_ERROR("getSCP: Database: nextMoveResponse Failed ("
            << DU_cmoveStatusString(dbStatus->status()) << "):");
    }

    if (dbStatus->status() == STATUS_Pending) {
        /* perform sub-op */
        cond = performGetSubOp(subImgSOPClass, subImgSOPInstance, subImgFileName);

        if (getCancelled) {
            dbStatus->setStatus(STATUS_GET_Cancel_SubOperationsTerminatedDueToCancelIndication);
            DCMQRDB_INFO("Get SCP: Received C-Cancel RQ");
       }

        if (cond != EC_Normal) {
            OFString temp_str;
            DCMQRDB_ERROR("getSCP: Get Sub-Op Failed: " << DimseCondition::dump(temp_str, cond));
            /* clear condition stack */
        }
    }
}

void DcmQueryRetrieveGetContext::buildFailedInstanceList(DcmDataset ** rspIds)
{
    OFBool ok;

    if (failedUIDs != NULL) {
        *rspIds = new DcmDataset();
        ok = DU_putStringDOElement(*rspIds, DCM_FailedSOPInstanceUIDList, failedUIDs);
        if (!ok) {
            DCMQRDB_ERROR("getSCP: failed to build DCM_FailedSOPInstanceUIDList");
        }
        free(failedUIDs);
        failedUIDs = NULL;
    }
}
