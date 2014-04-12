#!/bin/sh
cd $( dirname $0 )
cd ..
./osx/tolua++ -n ScriptMain -o binding.h scriptMain.pkg
