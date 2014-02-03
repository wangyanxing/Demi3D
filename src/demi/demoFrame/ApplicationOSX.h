
/********************************************************************
    File:       ApplicationOSX.h
    Creator:    demiwangya
*********************************************************************/

#ifndef DiApplicationOSX_h__
#define DiApplicationOSX_h__

#ifdef __OBJC__

#include "Application.h"

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@implementation NSApplication(_suppressUnimplementedActionWarning)
- (void)buttonPressed:(id)sender
{
}
@end


#if defined(MAC_OS_X_VERSION_10_6) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
@interface AppDelegate : NSObject <NSApplicationDelegate>
#else
@interface AppDelegate : NSObject
#endif
{
    NSTimer             *mTimer;

    NSDate              *mDate;

    NSTimeInterval      mLastFrameTime;
    
    CVDisplayLinkRef    mDisplayLink; //display link for managing rendering thread
}

-(void)go;
-(void)renderOneFrame:(id)sender;
-(void)shutdown;

@property(retain) NSTimer *mTimer;
@property(nonatomic) NSTimeInterval mLastFrameTime;

@end

static id mAppDelegate;

@implementation AppDelegate

@synthesize mTimer;
@dynamic mLastFrameTime;

-(NSTimeInterval)mLastFrameTime
{
    return mLastFrameTime;
}

-(void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    if (frameInterval >= 1)
    {
        mLastFrameTime = frameInterval;
    }
}

-(void)go
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mTimer = nil;

    // init the engine
    Demi::DemiDemo::GetApp()->OpenImpl();
    
    // rendering timer
    mTimer = [NSTimer scheduledTimerWithTimeInterval : (NSTimeInterval)(1.0f / 60.0f) * mLastFrameTime
                                                        target : self
                                                        selector : @selector(renderOneFrame:)
                                                        userInfo:nil
                                                        repeats : YES];
    [pool release];
}

-(void)applicationDidFinishLaunching:(NSNotification *)application
{
    mLastFrameTime = 1;
    mTimer = nil;

    [self go];
}

-(void)shutdown
{
    if (mDisplayLink)
    {
        CVDisplayLinkStop(mDisplayLink);
        CVDisplayLinkRelease(mDisplayLink);
        mDisplayLink = nil;
    }

    [NSApp terminate : nil];
}

-(void)renderOneFrame:(id)sender
{
    if (Demi::DemiDemo::GetApp() && Demi::DemiDemo::GetApp()->IsOpen())
    {
        Demi::DemiDemo::GetApp()->Update();
    }
    else
    {
        if (mTimer)
        {
            [mTimer invalidate];
            mTimer = nil;
        }

        [NSApp performSelector : @selector(terminate:) withObject:nil afterDelay : 0.0];
    }
}

@end

#else
#   error "Please set the compiler as objective c++ mode!"
#endif

#endif // ApplicationOSX_h__