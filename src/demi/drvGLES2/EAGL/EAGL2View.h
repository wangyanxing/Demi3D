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

#ifndef DiEAGL2View_H__
#define DiEAGL2View_H__

#include "ES2Prerequisites.h"
#include "Str.h"

using namespace Demi;

#ifdef __OBJC__

#import <UIKit/UIView.h>

@interface EAGL2View : UIView
{
    DiEAGL2Window* mWindow;
}

@property (nonatomic,assign) DiEAGL2Window* mWindow;

@end

#endif

#endif
