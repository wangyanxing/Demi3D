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

#ifndef _VIEWER_HELPER_H_
#define _VIEWER_HELPER_H_

#include "ViewerPrerequisites.h"
#include "BaseLayout/BaseLayout.h"
#include "Tools/Dialog.h"

namespace tools
{
    class ViewerHelper : public Dialog
    {
    public:
        ViewerHelper();
        virtual ~ViewerHelper();

    private:
        void notifyOk(MyGUI::Widget* _sender);

    private:
        MyGUI::Button* mButtonOk;
    };


} // namespace tools

#endif // _VIEWER_HELPER_H_
