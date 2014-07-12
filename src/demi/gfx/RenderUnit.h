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

#ifndef DiRenderUnit_h__
#define DiRenderUnit_h__


#include "GfxPrerequisites.h"
#include "Material.h"

namespace Demi 
{
    class DI_GFX_API DiRenderUnit : public DiBase
    {
    public:
        DiRenderUnit(void);

        virtual ~DiRenderUnit(void);

    public:

        enum Flag
        {
            RU_FLAG_NO_PRIMITIVE_STATIS = 1<<0,
            RU_FLAG_NO_BACHES_STATIS    = 1<<1,
        };

        virtual void            GetWorldTransform(DiMat4* xform) const;

        virtual uint32          GetWorldTransformNums() const{return 1;}

        virtual DiMaterialPtr   SetMaterial(const DiString& name);

        virtual void            PreRender(){}

        void                    SetMaterial(DiMaterialPtr mat);

        DiMaterialPtr           GetMaterial() {return mMaterial;}

        void                    ReleaseSourceData();

        void                    ReleaseIndexBuffer();

        void                    ReleaseVertexDeclaration();

        /** Set some special flags, e.g. no batch statistics 
         */
        void                    SetFlag(uint32 flag){mFlags = flag;}

        uint32                  GetFlag()const {return mFlags;}

    public:
        
        DiVector<DiVertexBuffer*>   mSourceData;

        DiIndexBuffer*              mIndexBuffer;    

        DiVertexDeclaration*        mVertexDecl;

        DiPrimitiveType             mPrimitiveType;

        uint32                      mPrimitiveCount;

        uint32                      mIndexOffset;

        uint32                      mVertexOffset;

        uint32                      mVerticesNum;

        uint32                      mBoneNum;

        DiMat4*                     mBoneMatrices;

        DiMaterialPtr               mMaterial;

        bool                        mIdentityProjection;

        uint32                      mFlags;
        
        std::function<void(DiMat4* xform)> mEventUpdateTransform;
    };

}

#endif
