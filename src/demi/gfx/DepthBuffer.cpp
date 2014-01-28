
#include "GfxPch.h"
#include "DepthBuffer.h"
#include "RenderTarget.h"

#pragma warning(disable:4389)

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