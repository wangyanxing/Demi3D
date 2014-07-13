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

#ifndef DiAsset_h__
#define DiAsset_h__

#include "GfxPrerequisites.h"

namespace Demi 
{
    class DI_GFX_API DiAsset : public DiBase
    {

    public:

        DiAsset(const DiString& name);

        virtual                 ~DiAsset(void);

    public:

        friend class            DiAssetManager;

        const DiString&         GetName(void) const { return mName; }

        virtual const DiString& GetAssetType() const = 0;

        virtual bool            LoadingComplete() const = 0;

        virtual bool            Load(DiDataStreamPtr data) = 0;

        virtual bool            Load() = 0;
        
        bool                    IsUseManualLoader(){ return mUsingManualLoader; }
        
        bool                    SetUseManualLoader(bool val){ mUsingManualLoader = val; }

    private:

        /** Disable the default assignment operator
         */
        DiAsset&  operator = (const DiAsset&) { return *this; }

    protected:

        DiString  mName;
        
        bool      mUsingManualLoader{ false };
    };
}

#endif
