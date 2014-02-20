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

#include "GfxPch.h"
#include "DebugHelper.h"

namespace Demi
{
    DiDebugHelper::DiDebugHelper(void)
        :DiTransformUnit(), DiRenderUnit()
    {

    }

    DiDebugHelper::~DiDebugHelper(void)
    {

    }

    void DiDebugHelper::GetWorldTransform(DiMat4* xform) const
    {

    }

    const DiAABB& DiDebugHelper::GetBoundingBox(void) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiDebugHelper::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }

    DiString& DiDebugHelper::GetType()
    {
        static DiString type = "DebugHelper";
        return type;
    }

    void DiDebugHelper::Update(DiCamera* camera)
    {

    }

}