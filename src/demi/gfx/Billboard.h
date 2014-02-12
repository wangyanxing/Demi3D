


#ifndef Billboard_h__
#define Billboard_h__

#include "GfxPrerequisites.h"

namespace Demi
{
    class DI_GFX_API DiBillboard
    {
    public:

        friend class DiBillboardSet;

    public:

        DiVec3                 mPosition;

        DiVec3                 mDirection;

        DiBillboardSet*        mParentSet;

        DiColor                mColour;

        DiRadian               mRotation;

    public:

        DiBillboard();

        DiBillboard(const DiVec3& position, DiBillboardSet* owner, const DiColor& colour = DiColor::White);

        virtual             ~DiBillboard();

    public:

        const DiRadian&     GetRotation(void) const { return mRotation; }

        void                SetRotation(const DiRadian& rotation);

        void                SetPosition(const DiVec3& position);

        void                SetPosition(float x, float y, float z);

        const DiVec3&       GetPosition(void) const;

        void                SetDimensions(float width, float height);

        void                ResetDimensions(void) { mOwnDimensions = false; }

        void                SetColour(const DiColor& colour);

        const DiColor&      GetColour(void) const;

        bool                HasOwnDimensions(void) const;

        float               GetOwnWidth(void) const;

        float               GetOwnHeight(void) const;

        void                NotifyOwner(DiBillboardSet* owner);

        bool                IsUseTexcoordRect(void) const { return mUseTexcoordRect; }

        void                SetTexcoordIndex(uint16 texcoordIndex);

        uint16              GetTexcoordIndex(void) const { return mTexcoordIndex; }

        void                SetTexcoordRect(const DiFloatRect& texcoordRect);

        void                SetTexcoordRect(float u0, float v0, float u1, float v1);

        const DiFloatRect&  GetTexcoordRect(void) const { return mTexcoordRect; }

        void                SetParentBillboardSet(DiBillboardSet* bbs);

        bool                mOwnDimensions;

        bool                mUseTexcoordRect;
        
        uint16              mTexcoordIndex;
        
        DiFloatRect         mTexcoordRect;
        
        float               mWidth;
                            
        float               mHeight;

    };
}
#endif // Billboard_h__