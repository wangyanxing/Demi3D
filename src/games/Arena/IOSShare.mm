
#include "IOSShare.h"
//#include "ArenaUIMainPane.h"
#import "IOSShareManager.h"

void IOSShare::share(const char* msg, float posX, float posY)
{
    NSString* major = [[[UIDevice currentDevice] systemVersion] substringWithRange:NSMakeRange(0,1)];
    int majorVersion = [major intValue];
    if(majorVersion < 7) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"You should update to iOS7 to share"
                                                        message:@"Please update your iOS!"
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
    }
    else
    {
        [[IOSShareManager getInstance] share:CGPointMake(posX, posY) message:[NSString stringWithUTF8String:msg]];
    }
}

void IOSShare::onAdLoad()
{
    //Demi::MainPaneControl::get()->showAdBar(true);
}