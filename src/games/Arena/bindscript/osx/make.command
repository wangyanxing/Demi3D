#!/bin/sh
cd $( dirname $0 )
cd ..
cd ..
./bindscript/osx/tolua++ -n arenaMain -o ArenaBinding.h ./bindscript/arenaMain.pkg
