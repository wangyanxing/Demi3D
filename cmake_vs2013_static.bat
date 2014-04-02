@echo off

pushd %~dp0

cmake -E make_directory build_static
cmake -E chdir build_static cmake -D DEMI_STATIC=1 -G "Visual Studio 12" ..\
pause