using System;
using System.Security;
using System.Runtime.InteropServices;

namespace Nvidia.TextureTools
{
	#region Enums

	#region public enum Format
	/// <summary>
	/// Compression format.
	/// </summary>
	public enum Format
	{
		// No compression.
		RGB,
		RGBA = RGB,

		// DX9 formats.
		DXT1,
		DXT1a,
		DXT3,
		DXT5,
		DXT5n,

		// DX10 formats.
		BC1 = DXT1,
		BC1a = DXT1a,
		BC2 = DXT3,
		BC3 = DXT5,
		BC3n = DXT5n,
		BC4,
		BC5,

        DXT1n,   // Not supported.
        CTX1,    // Not supported.

        BC6,
        BC7,
    }
	#endregion

    #region public enum PixelType
    /// <summary>
    /// Pixel types. These basically indicate how the output should be interpreted, but do not have any influence over the input. They are only relevant in RGBA mode.
    /// </summary>
    enum PixelType
    {
        UnsignedNorm = 0,
        SignedNorm = 1,   // Not supported yet.
        UnsignedInt = 2,  // Not supported yet.
        SignedInt = 3,    // Not supported yet.
        Float = 4,
        UnsignedFloat = 5,
    }
    #endregion

    #region public enum Quality
    /// <summary>
	/// Quality modes.
	/// </summary>
	public enum Quality
	{
		Fastest,
		Normal,
		Production,
		Highest,
	}
	#endregion

    #region public enum Decoder
    /// <summary>
    /// DXT decoder.
    /// </summary>
    enum Decoder
    {
        Decoder_D3D10,
        Decoder_D3D9,
        Decoder_NV5x,
    };
    #endregion

    #region public enum WrapMode
    /// <summary>
	/// Wrap modes.
	/// </summary>
	public enum WrapMode
	{
		Clamp,
		Repeat,
		Mirror,
	}
	#endregion

	#region public enum TextureType
	/// <summary>
	/// Texture types.
	/// </summary>
	public enum TextureType
	{
		Texture2D,
		TextureCube,
        Texture3D,
	}
	#endregion

	#region public enum InputFormat
	/// <summary>
	/// Input formats.
	/// </summary>
	public enum InputFormat
	{
        BGRA_8UB,   // Normalized [0, 1] 8 bit fixed point.
        RGBA_16F,   // 16 bit floating point.
        RGBA_32F,   // 32 bit floating point.
	}
	#endregion

	#region public enum MipmapFilter
	/// <summary>
	/// Mipmap downsampling filters.
	/// </summary>
	public enum MipmapFilter
	{
		Box,
		Triangle,
		Kaiser
	}
	#endregion

	#region public enum ResizeFilter
	/// <summary>
    /// Texture resize filters.
	/// </summary>
    public enum ResizeFilter
	{
        Box,
        Triangle,
        Kaiser,
        Mitchell
    }
	#endregion

	#region public enum RoundMode
	/// <summary>
	/// Extents rounding mode.
	/// </summary>
	public enum RoundMode
	{
		None,
		ToNextPowerOfTwo,
		ToNearestPowerOfTwo,
		ToPreviousPowerOfTwo
	}
	#endregion

	#region public enum AlphaMode
	/// <summary>
	/// Alpha mode.
	/// </summary>
	public enum AlphaMode
	{
		None,
		Transparency,
		Premultiplied
	}
	#endregion

    #region public enum CubeLayout
    /// <summary>Cube layout formats.</summary>
    /// (New in NVTT 2.1)
    public enum CubeLayout
    {
        VerticalCross,
        HorizontalCross,
        Column,
        Row,
        LatitudeLongitude
    };
    #endregion

    #region public enum EdgeFixup
    /// <summary>Cube map edge fixup methods.</summary> 
    /// (New in NVTT 2.1)
    public enum EdgeFixup
    {
        None,
        Stretch,
        Warp,
        Average,
    };
    #endregion

    #region public enum Error
    /// <summary>
	/// Error codes.
	/// </summary>
	public enum Error
	{
		InvalidInput,
		UserInterruption,
		UnsupportedFeature,
		CudaError,
		Unknown,
		FileOpen,
		FileWrite,
	}
	#endregion

	#endregion

	#region public class InputOptions
	/// <summary>
	/// Input options.
	/// </summary>
	public class InputOptions
	{
		#region Bindings
		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static IntPtr nvttCreateInputOptions();

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttDestroyInputOptions(IntPtr inputOptions);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsTextureLayout(IntPtr inputOptions, TextureType type, int w, int h, int d);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttResetInputOptionsTextureLayout(IntPtr inputOptions);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static bool nvttSetInputOptionsMipmapData(IntPtr inputOptions, IntPtr data, int w, int h, int d, int face, int mipmap);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsFormat(IntPtr inputOptions, InputFormat format);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsAlphaMode(IntPtr inputOptions, AlphaMode alphaMode);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsGamma(IntPtr inputOptions, float inputGamma, float outputGamma);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsWrapMode(IntPtr inputOptions, WrapMode mode);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsMipmapFilter(IntPtr inputOptions, MipmapFilter filter);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsMipmapGeneration(IntPtr inputOptions, bool generateMipmaps, int maxLevel);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsKaiserParameters(IntPtr inputOptions, float width, float alpha, float stretch);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsNormalMap(IntPtr inputOptions, bool b);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsConvertToNormalMap(IntPtr inputOptions, bool convert);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsHeightEvaluation(IntPtr inputOptions, float redScale, float greenScale, float blueScale, float alphaScale);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsNormalFilter(IntPtr inputOptions, float small, float medium, float big, float large);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsNormalizeMipmaps(IntPtr inputOptions, bool b);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsMaxExtents(IntPtr inputOptions, int d);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetInputOptionsRoundMode(IntPtr inputOptions, RoundMode mode);
		#endregion

		internal IntPtr options;

		public InputOptions()
		{
			options = nvttCreateInputOptions();
		}
		~InputOptions()
		{
			nvttDestroyInputOptions(options);
		}

		public void SetTextureLayout(TextureType type, int w, int h, int d)
		{
			nvttSetInputOptionsTextureLayout(options, type, w, h, d);
		}
		public void ResetTextureLayout()
		{
			nvttResetInputOptionsTextureLayout(options);
		}

		public void SetMipmapData(IntPtr data, int width, int height, int depth, int face, int mipmap)
		{
			nvttSetInputOptionsMipmapData(options, data, width, height, depth, face, mipmap);
		}

		public void SetFormat(InputFormat format)
		{
			nvttSetInputOptionsFormat(options, format);
		}

		public void SetAlphaMode(AlphaMode alphaMode)
		{
			nvttSetInputOptionsAlphaMode(options, alphaMode);
		}

		public void SetGamma(float inputGamma, float outputGamma)
		{
			nvttSetInputOptionsGamma(options, inputGamma, outputGamma);
		}

		public void SetWrapMode(WrapMode wrapMode)
		{
			nvttSetInputOptionsWrapMode(options, wrapMode);
		}

		public void SetMipmapFilter(MipmapFilter filter)
		{
			nvttSetInputOptionsMipmapFilter(options, filter);
		}

		public void SetMipmapGeneration(bool enabled)
		{
			nvttSetInputOptionsMipmapGeneration(options, enabled, -1);
		}

		public void SetMipmapGeneration(bool enabled, int maxLevel)
		{
			nvttSetInputOptionsMipmapGeneration(options, enabled, maxLevel);
		}

		public void SetKaiserParameters(float width, float alpha, float stretch)
		{
			nvttSetInputOptionsKaiserParameters(options, width, alpha, stretch);
		}

		public void SetNormalMap(bool b)
		{
			nvttSetInputOptionsNormalMap(options, b);
		}

		public void SetConvertToNormalMap(bool convert)
		{
			nvttSetInputOptionsConvertToNormalMap(options, convert);
		}

		public void SetHeightEvaluation(float redScale, float greenScale, float blueScale, float alphaScale)
		{
			nvttSetInputOptionsHeightEvaluation(options, redScale, greenScale, blueScale, alphaScale);
		}

		public void SetNormalFilter(float small, float medium, float big, float large)
		{
			nvttSetInputOptionsNormalFilter(options, small, medium, big, large);
		}

		public void SetNormalizeMipmaps(bool b)
		{
			nvttSetInputOptionsNormalizeMipmaps(options, b);
		}

		public void SetMaxExtents(int dim)
		{
			nvttSetInputOptionsMaxExtents(options, dim);
		}

		public void SetRoundMode(RoundMode mode)
		{
			nvttSetInputOptionsRoundMode(options, mode);
		}
	}
	#endregion

	#region public class CompressionOptions
	/// <summary>
	/// Compression options.
	/// </summary>
	public class CompressionOptions
	{
		#region Bindings
		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static IntPtr nvttCreateCompressionOptions();

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttDestroyCompressionOptions(IntPtr compressionOptions);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetCompressionOptionsFormat(IntPtr compressionOptions, Format format);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetCompressionOptionsQuality(IntPtr compressionOptions, Quality quality);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetCompressionOptionsColorWeights(IntPtr compressionOptions, float red, float green, float blue, float alpha);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetCompressionOptionsPixelFormat(IntPtr compressionOptions, uint bitcount, uint rmask, uint gmask, uint bmask, uint amask);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetCompressionOptionsQuantization(IntPtr compressionOptions, bool colorDithering, bool alphaDithering, bool binaryAlpha, int alphaThreshold);
		#endregion

		internal IntPtr options;

		public CompressionOptions()
		{
			options = nvttCreateCompressionOptions();
		}
		~CompressionOptions()
		{
			nvttDestroyCompressionOptions(options);
		}

		public void SetFormat(Format format)
		{
			nvttSetCompressionOptionsFormat(options, format);
		}
		
		public void SetQuality(Quality quality)
		{
			nvttSetCompressionOptionsQuality(options, quality);
		}

		public void SetColorWeights(float red, float green, float blue)
		{
			nvttSetCompressionOptionsColorWeights(options, red, green, blue, 1.0f);
		}

		public void SetColorWeights(float red, float green, float blue, float alpha)
		{
			nvttSetCompressionOptionsColorWeights(options, red, green, blue, alpha);
		}

		public void SetPixelFormat(uint bitcount, uint rmask, uint gmask, uint bmask, uint amask)
		{
			nvttSetCompressionOptionsPixelFormat(options, bitcount, rmask, gmask, bmask, amask);
		}

		public void SetQuantization(bool colorDithering, bool alphaDithering, bool binaryAlpha)
		{
			nvttSetCompressionOptionsQuantization(options, colorDithering, alphaDithering, binaryAlpha, 127);
		}

		public void SetQuantization(bool colorDithering, bool alphaDithering, bool binaryAlpha, int alphaThreshold)
		{
			nvttSetCompressionOptionsQuantization(options, colorDithering, alphaDithering, binaryAlpha, alphaThreshold);
		}
	}
	#endregion

	#region public class OutputOptions
	/// <summary>
	/// Output options.
	/// </summary>
	public class OutputOptions
	{
		#region Delegates
        public delegate void ErrorHandler(Error error);

        //[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        //public delegate void InternalErrorHandlerDelegate(Error error);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BeginImageDelegate(int size, int width, int height, int depth, int face, int miplevel);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate bool WriteDataDelegate(IntPtr data, int size);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void EndImageDelegate();
		#endregion

		#region Bindings
		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static IntPtr nvttCreateOutputOptions();

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttDestroyOutputOptions(IntPtr outputOptions);

		[DllImport("nvtt", CharSet = CharSet.Ansi), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetOutputOptionsFileName(IntPtr outputOptions, string fileName);

		//[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		//private extern static void nvttSetOutputOptionsErrorHandler(IntPtr outputOptions, ErrorHandler errorHandler);

		private void ErrorCallback(Error error)
		{
			if (Error != null) Error(error);
		}

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttSetOutputOptionsOutputHeader(IntPtr outputOptions, bool b);

        [DllImport("nvtt", CallingConvention = CallingConvention.Cdecl), SuppressUnmanagedCodeSecurity]
        private extern static void nvttSetOutputOptionsOutputHandler(IntPtr outputOptions, IntPtr beginImage, IntPtr writeData, IntPtr endImage);

		#endregion

		internal IntPtr options;

		public OutputOptions()
		{
			options = nvttCreateOutputOptions();
			//nvttSetOutputOptionsErrorHandler(options, new ErrorHandler(ErrorCallback));
		}
		~OutputOptions()
		{
			nvttDestroyOutputOptions(options);
		}

		public void SetFileName(string fileName)
		{
			nvttSetOutputOptionsFileName(options, fileName);
		}

		public event ErrorHandler Error;

		public void SetOutputHeader(bool b)
		{
			nvttSetOutputOptionsOutputHeader(options, b);
		}

        public void SetOutputHandler(BeginImageDelegate beginImage, WriteDataDelegate writeImage, EndImageDelegate endImage)
        {
            IntPtr ptrBeginImage = IntPtr.Zero;
            IntPtr ptrWriteData = IntPtr.Zero;
            IntPtr ptrEndImage = IntPtr.Zero;
            
            if (beginImage != null || writeImage != null || endImage != null)
            {
                ptrBeginImage = Marshal.GetFunctionPointerForDelegate(beginImage);
                ptrWriteData = Marshal.GetFunctionPointerForDelegate(writeImage);
                ptrEndImage = Marshal.GetFunctionPointerForDelegate(endImage);
            }

            nvttSetOutputOptionsOutputHandler(options, ptrBeginImage, ptrWriteData, ptrEndImage);
        }
	}
	#endregion

	#region public static class Compressor
	public class Compressor
	{
		#region Bindings
		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static IntPtr nvttCreateCompressor();

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static void nvttDestroyCompressor(IntPtr compressor);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static bool nvttCompress(IntPtr compressor, IntPtr inputOptions, IntPtr compressionOptions, IntPtr outputOptions);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private extern static int nvttEstimateSize(IntPtr compressor, IntPtr inputOptions, IntPtr compressionOptions);

		[DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
		private static extern IntPtr nvttErrorString(Error error);

		#endregion

		internal IntPtr compressor;

		public Compressor()
		{
			compressor = nvttCreateCompressor();
		}

		~Compressor()
		{
			nvttDestroyCompressor(compressor);
		}

		public bool Compress(InputOptions input, CompressionOptions compression, OutputOptions output)
		{
			return nvttCompress(compressor, input.options, compression.options, output.options);
		}

		public int EstimateSize(InputOptions input, CompressionOptions compression)
		{
			return nvttEstimateSize(compressor, input.options, compression.options);
		}

		public static string ErrorString(Error error)
		{
			return Marshal.PtrToStringAnsi(nvttErrorString(error));
		}

	}
	#endregion

    #region public static class Surface
    public class Surface
    {
        #region Bindings
        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static IntPtr nvttCreateSurface();

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static IntPtr nvttCreateSurface(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static void nvttDestroySurface(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static void nvttSurfaceSetWrapMode(IntPtr surface, WrapMode wrapMode);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static WrapMode nvttSurfaceGetWrapMode(IntPtr surface);
        
        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static void nvttSurfaceSetAlphaMode(IntPtr surface, AlphaMode alphaMode);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static AlphaMode nvttSurfaceGetAlphaMode(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static void nvttSurfaceSetNormalMap(IntPtr surface, bool isNormalMap);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static bool nvttSurfaceGetNormalMap(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static int nvttSurfaceGetWidth(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static int nvttSurfaceGetHeight(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static int nvttSurfaceGetDepth(IntPtr surface);
        #endregion

        internal IntPtr surface;

        public Surface()
        {
            surface = nvttCreateSurface();
        }

        public Surface(Surface s)
        {
            surface = nvttCreateSurface(surface);
        }

        ~Surface()
        {
            nvttDestroySurface(surface);
        }

        #region Properties
        public WrapMode WrapMode {
            set { nvttSurfaceSetWrapMode(surface, value); }
            get { return nvttSurfaceGetWrapMode(surface); }
        }

        public AlphaMode AlphaMode {
            set { nvttSurfaceSetAlphaMode(surface, value); }
            get { return nvttSurfaceGetAlphaMode(surface); }
        }

        public bool IsNormalMap
        {
            set { nvttSurfaceSetNormalMap(surface, value); }
            get { return nvttSurfaceGetNormalMap(surface); }
        }

        //public bool isNull();

        public int Width
        {
            get { return nvttSurfaceGetWidth(surface); }
        }

        public int Height
        {
            get { return nvttSurfaceGetHeight(surface); }
        }

        public int Depth
        {
            get { return nvttSurfaceGetDepth(surface); }
        }
        #endregion

        //public TextureType type();
        //public int countMipmaps();
        //public int countMipmaps(int min_size);
        //public float alphaTestCoverage(float alphaRef = 0.5);
        //public float average(int channel, int alpha_channel = -1, float gamma = 2.2f);
        //public IntPtr data();
        //public IntPtr channel(int i);
        //public void histogram(int channel, float rangeMin, float rangeMax, int binCount, int binPtr[]);
        //public void range(int channel, out float rangeMin, out float rangeMax, int alpha_channel = -1, float alpha_ref = 0.f);

        // @@ TODO
    }
    #endregion

    #region public static class CubeSurface
    public class CubeSurface
    {
        #region Bindings
        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static IntPtr nvttCreateCubeSurface();

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static IntPtr nvttCreateCubeSurface(IntPtr surface);

        [DllImport("nvtt"), SuppressUnmanagedCodeSecurity]
        private extern static void nvttDestroyCubeSurface(IntPtr surface);
        #endregion

        internal IntPtr cubeSurface;

        public CubeSurface()
        {
            cubeSurface = nvttCreateCubeSurface();
        }

        public CubeSurface(CubeSurface cs)
        {
            cubeSurface = nvttCreateCubeSurface(cs.cubeSurface);
        }

        ~CubeSurface()
        {
            nvttDestroyCubeSurface(cubeSurface);
        }


        //public bool IsNull();         // @@ Use properties?
        //public int EdgeLength();      // @@ Use properties?
        //public int CountMipmaps();    // @@ Use properties?

        //public bool Load(String fileName, int mipmap);
        //public bool Save(String fileName) const;

        //public Surface & Face(int face);
        //public const Surface & Face(int face) const;


        // @@ TODO
    }
    #endregion

} // Nvidia.TextureTools namespace
