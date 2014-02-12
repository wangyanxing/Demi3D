


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