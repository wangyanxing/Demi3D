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

#include "DrvD3D9Pch.h"
#include "D3D9Device.h"
#include "D3D9ResEnum.h"
#include "D3D9Driver.h"

namespace Demi
{
    D3D9Driver::D3D9Driver()
    {
        mAdapterNumber = 0;
        ZeroMemory(&mD3D9DeviceCaps, sizeof(mD3D9DeviceCaps));
        ZeroMemory(&mAdapterIdentifier, sizeof(mAdapterIdentifier));
        ZeroMemory(&mDesktopDisplayMode, sizeof(mDesktopDisplayMode));
        mVideoModeList = NULL;
    }

    D3D9Driver::D3D9Driver(const D3D9Driver &ob)
    {
        mAdapterNumber = ob.mAdapterNumber;
        mD3D9DeviceCaps = ob.mD3D9DeviceCaps;
        mAdapterIdentifier = ob.mAdapterIdentifier;
        mDesktopDisplayMode = ob.mDesktopDisplayMode;
        mVideoModeList = NULL;
    }

    D3D9Driver::D3D9Driver(unsigned int adapterNumber,
        const D3DCAPS9& deviceCaps,
        const D3DADAPTER_IDENTIFIER9& adapterIdentifier,
        const D3DDISPLAYMODE& desktopDisplayMode)
    {
        mAdapterNumber = adapterNumber;
        mD3D9DeviceCaps = deviceCaps;
        mAdapterIdentifier = adapterIdentifier;
        mDesktopDisplayMode = desktopDisplayMode;
        mVideoModeList = NULL;
    }

    D3D9Driver::~D3D9Driver()
    {
        if (mVideoModeList != NULL)
        {
            DI_DELETE mVideoModeList;
            mVideoModeList = NULL;
        }
    }

    DiString D3D9Driver::DriverName() const
    {
        return mAdapterIdentifier.Driver;
    }

    DiString D3D9Driver::DriverDescription() const
    {
        DiString ret;
        ret.Format("Monitor-%d-%s", mAdapterNumber + 1, mAdapterIdentifier.Description);
        return  ret;
    }

    D3D9VideoModeList* D3D9Driver::getVideoModeList()
    {
        if (!mVideoModeList)
            mVideoModeList = DI_NEW D3D9VideoModeList(this);

        return mVideoModeList;
    }

    D3D9DriverList::D3D9DriverList()
    {
        enumerate();
    }

    D3D9DriverList::~D3D9DriverList(void)
    {
        mDriverList.clear();
    }

    BOOL D3D9DriverList::enumerate()
    {
        IDirect3D9* lpD3D9 = DiD3D9Driver::D3D9;

        for (UINT iAdapter = 0; iAdapter < lpD3D9->GetAdapterCount(); ++iAdapter)
        {
            D3DADAPTER_IDENTIFIER9 adapterIdentifier;
            D3DDISPLAYMODE d3ddm;
            D3DCAPS9 d3dcaps9;

            lpD3D9->GetAdapterIdentifier(iAdapter, 0, &adapterIdentifier);
            lpD3D9->GetAdapterDisplayMode(iAdapter, &d3ddm);
            lpD3D9->GetDeviceCaps(iAdapter, D3DDEVTYPE_HAL, &d3dcaps9);

            mDriverList.push_back(D3D9Driver(iAdapter, d3dcaps9, adapterIdentifier, d3ddm));
        }

        return TRUE;
    }

    size_t D3D9DriverList::count() const
    {
        return mDriverList.size();
    }

    D3D9Driver* D3D9DriverList::item(size_t index)
    {
        return &mDriverList.at(index);
    }

    D3D9Driver* D3D9DriverList::item(const DiString &name)
    {
        auto it = mDriverList.begin();
        if (it == mDriverList.end())
            return nullptr;

        for (; it != mDriverList.end(); ++it)
        {
            if (it->DriverDescription() == name)
                return &(*it);
        }

        return nullptr;
    }
}