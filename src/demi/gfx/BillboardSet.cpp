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
#include "BillboardSet.h"
#include "Camera.h"
#include "RenderBatchGroup.h"
#include "Billboard.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "CullNode.h"
#include "GfxDriver.h"

namespace Demi
{
    DiBillboardSet::DiBillboardSet( const DiString& name, 
        unsigned int poolSize /*= 20*/, bool externalDataSource /*= false*/ ):
        DiTransformUnit(name),
        mOriginType( BBO_CENTER ),
        mRotationType( BBR_TEXCOORD ),
        mAllDefaultSize( true ),
        mAutoExtendPool( true ),
        mSortingEnabled(false),
        mAccurateFacing(false),
        mAllDefaultRotation(true),
        mWorldSpace(false),
        mBillboardType(BBT_POINT),
        mCommonDirection(DiVec3::UNIT_Z),
        mCommonUpVector(DiVec3::UNIT_Y),
        mBuffersCreated(false),
        mPoolSize(poolSize),
        mExternalData(externalDataSource),
        mAutoUpdate(true),
        mBillboardDataChanged(true),
        mNumVisibleBillboards(0),
        mLockPtr(nullptr),
        mLockSize(0)
    {
        SetDefaultDimensions( 5, 5 );
        SetTextureStacksAndSlices( 1, 1 );
        SetPoolSize(poolSize);
    }

    DiBillboardSet::DiBillboardSet( unsigned int poolSize /*= 20*/, 
        bool externalDataSource /*= false*/ ):
        DiTransformUnit(),
        mOriginType( BBO_CENTER ),
        mRotationType( BBR_TEXCOORD ),
        mAllDefaultSize( true ),
        mAutoExtendPool( true ),
        mSortingEnabled(false),
        mAccurateFacing(false),
        mAllDefaultRotation(true),
        mWorldSpace(false),
        mBillboardType(BBT_POINT),
        mCommonDirection(DiVec3::UNIT_Z),
        mCommonUpVector(DiVec3::UNIT_Y),
        mBuffersCreated(false),
        mPoolSize(poolSize),
        mExternalData(externalDataSource),
        mAutoUpdate(true),
        mBillboardDataChanged(true),
        mNumVisibleBillboards(0),
        mLockPtr(nullptr),
        mLockSize(0)
    {
        SetDefaultDimensions( 5, 5 );
        SetTextureStacksAndSlices( 1, 1 );
        SetPoolSize(poolSize);
    }

    DiBillboardSet::~DiBillboardSet()
    {
        BillboardPool::iterator i;
        for (i = mBillboardPool.begin(); i != mBillboardPool.end(); ++i)
        {
            DI_DELETE *i;
        }

        DestroyBuffers();
    }

    const DiAABB& DiBillboardSet::GetBoundingBox( void ) const
    {
        return mAABB;
    }

    void DiBillboardSet::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        bg->AddRenderUnit(this);
    }

    void DiBillboardSet::Update(DiCamera* camera)
    {
        mCurrentCamera = camera;

        mCamQ = mCurrentCamera->GetDerivedOrientation();
        mCamPos = mCurrentCamera->GetDerivedPosition();
        if (!mWorldSpace)
        {
            mCamQ = mParentNode->GetDerivedOrientation().UnitInverse() * mCamQ;
            mCamPos = mParentNode->GetDerivedOrientation().UnitInverse() *
                (mCamPos - mParentNode->GetDerivedPosition()) / mParentNode->GetDerivedScale();
        }

        mCamDir = mCamQ * DiVec3::NEGATIVE_UNIT_Z;

        mPrimitiveCount = mNumVisibleBillboards * 2;

        if (!mExternalData && (mAutoUpdate || mBillboardDataChanged || !mBuffersCreated))
        {
            BeginBillboards(mActiveBillboards.size());
            ActiveBillboardList::iterator it;
            for (it = mActiveBillboards.begin();
                it != mActiveBillboards.end();
                ++it)
            {
                InjectBillboard(*(*it));
            }
            EndBillboards();
            mBillboardDataChanged = false;
        }
    }

    DiBillboard* DiBillboardSet::CreateBillboard( const DiVec3& position,
        const DiColor& colour /*= DiColor::White */ )
    {
        if( mFreeBillboards.empty() )
        {
            if( mAutoExtendPool )
            {
                SetPoolSize( GetPoolSize() * 2 );
            }
            else
            {
                return 0;
            }
        }

        DiBillboard* newBill = mFreeBillboards.front();
        mActiveBillboards.splice(
            mActiveBillboards.end(), mFreeBillboards, mFreeBillboards.begin());
        newBill->SetPosition(position);
        newBill->SetColour(colour);
        newBill->mDirection = DiVec3::ZERO;
        newBill->SetRotation(DiRadian(0));
        newBill->SetTexcoordIndex(0);
        newBill->ResetDimensions();
        newBill->NotifyOwner(this);

        float adjust = std::max(mDefaultWidth, mDefaultHeight);
        DiVec3 vecAdjust(adjust, adjust, adjust);
        DiVec3 newMin = position - vecAdjust;
        DiVec3 newMax = position + vecAdjust;

        mAABB.Merge(newMin);
        mAABB.Merge(newMax);

        return newBill;
    }

    DiBillboard* DiBillboardSet::CreateBillboard( float x, float y, float z,
        const DiColor& colour /*= DiColor::White */ )
    {
        return CreateBillboard(DiVec3(x,y,z),colour);
    }

    int DiBillboardSet::GetNumBillboards( void ) const
    {
        return static_cast< int >( mActiveBillboards.size() );
    }

    void DiBillboardSet::SetAutoextend( bool autoextend )
    {
        mAutoExtendPool = autoextend;
    }

    bool DiBillboardSet::GetAutoextend( void ) const
    {
        return mAutoExtendPool;
    }

    void DiBillboardSet::SetSortingEnabled( bool sortenable )
    {
        mSortingEnabled = sortenable;
    }

    bool DiBillboardSet::GetSortingEnabled( void ) const
    {
        return mSortingEnabled;
    }

    void DiBillboardSet::SetPoolSize( size_t size )
    {
        if (!mExternalData)
        {
            size_t currSize = mBillboardPool.size();
            if (currSize >= size)
                return;

            this->IncreasePool(size);

            for( size_t i = currSize; i < size; ++i )
            {
                mFreeBillboards.push_back( mBillboardPool[i] );
            }
        }

        mPoolSize = size;

        DestroyBuffers();
    }

    unsigned int DiBillboardSet::GetPoolSize( void ) const
    {
        return static_cast< unsigned int >( mBillboardPool.size() );
    }

    void DiBillboardSet::Clear()
    {
        mFreeBillboards.splice(mFreeBillboards.end(), mActiveBillboards);
    }

    DiBillboard* DiBillboardSet::GetBillboard( unsigned int index ) const
    {
        DI_ASSERT_MESSAGE(index < mActiveBillboards.size(), "invalid billboard index" );

        ActiveBillboardList::const_iterator it;
        if( index >= ( mActiveBillboards.size() >> 1 ) )
        {
            index = static_cast<unsigned int>(mActiveBillboards.size()) - index;
            for( it = mActiveBillboards.end(); index; --index, --it );
        }
        else
        {
            for( it = mActiveBillboards.begin(); index; --index, ++it );
        }

        return *it;
    }

    void DiBillboardSet::RemoveBillboard( unsigned int index )
    {
        DI_ASSERT_MESSAGE(index < mActiveBillboards.size(), "Billboard id error." );

        ActiveBillboardList::iterator it;
        if( index >= ( mActiveBillboards.size() >> 1 ) )
        {
            index = static_cast<unsigned int>(mActiveBillboards.size()) - index;
            for( it = mActiveBillboards.end(); index; --index, --it );
        }
        else
        {
            for( it = mActiveBillboards.begin(); index; --index, ++it );
        }

        mFreeBillboards.splice(mFreeBillboards.end(), mActiveBillboards, it);
    }

    void DiBillboardSet::RemoveBillboard( DiBillboard* pBill )
    {
        ActiveBillboardList::iterator it =
            std::find(mActiveBillboards.begin(), mActiveBillboards.end(), pBill);
        DI_ASSERT_MESSAGE(
            it != mActiveBillboards.end() ,
            "Cannot remove the specific billboard instance" );

        mFreeBillboards.splice(mFreeBillboards.end(), mActiveBillboards, it);
    }

    void DiBillboardSet::SetBillboardOrigin( BillboardOrigin origin )
    {
        mOriginType = origin;
    }

    Demi::BillboardOrigin DiBillboardSet::GetBillboardOrigin( void ) const
    {
        return mOriginType;
    }

    void DiBillboardSet::SetBillboardRotationType( BillboardRotationType rotationType )
    {
        mRotationType = rotationType;
    }

    Demi::BillboardRotationType DiBillboardSet::GetBillboardRotationType( void ) const
    {
        return mRotationType;
    }

    void DiBillboardSet::SetDefaultDimensions( float width, float height )
    {
        mDefaultWidth = width;
        mDefaultHeight = height;
    }

    void DiBillboardSet::SetDefaultWidth( float width )
    {
        mDefaultWidth = width;
    }

    float DiBillboardSet::GetDefaultWidth( void ) const
    {
        return mDefaultWidth;
    }

    void DiBillboardSet::SetDefaultHeight( float height )
    {
        mDefaultHeight = height;
    }

    float DiBillboardSet::GetDefaultHeight( void ) const
    {
        return mDefaultHeight;
    }

    void DiBillboardSet::NotifyCurrentCamera( DiCamera* cam )
    {
        mCurrentCamera = cam;

        mCamQ = mCurrentCamera->GetDerivedOrientation();
        mCamPos = mCurrentCamera->GetDerivedPosition();
        if (!mWorldSpace)
        {
            mCamQ = mParentNode->GetDerivedOrientation().UnitInverse() * mCamQ;
            mCamPos = mParentNode->GetDerivedOrientation().UnitInverse() *
                (mCamPos - mParentNode->GetDerivedPosition()) / mParentNode->GetDerivedScale();
        }

        mCamDir = mCamQ * DiVec3::NEGATIVE_UNIT_Z;
    }

    void DiBillboardSet::BeginBillboards( size_t numBillboards /*= 0*/ )
    {
        if(!mBuffersCreated)
        {
            CreateBuffers();
        }

        GetParametricOffsets(mLeftOff, mRightOff, mTopOff, mBottomOff);

        if (mBillboardType != BBT_ORIENTED_SELF &&
            mBillboardType != BBT_PERPENDICULAR_SELF && 
            !(mAccurateFacing && mBillboardType != BBT_PERPENDICULAR_COMMON))
        {
            GenBillboardAxes(&mCamX, &mCamY);
            GenVertOffsets(mLeftOff, mRightOff, mTopOff, mBottomOff,
                mDefaultWidth, mDefaultHeight, mCamX, mCamY, mVOffset);
        }

        mNumVisibleBillboards = 0;
        DI_ASSERT(!mSourceData.empty());
        DI_ASSERT(!mLockPtr);

        if (numBillboards)
        {
            numBillboards = DiMath::Min(mPoolSize, numBillboards);

            size_t billboardSize;
            
            billboardSize = mSourceData[0]->GetStride() * sizeof(float);
            
            DI_ASSERT (numBillboards * billboardSize <= mSourceData[0]->GetBufferSize());

            mLockPtr = DI_NEW float[mSourceData[0]->GetStride() * numBillboards];
            mLockSize = billboardSize*numBillboards;
        }
        else
        {
            mLockPtr = DI_NEW float[mSourceData[0]->GetBufferSize() / sizeof(float)];
            mLockSize = mSourceData[0]->GetBufferSize();
        }
    }

    void DiBillboardSet::InjectBillboard( const DiBillboard& bb )
    {
        if (mNumVisibleBillboards == mPoolSize) return;

        if ((mBillboardType == BBT_ORIENTED_SELF ||
            mBillboardType == BBT_PERPENDICULAR_SELF ||
            (mAccurateFacing && mBillboardType != BBT_PERPENDICULAR_COMMON)))
        {
            GenBillboardAxes(&mCamX, &mCamY, &bb);
        }

        if( mAllDefaultSize )
        {
            if ((mBillboardType == BBT_ORIENTED_SELF ||
                   mBillboardType == BBT_PERPENDICULAR_SELF ||
                   (mAccurateFacing && mBillboardType != BBT_PERPENDICULAR_COMMON)))
            {
                GenVertOffsets(mLeftOff, mRightOff, mTopOff, mBottomOff,
                    mDefaultWidth, mDefaultHeight, mCamX, mCamY, mVOffset);
            }
            GenVertices(mVOffset, bb);
        }
        else
        {
            DiVec3 vOwnOffset[4];
            if (mBillboardType == BBT_ORIENTED_SELF ||
                mBillboardType == BBT_PERPENDICULAR_SELF ||
                bb.mOwnDimensions ||
                (mAccurateFacing && mBillboardType != BBT_PERPENDICULAR_COMMON))
            {
                GenVertOffsets(mLeftOff, mRightOff, mTopOff, mBottomOff,
                    bb.mWidth, bb.mHeight, mCamX, mCamY, vOwnOffset);
                GenVertices(vOwnOffset, bb);
            }
            else
            {
                GenVertices(mVOffset, bb);
            }
        }
        mNumVisibleBillboards++;
    }

    void DiBillboardSet::EndBillboards( void )
    {
        mSourceData[0]->WriteData(0, mLockSize, mLockPtr);
        
        DI_DELETE[] mLockPtr;
        mLockPtr = nullptr;
        mLockSize = 0;
    }

    void DiBillboardSet::SetBounds( const DiAABB& box )
    {
        mAABB = box;
    }

    void DiBillboardSet::NotifyBillboardResized( void )
    {
        mAllDefaultSize = false;
    }

    void DiBillboardSet::NotifyBillboardRotated( void )
    {
        mAllDefaultRotation = false;
    }

    void DiBillboardSet::SetBillboardType( BillboardType bbt )
    {
        mBillboardType = bbt;
    }

    Demi::BillboardType DiBillboardSet::GetBillboardType( void ) const
    {
        return mBillboardType;
    }

    void DiBillboardSet::SetCommonDirection( const DiVec3& vec )
    {
        mCommonDirection = vec;
    }

    const DiVec3& DiBillboardSet::GetCommonDirection( void ) const
    {
        return mCommonDirection;
    }

    void DiBillboardSet::SetCommonUpVector( const DiVec3& vec )
    {
        mCommonUpVector = vec;
    }

    const DiVec3& DiBillboardSet::GetCommonUpVector( void ) const
    {
        return mCommonUpVector;
    }

    void DiBillboardSet::UpdateBounds( void )
    {
        if (mActiveBillboards.empty())
        {
            mAABB.SetNull();
        }
        else
        {
            float maxSqLen = -1.0f;

            DiVec3 min(DiMath::POS_INFINITY, DiMath::POS_INFINITY, DiMath::POS_INFINITY);
            DiVec3 max(DiMath::NEG_INFINITY, DiMath::NEG_INFINITY, DiMath::NEG_INFINITY);
            ActiveBillboardList::iterator i, iend;

            iend = mActiveBillboards.end();
            DiMat4 invWorld;
            if (mWorldSpace && mParentNode)
            {
                invWorld = mParentNode->GetFullTransform().inverse();
            }

            for (i = mActiveBillboards.begin(); i != iend; ++i)
            {
                DiVec3 pos = (*i)->GetPosition();
                
                if (mWorldSpace && mParentNode)
                    pos = invWorld * pos;
                
                min.makeFloor(pos);
                max.makeCeil(pos);

                maxSqLen = std::max(maxSqLen, pos.squaredLength());
            }
            
            float adjust = std::max(mDefaultWidth, mDefaultHeight);
            DiVec3 vecAdjust(adjust, adjust, adjust);
            min -= vecAdjust;
            max += vecAdjust;

            mAABB.SetExtents(min, max);

        }

        if (mParentNode)
        {
            mParentNode->NeedUpdate();
        }
    }

    void DiBillboardSet::SetTextureCoords( DiFloatRect const * coords, uint16 numCoords )
    {
        if( !numCoords || !coords ) 
        {
            SetTextureStacksAndSlices( 1, 1 );
            return;
        }
        TextureCoordSets().swap( mTextureCoords );
        mTextureCoords.resize( numCoords );
        for (uint16 i=0; i < numCoords; i++)
        {
            mTextureCoords[i] = coords[i];
        }
    }

    void DiBillboardSet::SetTextureStacksAndSlices( uint8 stacks, uint8 slices )
    {
        if( stacks == 0 ) stacks = 1;
        if( slices == 0 ) slices = 1;
        TextureCoordSets().swap( mTextureCoords );
        mTextureCoords.resize( (size_t)stacks * slices );
        unsigned int coordIndex = 0;
        for( uint32 v = 0; v < stacks; ++v ) 
        {
            float top = (float)v / (float)stacks;
            float bottom = ((float)v + 1) / (float)stacks;
            for( uint32 u = 0; u < slices; ++u ) 
            {
                DiFloatRect & r = mTextureCoords[coordIndex];
                r.left = (float)u / (float)slices;
                r.bottom = bottom;
                r.right = ((float)u + 1) / (float)slices;
                r.top = top;
                ++coordIndex;
            }
        }
        DI_ASSERT( coordIndex == (size_t)stacks * slices );
    }

    DiFloatRect const* DiBillboardSet::GetTextureCoords( uint16 * oNumCoords )
    {
        *oNumCoords = (uint16)mTextureCoords.size();
        return &mTextureCoords.front();
    }

    void DiBillboardSet::SetAutoUpdate( bool autoUpdate )
    {
        if (autoUpdate != mAutoUpdate)
        {
            mAutoUpdate = autoUpdate;
            DestroyBuffers();
        }
    }

    void DiBillboardSet::IncreasePool( size_t size )
    {
        size_t oldSize = mBillboardPool.size();

        mBillboardPool.reserve(size);
        mBillboardPool.resize(size);

        for( size_t i = oldSize; i < size; ++i )
        {
            mBillboardPool[i] = DI_NEW DiBillboard();
        }
    }

    void DiBillboardSet::GenBillboardAxes( DiVec3* pX, DiVec3 *pY, const DiBillboard* bb /*= 0*/ )
    {
        if (mAccurateFacing && 
            (mBillboardType == BBT_POINT || 
            mBillboardType == BBT_ORIENTED_COMMON ||
            mBillboardType == BBT_ORIENTED_SELF))
        {
            mCamDir = bb->mPosition - mCamPos;
            mCamDir.normalise();
        }

        switch (mBillboardType)
        {
        case BBT_POINT:
            if (mAccurateFacing)
            {
                // Point billboards will have 'up' based on but not equal to cameras
                // Use pY temporarily to avoid allocation
                *pY = mCamQ * DiVec3::UNIT_Y;
                *pX = mCamDir.crossProduct(*pY);
                pX->normalise();
                *pY = pX->crossProduct(mCamDir); // both normalised already
            }
            else
            {
                // Get camera axes for X and Y (depth is irrelevant)
                *pX = mCamQ * DiVec3::UNIT_X;
                *pY = mCamQ * DiVec3::UNIT_Y;
            }
            break;

        case BBT_ORIENTED_COMMON:
            // Y-axis is common direction
            // X-axis is cross with camera direction
            *pY = mCommonDirection;
            *pX = mCamDir.crossProduct(*pY);
            pX->normalise();
            break;

        case BBT_ORIENTED_SELF:
            // Y-axis is direction
            // X-axis is cross with camera direction
            // Scale direction first
            *pY = bb->mDirection;
            *pX = mCamDir.crossProduct(*pY);
            pX->normalise();
            break;

        case BBT_PERPENDICULAR_COMMON:
            // X-axis is up-vector cross common direction
            // Y-axis is common direction cross X-axis
            *pX = mCommonUpVector.crossProduct(mCommonDirection);
            *pY = mCommonDirection.crossProduct(*pX);
            break;

        case BBT_PERPENDICULAR_SELF:
            // X-axis is up-vector cross own direction
            // Y-axis is own direction cross X-axis
            *pX = mCommonUpVector.crossProduct(bb->mDirection);
            pX->normalise();
            *pY = bb->mDirection.crossProduct(*pX);
            break;
        default:
            break;
        }
    }

    void DiBillboardSet::GetParametricOffsets( float& left, float& right, float& top, float& bottom )
    {
        switch( mOriginType )
        {
        case BBO_TOP_LEFT:
            left = 0.0f;
            right = 1.0f;
            top = 0.0f;
            bottom = -1.0f;
            break;

        case BBO_TOP_CENTER:
            left = -0.5f;
            right = 0.5f;
            top = 0.0f;
            bottom = -1.0f;
            break;

        case BBO_TOP_RIGHT:
            left = -1.0f;
            right = 0.0f;
            top = 0.0f;
            bottom = -1.0f;
            break;

        case BBO_CENTER_LEFT:
            left = 0.0f;
            right = 1.0f;
            top = 0.5f;
            bottom = -0.5f;
            break;

        case BBO_CENTER:
            left = -0.5f;
            right = 0.5f;
            top = 0.5f;
            bottom = -0.5f;
            break;

        case BBO_CENTER_RIGHT:
            left = -1.0f;
            right = 0.0f;
            top = 0.5f;
            bottom = -0.5f;
            break;

        case BBO_BOTTOM_LEFT:
            left = 0.0f;
            right = 1.0f;
            top = 1.0f;
            bottom = 0.0f;
            break;

        case BBO_BOTTOM_CENTER:
            left = -0.5f;
            right = 0.5f;
            top = 1.0f;
            bottom = 0.0f;
            break;

        case BBO_BOTTOM_RIGHT:
            left = -1.0f;
            right = 0.0f;
            top = 1.0f;
            bottom = 0.0f;
            break;
        }
    }

    void DiBillboardSet::GenVertices( const DiVec3* const offsets, const DiBillboard& bb )
    {
        RGBA colour = bb.mColour.GetAsARGB();
        RGBA* pCol;

        DI_ASSERT( bb.mUseTexcoordRect || bb.mTexcoordIndex < mTextureCoords.size() );

        const DiFloatRect & r =
            bb.mUseTexcoordRect ? bb.mTexcoordRect : mTextureCoords[bb.mTexcoordIndex];

        if (mAllDefaultRotation || bb.mRotation == DiRadian(0))
        {
            // pos
            *mLockPtr++ = offsets[0].x + bb.mPosition.x;
            *mLockPtr++ = offsets[0].y + bb.mPosition.y;
            *mLockPtr++ = offsets[0].z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            // UV
            *mLockPtr++ = r.left;
            *mLockPtr++ = r.top;

            *mLockPtr++ = offsets[1].x + bb.mPosition.x;
            *mLockPtr++ = offsets[1].y + bb.mPosition.y;
            *mLockPtr++ = offsets[1].z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            // UV
            *mLockPtr++ = r.right;
            *mLockPtr++ = r.top;

            // POS
            *mLockPtr++ = offsets[2].x + bb.mPosition.x;
            *mLockPtr++ = offsets[2].y + bb.mPosition.y;
            *mLockPtr++ = offsets[2].z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            // UV
            *mLockPtr++ = r.left;
            *mLockPtr++ = r.bottom;

            // POS
            *mLockPtr++ = offsets[3].x + bb.mPosition.x;
            *mLockPtr++ = offsets[3].y + bb.mPosition.y;
            *mLockPtr++ = offsets[3].z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            // UV
            *mLockPtr++ = r.right;
            *mLockPtr++ = r.bottom;
        }
        else if (mRotationType == BBR_VERTEX)
        {
            DiVec3 axis = (offsets[3] - offsets[0]).crossProduct(offsets[2] - offsets[1]).normalisedCopy();

            DiMat3 rotation;
            rotation.FromAngleAxis(axis, bb.mRotation);

            DiVec3 pt;

            pt = rotation * offsets[0];
            *mLockPtr++ = pt.x + bb.mPosition.x;
            *mLockPtr++ = pt.y + bb.mPosition.y;
            *mLockPtr++ = pt.z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));

            *mLockPtr++ = r.left;
            *mLockPtr++ = r.top;

            pt = rotation * offsets[1];
            *mLockPtr++ = pt.x + bb.mPosition.x;
            *mLockPtr++ = pt.y + bb.mPosition.y;
            *mLockPtr++ = pt.z + bb.mPosition.z;
            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            *mLockPtr++ = r.right;
            *mLockPtr++ = r.top;

            pt = rotation * offsets[2];
            *mLockPtr++ = pt.x + bb.mPosition.x;
            *mLockPtr++ = pt.y + bb.mPosition.y;
            *mLockPtr++ = pt.z + bb.mPosition.z;
            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            *mLockPtr++ = r.left;
            *mLockPtr++ = r.bottom;

            pt = rotation * offsets[3];
            *mLockPtr++ = pt.x + bb.mPosition.x;
            *mLockPtr++ = pt.y + bb.mPosition.y;
            *mLockPtr++ = pt.z + bb.mPosition.z;
            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            *mLockPtr++ = r.right;
            *mLockPtr++ = r.bottom;
        }
        else
        {
            const float cos_rot  ( DiMath::Cos(bb.mRotation)   );
            const float sin_rot  ( DiMath::Sin(bb.mRotation)   );

            float width = (r.right-r.left)/2;
            float height = (r.bottom-r.top)/2;
            float mid_u = r.left+width;
            float mid_v = r.top+height;

            float cos_rot_w = cos_rot * width;
            float cos_rot_h = cos_rot * height;
            float sin_rot_w = sin_rot * width;
            float sin_rot_h = sin_rot * height;

            *mLockPtr++ = offsets[0].x + bb.mPosition.x;
            *mLockPtr++ = offsets[0].y + bb.mPosition.y;
            *mLockPtr++ = offsets[0].z + bb.mPosition.z;
            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            *mLockPtr++ = mid_u - cos_rot_w + sin_rot_h;
            *mLockPtr++ = mid_v - sin_rot_w - cos_rot_h;

            *mLockPtr++ = offsets[1].x + bb.mPosition.x;
            *mLockPtr++ = offsets[1].y + bb.mPosition.y;
            *mLockPtr++ = offsets[1].z + bb.mPosition.z;
            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            *mLockPtr++ = mid_u + cos_rot_w + sin_rot_h;
            *mLockPtr++ = mid_v + sin_rot_w - cos_rot_h;

            *mLockPtr++ = offsets[2].x + bb.mPosition.x;
            *mLockPtr++ = offsets[2].y + bb.mPosition.y;
            *mLockPtr++ = offsets[2].z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;

            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));

            *mLockPtr++ = mid_u - cos_rot_w - sin_rot_h;
            *mLockPtr++ = mid_v - sin_rot_w + cos_rot_h;

            *mLockPtr++ = offsets[3].x + bb.mPosition.x;
            *mLockPtr++ = offsets[3].y + bb.mPosition.y;
            *mLockPtr++ = offsets[3].z + bb.mPosition.z;

            pCol = static_cast<RGBA*>(static_cast<void*>(mLockPtr));
            *pCol++ = colour;
            mLockPtr = static_cast<float*>(static_cast<void*>(pCol));
            *mLockPtr++ = mid_u + cos_rot_w - sin_rot_h;
            *mLockPtr++ = mid_v + sin_rot_w + cos_rot_h;
        }
    }

    void DiBillboardSet::GenVertOffsets( float inleft, float inright, 
        float intop, float inbottom, float width, float height, 
        const DiVec3& x, const DiVec3& y, DiVec3* pDestVec )
    {
        DiVec3 vLeftOff, vRightOff, vTopOff, vBottomOff;

        vLeftOff   = x * ( inleft   * width );
        vRightOff  = x * ( inright  * width );
        vTopOff    = y * ( intop   * height );
        vBottomOff = y * ( inbottom * height );

        pDestVec[0] = vLeftOff  + vTopOff;
        pDestVec[1] = vRightOff + vTopOff;
        pDestVec[2] = vLeftOff  + vBottomOff;
        pDestVec[3] = vRightOff + vBottomOff;
    }

    void DiBillboardSet::CreateBuffers( void )
    {
        mSourceData.push_back(Driver->CreateVertexBuffer());
        mIndexBuffer = Driver->CreateIndexBuffer();
        mVertexDecl = Driver->CreateVertexDeclaration();
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3,  VERT_USAGE_POSITION);
        mVertexDecl->AddElement(0, VERT_TYPE_COLOR,   VERT_USAGE_COLOR);
        mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2,  VERT_USAGE_TEXCOORD);
        mVertexDecl->Create();

        mPrimitiveType = PT_TRIANGLELIST;
        
        uint32 vertCount = mPoolSize * 4;
        uint32 stride = sizeof(float)*3 + sizeof(ARGB) + sizeof(float)*2;
        uint32 vbSize = vertCount * stride;

        mVerticesNum = vertCount;

        mSourceData[0]->Create(vbSize);
        mSourceData[0]->SetStride(stride);

        uint32 indexCount = mPoolSize * 6;
        mIndexBuffer->Create(indexCount * 16, IB_16BITS, RU_WRITE_ONLY);

        uint16* iBuffer = DI_NEW uint16[indexCount];
        uint16* pIdx = iBuffer;
        for(size_t idx, idxOff, bboard = 0;
            bboard < mPoolSize;
            ++bboard )
        {
            idx    = bboard * 6;
            idxOff = bboard * 4;

            pIdx[idx] = static_cast<unsigned short>(idxOff);
            pIdx[idx+1] = static_cast<unsigned short>(idxOff + 2);
            pIdx[idx+2] = static_cast<unsigned short>(idxOff + 1);
            pIdx[idx+3] = static_cast<unsigned short>(idxOff + 1);
            pIdx[idx+4] = static_cast<unsigned short>(idxOff + 2);
            pIdx[idx+5] = static_cast<unsigned short>(idxOff + 3);
        }

        mIndexBuffer->WriteData(0, indexCount * sizeof(uint16), iBuffer);
        DI_DELETE[] iBuffer;

        mBuffersCreated = true;
    }

    void DiBillboardSet::DestroyBuffers( void )
    {
        ReleaseVertexDeclaration();
        ReleaseSourceData();
        ReleaseIndexBuffer();

        mBuffersCreated = false;
    }

    Demi::DiSortMode DiBillboardSet::GetSortMode( void ) const
    {
        if (mAccurateFacing ||
            mBillboardType == BBT_PERPENDICULAR_SELF ||
            mBillboardType == BBT_PERPENDICULAR_COMMON)
        {
            return SM_DISTANCE;
        }
        else
        {
            return SM_DIRECTION;
        }
    }

    DiString& DiBillboardSet::GetType()
    {
        static DiString type = "BillboardSet";
        return type;
    }
}