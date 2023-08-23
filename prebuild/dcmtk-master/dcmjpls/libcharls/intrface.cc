// 
// (C) Jan de Vaan 2007-2010, all rights reserved. See the accompanying "License.txt" for licensed use. 
// 


#include "config.h"
#include "util.h"
#include "intrface.h"
#include "header.h"


JLS_ERROR CheckInput(const void* compressedData, size_t compressedLength, const void* uncompressedData, size_t uncompressedLength, const JlsParameters* pparams)
{
	if (pparams == NULL)
		return InvalidJlsParameters;

	if (compressedLength == 0)
		return InvalidJlsParameters;

	if (compressedData == NULL)
		return InvalidJlsParameters;

	if (uncompressedData == NULL)
		return InvalidJlsParameters;

	if (pparams->width < 1 || pparams->width > 65535)
		return ParameterValueNotSupported;

	if (pparams->height < 1 || pparams->height > 65535)
		return ParameterValueNotSupported;

	int bytesperline = pparams->bytesperline < 0 ? -pparams->bytesperline : pparams->bytesperline;

	if (uncompressedLength < size_t(bytesperline * pparams->height))
		return InvalidJlsParameters;

	return CheckParameterCoherent(pparams);
}

JLS_ERROR CheckInput(const void* uncompressedData, size_t uncompressedLength, const JlsParameters* pparams)
{
	if (pparams == NULL)
		return InvalidJlsParameters;

	if (uncompressedData == NULL)
		return InvalidJlsParameters;

	if (pparams->width < 1 || pparams->width > 65535)
		return ParameterValueNotSupported;

	if (pparams->height < 1 || pparams->height > 65535)
		return ParameterValueNotSupported;

	int bytesperline = pparams->bytesperline < 0 ? -pparams->bytesperline : pparams->bytesperline;

	if (uncompressedLength < size_t(bytesperline * pparams->height))
		return InvalidJlsParameters;

	return CheckParameterCoherent(pparams);
}



extern "C"
{

CHARLS_IMEXPORT(JLS_ERROR) JpegLsEncode(BYTE **buf, size_t *buf_size, size_t* pcbyteWritten, const void* uncompressedData, size_t uncompressedLength, struct JlsParameters* pparams)
{
	*pcbyteWritten = 0;

	JlsParameters info = *pparams;
	if(info.bytesperline == 0)
	{
		info.bytesperline = info.width * ((info.bitspersample + 7)/8);
		if (info.ilv != ILV_NONE)
		{
			info.bytesperline *= info.components;
		}
	}
	
	JLS_ERROR parameterError = CheckInput(uncompressedData, uncompressedLength, &info);

	if (parameterError != OK)
		return parameterError;

	if (pcbyteWritten == NULL)
		return InvalidJlsParameters;

	Size size = Size(info.width, info.height);
	JLSOutputStream stream;
	
	stream.Init(size, info.bitspersample, info.components);
	
	if (info.colorTransform != 0)
	{
		stream.AddColorTransform(info.colorTransform);
	}

	if (info.ilv == ILV_NONE)
	{
		LONG cbyteComp = size.cx*size.cy*((info.bitspersample +7)/8);
		for (LONG component = 0; component < info.components; ++component)
		{
			const BYTE* compareData = static_cast<const BYTE*>(uncompressedData) + component*cbyteComp;
			stream.AddScan(compareData, &info);
		}
	}
	else 
	{
		stream.AddScan(uncompressedData, &info);
	}

	try
	{
		stream.Write(buf, buf_size, 0);
	}
	catch (const alloc_fail&)
	{
		return MemoryAllocationFailure;
	}

	*pcbyteWritten = stream.GetBytesWritten();	
	return OK;
}

CHARLS_IMEXPORT(JLS_ERROR) JpegLsDecode(void* uncompressedData, size_t uncompressedLength, const void* compressedData, size_t compressedLength, JlsParameters* info)
{
	JLSInputStream reader((BYTE*)compressedData, compressedLength);

	if(info != NULL)
	{
	 	reader.SetInfo(info);
	}

	try
	{
		reader.Read(uncompressedData, uncompressedLength);
		return OK;
	}
	catch (JlsException& e)
	{
		return e._error;
	}
}


CHARLS_IMEXPORT(JLS_ERROR) JpegLsDecodeRect(void* uncompressedData, size_t uncompressedLength, const void* compressedData, size_t compressedLength, JlsRect roi, JlsParameters* info)
{
	JLSInputStream reader((BYTE*)compressedData, compressedLength);

	if(info != NULL)
	{
	 	reader.SetInfo(info);
	}

	reader.SetRect(roi);

	try
	{
		reader.Read(uncompressedData, uncompressedLength);
		return OK;
	}
	catch (JlsException& e)
	{
		return e._error;
	}
}


CHARLS_IMEXPORT(JLS_ERROR) JpegLsVerifyEncode(const void* uncompressedData, size_t uncompressedLength, const void* compressedData, size_t compressedLength)
{
	JlsParameters info = JlsParameters();

	JLS_ERROR error = JpegLsReadHeader(compressedData, compressedLength, &info);
	if (error != OK)
		return error;

	error = CheckInput(compressedData, compressedLength, uncompressedData, uncompressedLength, &info);

	if (error != OK)
		return error;
	
	Size size = Size(info.width, info.height);
	
	JLSOutputStream stream;
	
	stream.Init(size, info.bitspersample, info.components);

	if (info.ilv == ILV_NONE)
	{
		LONG cbyteComp = size.cx*size.cy*((info.bitspersample +7)/8);
		for (LONG component = 0; component < info.components; ++component)
		{
			const BYTE* compareData = static_cast<const BYTE*>(uncompressedData) + component*cbyteComp;
			stream.AddScan(compareData, &info);
		}
	}
	else 
	{
		stream.AddScan(uncompressedData, &info);
	}
	
	size_t buf_size = compressedLength + 16;
	BYTE *buf = new BYTE[buf_size];

	memcpy(buf, compressedData, compressedLength);

	stream.EnableCompare(true);

	try
	{
		stream.Write(&buf, &buf_size, 0);
	}
	catch (const alloc_fail&)
	{
		return MemoryAllocationFailure;
	}

	return OK;
}

 
CHARLS_IMEXPORT(JLS_ERROR) JpegLsReadHeader(const void* compressedData, size_t compressedLength, JlsParameters* pparams)
{
	try
	{
		JLSInputStream reader((BYTE*)compressedData, compressedLength);
		reader.ReadHeader();	
		JlsParameters info = reader.GetMetadata();
		*pparams = info;	
		return OK;
	}
	catch (JlsException& e)
	{
		return e._error;
	}

}

}
