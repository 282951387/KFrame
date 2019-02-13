@echo off

SETLOCAL ENABLEDELAYEDEXPANSION

rem ===========================================================================
set releasepath=..\9.Template\1.Server
set localpath=.

rd /Q /S %releasepath%
if not exist %releasepath% (mkdir %releasepath% )
rem ===========================================================================

rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release _bin path begin"
set binpath=_bin
if not exist %releasepath%\%binpath% (mkdir %releasepath%\%binpath% )

rem _gcm=================================
echo "release gcm begin"
set gcmpath=_bin\_gcm\builder
if not exist %releasepath%\%gcmpath% (mkdir %releasepath%\%gcmpath% )
copy /y %localpath%\%gcmpath%\gcm_build.exe %releasepath%\%gcmpath%\
copy /y %localpath%\%gcmpath%\gcm_build %releasepath%\%gcmpath%\

set gcmconfinput=_bin\_gcm\conf_input
if not exist %releasepath%\%gcmconfinput% (mkdir %releasepath%\%gcmconfinput% )
xcopy /y %localpath%\%gcmconfinput%\* %releasepath%\%gcmconfinput%
echo "release gcm end"
rem ====================================

rem bin=================================
echo "release bin begin"
set binpath=_bin\bin
if not exist %releasepath%\%binpath% (mkdir %releasepath%\%binpath% )
xcopy /y /S %localpath%\%binpath% %releasepath%\%binpath%\
echo "release bin end"
rem ====================================

rem setting=================================
echo "release setting begin"
set settingpath=_bin\setting
if not exist %releasepath%\%settingpath% (mkdir %releasepath%\%settingpath% )
xcopy /y %localpath%\%settingpath%\* %releasepath%\%settingpath%\
echo "release setting end"
rem ====================================

rem startup=================================
echo "release startup begin"
set startuppath=_bin\startup
if not exist %releasepath%\%startuppath% (mkdir %releasepath%\%startuppath% )
xcopy /y /S %localpath%\%startuppath% %releasepath%\%startuppath%\
echo "release startup end"
rem ====================================

echo "release _bin path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release _lib path begin"
set libpath=_lib

if not exist %releasepath%\%libpath% (mkdir %releasepath%\%libpath%)
xcopy /y /S %localpath%\%libpath% %releasepath%\%libpath%\

del %releasepath%\%libpath%\linux\debug\KFProtocol*.a
del %releasepath%\%libpath%\linux\release\KFProtocol*.a

del %releasepath%\%libpath%\win64\debug\KFProtocol*.lib
del %releasepath%\%libpath%\win64\release\KFProtocol*.lib

echo "release _lib path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release _build path begin"
set buildpath=_build

if not exist %releasepath%\%buildpath%\linux (mkdir %releasepath%\%buildpath%\linux )

if not exist %releasepath%\%buildpath%\linux\tool (mkdir %releasepath%\%buildpath%\linux\tool )
xcopy /y %localpath%\%buildpath%\linux\tool\* %releasepath%\%buildpath%\linux\tool\

echo "release _build path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release KFCommon path begin"
set commonpath=KFCommon
if not exist %releasepath%\%commonpath% (mkdir %releasepath%\%commonpath% )
xcopy /y %localpath%\%commonpath%\* %releasepath%\%commonpath%\

echo "release KFCommon path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release KFContrib path begin"
set contribpath=KFContrib
if not exist %releasepath%\%contribpath% (mkdir %releasepath%\%contribpath% )

set subcontribpath=KFConHash
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
xcopy /y %localpath%\%contribpath%\%subcontribpath%\*.h %releasepath%\%contribpath%\%subcontribpath%\
xcopy /y %localpath%\%contribpath%\%subcontribpath%\*.inl %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFCore
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\*.h %releasepath%\%contribpath%\%subcontribpath%\
copy /y %localpath%\%contribpath%\%subcontribpath%\*.inl %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFGlobal
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\KFGlobal.h %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFMath3D
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\*.h %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFLogger
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\KFLogger.h %releasepath%\%contribpath%\%subcontribpath%\
copy /y %localpath%\%contribpath%\%subcontribpath%\KFSpdLog.h %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFMemory
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\KFMalloc.h %releasepath%\%contribpath%\%subcontribpath%\
copy /y %localpath%\%contribpath%\%subcontribpath%\KFMemoryDefine.h %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFPlugin
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
xcopy /y %localpath%\%contribpath%\%subcontribpath%\*.h %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFProto
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\KFProto.h %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFUtility
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
xcopy /y %localpath%\%contribpath%\%subcontribpath%\*.h %releasepath%\%contribpath%\%subcontribpath%\
xcopy /y %localpath%\%contribpath%\%subcontribpath%\*.inl %releasepath%\%contribpath%\%subcontribpath%\

set subcontribpath=KFProtocol
if not exist %releasepath%\%contribpath%\%subcontribpath% (mkdir %releasepath%\%contribpath%\%subcontribpath% )
copy /y %localpath%\%contribpath%\%subcontribpath%\CMakeLists.txt %releasepath%\%contribpath%\%subcontribpath%\
echo "release KFContrib path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release KFLibrary path begin"
set librarypath=KFLibrary
if not exist %releasepath%\%librarypath% (mkdir %releasepath%\%librarypath% )

set sublibrarypath=lua\lua53
if not exist %releasepath%\%librarypath%\%sublibrarypath% (mkdir %releasepath%\%librarypath%\%sublibrarypath% )
xcopy /y %localpath%\%librarypath%\%sublibrarypath%\*.h %releasepath%\%librarypath%\%sublibrarypath%\
xcopy /y %localpath%\%librarypath%\%sublibrarypath%\*.inl %releasepath%\%librarypath%\%sublibrarypath%\

set sublibrarypath=luaplus
if not exist %releasepath%\%librarypath%\%sublibrarypath% (mkdir %releasepath%\%librarypath%\%sublibrarypath% )
if not exist %releasepath%\%librarypath%\%sublibrarypath%\tilde (mkdir %releasepath%\%librarypath%\%sublibrarypath%\tilde )
xcopy /y %localpath%\%librarypath%\%sublibrarypath%\*.h %releasepath%\%librarypath%\%sublibrarypath%\
xcopy /y %localpath%\%librarypath%\%sublibrarypath%\*.inl %releasepath%\%librarypath%\%sublibrarypath%\
xcopy /y %localpath%\%librarypath%\%sublibrarypath%\tilde\*.h %releasepath%\%librarypath%\%sublibrarypath%\tilde\
xcopy /y %localpath%\%librarypath%\%sublibrarypath%\tilde\*.inl %releasepath%\%librarypath%\%sublibrarypath%\tilde\

set sublibrarypath=rapidjson
if not exist %releasepath%\%librarypath%\%sublibrarypath% (mkdir %releasepath%\%librarypath%\%sublibrarypath% )
xcopy /y /S %localpath%\%librarypath%\%sublibrarypath%\* %releasepath%\%librarypath%\%sublibrarypath%\

set sublibrarypath=spdlog
if not exist %releasepath%\%librarypath%\%sublibrarypath% (mkdir %releasepath%\%librarypath%\%sublibrarypath% )
xcopy /y /S %localpath%\%librarypath%\%sublibrarypath%\* %releasepath%\%librarypath%\%sublibrarypath%\

set sublibrarypath=google
if not exist %releasepath%\%librarypath%\%sublibrarypath% (mkdir %releasepath%\%librarypath%\%sublibrarypath% )
xcopy /y /S %localpath%\%librarypath%\%sublibrarypath%\protobuf\src\google\* %releasepath%\%librarypath%\%sublibrarypath%\

echo "release KFLibrary path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
echo "release KFPlugin path begin"
set pluginpath=KFInterface
if not exist %releasepath%\%pluginpath% (mkdir %releasepath%\%pluginpath% )

cd KFPlugin
for /d %%i in (*) do (
	if not exist ..\%releasepath%\%pluginpath%\%%i (mkdir ..\%releasepath%\%pluginpath%\%%i )
	copy /y %%i\*.h ..\%releasepath%\%pluginpath%\%%i\
)
cd ..\
echo "release KFPlugin path end"
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
rem ===========================================================================
