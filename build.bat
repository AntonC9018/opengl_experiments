@echo off

pushd .
cd src
cd descriptors
IF %ERRORLEVEL% NEQ 0 GOTO build_failed
..\..\..\build\shader_descriptor.exe types.h uniform_buffer.h generic.h;..\..\assets\shader_src\generic.fs;..\..\assets\shader_src\generic.vs grid.h;..\..\assets\shader_src\grid.fs;..\..\assets\shader_src\grid.vs
IF %ERRORLEVEL% NEQ 0 GOTO build_failed
popd
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin/msbuild.exe" opengl_test.sln /p:OutDir=..\build\

GOTO end_script

:build_failed
popd
echo ************ Build failed ************
exit /b 1

:end_script