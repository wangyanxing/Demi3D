
/********************************************************************
    File:       Asset.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GfxPrerequisites.h"

namespace Demi 
{
    class DI_GFX_API DiAsset : public DiBase
    {

    public:

        DiAsset(const DiString& name);

        virtual                ~DiAsset(void);

    public:

        friend class            DiAssetManager;

        const DiString&         GetName(void) const { return mName; }

        virtual DiAssetType     GetAssetType() const = 0;

        virtual bool            LoadingComplete() const = 0;

        virtual    bool         Load(DiDataStreamPtr data) = 0;

        virtual    bool         Load() = 0;

    private:

        DiAsset &               operator = (const DiAsset&) { return *this; }

    protected:

        DiString                mName;

        uint32                  mByteSize;
    };

    typedef shared_ptr<DiAsset> DiAssetPtr;
}