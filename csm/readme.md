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



### MinGW（不打算支持了）

这里分msys 和 msys2 两个情况。

但是MinGW不管什么版本，跟windows代码级别不兼容，跟linux代码级别也不兼容，真的没有支持的必要了。



### ubuntu 64bit（其他linux不再赘述）

这个编译流程跟wsl不能兼容，因为wsl有很多系统设置和换行符号都不一致。

这里测试的是focal版本的gcc 9.4版本。

```

// 创建虚拟文件目录
sudo debootstrap focal rootfs

// 进入虚拟文件目录
chroot rootfs

// 必要的工具
sudo apt install git
sudo apt install python-is-python3
sudo apt install cmake
sudo apt install g++

// 必要的库
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

限制

​		android的应用更适合使用静态库的方式，所以我们这里优先保证的是静态库可用。如果动态库编译不过去，不要慌张，可以尝试静态库方式。有些库在移动设备上没有用途的，所以也会自动跳过。

​		这里默认是以 arm64 api28 ndk23 作为范例的。用户可以自行修改这些配置。

第一步：

​		安装 Android Studio 安装对应的ADK和NDK版本。直接 cmake toolchain 早就行不通了，所以目前采用 standalone 方式比较简单。

​		确认sdk版本是28（android 9），ndk版本是23（23.0.7599858）。因为这个版本目前的设备都能兼容，而且也不是太老。做测试比较合适。

第二步：

​		安装 ninja 添加到PATH里。确认控制台可以直接运行 ninja 这个命令。

第三步：

​		安装python 2 或者 3，添加到PATH里。确认控制台里支持python这个命令。

​		安装cmake最新版，添加到PATH里。确认控制台可以直接运行cmake这个命令。

第四步：

​		按照以下步骤执行。结果会安装到c盘，所以需要c盘有足够空间。目测10G以上就够了。

```
# 如果没有设置NDK环境变量
# 一般用户都会安装多个NDK版本，所以这里最好是手工选择一个版本。
# 在CMD控制里执行 D:\Android\Sdk\ndk\23.0.7599858\build\tools 中运行
python make_standalone_toolchain.py --arch arm64 --api 28 --install-dir=d:/ndk-toolchain

# 如果你设置了 NDK 这个环境变量也可以改成对应的路径形式。
python %NDK%\build\tools\make_standalone_toolchain.py --arch arm64 --api 28 --install-dir=d:/ndk-toolchain

# 如果设置不正确就会跳转到会默认设置，也就会提示这个信息。
Android: Targeting API '16' with architecture 'arm', ABI 'armeabi-v7a', and processor 'armv7-a'
# 这个 Targeting API 指的是 NDK 的版本。不是SDK的版本。

# 等待执行结束，确认 ndk-toolchain 目录已经生成。
d:/ndk-toolchain

# 然后直接在控制台里执行。
cd csm
build_windows_ndk.bat

# 生成的数据会安装到 C:\dev 目录下。
# 这个是强制的目录，不建议修改。


```

这里有几个必要属性：

1. API 这里限制 API LEVEL 也就是安卓版本的最低兼容。
2. ARCH
3. ABI 这个就是arch的子定义，一个arch可以包含多种ABI。
4. 处理器类型



#### 已知问题

1. 编译curl的时候第一次肯定会失败，重复执行一次就可以成功了。这个问题是cmake本地测试造成的问题，不能禁止本地测试，这样会导致很多参数设置错误，也没法修复这个bug。只能手工重复一次。
2. 目前nmake无法支持多核编译，所以暂时速度慢。不影响使用，以后再解决。问题解决了，直接用ninja替换了nmake。



### EMCC（windows host）

下载emsdk 并且初始化安装最新版编译器。然后启动emsdk的控制台。(emcmdprompt.bat)

```
./emsdk install latest
./emsdk activate latest
```

因为版本差异较大，所以只能针对一个版本确认。当前版本 3.1.14



### XCode

未测试



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

