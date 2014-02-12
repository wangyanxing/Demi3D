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

#ifndef DiDeviceLostListener_h__
#define DiDeviceLostListener_h__


namespace Demi 
{
    class DI_GFX_API DiDeviceLostListener : public DiBase
    {
    public:

        DiDeviceLostListener()
        {
            mDeviceLost = false;
        }

        virtual void    OnDeviceLost()    = 0;

        virtual void    OnDeviceReset()    = 0;

    protected:

        bool            mDeviceLost;
    };
}

#endif
