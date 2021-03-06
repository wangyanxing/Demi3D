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
#include "DepthBuffer.h"
#include "RenderTarget.h"

#if DEMI_COMPILER == DEMI_COMPILER_MSVC
#   pragma warning(disable:4389)
#endif

namespace Demi
{
    DiDepthBuffer::DiDepthBuffer( )
    {
    }

    DiDepthBuffer::~DiDepthBuffer()
    {
        DetachFromAllRenderTargets();
    }

    void DiDepthBuffer::SetPoolId( uint16 poolId )
    {
        mPoolId = poolId;
        DetachFromAllRenderTargets();
    }

    uint16 DiDepthBuffer::GetPoolId() const
    {
        return mPoolId;
    }

    uint16 DiDepthBuffer::GetBitDepth() const
    {
        return mBitDepth;
    }

    uint32 DiDepthBuffer::GetWidth() const
    {
        return mWidth;
    }

    uint32 DiDepthBuffer::GetHeight() const
    {
        return mHeight;
    }

    bool DiDepthBuffer::IsManual() const
    {
        return mManual;
    }

    void DiDepthBuffer::NotifyRenderTargetAttached( DiRenderTarget *renderTarget )
    {
        DI_ASSERT( mAttachedRenderTargets.find( renderTarget ) == mAttachedRenderTargets.end() );

        mAttachedRenderTargets.insert( renderTarget );
    }

    void DiDepthBuffer::NotifyRenderTargetDetached( DiRenderTarget *renderTarget )
    {
        RenderTargetSet::iterator itor = mAttachedRenderTargets.find( renderTarget );
        DI_ASSERT( itor != mAttachedRenderTargets.end() );

        mAttachedRenderTargets.erase( itor );
    }

    void DiDepthBuffer::DetachFromAllRenderTargets()
    {
        RenderTargetSet::const_iterator itor = mAttachedRenderTargets.begin();
        RenderTargetSet::const_iterator end  = mAttachedRenderTargets.end();
        while( itor != end )
        {
            (*itor)->ClearDepthBuffer();
            ++itor;
        }

        mAttachedRenderTargets.clear();
    }
}