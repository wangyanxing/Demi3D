


#ifndef DiStaticBillboardSet_h__
#define DiStaticBillboardSet_h__


#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include "BillboardSet.h"

namespace Demi
{
    class SBMaterialRef;
    typedef DiMap<DiMaterial*, SBMaterialRef*> SBMaterialRefList;

    class DiStaticBillboardSet
    {
        class StaticBillboard
        {
        public:
            StaticBillboard(const DiVec3 &pos, float xScale, float yScale,
                const DiColor &clr, uint16 texcrdIndexU, uint16 texcrdIndexV) :
                xPos((float)pos.x), yPos((float)pos.y), zPos((float)pos.z), xScaleHalf(0.5f * xScale), yScaleHalf(0.5f * yScale),
                texcoordIndexU(texcrdIndexU), texcoordIndexV(texcrdIndexV)
            {
                color = clr.GetAsARGB();
            }

            float xPos, yPos, zPos;
            float xScaleHalf, yScaleHalf;
            uint32 color;
            uint16 texcoordIndexU, texcoordIndexV;
        };

    public:
        
        DiStaticBillboardSet(DiCullNode *rootNode);

        ~DiStaticBillboardSet();

    public:

        void                    CreateBillboard(const DiVec3 &position, float xScale = 1.0f, float yScale = 1.0f,
                                    const DiColor &color = DiColor::White, 
                                    uint16 texcoordIndexU = 0, uint16 texcoordIndexV = 0);

        void                    SetBillboardOrigin(BillboardOrigin origin);

        BillboardOrigin         GetBillboardOrigin() const   { return mBBOrigin; }

        void                    SetVisible(bool visible);

        void                    SetFade(bool enabled, float visibleDist, float invisibleDist);

        void                    Build();

        void                    Clear();

        void                    SetMaterial(const DiString &materialName);

        void                    SetTextureStacksAndSlices(uint16 stacks, uint16 slices);

        static void             UpdateAll(const DiVec3 &cameraDirection);

    private:

        DiMaterialPtr           GetFadeMaterial(const DiMaterialPtr &protoMaterial, float visibleDist, float invisibleDist);

    private:

        typedef DiVector<StaticBillboard*>  BillboardBuf;

        bool                    mVisible;

        bool                    mFadeEnabled;

        DiSceneManager*         mSceneManager;

        DiCullNode*             mCullNode;

        DiModelPtr              mModel;

        DiMeshPtr               mMesh;

        DiString                mModelName;

        DiMaterialPtr           mMaterial;

        DiMaterialPtr           mFadeMaterial;

        float                   mUFactor;

        float                   mVFactor;

        BillboardOrigin         mBBOrigin;

        float                   mFadeVisibleDist;

        float                   mFadeInvisibleDist;

        BillboardBuf            mBillboardBuffer;

    private:

        typedef std::map<DiString, DiMaterialPtr> FadedMaterialMap;

        static FadedMaterialMap sFadedMaterial;

        static unsigned int     sSelfInstances;

        static unsigned long    GUID;

        static DiString         GetUniqueID(const DiString &prefix);
    };

    //////////////////////////////////////////////////////////////////////////

    class SBMaterialRef
    {
    public:
        static void                         AddMaterialRef(const DiMaterialPtr &matP, BillboardOrigin o);

        static void                         RemoveMaterialRef(const DiMaterialPtr &matP);

        inline static SBMaterialRefList&    GetList() { return sSelfList; }

        inline DiMaterial*                  GetMaterial() { return mMaterial; }

        inline BillboardOrigin              GetOrigin() { return mOrigin; }

    private:

        SBMaterialRef(DiMaterial *mat, BillboardOrigin o);

        static SBMaterialRefList            sSelfList;

        uint32                              mRefCount;

        DiMaterial*                         mMaterial;

        BillboardOrigin                     mOrigin;
    };
}


#endif
