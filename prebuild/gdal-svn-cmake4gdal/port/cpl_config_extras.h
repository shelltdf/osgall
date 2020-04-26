
#ifndef INCLUDED_CPL_CONFIG_EXTRAS
#define INCLUDED_CPL_CONFIG_EXTRAS

#if defined(__APPLE__)

#ifdef __BIG_ENDIAN__
  #define HOST_FILLORDER FILLORDER_MSB2LSB
#else
  #define HOST_FILLORDER FILLORDER_LSB2MSB
#endif


#ifdef __LP64__
  #define SIZEOF_UNSIGNED_LONG 8
#else
  #define SIZEOF_UNSIGNED_LONG 4
#endif

#ifdef __LP64__
  #define SIZEOF_VOIDP 8
#else
  #define SIZEOF_VOIDP 4
#endif

#ifdef __BIG_ENDIAN__
  #define WORDS_BIGENDIAN 1
#else
  #undef WORDS_BIGENDIAN
#endif

#undef VSI_STAT64
#undef VSI_STAT64_T

#define VSI_STAT64 stat
#define VSI_STAT64_T stat

#endif // APPLE


#if ANDROID 
//#include <stdarg.h>
//#include <cstdarg>
#include <machine\_types.h>
typedef __va_list va_list;

//typedef __builtin_va_list va_list;
//#define va_start(v,l)   __builtin_va_start(v,l)
//#define va_end(v)   __builtin_va_end(v)
//#define va_arg(v,l) __builtin_va_arg(v,l)
//#if !defined(__STRICT_ANSI__) || __STDC_VERSION__ + 0 >= 199900L || defined(__GXX_EXPERIMENTAL_CXX0X__)
//#define va_copy(d,s)    __builtin_va_copy(d,s)
//#endif
//#define __va_copy(d,s)  __builtin_va_copy(d,s)
//typedef __builtin_va_list __gnuc_va_list;
//typedef __gnuc_va_list va_list;
//typedef va_list __va_list;
#endif


#endif //INCLUDED_CPL_CONFIG_EXTRAS
