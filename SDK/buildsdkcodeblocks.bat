@echo off
set GENERATOR="CodeBlocks - MinGW Makefiles"
set PATH=%PATH%;C:\Program Files (x86)\CodeBlocks

rem check 7z and dot
7z > NUL
if errorlevel 1 goto 7zerror

set BUILD_DIR=codeblocks

mkdir %BUILD_DIR%
pushd %BUILD_DIR%
rem call CMake
cmake -DPROCEDURAL_INSTALL_SAMPLES_SOURCE:BOOL=TRUE -DPROCEDURAL_INSTALL_DOCS:BOOL=TRUE -DOGRE_HOME:PATH=E:\Prog\Builds\Ogre18_CB\sdk -DCMAKE_BUILD_TYPE=Release -G%GENERATOR% ..\..
if errorlevel 1 goto cmakeerror

rem Read PROCEDURAL version
set /p PROCEDURALVERSION=<version.txt

rem build docs explicitly since INSTALL doesn't include it
codeblocks.exe PROCEDURAL.cbp --build --target=doc

if errorlevel 1 goto msvcerror

rem Delete unnecessary doc files
pushd docs\api\html
del /Q/F *.hhk *.hhc *.map *.md5 *.dot *.hhp *.plist
popd

rem Build main binaries
codeblocks.exe PROCEDURAL.cbp --build --target=install

popd

rem Package up
set SDKNAME=ProceduralSDK_CB_v%PROCEDURALVERSION%
rmdir /S/Q %SDKNAME%
move %BUILD_DIR%\sdk %SDKNAME%
del /Q/F %SDKNAME%.exe
rem create self-extracting 7zip archive
7z a -r -y -sfx7z.sfx %SDKNAME%.exe %SDKNAME%

echo Done! Test %SDKNAME%.exe and then release
goto end

:paramErr
echo Required: Build tool (vc71, vc8, vc8x64, vc9, vc9x64, vc10, vc10x64)
set errorlevel=1
goto end

:envErr
echo You need to run this script after running vcvars32.bat
set errorlevel=1
goto end

:cmakeerror
popd
echo CMake not found on your path or CMake error - see above and correct
goto end

:7zerror
echo 7z.exe not found on your path, please add
goto end

:msvcerror
popd
echo Neither devenv.exe nor VCExpress are on your path, use vcvars32.bat
goto end

:end

