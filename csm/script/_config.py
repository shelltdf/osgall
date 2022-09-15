
ENABLE_QT = True
Qt_LIB = "D:/Qt/Qt5.9.0/5.9/msvc2015_64/lib"
Qt5_DIR = Qt_LIB + "/cmake/Qt5"
Qt5Core_DIR = Qt_LIB + "/cmake/Qt5Core"
Qt5Widgets_DIR = Qt_LIB + "/cmake/Qt5Widgets"
Qt5OpenGL_DIR = Qt_LIB + "/cmake/Qt5OpenGL"

ENABLE_FBX = True
FBX_INCLUDE_DIR = "D:/Program Files/Autodesk/FBX/FBX SDK/2016.1.2/include"
FBX_LIBRARY = "D:/Program Files/Autodesk/FBX/FBX SDK/2016.1.2/lib/vs2015/x64/release/libfbxsdk-md.lib"
FBX_LIBRARY_DEBUG = "D:/Program Files/Autodesk/FBX/FBX SDK/2016.1.2/lib/vs2015/x64/debug/libfbxsdk-md.lib"

ENABLE_OPENCV = True
OPENCV_DIR = "D:/opencv/build/x64/vc14/lib"

ENABLE_CGAL = True
CGAL_DIR = "D:/CGAL-4.11.2"

#android ndk setting
ANDROID_NDK_STANDALONE_TOOLCHAIN_PATH = "D:/ndk-toolchain/"    #ndk dir
ANDROID_NDK_PATH = "D:/Android/Sdk/ndk/23.0.7599858/"    #ndk dir
ANDROID_API_LEVEL = 23 #9 15 21 android6-23
ANDROID_ABI = "arm64-v8a" # armeabi-v7a arm64-v8a

ANDROID_LIB_NAME = 'aarch64-linux-android'

GL_VER = 0 # 0-AUTO 1-GL1 2-GL2 3-GL3
GLES_VER = 0 # 0-GL 1-GLES1 2-GLES2 3-GLES3
