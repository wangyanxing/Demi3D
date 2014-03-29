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

#ifndef DiD3D9Device_h__
#define DiD3D9Device_h__

#include "D3D9Prerequisites.h"

namespace Demi
{
    class DI_D3D9DRV_API D3D9Driver : public DiBase
    {

    public:
        // Constructors
        D3D9Driver();						// Default
        D3D9Driver(const D3D9Driver &ob);	// Copy
        D3D9Driver(unsigned int adapterNumber,
            const D3DCAPS9& deviceCaps,
            const D3DADAPTER_IDENTIFIER9& adapterIdentifer,
            const D3DDISPLAYMODE& desktopDisplayMode);
        ~D3D9Driver();


        const D3DCAPS9&		getD3D9DeviceCaps() const { return mD3D9DeviceCaps; }
        DiString            DriverName() const;
        DiString            DriverDescription() const;

        unsigned int					getAdapterNumber() const { return mAdapterNumber; }
        const D3DADAPTER_IDENTIFIER9&	getAdapterIdentifier() const { return mAdapterIdentifier; }
        const D3DDISPLAYMODE&			getDesktopMode() const { return mDesktopDisplayMode; }
        D3D9VideoModeList*				getVideoModeList();

    private:
        // Adapter number.
        unsigned int			mAdapterNumber;

        // Device caps.
        D3DCAPS9				mD3D9DeviceCaps;

        // Adapter identifier
        D3DADAPTER_IDENTIFIER9	mAdapterIdentifier;

        // Desktop display mode.
        D3DDISPLAYMODE			mDesktopDisplayMode;

        // Video modes list.
        D3D9VideoModeList*		mVideoModeList;
    };

    class DI_D3D9DRV_API D3D9DriverList : public DiBase
    {
    private:
        DiVector<D3D9Driver> mDriverList;

    public:
        D3D9DriverList();
        ~D3D9DriverList();

        BOOL enumerate();
        size_t count() const;
        D3D9Driver* item(size_t index);

        D3D9Driver* item(const DiString &name);
    };
}

#endif
