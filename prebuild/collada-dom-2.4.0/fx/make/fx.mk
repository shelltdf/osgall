include make/common.mk

src := $(wildcard src/*.cpp)

includeOpts := -Iinclude -Iinclude/$(colladaVersion)

ifneq ($(findstring $(os),linux mac),)
includeOpts += -I../dom/include
includeOpts += -I../dom/include/dae
includeOpts += -I../dom/include/1.4
ccFlags += -fPIC
ccFlags += -DDEFAULT_CG_PLATFORM=\"PC-OGL\" 
else 
ifeq ($(os),windows)
ccFlags += -DDOM_DYNAMIC -DDOM_EXPORT
endif
endif

ifneq ($(findstring libxml,$(xmlparsers)),)
ccFlags += -DDOM_INCLUDE_LIBXML
ifeq ($(os),windows)
includeOpts += -Iexternal-libs/libxml2/include
libOpts += -Lexternal-libs/libxml2/$(buildID)/lib -lxml2 -lws2_32 -lz
else
includeOpts += -I/usr/include/libxml2
libOpts += -lxml2
endif
endif

ifneq ($(findstring tinyxml,$(xmlparsers)),)
ccFlags += -DDOM_INCLUDE_TINYXML
includeOpts += -Iexternal-libs/tinyxml/
libOpts += external-libs/tinyxml/lib/$(buildID)/libtinyxml.a
endif

ifeq ($(os),linux)
libOpts += -lpcre -lpcrecpp
ccFlags += -DCFX_PLATFORM_INCLUDE=\"cfxLinux.h\" -DLINUX
else 
# On Mac, Windows and PS3 we need to be told where to find pcre
ifeq ($(os),windows)
ccFlags += -DPCRE_STATIC
endif
includeOpts += -Iexternal-libs/pcre
libOpts += $(addprefix external-libs/pcre/lib/$(buildID)/,libpcrecpp.a libpcre.a )
endif

libName := libcollada$(colladaVersionNoDots)fx$(debugSuffix)
libVersion := $(domVersion)
libVersionNoDots := $(subst .,,$(libVersion))

targets :=
ifeq ($(os),linux)
# On Linux we build a static lib and a shared lib
targets += $(addprefix $(outPath),$(libName).a)
targets += $(addprefix $(outPath),$(libName).so)

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
includeOpts += -I../dom/include
includeOpts += -I../dom/include/dae
includeOpts += -I../dom/include/1.4
includeOpts += -I../include
ccFlags += -DCFX_PLATFORM_INCLUDE=\"cfxPS3.h\"
ccFlags += -DDEFAULT_CG_PLATFORM=\"PC-OGL\" 
ccFlags += -DNO_BOOST -DNO_ZAE
targets += $(addprefix $(outPath),$(libName).a)
endif
endif
endif
endif

include make/rules.mk
