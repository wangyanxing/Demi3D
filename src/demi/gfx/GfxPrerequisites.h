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

#ifndef DiGfxPrerequisites_h__
#define DiGfxPrerequisites_h__

#include "CommonEnum.h"
#include "Str.h"

namespace Demi 
{
    class    DiLogger;
    class    DiArchive;
    class    DiFileArchive;
    class    DiZipArchive;
    class    DiArchiveManager;
    class    DiSystem;
    class    DiGpuVariable;
    class    DiShaderEnvironment;
    class    DiMaterial;
    class    DiShaderParameter;
    class    DiAsset;
    class    DiAssetManager;
    class    DiTexture;
    class    DiCamera;
    class    DiViewport;
    class    DiGraphics;
    class    DiRenderPipeline;
    class    DiRenderBatchGroup;
    class    DiRenderUnit;
    class    DiTransformUnit;
    class    DiRenderVisitor;
    class    DiNode;
    class    DiCullNode;
    class    DiCullUnit;
    class    DiSceneCuller;
    class    DiOctreeCuller;
    class    DiOctreeCullUnit;
    class    DiFrustum;
    class    DiSceneManager;
    class    DiStateManager;
    class    DiLight;
    class    DiDirLight;
    class    DiPointLight;
    class    DiSkyLight;
    class    DiDeviceLostListener;
    class    DiRenderTarget;
    class    DiVertexBuffer;
    class    DiIndexBuffer;
    class    DiVertexDeclaration;
    class    DiVertexElements;
    class    DiRenderWindow;
    class    DiRenderDevice;
    class    DiShaderProgram;
    class    DiQuadNode;
    class    DiSprite;
    class    DiPostEffectRt;
    class    DiPostEffect;
    class    DiPostEffectPass;
    class    DiPostEffectManager;
    class    DiPixelBox;
    class    DiBox;
    class    DiRandomTable;
    class    DiInstanceBatch;
    class    DiInstanceManager;
    class    DiInstancedModel;
    class    DiInstanceBatchShader;
    class    DiInstanceBatchHardware;
    class    DiOctree;
    class    DiAABBQuery;
    class    DiSphereSceneQuery;
    class    DiPBVListSceneQuery;
    class    DiRaySceneQuery;
    class    DiShadowManager;
    class    DiTransPlane;
    class    DiDepthBuffer;
    class    DiSkybox;
    class    DiShaderManager;
    class    DiGBuffer;
    class    DiBillboard;
    class    DiBillboardSet;
    class    DiScene;
    class    DiMotion;
    class    DiEntity;
    class    DiSceneNode;
    class    DiOctant;
    class    DiOctantRoot;
    class    DiGfxDriver;
    class    DiShaderInstance;
    class    DiTextureDrv;
    class    DiWindow;
    class    DiWindowManager;
    class    DiCommandManager;
    class    DiConsoleVar;
    class    DiConsoleVarListener;
    struct   DiFoliageLayerDesc;
    struct   DiRenderUnitList;
    class    DiDebugHelper;
    class    DiMesh;
    class    DiSubMesh;
    class    DiModel;
    class    DiAnimModel;
    class    DiSubModel;
    class    DiObject;
    class    DiBone;
    class    DiSkeleton;
    class    DiSkeletonInstance;
    class    DiAttachNode;
    class    DiAttachSet;
    class    DiAttachSetInstance;
    class    DiKeyFrame;
    class    DiTransformKeyFrame;
    class    DiAnimation;
    class    DiAnimationClip;
    class    DiClipController;
    class    DiClipControllerSet;
    class    DiNodeClip;
    class    DiPostController;
    class    DiConsoleLogger;
    class    DiSimpleShape;
    class    DiGfxCaps;
    class    DiTransAxes;

    class    DiBspNode;
    class    DiBspScene;

    class BoneMemoryManager;
    class BoneArrayMemoryManager;
    class SkeletonDef;
    class SkeletonInstance;
    class SkeletonManager;
    class SkeletonAnimation;
    class SimpleMatrixAf4x3;
    class DiNewBone;

    typedef shared_ptr<DiOctree>            DiOctreePtr;
    typedef shared_ptr<DiOctreeCullUnit>    DiOctreeCullUnitPtr;
    typedef shared_ptr<DiCullUnit>          DiCullUnitPtr;
    typedef shared_ptr<DiAnimModel>         DiAnimModelPtr;
    typedef shared_ptr<StringVec>           DiStringVecPtr;
    typedef shared_ptr<DiArchive>           ArchivePtr;
    typedef shared_ptr<DiAsset>             DiAssetPtr;
    typedef shared_ptr<DiDebugHelper>       DiDebugHelperPtr;
    typedef shared_ptr<DiDirLight>          DiDirLightPtr;
    typedef shared_ptr<DiInstanceBatch>     DiInstanceBatchPtr;
    typedef shared_ptr<DiInstancedModel>    DiInstancedModelPtr;
    typedef shared_ptr<DiMaterial>          DiMaterialPtr;
    typedef shared_ptr<DiMesh>              DiMeshPtr;
    typedef shared_ptr<DiModel>             DiModelPtr;
    typedef shared_ptr<DiMotion>            DiMotionPtr;
    typedef shared_ptr<DiPointLight>        DiPointLightPtr;
    typedef shared_ptr<DiScene>             DiScenePtr;
    typedef shared_ptr<DiSimpleShape>       DiSimpleShapePtr;
    typedef shared_ptr<DiSkyLight>          DiSkyLightPtr;
    typedef shared_ptr<DiTexture>           DiTexturePtr;
    typedef shared_ptr<DiTransformUnit>     DiTransUnitPtr;
    typedef shared_ptr<DiBillboardSet>      DiBillboardSetPtr;
    typedef shared_ptr<DiTransAxes>    DiTransAxesPtr;
}

#include "GfxBase.h"


#endif
