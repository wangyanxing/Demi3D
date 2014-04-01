#!/bin/sh
cd $( dirname $0 )
BUILD=build_osx
cmake -E make_directory $BUILD
cmake -E chdir $BUILD cmake -D DEMI_BUILD_PLATFORM_APPLE_IOS=0 -G Xcode ..