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

#ifndef DiK2Prerequisites_h__
#define DiK2Prerequisites_h__

#include "K2Defines.h"

namespace Demi
{
    class DiK2Model;
    class DiK2Animation;
    class DiK2Clip;
    class DiK2Skeleton;
    class DiK2ModelAsset;
    class DiK2BonesData;
    class DiK2World;
    class DiTerrain;
    class DiTerrainChunk;
    class DiTerrainDesc;
    class DiWaterMap;
    class DiWaterChunk;
    class DiFoliageMap;
    class DiFoliageChunk;
    class DiFoliageLayer;
    class DiCliffMap;
    class DiCliff;
    class DiK2TileMap;
    class DiK2VertexColorMap;
    class DiK2TileCliffMap;
    class DiK2HeightMap;
    class DiK2VertexBlockerMap;
    class DiK2RenderObject;
    class DiK2StaticObj;
    class DiK2AnimatedObj;

    struct K2KeyFrames;
    struct K2Anim;

    template <typename T>
    class DiParticlePoolBase;
    class DiParticleSystemListener;
    class DiElementListener;
    class DiParticle;
    class DiParticleEmitter;
    class DiParticleController;
    class DiParticleSystem;
    class DiParticleElement;
    class DiEffectManager;
    class DiElementListener;
    class DiIVisualData;
    class DiVisualParticle;
    class DiParticleRenderer;
    class DiParticlePool;
    class DiParticleSystemFactory;
    class DiParticleEmitterFactory;
    class DiParticleControllerFactory;
    class DiParticleRendererFactory;
    class DiPointEmitter;
    class DiBoxEmitter;
    class DiCircleEmitter;
    class DiLineEmitter;
    
    class DiColorController;
    class DiVortexController;
    class DiJetController;
    class DiScaleController;
    class DiGravityController;
    class DiGeometryRotatorController;
    class DiTextureRotatorController;
    class DiRandomiserController;
    class DiBaseForceController;
    class DiSineForceController;
    class DiLinearForceController;
    class DiBaseColliderController;
    class DiBoxColliderController;
    class DiPlaneColliderController;
    class DiSphereColliderController;
    
    struct DiFxEvent;

    typedef shared_ptr<DiK2Model>        DiK2ModelPtr;
    typedef shared_ptr<DiK2BonesData>    DiK2BonesDataPtr;
    typedef shared_ptr<DiK2ModelAsset>   DiK2ModelAssetPtr;
    typedef shared_ptr<DiTerrainDesc>    DiTerrainDescPtr;
    typedef shared_ptr<DiTerrain>        DiTerrainPtr;
    typedef shared_ptr<DiParticleSystem> DiParticleSystemPtr;
}

#include "K2Coords.h"
#include "K2Common.h"
#include "EffectCommon.h"

#endif
