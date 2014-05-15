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

#ifndef BillboardSet_h__
#define BillboardSet_h__

#include "GfxPrerequisites.h"
#include "RenderUnit.h"
#include "TransformUnit.h"

namespace Demi
{
    enum BillboardOrigin
    {
        BBO_TOP_LEFT,
        BBO_TOP_CENTER,
        BBO_TOP_RIGHT,
        BBO_CENTER_LEFT,
        BBO_CENTER,
        BBO_CENTER_RIGHT,
        BBO_BOTTOM_LEFT,
        BBO_BOTTOM_CENTER,
        BBO_BOTTOM_RIGHT
    };

    enum BillboardRotationType
    {
        BBR_VERTEX,
        BBR_TEXCOORD
    };

    enum BillboardType
    {
        BBT_POINT,
        BBT_ORIENTED_COMMON,
        BBT_ORIENTED_SELF,
        BBT_PERPENDICULAR_COMMON,
        BBT_PERPENDICULAR_SELF,
        BBT_ORIENTED_SHAPE
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiBillboardSet : public DiRenderUnit, public DiTransformUnit
    {
    public:

        DiBillboardSet( const DiString& name, unsigned int poolSize = 20, 
            bool externalDataSource = false);

        DiBillboardSet( unsigned int poolSize = 20, 
            bool externalDataSource = false);

        ~DiBillboardSet();

    public:

        const DiAABB&           GetBoundingBox(void) const;

        void                    AddToBatchGroup(DiRenderBatchGroup* bg);

        void                    Update(DiCamera* camera);

        DiBillboard*            CreateBillboard(
                                   const DiVec3& position,
                                   const DiColor& colour = DiColor::White );

        DiBillboard*            CreateBillboard(
                                   float x, float y, float z,
                                   const DiColor& colour = DiColor::White );

        int                     GetNumBillboards(void) const;

        void                    SetAutoextend(bool autoextend);

        bool                    GetAutoextend(void) const;

        void                    SetSortingEnabled(bool sortenable);

        bool                    GetSortingEnabled(void) const;

        void                    SetPoolSize(size_t size);

        unsigned int            GetPoolSize(void) const;

        void                    Clear();

        DiBillboard*            GetBillboard(unsigned int index) const;

        void                    RemoveBillboard(unsigned int index);

        void                    RemoveBillboard(DiBillboard* pBill);

        void                    SetBillboardOrigin(BillboardOrigin origin);

        BillboardOrigin         GetBillboardOrigin(void) const;

        void                    SetBillboardRotationType(BillboardRotationType rotationType);

        BillboardRotationType   GetBillboardRotationType(void) const;

        void                    SetDefaultDimensions(float width, float height);

        void                    SetDefaultWidth(float width);

        float                   GetDefaultWidth(void) const;

        void                    SetDefaultHeight(float height);

        float                   GetDefaultHeight(void) const;

        void                    NotifyCurrentCamera(DiCamera* cam);

        void                    BeginBillboards(size_t numBillboards = 0);

        void                    InjectBillboard(const DiBillboard& bb);

        void                    EndBillboards(void);

        void                    SetBounds(const DiAABB& box);

        void                    NotifyBillboardResized(void);

        void                    NotifyBillboardRotated(void);

        void                    SetBillboardType(BillboardType bbt);

        BillboardType           GetBillboardType(void) const;

        void                    SetCommonDirection(const DiVec3& vec);

        const DiVec3&           GetCommonDirection(void) const;

        void                    SetCommonUpVector(const DiVec3& vec);

        const DiVec3&           GetCommonUpVector(void) const;

        void                    UpdateBounds(void);

        void                    SetBillboardsInWorldSpace(bool ws) { mWorldSpace = ws; }

        void                    SetTextureCoords( DiFloatRect const * coords, uint16 numCoords );

        void                    SetTextureStacksAndSlices( uint8 stacks, uint8 slices );

        DiFloatRect const*      GetTextureCoords( uint16 * oNumCoords );

        void                    SetAutoUpdate(bool autoUpdate);

        bool                    GetAutoUpdate(void) const { return mAutoUpdate; }

        void                    NotifyBillboardDataChanged(void) { mBillboardDataChanged = true; }

        DiSortMode              GetSortMode(void) const;

        virtual DiString&       GetType();

    protected:

        virtual void            IncreasePool(size_t size);
       
        void                    GenBillboardAxes(DiVec3* pX, DiVec3 *pY, const DiBillboard* pBill = 0);

        void                    GetParametricOffsets(float& left, float& right, float& top, float& bottom);

        void                    GenVertices(const DiVec3* const offsets, const DiBillboard& pBillboard);

        void                    GenVertOffsets(float inleft, float inright, float intop, float inbottom,
                                    float width, float height,
                                    const DiVec3& x, const DiVec3& y, DiVec3* pDestVec);

    protected:

        typedef DiList<DiBillboard*>    ActiveBillboardList;
        typedef DiList<DiBillboard*>    FreeBillboardList;
        typedef DiVector<DiBillboard*>  BillboardPool;
        typedef DiVector<DiFloatRect>   TextureCoordSets;

        DiAABB                          mAABB;

        float                           mBoundingRaidus;

        BillboardOrigin                 mOriginType;

        BillboardRotationType           mRotationType;

        float                           mDefaultWidth;

        float                           mDefaultHeight;

        bool                            mAllDefaultSize;

        bool                            mAutoExtendPool;

        bool                            mSortingEnabled;

        bool                            mAccurateFacing;

        bool                            mAllDefaultRotation;

        bool                            mWorldSpace;

        ActiveBillboardList             mActiveBillboards;

        FreeBillboardList               mFreeBillboards;

        BillboardPool                   mBillboardPool;

        float*                          mLockPtr;

        uint32                          mLockSize;

        DiVec3                          mVOffset[4];

        DiCamera*                       mCurrentCamera;

        float                           mLeftOff, mRightOff, mTopOff, mBottomOff;

        DiVec3                          mCamX, mCamY;

        DiVec3                          mCamDir;

        DiQuat                          mCamQ;

        DiVec3                          mCamPos;

        TextureCoordSets                mTextureCoords;

        BillboardType                   mBillboardType;

        DiVec3                          mCommonDirection;

        DiVec3                          mCommonUpVector;

        unsigned short                  mNumVisibleBillboards;

    private:

        bool                            mBuffersCreated;
       
        size_t                          mPoolSize;
       
        bool                            mExternalData;
        
        bool                            mAutoUpdate;
        
        bool                            mBillboardDataChanged;

        void                            CreateBuffers(void);

        void                            DestroyBuffers(void);
    };
}

#endif // BillboardSet_h__