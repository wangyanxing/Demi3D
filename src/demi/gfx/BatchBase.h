
/********************************************************************
    File:       BatchBase.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "RenderUnit.h"

namespace Demi
{
    class DI_GFX_API DiTerrainBatchBase : public DiRenderUnit
    {
    public:

        virtual void UpdateMaterialParams(){}
    };
}