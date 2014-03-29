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

#ifndef DiD3D9ResEnum_h__
#define DiD3D9ResEnum_h__

#include "D3D9Prerequisites.h"

namespace Demi
{
    static unsigned int modeCount = 0;

    /** Resolution mode
     */
    class DI_D3D9DRV_API D3D9VideoMode
    {
    private:
        D3DDISPLAYMODE mDisplayMode;
        unsigned int modeNumber;

    public:
        D3D9VideoMode() { modeNumber = ++modeCount; ZeroMemory(&mDisplayMode, sizeof(D3DDISPLAYMODE)); }
        
        D3D9VideoMode(const D3D9VideoMode &ob) { modeNumber = ++modeCount; mDisplayMode = ob.mDisplayMode; }
        
        D3D9VideoMode(D3DDISPLAYMODE d3ddm) { modeNumber = ++modeCount; mDisplayMode = d3ddm; }
        
        ~D3D9VideoMode()
        {
            modeCount--;
        }

        unsigned int getWidth() const { return mDisplayMode.Width; }
        unsigned int getHeight() const { return mDisplayMode.Height; }
        D3DFORMAT getFormat() const { return mDisplayMode.Format; }
        unsigned int getRefreshRate() const { return mDisplayMode.RefreshRate; }
        unsigned int getColourDepth() const;
        D3DDISPLAYMODE getDisplayMode() const { return mDisplayMode; }
        void increaseRefreshRate(unsigned int rr) { mDisplayMode.RefreshRate = rr; }
        DiString getDescription() const;
    };

    class DI_D3D9DRV_API D3D9VideoModeList : public DiBase
    {
    private:
        D3D9Driver* mDriver;
        DiVector<D3D9VideoMode> mModeList;

    public:
        D3D9VideoModeList(D3D9Driver* pDriver);
        ~D3D9VideoModeList();

        BOOL enumerate();

        D3D9VideoMode* item(size_t index);
        size_t count();

        D3D9VideoMode* item(const DiString &name);
    };
}

#endif
