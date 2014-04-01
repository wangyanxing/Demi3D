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

#ifndef DiEAGL2ViewController_H__
#define DiEAGL2ViewController_H__

#include "ES2Prerequisites.h"
#import <UIKit/UIViewController.h>

#import "EAGL2Util.h"

using namespace Demi;

@interface EAGL2ViewController : UIViewController
{
    DiEAGL2Util *mGLSupport;
}

@property (assign) DiEAGL2Util *mGLSupport;

@end

#endif
