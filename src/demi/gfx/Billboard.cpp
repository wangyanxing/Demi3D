
#include "Billboard.h"
#include "BillboardSet.h"

namespace Demi
{
    DiBillboard::DiBillboard()
        :mOwnDimensions(false),
        mUseTexcoordRect(false),
        mTexcoordIndex(0),
        mPosition(DiVec3::ZERO),
        mDirection(DiVec3::ZERO),        
        mParentSet(0),
        mColour(DiColor::White),
        mRotation(0)
    {

    }

    DiBillboard::DiBillboard( const DiVec3& position, DiBillboardSet* owner, const DiColor& colour /*= DiColor::White*/ )
        : mOwnDimensions(false)
        , mUseTexcoordRect(false)
        , mTexcoordIndex(0)
        , mPosition(position)
        , mDirection(DiVec3::ZERO)
        , mParentSet(owner)
        , mColour(colour)
        , mRotation(0)
    {

    }

    DiBillboard::~DiBillboard()
    {

    }

    void DiBillboard::SetRotation( const DiRadian& rotation )
    {
        mRotation = rotation;
        if (mRotation != DiRadian(0))
        {
            mParentSet->NotifyBillboardRotated();
        }
    }

    void DiBillboard::SetPosition( const DiVec3& position )
    {
        mPosition = position;
    }

    void DiBillboard::SetPosition( float x, float y, float z )
    {
        mPosition.x = x;
        mPosition.y = y;
        mPosition.z = z;
    }

    const DiVec3& DiBillboard::GetPosition( void ) const
    {
        return mPosition;
    }

    void DiBillboard::SetDimensions( float width, float height )
    {
        mOwnDimensions = true;
        mWidth = width;
        mHeight = height;
        if (mParentSet)
        {
            mParentSet->NotifyBillboardResized();
        }
    }

    void DiBillboard::SetColour( const DiColor& colour )
    {
        mColour = colour;
    }

    const DiColor& DiBillboard::GetColour( void ) const
    {
        return mColour;
    }

    bool DiBillboard::HasOwnDimensions( void ) const
    {
        return mOwnDimensions;
    }

    float DiBillboard::GetOwnWidth( void ) const
    {
        return mWidth;
    }

    float DiBillboard::GetOwnHeight( void ) const
    {
        return mHeight;
    }

    void DiBillboard::NotifyOwner( DiBillboardSet* owner )
    {
        mParentSet = owner;
    }

    void DiBillboard::SetTexcoordIndex( uint16 texcoordIndex )
    {
        mTexcoordIndex = texcoordIndex;
        mUseTexcoordRect = false;
    }

    void DiBillboard::SetTexcoordRect( const DiFloatRect& texcoordRect )
    {
        mTexcoordRect = texcoordRect;
        mUseTexcoordRect = true;
    }

    void DiBillboard::SetTexcoordRect( float u0, float v0, float u1, float v1 )
    {
        SetTexcoordRect(DiFloatRect(u0, v0, u1, v1));
    }

    void DiBillboard::SetParentBillboardSet( DiBillboardSet* bbs )
    {
        mParentSet = bbs;
    }

}