@echo off

pushd %~dp0

cmake -E make_directory build
cmake -E chdir build cmake -G "Visual Studio 12" ..\