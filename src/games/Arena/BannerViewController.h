#import <UIKit/UIKit.h>

@interface BannerViewController : UIViewController
+ (BannerViewController *) getInstance;
- (instancetype)initWithContentViewController:(UIViewController *)contentController;
- (void) hideBanner;
- (void) showBanner;

@end