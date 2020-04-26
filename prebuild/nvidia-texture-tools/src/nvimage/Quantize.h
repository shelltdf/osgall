// This code is in the public domain -- castanyo@yahoo.es

#ifndef NV_IMAGE_QUANTIZE_H
#define NV_IMAGE_QUANTIZE_H

#include <nvimage/nvimage.h>


namespace nv
{
	class Image;

	namespace Quantize
	{
		NVIMAGE_API void RGB16(Image * img);
		NVIMAGE_API void BinaryAlpha(Image * img, int alpha_threshold = 127);
		NVIMAGE_API void Alpha4(Image * img);
		
		NVIMAGE_API void FloydSteinberg_RGB16(Image * img);
		NVIMAGE_API void FloydSteinberg_BinaryAlpha(Image * img, int alpha_threshold = 127);
		NVIMAGE_API void FloydSteinberg_Alpha4(Image * img);

		NVIMAGE_API void Truncate(Image * image, uint rsize, uint gsize, uint bsize, uint asize);
		NVIMAGE_API void FloydSteinberg(Image * image, uint rsize, uint gsize, uint bsize, uint asize);

		// @@ Add palette quantization algorithms!
	}
}


#endif // NV_IMAGE_QUANTIZE_H
