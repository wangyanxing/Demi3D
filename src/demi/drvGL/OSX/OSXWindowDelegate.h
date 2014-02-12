/**********************************************************************
  This source file is a part of Demi3D
   __  ___  __  __  __
  |  \|_ |\/||   _)|  \ 
  |__/|__|  ||  __)|__/ 

  Copyright (c) 2013-2014 Demi team
  https://github.com/wangyanxing/Demi3D

  Released under the MIT License
  https://github.com/wangyanxing/Demi3D/blob/master/License.txt
***********************************************************************/

#ifndef OSXWindowDelegate_h__
#define OSXWindowDelegate_h__

#include "Window.h"

#import <Cocoa/Cocoa.h>

@interface OSXWindowDelegate : NSObject
{
@private
    Demi::DiWindow *demiWindow;
	NSWindow *window;
}

-(id)initWithNSWindow:(NSWindow*)nswin demiWindow:(Demi::DiWindow*)demiwin;

@end

#endif