include make/common.mk


ifneq ($(findstring $(os),linux mac),)
    ccFlags += -fPIC
else
    ifeq ($(os),windows)
        ccFlags += -DDOM_DYNAMIC -DDOM_EXPORT
    endif
endif

ifneq ($(findstring libxml,$(xmlparsers)),)
    ccFlags += -DDOM_INCLUDE_LIBXML
    includeOpts += -I/usr/include/libxml2
    libOpts += -lxml2

    ifeq ($(os),windows)
        includeOpts += -I../dom/external-libs/libxml2/include
        libOpts += -L../dom/external-libs/libxml2/$(buildID)/lib -lxml2 -lws2_32 -lz
    else
        includeOpts += -I/usr/include/libxml2
        libOpts += -lxml2
    endif
endif

ifneq ($(findstring tinyxml,$(xmlparsers)),)
    ccFlags += -DDOM_INCLUDE_TINYXML
    includeOpts += -I../dom/external-libs/tinyxml/
    libOpts += -L../dom/external-libs/tinyxml/lib/$(buildID)/libtinyxml.a
endif

ifeq ($(os),linux)
    libOpts += -lpcre -lpcrecpp
else 
    # On Mac, Windows and PS3 we need to be told where to find pcre
    ifeq ($(os),windows)
        ccFlags += -DPCRE_STATIC
    endif
    includeOpts += -I../dom/external-libs/pcre
    libOpts += $(addprefix ../dom/external-libs/pcre/lib/$(buildID)/,libpcrecpp.a libpcre.a )
endif

libName := viewer
libVersion := $(domVersion)
libVersionNoDots := $(subst .,,$(libVersion))

targets :=
ifeq ($(os),linux)
    # On Linux we build a static lib and a shared lib
    targets := $(outPath)viewer$(exeSuffix)
    src += projects/linux/main.cpp

    includeOpts += -Iexternal-libs/minizip/include

    includeOpts += -I../dom/include
    includeOpts += -I../dom/include/dae
    includeOpts += -I../dom/include/$(colladaVersion)
    includeOpts += -I../fx/include/
    includeOpts += -I../rt/include
    includeOpts += -I../rt/external-libs/bullet/include -I../

    ccFlags += -DCFX_PLATFORM_INCLUDE=\"cfxLinux.h\"
    ccFlags += -DCRT_PLATFORM_INCLUDE=\"CrtLinux.h\"

    libOpts += -lboost_filesystem
    libOpts += -L../dom/build/linux-$(colladaVersion)$(debugSuffix)/
    libOpts += -lcollada$(colladaVersionNoDots)dom$(debugSuffix)

    libOpts += -lminizip$(debugSuffix)
    # as we link minizip static on osx, we need to link against zlib, too.
    libOpts += -lz

    libOpts += -L../rt/build/linux-$(colladaVersion)$(debugSuffix)
    libOpts += -lcollada$(colladaVersionNoDots)rt$(debugSuffix)

    libOpts += -L../fx/build/linux-$(colladaVersion)$(debugSuffix)
    libOpts += -lcollada$(colladaVersionNoDots)fx$(debugSuffix)

    libOpts += -L../rt/external-libs/bullet/lib/$(os)
    libOpts += -lbulletopenglsupport -lbulletdynamics -lbulletcollision -lbulletmath

    libOpts += -I/usr/include/GL -I/usr/include/Cg
    libOpts += -lGL -lGLU -lglut -lCg -lCgGL
    libOpts += -lpthread

    sharedLibSearchPaths += $(abspath ../dom/build/linux-$(colladaVersion)$(debugSuffix))
    sharedLibSearchPaths += $(abspath ../rt/build/linux-$(colladaVersion)$(debugSuffix))
    sharedLibSearchPaths += $(abspath ../fx/build/linux-$(colladaVersion)$(debugSuffix))
else
ifeq ($(os),windows)
            # On Windows we build a static lib and a DLL
            windowsLibName := libcollada$(colladaVersionNoDots)dom
            targets += $(addprefix $(outPath),$(windowsLibName)$(debugSuffix).a)
            targets += $(addprefix $(outPath),$(windowsLibName)$(libVersionNoDots)$(debugSuffix).dll)

else
ifeq ($(os),mac)
                # On Mac we build a framework
                targets += $(addprefix $(outPath),Collada$(colladaVersionNoDots)Dom$(debugSuffix).framework)
                frameworkHeadersPath = $(framework)/Versions/$(libVersion)/Headers
                copyFrameworkHeadersCommand = cp -R include/* $(frameworkHeadersPath) && \
                mv $(frameworkHeadersPath)/$(colladaVersion)/dom $(frameworkHeadersPath)/dom && \
                find -E $(frameworkHeadersPath) -maxdepth 1 -type d -regex '.*[0-9]+\.[0-9]+' | xargs rm -r
                frameworkResourcesPath = $(framework)/Versions/$(libVersion)/Resources
                sedReplaceExpression := -e 's/(colladaVersionNoDots)/$(colladaVersionNoDots)/g' \
                        -e 's/(domVersion)/$(domVersion)/g' \
                        -e 's/(debugSuffix)/$(debugSuffix)/g'
                copyFrameworkResourcesCommand = cp -R make/macFrameworkResources/* $(frameworkResourcesPath) && \
                sed $(sedReplaceExpression) make/macFrameworkResources/Info.plist > $(frameworkResourcesPath)/Info.plist && \
                sed $(sedReplaceExpression) make/macFrameworkResources/English.lproj/InfoPlist.strings > $(frameworkResourcesPath)/English.lproj/InfoPlist.strings

else
ifeq ($(os),ps3)
                # On PS3 we build a static lib, since PS3 doesn't support shared libs
                src += projects/vc8-PS3/main.cpp projects/vc8-PS3/BrowserUI.cpp

                # If we use windows as host, we use SCE_PS3_ROOT, if we use linux as host, we use CELL_SDK
                ifeq ($(host), linux)
                    PS3_SDK_ROOT := $(CELL_SDK)
                else
                    ifeq ($(host), windows)
                        PS3_SDK_ROOT := $(SCE_PS3_ROOT)
                    endif
                endif

                includeOpts += -I../dom/include
                includeOpts += -I../dom/include/dae
                includeOpts += -I../dom/include/$(colladaVersion)
                includeOpts += -I../fx/include/
                includeOpts += -I../rt/include
                includeOpts += -I../rt/external-libs/bullet/include
				includeOpts += -I$(PS3_SDK_ROOT)/samples/fw/include -I$(PS3_SDK_ROOT)/samples/fw/include/psgl -I$(PS3_SDK_ROOT)/target/common/include -I$(PS3_SDK_ROOT)/target/ppu/include
				
				includeOpts += -Iprojects/vc8-PS3

                libOpts += -L../rt/build/ps3-1.4$(debugSuffix)
                libOpts +=  -lcollada$(colladaVersionNoDots)rt$(debugSuffix)

                libOpts += -L../fx/build/ps3-1.4$(debugSuffix)
                libOpts +=  -lcollada$(colladaVersionNoDots)fx$(debugSuffix)

                libOpts += -L../dom/build/ps3-1.4$(debugSuffix)
                libOpts += -lcollada$(colladaVersionNoDots)dom$(debugSuffix)

                includeOpts += -I../dom/external-libs/pcre
                libOpts += $(addprefix ../dom/external-libs/pcre/lib/$(buildID)/,libpcrecpp.a libpcre.a )

                libOpts += -L../dom/external-libs/tinyxml/lib/$(os)
                libOpts += -ltinyxml

                libOpts += -L$(PS3_SDK_ROOT)/samples/fw/
                libOpts += -L$(PS3_SDK_ROOT)/target/ppu/lib/PSGL/RSX/debug/
                libOpts += -L$(PS3_SDK_ROOT)/target/ppu/lib/

                libOpts += -L$(PS3_SDK_ROOT)/samples/fw
                libOpts += -lfw -lPSGL -lPSGLU -lPSGLFX -lPSGLcgc
                libOpts += -lm -lio_stub -lcgc -lgcm_cmd -lgcm_sys_stub -lsysmodule_stub -lresc_stub -lusbd_stub -lfs_stub -lsysutil_stub -ldbgfont

                ccFlags += -DSN_TARGET_PS3 -D__GCC__ -DPSGL -D__CELLOS_LV2__ -DNO_BULLET -DTINYXML -DBOOST_NO_STD_LOCALE -DNO_BOOST -DNO_ZAE
                ccFlags += -DCFX_PLATFORM_INCLUDE=\"cfxPS3.h\"
                ccFlags += -DCRT_PLATFORM_INCLUDE=\"CrtPS3.h\"

                outPath := ./bin/
                targets := $(outPath)viewer$(exeSuffix)

endif
endif
endif
endif

ifeq ($(os),ps3)
    # On PS3 we need to make a .self from the .elf
    postCreateExeCommand := make_fself $(targets) $(targets:.elf=.self)
endif

include make/rules.mk
