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

#include "EAGL2View.h"
#include "Camera.h"
#include "EAGL2Window.h"

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIWindow.h>
#import <UIKit/UIDevice.h>

using namespace Demi;

@implementation EAGL2View

@synthesize mWindow;

- (NSString *)description
{
    return [NSString stringWithFormat:@"EAGL2View frame dimensions x: %.0f y: %.0f w: %.0f h: %.0f", 
            [self frame].origin.x,
            [self frame].origin.y,
            [self frame].size.width,
            [self frame].size.height];
}

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

- (void)layoutSubviews
{
    // Change the viewport orientation based upon the current device orientation.
    // Note: This only operates on the main viewport, usually the main view.

    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];

    // Return if the orientation is not a valid interface orientation(face up, face down)
    if(!UIDeviceOrientationIsValidInterfaceOrientation(deviceOrientation))
        return;

    // Check if orientation is supported
    NSString *rotateToOrientation = @"";
    if(deviceOrientation == UIDeviceOrientationPortrait)
        rotateToOrientation = @"UIInterfaceOrientationPortrait";
    else if(deviceOrientation == UIDeviceOrientationPortraitUpsideDown)
        rotateToOrientation = @"UIInterfaceOrientationPortraitUpsideDown";
    else if(deviceOrientation == UIDeviceOrientationLandscapeLeft)
        rotateToOrientation = @"UIInterfaceOrientationLandscapeLeft";
    else if(deviceOrientation == UIDeviceOrientationLandscapeRight)
        rotateToOrientation = @"UIInterfaceOrientationLandscapeRight";

    NSArray *supportedOrientations = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"UISupportedInterfaceOrientations"];

    BOOL supported = [supportedOrientations containsObject:rotateToOrientation];

    if (!supported)
        return;

    // Get the window using the name that we saved
    if(mWindow != NULL)
    {
        // Get the window size and initialize temp variables
        unsigned int w = 0, h = 0;
        unsigned int width = (uint)self.bounds.size.width;
        unsigned int height = (uint)self.bounds.size.height;

        if (UIDeviceOrientationIsLandscape(deviceOrientation))
        {
            w = std::max(width, height);
            h = std::min(width, height);
        }
        else
        {
            h = std::max(width, height);
            w = std::min(width, height);
        }

        width = w;
        height = h;

        // Resize the window
        mWindow->Resize(width, height);
        
        // TODO: Aspect ratio
    }
}

@end
