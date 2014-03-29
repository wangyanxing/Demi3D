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
#include "D3D9ResEnum.h"
#include "D3D9Driver.h"
#include "D3D9Device.h"

namespace Demi
{
    DiString D3D9VideoMode::getDescription() const
    {
        unsigned int colourDepth = 16;
        if (mDisplayMode.Format == D3DFMT_X8R8G8B8 ||
            mDisplayMode.Format == D3DFMT_A8R8G8B8 ||
            mDisplayMode.Format == D3DFMT_R8G8B8)
            colourDepth = 32;

        DiString ret;
        ret.Format("%d x %d @ %d-bit color", mDisplayMode.Width, mDisplayMode.Height, colourDepth);
        return ret;
    }

    unsigned int D3D9VideoMode::getColourDepth() const
    {
        unsigned int colourDepth = 16;
        if (mDisplayMode.Format == D3DFMT_X8R8G8B8 ||
            mDisplayMode.Format == D3DFMT_A8R8G8B8 ||
            mDisplayMode.Format == D3DFMT_R8G8B8)
            colourDepth = 32;

        return colourDepth;
    }

    D3D9VideoModeList::D3D9VideoModeList(D3D9Driver* pDriver)
    {
        DI_ASSERT(pDriver);
        mDriver = pDriver;
        enumerate();
    }

    D3D9VideoModeList::~D3D9VideoModeList()
    {
        mDriver = NULL;
        mModeList.clear();
    }

    BOOL D3D9VideoModeList::enumerate()
    {
        UINT iMode;
        IDirect3D9* pD3D = DiD3D9Driver::D3D9;
        UINT adapter = mDriver->getAdapterNumber();

        for (iMode = 0; iMode < pD3D->GetAdapterModeCount(adapter, D3DFMT_R5G6B5); iMode++)
        {
            D3DDISPLAYMODE displayMode;
            pD3D->EnumAdapterModes(adapter, D3DFMT_R5G6B5, iMode, &displayMode);

            // Filter out low-resolutions
            if (displayMode.Width < 640 || displayMode.Height < 400)
                continue;

            // Check to see if it is already in the list (to filter out refresh rates)
            BOOL found = FALSE;
            for (auto it = mModeList.begin(); it != mModeList.end(); ++it)
            {
                D3DDISPLAYMODE oldDisp = it->getDisplayMode();
                if (oldDisp.Width == displayMode.Width &&
                    oldDisp.Height == displayMode.Height &&
                    oldDisp.Format == displayMode.Format)
                {
                    // Check refresh rate and favour higher if poss
                    if (oldDisp.RefreshRate < displayMode.RefreshRate)
                        it->increaseRefreshRate(displayMode.RefreshRate);
                    found = TRUE;
                    break;
                }
            }

            if (!found)
                mModeList.push_back(D3D9VideoMode(displayMode));
        }

        for (iMode = 0; iMode < pD3D->GetAdapterModeCount(adapter, D3DFMT_X8R8G8B8); iMode++)
        {
            D3DDISPLAYMODE displayMode;
            pD3D->EnumAdapterModes(adapter, D3DFMT_X8R8G8B8, iMode, &displayMode);

            // Filter out low-resolutions
            if (displayMode.Width < 640 || displayMode.Height < 400)
                continue;

            // Check to see if it is already in the list (to filter out refresh rates)
            BOOL found = FALSE;
            for (auto it = mModeList.begin(); it != mModeList.end(); ++it)
            {
                D3DDISPLAYMODE oldDisp = it->getDisplayMode();
                if (oldDisp.Width == displayMode.Width &&
                    oldDisp.Height == displayMode.Height &&
                    oldDisp.Format == displayMode.Format)
                {
                    // Check refresh rate and favour higher if poss
                    if (oldDisp.RefreshRate < displayMode.RefreshRate)
                        it->increaseRefreshRate(displayMode.RefreshRate);
                    found = TRUE;
                    break;
                }
            }

            if (!found)
                mModeList.push_back(D3D9VideoMode(displayMode));
        }

        return TRUE;
    }

    size_t D3D9VideoModeList::count()
    {
        return mModeList.size();
    }

    D3D9VideoMode* D3D9VideoModeList::item(size_t index)
    {
        auto p = mModeList.begin();
        return &p[index];
    }

    D3D9VideoMode* D3D9VideoModeList::item(const DiString &name)
    {
        auto it = mModeList.begin();
        if (it == mModeList.end())
            return nullptr;

        for (; it != mModeList.end(); ++it)
        {
            if (it->getDescription() == name)
                return &(*it);
        }

        return nullptr;
    }
}