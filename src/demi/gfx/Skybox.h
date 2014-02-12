


#ifndef DiSkybox_h__
#define DiSkybox_h__


#include "RenderUnit.h"
#include "TransformUnit.h"

namespace Demi
{
    class DI_GFX_API DiSkybox : public DiBase
    {
    public:

        DiSkybox(DiSceneManager* sm);

        virtual            ~DiSkybox();

    public:

        void            Enable(float distance, const DiString& textureName);

        void            Enable(bool enable);

        void            Update();

    protected:

        class DiSkyboxShape : 
            public Demi::DiRenderUnit, 
            public Demi::DiTransformUnit
        {
        public:

            DiSkyboxShape();

            ~DiSkyboxShape();

            DiString&       GetType();

            void            CullingUpdate(Demi::DiRenderBatchGroup*,Demi::DiCamera*);

            void            AddToBatchGroup(DiRenderBatchGroup* bg);

            const DiAABB&   GetBoundingBox(void) const;

            void            Create(float distance, const DiQuat& quat = DiQuat::IDENTITY);
        };

    protected:

        void                CreateMaterial();

    protected:

        float                       mDistance;

        DiString                    mTextureName;

        DiCullNode*                 mNode;

        DiSceneManager*             mSceneManager;

        shared_ptr<DiSkyboxShape>   mSkybox;

        DiMaterialPtr               mSkyMaterial;
    };
}

#endif
