//
//  IOSShareManager.m
//  IOS7ShareSample
//
//  Created by LiuYanghui on 14-3-22.
//
//

#import "IOSShareManager.h"

@interface IOSShareManager()
@property (strong, nonatomic) UIPopoverController *activityPopover;
@end

@implementation IOSShareManager

+ (IOSShareManager *) getInstance
{
    static IOSShareManager* gameMgr = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        gameMgr = [[self alloc] init];
    });
    return gameMgr;
}

- (void)share:(CGPoint)pos message:(NSString*) msg
{
    // 你可以先截屏保存到document下，这里的image读取document下面。
    //UIImage *image = [UIImage imageNamed:@"HelloWorld.png"];
    //NSString *mesg = @"You can add some description info here!";
    
    UIActivityViewController *activityViewController = [[UIActivityViewController alloc] initWithActivityItems:@[msg] applicationActivities:nil];
    
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        //iPhone 从底部向上滑出view
        [self.viewController presentViewController:activityViewController animated:YES completion:nil];
    } else {
        //iPad, 弹出view
        if (![self.activityPopover isPopoverVisible]) {
            self.activityPopover = [[UIPopoverController alloc] initWithContentViewController:activityViewController];
            [self.activityPopover presentPopoverFromRect:CGRectMake(pos.x, pos.y, 0, 0) inView:((UIViewController *)self.viewController).view permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        } else {
            //Dismiss view，当重复点击时
            [self.activityPopover dismissPopoverAnimated:YES];
        }
    }
}

- (id)init
{
    self = [super init];
    if (self) {
        // init code here
        
    }
    return self;
}

- (void) dealloc
{
    [super dealloc];
}

@end