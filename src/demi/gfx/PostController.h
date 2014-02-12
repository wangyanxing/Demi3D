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

#ifndef DiPostController_h__
#define DiPostController_h__


namespace Demi
{
    class DI_GFX_API DiPostController : public DiBase
    {
    public:

        DiPostController(DiAnimModel* parent);

        virtual                 ~DiPostController();

    public:

        bool                    IsEnabled() const { return mEnabled; }

        void                    SetEnabled(bool val) { mEnabled = val; }

        virtual void            Update(float deltaTime) = 0;

    public:

        bool                    mEnabled;

        DiAnimModel*            mParent;
    };
}

#endif
