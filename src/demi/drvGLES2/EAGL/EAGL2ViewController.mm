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

#import "EAGL2ViewController.h"

using namespace Demi;

@implementation EAGL2ViewController

@synthesize mGLSupport;

- (id)init
{
    if ((self = [super initWithNibName:nil bundle:nil]))
    {
    }
    return self;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]))
    {
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
}

- (void)loadView
{
    [super loadView];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
}

- (void)viewDidUnload
{
    [super viewDidUnload];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    NSString *rotateToOrientation = @"";
    if(interfaceOrientation == UIInterfaceOrientationPortrait)
        rotateToOrientation = @"UIInterfaceOrientationPortrait";
    else if(interfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
        rotateToOrientation = @"UIInterfaceOrientationPortraitUpsideDown";
    else if(interfaceOrientation == UIInterfaceOrientationLandscapeLeft)
        rotateToOrientation = @"UIInterfaceOrientationLandscapeLeft";
    else if(interfaceOrientation == UIInterfaceOrientationLandscapeRight)
        rotateToOrientation = @"UIInterfaceOrientationLandscapeRight";

    // Inform the view that it needs to call layoutSubviews
    [self.view setNeedsDisplay];

    if(mGLSupport->interfaceOrientationIsSupported(rotateToOrientation))
        return YES;
    else
        return NO;
}

@end
