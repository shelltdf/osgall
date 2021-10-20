# android ndk compile



# 直接设置cmake toolchain 是不行的



# 使用make_standalone_toolchain

```
make_standalone_toolchain.py -h

make_standalone_toolchain.py --arch arm64 --api 28 --install-dir=d:/ndk-toolchain

如果不设置就会默认
Android: Targeting API '16' with architecture 'arm', ABI 'armeabi-v7a', and processor 'armv7-a'
```

这里有几个必要属性：

1. API 这里限制API LEVEL也就是安卓版本的最低兼容。
2. ARCH
3. ABI 这个就是arch的子定义，一个arch可以包含多种ABI。
4. 处理器类型

## 使用nmake

因为默认肯定会安装vs，所以直接使用nmake比较简答，而且可以避免安装mingw cygwin这些环境。



## 设置CMake宏 新版本

```
ANDROID_API_LEVEL = 23 #9 15 21 android6-23
ANDROID_ABI = "arm64-v8a" # armeabi-v7a arm64-v8a

这要设置这两个就可以了

ANDROID_PLATFORM 是 ANDROID_NATIVE_API_LEVEL 的别名。程序无法在低于这个设置的系统上运行，所以这个API尽量低。

ANDROID_STL 也需要设置静态动态。

```



## 设置CMake宏 老版本



# CMake里的标记冲突

在使用windows控制台生成nmake的编译流程中，ANDROID有效的同时MINGW也是有效的，这个就很奇怪了。

这回影响到一些判断的正确性。



# 修改CMake脚本以后必须删除CMakeCache文件

不然修改不会起效。



# release默认编译是rd的版本

必须强制设置 CMAKE_BUILD_TYPE



# 具体库问题

## Generating proj.db

这个需要本地的 sqlite 来产生。



## CURL第一次编译不通过

第二次就好了。。。。。。



## gdal 错误

```
GUIntBig 
少了 #define HAVE_LONG_LONG 1 定义

#define HAVE_UINTPTR_T 1 解决一个cast问题

#define HAVE_DLFCN_H 动态库问题

SIZEOF_VOIDP 让cmake产生定义

# define SIZEOF_VOIDP 8 强制64 这里没法判断32/64所以先写死
CMAKE_CL_64 能都跨平台适应

ld: error: undefined symbol: iconv_open
这个连接错误。
libiconv libiconv_close libiconv_open

if(ANDROID)
  add_definitions(-DLIBICONV_PLUG)
endif()

```



## liblas 

因为这个需要boost ，但是boost目前没法编译，所以就跳过了。



## giflib

```
D:\_work_bt\_osgall_github\prebuild\giflib-5.1.4\lib\egif_lib.c:64:6: error: use of undeclared identifier 'S_IREAD'
                          S_IREAD | S_IWRITE);

因为 stat.h 里的定义

if(ANDROID)
add_definitions(-D__USE_GNU)
endif(ANDROID)

```



## ilmbase openexr

这里有一些需要运行本地程序的配置，所以暂时跳过。



## NVTT

跳过



## vpb不需要

跳过



## osg

总是体制 GIFLIB 无法install。

根本不清楚哪里设置了个过程，非常奇怪。

是 ANDROID_3RD_PARTY() 的问题，直接注释掉就行了。我们不用这个方法。



## libxml2

```
ld: error: duplicate symbol: xmlLastError

```

升级最新版就好了。



# 排除系统自带的库

```

sqlite 这个目前无法确定
zlib 这个的确存在与默认环境中

pthreads ndk是否默认就支持

hdf 这个是因为编译错误

glew freeglut 这两个没有必要再安卓下使用

crunch 编译错误

crashrpt 这东西貌似根本不支持安卓

```

