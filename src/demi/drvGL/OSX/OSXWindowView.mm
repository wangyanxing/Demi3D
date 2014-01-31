
#include "GfxPch.h"
#include "OSXWindowView.h"
#include "RenderWindow.h"

using namespace Demi;

@implementation DemiView

- (id)initWithFrame:(NSRect)f
{
	if((self = [super initWithFrame:f]))
    {
        NSApplicationLoad();
        window = 0;
    }
	return self;
}

- (id)initWithGLOSXWindow:(DiWindow*)w
{
    uint32 wid = 0;
    uint32 hei = 0;
    w->GetWindowSize(wid, hei);
	if((self = [super initWithFrame:NSMakeRect(0, 0, wid, hei)]))
    {
        window = w;
    }
	return self;
}

- (void)setDemiWindow:(DiWindow*)w
{
	window = w;
}

- (DiWindow*)demiWindow
{
	return window;
}

- (void)setFrameSize:(NSSize)s
{
	[super setFrameSize:s];
    if (window)
        window->OnMoveOrResize();
}

- (void)drawRect:(NSRect)r
{
	if(window)
		window->Update();
}

- (BOOL)acceptsFirstResponder
{
    return NO;
}

- (BOOL)canBecomeKeyView
{
    return NO;
}

@end
