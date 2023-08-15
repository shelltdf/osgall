/*
 *
 *  Copyright (C) 1993-2018, OFFIS e.V.
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
 *  Purpose: class DcmQueryRetrieveStoreContext
 *
 */

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/dcmqrdb/dcmqrcbs.h"

#include "dcmtk/dcmqrdb/dcmqrcnf.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmqrdb/dcmqropt.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmqrdb/dcmqrdbs.h"
#include "dcmtk/dcmqrdb/dcmqrdbi.h"


void DcmQueryRetrieveStoreContext::updateDisplay(T_DIMSE_StoreProgress * progress)
{
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
        printf("RECV: ");
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


void DcmQueryRetrieveStoreContext::saveImageToDB(
    T_DIMSE_C_StoreRQ *req,             /* original store request */
    const char *imageFileName,
    /* out */
    T_DIMSE_C_StoreRSP *rsp,            /* final store response */
    DcmDataset **stDetail)
{
    OFCondition dbcond = EC_Normal;
    DcmQueryRetrieveDatabaseStatus dbStatus(STATUS_Success);

    /* Store image */
    if (options_.ignoreStoreData_) {
        rsp->DimseStatus = STATUS_Success;
        *stDetail = NULL;
        return; /* nothing else to do */
    }

    if (status == STATUS_Success)
    {
        dbcond = dbHandle.storeRequest(
            req->AffectedSOPClassUID, req->AffectedSOPInstanceUID,
            imageFileName, &dbStatus);
        if (dbcond.bad())
        {
            OFString temp_str;
            DCMQRDB_ERROR("storeSCP: Database: storeRequest Failed ("
                << DU_cstoreStatusString(dbStatus.status()) << "): "
                << DimseCondition::dump(temp_str, dbcond));
        }
        status = dbStatus.status();
    }

    rsp->DimseStatus = status;
    *stDetail = dbStatus.extractStatusDetail();
}

void DcmQueryRetrieveStoreContext::writeToFile(
    DcmFileFormat *ff,
    const char* fname,
    T_DIMSE_C_StoreRSP *rsp)
{
    E_TransferSyntax xfer = options_.writeTransferSyntax_;
    if (xfer == EXS_Unknown) xfer = ff->getDataset()->getOriginalXfer();

    OFCondition cond = ff->saveFile(fname, xfer, options_.sequenceType_,
        options_.groupLength_, options_.paddingType_, (Uint32)options_.filepad_,
        (Uint32)options_.itempad_, (options_.useMetaheader_) ? EWM_fileformat : EWM_dataset);

    if (cond.bad())
    {
      DCMQRDB_ERROR("storescp: Cannot write image file: " << fname);
      rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;

      // delete incomplete file
      OFStandard::deleteFile(fname);
    }
}

void DcmQueryRetrieveStoreContext::checkRequestAgainstDataset(
    T_DIMSE_C_StoreRQ *req,     /* original store request */
    const char* fname,          /* filename of dataset */
    DcmDataset *dataSet,        /* dataset to check */
    T_DIMSE_C_StoreRSP *rsp,    /* final store response */
    OFBool uidPadding)          /* correct UID passing */
{
    DcmFileFormat ff;

    if (dataSet == NULL)
    {
      ff.loadFile(fname);
      dataSet = ff.getDataset();
    }

    /* which SOP class and SOP instance ? */
    DIC_UI sopClass;
    DIC_UI sopInstance;

    if (!DU_findSOPClassAndInstanceInDataSet(dataSet, sopClass, sizeof(sopClass), sopInstance, sizeof(sopInstance), uidPadding))
    {
        DCMQRDB_ERROR("Bad image file: " << fname);
        rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
    } else if (strcmp(sopClass, req->AffectedSOPClassUID) != 0) {
        rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
    } else if (strcmp(sopInstance, req->AffectedSOPInstanceUID) != 0) {
        rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
    }
}

void DcmQueryRetrieveStoreContext::callbackHandler(
    /* in */
    T_DIMSE_StoreProgress *progress,    /* progress state */
    T_DIMSE_C_StoreRQ *req,             /* original store request */
    char *imageFileName,       /* being received into */
    DcmDataset **imageDataSet, /* being received into */
    /* out */
    T_DIMSE_C_StoreRSP *rsp,            /* final store response */
    DcmDataset **stDetail)
{
    updateDisplay(progress);

    if (progress->state == DIMSE_StoreEnd) {

        if (!options_.ignoreStoreData_ && rsp->DimseStatus == STATUS_Success) {
            if ((imageDataSet)&&(*imageDataSet)) {
                checkRequestAgainstDataset(req, NULL, *imageDataSet, rsp, correctUIDPadding);
            } else {
                checkRequestAgainstDataset(req, imageFileName, NULL, rsp, correctUIDPadding);
            }
        }

        if (!options_.ignoreStoreData_ && rsp->DimseStatus == STATUS_Success) {
            if ((imageDataSet)&&(*imageDataSet)) {
                writeToFile(dcmff, fileName, rsp);
            }
            if (rsp->DimseStatus == STATUS_Success) {
                saveImageToDB(req, fileName, rsp, stDetail);
            }
        }

        OFString str;
        if (rsp->DimseStatus != STATUS_Success)
            DCMQRDB_WARN("NOTICE: StoreSCP:" << OFendl << DIMSE_dumpMessage(str, *rsp, DIMSE_OUTGOING));
        else
            DCMQRDB_INFO("Sending:" << OFendl << DIMSE_dumpMessage(str, *rsp, DIMSE_OUTGOING));
        status = rsp->DimseStatus;
    }
}
