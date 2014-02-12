


#ifndef DiRenderPipeline_h__
#define DiRenderPipeline_h__


#include "GfxPrerequisites.h"
#include "RenderBatchGroup.h"
#include "GpuProgram.h"
#include "Pool.h"

namespace Demi 
{

    class DI_GFX_API DiRenderPipeline : public DiBase
    {
    public:

        enum Pass
        {
            P_GBUFFER_PASS,
            P_SHADOW_PASS,
            P_LIGHTING_PASS,
            P_CUSTOM_RTT_PASS
        };

        DiRenderPipeline();

        ~DiRenderPipeline();

        friend class DiRenderBatchGroup;

        typedef DiVector<DiRenderBatchGroup*> BatchGroups;
        typedef DiObjectPool<DiMat4>        MatrixPool;
        struct DiMat4Array
        {
            DiMat4    matrixArray[MAX_WORLD_MATS];
        };
        typedef DiObjectPool<DiMat4Array>    MatrixArrayPool;

    public:

        DiShaderEnvironment*        GetShaderEnvironment() const {return mShaderEnv;}

        DiRenderBatchGroup*         GetBatchGroup(uint32 typeID);

        void                        ClearGroup();

        void                        Render(DiSceneManager*sm, DiCamera* cam, DiRenderTarget* rt);

        void                        Init();

        void                        Release();

        void                        BindMeshContext(DiRenderUnit* batch,DiMat4* mat);

        void                        BindShadowCamera(DiCamera* cam);

        bool                        RenderSingleBatch(DiRenderUnit* batch, DiMat4* mat);

        void                        Render2DBatch(DiRenderUnit* batch, DiMaterial* material);

        MatrixPool&                 GetMatrixPool() { return mMatrixPool; }

        MatrixArrayPool&            GetMatricsArrayPool() { return mMatricsArrayPool; }

        bool                        GetPostEnable() const { return mPostEnable; }

        void                        SetPostEnable(bool val) { mPostEnable = val; }

        Pass                        GetCurrentPass() const { return mCurrentPass; }

        void                        SetCurrentPass(Pass val) { mCurrentPass = val; }

        void                        SetCurrentSceneManager(DiSceneManager* val) { mCurrentSceneMgr = val; }

        void                        ProcessGBuffers();

        void                        SetGlobalParams(DiSceneManager*sm, DiCamera* cam);

        bool                        GetEnableGBuffer() const { return mEnableGbuffer; }

        void                        SetEnableGBuffer(bool v) { mEnableGbuffer = v; }

    protected:

        DiShaderEnvironment*        mShaderEnv;

        BatchGroups                 mBatchGroups;

        DiViewport*                 mActiveVp;

        bool                        mEnableInterface;

        MatrixPool                  mMatrixPool;

        MatrixArrayPool             mMatricsArrayPool;

        bool                        mPostEnable;

        Pass                        mCurrentPass;

        DiSceneManager*             mCurrentSceneMgr;

        bool                        mEnableGbuffer;
    };
}

#endif
