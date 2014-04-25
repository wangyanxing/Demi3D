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

        static HonViewerApp* GetViewerApp();

        MainPaneControl*  getMainPane(){ return mMainPane; }

        K2ModelViewer*    GetModelViewer() { return mModelViewer; }

        virtual void      mouseMoved(const OIS::MouseEvent& evt);

        virtual void      mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

        virtual void      mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

    private:

        //BackgroundControl*  mBackground;

        MainPaneControl*    mMainPane;

        K2ModelViewer*      mModelViewer;
    };
}

#endif
