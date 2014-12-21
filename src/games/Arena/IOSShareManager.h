//
//  IOSShareManager.h
//  IOS7ShareSample
//
//  Created by LiuYanghui on 14-3-22.
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface IOSShareManager : NSObject

+ (IOSShareManager *) getInstance;
- (void)share:(CGPoint)pos message:(NSString*) msg;

@property (nonatomic, readwrite, retain) id viewController;

@end