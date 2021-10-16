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



### Ninjia

因为ninja在不同系统上的连接库设置不一致，所以暂时不实现这个功能。



### ubuntu 64bit

这个编译流程跟wsl不能兼容，因为wsl有很多系统设置和换行符号都不一致。



### NDK（windows host）

安装 Android Studio 安装对应的ADK和NDK版本。





### EMCC（windows host）

下载emsdk 并且初始化安装最新版编译器。然后启动emsdk的控制台。





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

