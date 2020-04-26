支持环境版本
    [最后一个编译版本在最上面]
    cmake ndk-10e
    standalone ndk-10e
    standalone vs 2013 64bit
    cmake vs 2013 64bit
    mingw w64
    vs 2015 32bit
    
已知的版本问题
    在vs1015 64bit上编译错误
        libxml
        leveldb
        osgbullet
        
编译顺序
    prebuild_ffmpeg (在 prebuild_osg 之前)
    prebuild_opencv (依赖ffmpeg)
    prebuild_boost
    prebuild
    prebuild_osg
    prebuild_osgbullet
    prebuild_osgearth
    prebuild_vtk
    prebuild_pcl
    
依赖库预编译[cmake]

    windows-vc:
        启动vcvarsall.bat
        修改common.py中的cmake路径
        运行python.exe prebuild.py vc
        (文件自动安装到3rdparty目录)
        
    windows-mingw:
        启动msys.bat(需要安装win32版本的 cmake python2)
        修改common.py中的cmake路径
        运行python.exe prebuild.py mingw
        (文件自动安装到3rdparty目录)
        
    windows-ndk:
        (安装NDK到任意目录)
        启动msys
        设置环境变量 export ANDROID_NDK=/d/android-ndk-r10d
        修改common.py中的cmake路径
        运行prebuild.py ndk
        (生成的文件存放在ABI名称对应的目录中，但是因为标准库并不支持obj/loacl/ABI路径所以需要手工调整。)

        
依赖库预编译[非cmake]

    boost(无法cmake编译)
        vc版本：
            修改common.py中的配置信息
            打开vc控制台 运行prebuild_boost.py vc
        
    ffmpeg+x264(无法cmake编译)
    
        mingw-dll:
            (widnows下只能用mingw编译dll方式 手工补充h和dll依赖)
            (注意wrt版本 目前321版本是错误的 只能使用320-2)
            (编译64bit版本 可安装mingw w64 然后修改fstab文件内的 /mingw挂载点  路径必须是/无空格)
            编译安装yasm(32 64根据环境自动识别)
                ./configure
                make install
            编译安装x264 static版本 不知道为什么只要不是/usr/local/lib路径就提示找不到libx264
                （32bit）
                ./configure --enable-static --enable-shared
				make install
                （64bit）
                ./configure --prefix=/mingw --enable-win32thread --host=x86_64-w64-mingw32 --enable-static --enable-shared 
				make install
			python prebuild_ffmpeg vc

        其他gcc编译器
            直接静态编译安装（全自动）
            
        android版本
            mingw交叉编译（不确定是否必须是mingw32）
            （因为只能使用ndk的toolchian编译，所以多个arch版本无法同时保留配置）
            修改一下 common.py setup_ndk_standalone.sh 脚本中的 ndk路径 api版本 arch 编译器版本
            运行一下命令行:
                python make_ndk_standalone.py      在更换工具链的时候才需要重新执行(需要系统权限 因为tmp在c盘 有可能需要执行两次 因为目录存在关系的问题)
                source setup_ndk_standalone.sh     设定编译需要的环境变量
                python prebuild_ffmpeg.py ndk      执行编译  内涵x264编译

            ndk-10e 之后的版本都无法通过
            
项目编译
    因为针对安卓发布的程序只能有一个入口 所以项目是按发布程序拆分的
    build.py vc test_cube(项目名称)
    
    
codework的cmake基本编译方式
    1 启动vs命令行
    2 set ANDROID_NDK=d:\NVPACK\android-ndk-r10e
    3 "C:\Program Files (x86)\CMake\bin\cmake.exe" -DCMAKE_TOOLCHAIN_FILE=..\android.toolchain.cmake -G "Visual Studio 11 2012" ..

    