#!/bin/sh
cd $( dirname $0 )
BUILD=build_ios_release
cmake -E make_directory $BUILD
cmake -E chdir $BUILD cmake -D DEMI_BUILD_PLATFORM_APPLE_IOS=1 -DCMAKE_BUILD_TYPE=RELEASE -G Xcode ..