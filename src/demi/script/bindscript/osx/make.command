#!/bin/sh
cd $( dirname $0 )
cd ..
cd ..
./bindscript/osx/tolua++ -n ScriptMain -o binding.h ./bindscript/scriptMain.pkg
