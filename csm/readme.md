## 依赖环境

* CMake
* python2 或者 python3
* ninja 可选

## 编译支持

* dynamic / static 
* release / debug 

## 编译架构[尚未支持]

* HOST编译器：msvc mingw cygwin gcc clang emc
* 交叉编译目标：native ndk
* 位宽：32 64
  
## 编译说明

### Visual Studio 2015

参考 2017。

### Visual Studio 2017

1. 启动控制台

2. 输入命令（假设编译zlib，使用all取代zlib可以编译全部库。使用vs2017编译32bit版本）（因为不需要下载了所以取消了source操作）

```
cmake-git.py source zlib
cmake-git.py install zlib -arch vs2017-64 [-release] [-debug] [-dynamic] [-static]
```

3. 等待结束

### Visual Studio 2019

参考 2017。



### Ninja（不打算支持了）

因为ninja在不同系统上的连接库设置不一致，所以暂时不实现这个功能。

make命令是直接支持 -j 多线程的，msbuild也支持多线程命令行。所以貌似没有了必要。



### MinGW

这里分msys 和 msys2 两个情况。



### ubuntu 64bit

这个编译流程跟wsl不能兼容，因为wsl有很多系统设置和换行符号都不一致。

这里测试的是focal版本的gcc 9.4版本。

```
sudo debootstrap focal rootfs
```


```

//必要的工具
sudo apt install git
sudo apt install python-is-python3
sudo apt install cmake
sudo apt install g++

//必要的库
sudo apt install libgl1-mesa-dev
sudo apt install libglu1-mesa-dev
sudo apt install libxi-dev //这个是针对x11丢失头文件的情况
sudo apt install libpthread-stubs0-dev //这个没用

```


```
CMAKE_CODEBLOCKS_MAKE_ARGUMENTS 这个无法决定j的数量，优先级没有make -j 高。

pthreads 在Ubuntu上是有问题的，主要是eigen3的需求。

```



### NDK（windows host）

安装 Android Studio 安装对应的ADK和NDK版本。

直接 cmake toolchain 早就行不通了，所以目前采用 standalone 方式比较简单。目前最新的是NDK 23版本。

```
make_standalone_toolchain.py --arch arm64 --api 28 --install-dir=d:/ndk-toolchain

然后直接在windows控制台里执行 build 命令。

如果不设置就会默认
Android: Targeting API '16' with architecture 'arm', ABI 'armeabi-v7a', and processor 'armv7-a'

```

这里有几个必要属性：

1. API 这里限制API LEVEL也就是安卓版本的最低兼容。
2. ARCH
3. ABI 这个就是arch的子定义，一个arch可以包含多种ABI。
4. 处理器类型





### EMCC（windows host）

下载emsdk 并且初始化安装最新版编译器。然后启动emsdk的控制台。



### XCode



## 依赖关系

``` mermaid
graph LR
	anttweakbar
	boost
    curl --> zlib
    draco
    eigen
    flann
    freeglut
    freetype --> libpng
    gdal --> libpng
    gdal --> libgeotiff
    gdal --> libiconv
    gdal --> curl
    gdal --> libjpeg
    gdal --> proj4
    geos
    glew
    ifcplusplus --> osg
    ifcplusplus --> boost
    leveldb
    libbzip2
    libgeotiff --> libtiff
    libiconv
    libjpeg
    liblas --> boost
    libpng --> zlib
    libtiff --> zlib
    libtiff --> libjpeg
    ogre3d
    ork --> stb
    ork --> tinyxml
    ork --> glew
    ork --> freeglut
    ork --> pthread
    osg --> gdal
    osg --> libpng
    osg --> freetype
    osgearth --> osg
    osgearth --> geos
    osgeffect --> osg
    osgentity --> osg
    osgsplit --> osg
    osgsplit --> boost
    osgsplit --> ifcplusplus
    osgsplit --> liblas
    osgsplit --> osgeffect
    osgsplit --> osgentity
    paraview
    proj4
    proland --> ork
    proland --> anttweakbar
    proland --> libtiff
    pthread
    qhull
    stamen
    stb
    tinyxml
    zlib
    _FBX_SDK
    _Qt
    _SilverLining
    _Triton
    
```

