
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef VisualParticle_h__
#define VisualParticle_h__

#include "FxPrerequisites.h"
#include "Particle.h"

namespace Demi
{
    class DEMI_FX_API DiIVisualData
    {
    public:
        DiIVisualData(void) {};

        virtual ~DiIVisualData(void) {};

        virtual void SetVisible(bool visible) = 0;
    };
    
    //////////////////////////////////////////////////////////////////////////

    class DEMI_FX_API DiVisualParticle : public DiParticle
    {
    public:
        DiVisualParticle(void);

        virtual            ~DiVisualParticle(void) {};

    public:

        void            SetOwnDimensions(float newWidth, float newHeight, float newDepth);

        virtual void    InitForEmission(void);

        virtual void    InitForExpiration(DiParticleElement* technique, float timeElapsed);

        void            CalculateBoundingSphereRadius(void);

    public:

        // 当前颜色和原始颜色
        DiColor         colour;

        DiColor         originalColour;

        // zRotation用于在2D空间旋转粒子，即z方向
        // 注意zRotation和下面的orientation之间没有关系，各自独立
        DiRadian        zRotation;

        DiRadian        zRotationSpeed;

        // 粒子的朝向，对一些特殊渲染器，如box渲染器有效
        DiQuat             orientation;

        DiQuat             originalOrientation;

        // 与orientation有关的旋转速度
        float            rotationSpeed;

        // 旋转轴，也与orientation相关
        DiVec3            rotationAxis;

        bool            ownDimensions;

        float             width;

        float             height;

        float             depth;

        // 粒子半径，用于内粒子碰撞
        float            radius;

        float             textureAnimationTimeStep;

        float             textureAnimationTimeStepCount;

        uint16            textureCoordsCurrent;

        bool            textureAnimationDirectionUp;
    };
}

#endif // VisualParticle_h__
