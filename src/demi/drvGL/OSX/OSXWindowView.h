


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