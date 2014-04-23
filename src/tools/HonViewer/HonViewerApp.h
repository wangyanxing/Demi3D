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

#ifndef HonViewerApp_h__
#define HonViewerApp_h__

#include "ViewerPrerequisites.h"
#include "Application.h"

namespace Demi 
{
    class HonViewerApp : public DemiDemo
    {
    public:

        HonViewerApp();

        virtual ~HonViewerApp();

    public:

        virtual void      Update();

        virtual void      Close();

        virtual void      OpenImpl();

    private:

        BackgroundControl*  mBackground;

        MainPaneControl*    mMainPane;
    };
}

#endif
