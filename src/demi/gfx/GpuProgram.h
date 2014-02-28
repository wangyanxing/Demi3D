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

#ifndef DiGpuProgram_h__
#define DiGpuProgram_h__


#include "Skeleton.h"
#include "GfxDriver.h"
#include "Shadows.h"
#include "Any.h"

#include <functional>

namespace Demi 
{
    enum DiVsVersion
    {
        VS_INVALID = -1,

        VS_NONE    = 0,
        VS_1_1,
        VS_2_0,
        VS_2_a,
        VS_3_0,
        VS_4_0,

        NUM_VERTEX_SHADER_VERSIONS
    };

    enum DiPsVersion
    {
        PS_INVALID = -1,

        PS_NONE    = 0,
        PS_1_1,
        PS_1_2,
        PS_1_3,
        PS_1_4,
        PS_2_0,
        PS_2_b,
        PS_2_a,
        PS_3_0,
        PS_4_0,

        NUM_PIXEL_SHADER_VERSIONS
    };

    typedef DiPair<DiString,DiString>   DiMarcoDefine;
    typedef DiVector<DiMarcoDefine>     DiMarcoDefineList;

    struct DiCompileDesc
    {

        void                AddMarco(const DiString& name, const DiString& def = DiString::BLANK);

        void                RemoveMarco(const DiString& name);

        DiString            profile;

        DiMarcoDefineList   marcos;

        DiString            entryName;
    };

    //////////////////////////////////////////////////////////////////////////
    class DiShaderEnvironment
    {
    public:
        DiShaderEnvironment(void);

    public:
        DiMat4        modelMatrix;

        DiMat4        viewMatrix;
        DiMat4        projMatrix;
        DiMat4        modelViewMatrix;
        DiMat4        modelViewProjMatrix;
        DiMat4        viewProjMatrix;
        DiMat4        texMatrix;

        DiVec3        eyePosition;
        DiVec3        eyeDirection;

        DiVec4        farnearPlane;

        DiColor       globalAmbient;
        DiColor       ambientColor;
        DiColor       diffuseColor;
        DiColor       specularColor;
        float         opacity;
        float         shininess;

        DiVec4        boneMatrices[MAX_BONE_NUM * 3];
        DiVec4        modelMatrices[MAX_WORLD_MATS * 3];
        uint32        numBones;
        uint32        numModelMatrices;

        float         time;
        DiVec4        viewportSize;
        DiVec4        texelOffsets;

        int           numDirLights;
        DiColor       dirLightsColor[MAX_LIGHTS];
        DiVec4        dirLightsDir[MAX_LIGHTS];

        int           numPointLights;
        DiColor       pointLightsColor[MAX_LIGHTS];
        DiVec4        pointLightsPosition[MAX_LIGHTS];
        DiVec4        pointLightsAttenuation[MAX_LIGHTS];

        bool          hasSkyLight;
        DiColor       skyLightColor;
        DiColor       groundColor;
        DiVec3        skyLightDir;

        void          SetIdentity();
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_GFX_API DiShaderInstance : public DiBase
    {
    public:

        DiShaderInstance(DiShaderType type){}
        
        virtual                 ~DiShaderInstance(){}
        

        virtual void            Bind(const DiShaderEnvironment& shaderEnv) = 0;

        virtual void            Release() = 0;

        virtual DiShaderType    GetType() = 0;

        virtual bool            Compile(const DiString& code) = 0;

        DiCompileDesc           mCompileDesc;
    };
}

#endif
