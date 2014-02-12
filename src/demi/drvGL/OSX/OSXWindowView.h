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

#ifndef OSXWindowView_h__
#define OSXWindowView_h__

#include "Window.h"

#include <Cocoa/Cocoa.h>

@interface DemiView : NSView
{
	Demi::DiWindow *window;
}

- (id)initWithFrame:(NSRect)f;

- (id)initWithGLOSXWindow:(Demi::DiWindow*)w;

- (void)setDemiWindow:(Demi::DiWindow*)w;

- (Demi::DiWindow*)demiWindow;

- (void)setFrameSize:(NSSize)s;

@end

#endif