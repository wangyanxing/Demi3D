
#include "D3D9Shader.h"
#include "AssetManager.h"
#include "Texture.h"

namespace Demi
{
    DiShaderConstants::DiShaderConstants(DiShaderType tp)
    {
        type = tp;
        table = NULL;

        modelMatrix = -1;
        viewMatrix = -1;
        projMatrix = -1;
        modelViewMatrix = -1;
        modelViewProjMatrix = -1;
        viewProjMatrix = -1;
        texViewProjMatrix = -1;

        eyePosition = -1;
        eyeDirection = -1;
        eyePositionObjSpace = -1;

        farnearPlane = -1;

        boneMatrices = -1;
        modelMatrices = -1;

        globalAmbient = -1;
        ambientColor = -1;
        diffuseColor = -1;
        specularColor = -1;
        opacity = -1;
        shininess = -1;

        time = -1;
        viewportSize = -1;
        texelOffsets = -1;

        numDirLights = -1;
        dirLightsColor = -1;
        dirLightsDir = -1;

        numPointLights = -1;
        pointLightsColor = -1;
        pointLightsPosition = -1;
        pointLightsAttenuation = -1;
    }

    DiShaderConstants::~DiShaderConstants(void)
    {
        SAFE_RELEASE(table);
    }

    void DiShaderConstants::LoadConstants(void)
    {
        if (table)
        {
            modelMatrix = GetRegisterID("g_modelMatrix");
            viewMatrix = GetRegisterID("g_viewMatrix");
            projMatrix = GetRegisterID("g_projMatrix");
            modelViewMatrix = GetRegisterID("g_modelViewMatrix");
            modelViewProjMatrix = GetRegisterID("g_modelViewProjMatrix");
            viewProjMatrix = GetRegisterID("g_viewProjMatrix");
            texViewProjMatrix = GetRegisterID("g_texViewProjMatrix");

            eyePosition = GetRegisterID("g_eyePosition");
            eyeDirection = GetRegisterID("g_eyeDirection");
            eyePositionObjSpace = GetRegisterID("g_eyePositionObjSpace");

            farnearPlane = GetRegisterID("g_farnearPlane");

            boneMatrices = GetRegisterID("g_boneMatrices");
            modelMatrices = GetRegisterID("g_modelMatrices");

            globalAmbient = GetRegisterID("g_globalAmbient");
            ambientColor = GetRegisterID("g_ambientColor");
            diffuseColor = GetRegisterID("g_diffuseColor");
            specularColor = GetRegisterID("g_specularColor");
            opacity = GetRegisterID("g_opacity");
            shininess = GetRegisterID("g_shininess");

            time = GetRegisterID("g_time");
            viewportSize = GetRegisterID("g_viewportSize");
            texelOffsets = GetRegisterID("g_texelOffsets");

            numDirLights = GetRegisterID("g_numDirLights");
            dirLightsColor = GetRegisterID("g_dirLightsColor");
            dirLightsDir = GetRegisterID("g_dirLightsDir");

            numPointLights = GetRegisterID("g_numPointLights");
            pointLightsColor = GetRegisterID("g_pointLightsColor");
            pointLightsPosition = GetRegisterID("g_pointLightsPosition");
            pointLightsAttenuation = GetRegisterID("g_pointLightsAttenuation");
        }
    }

    void DiShaderConstants::BindEnvironment(const DiShaderEnvironment& shaderEnv) const
    {
        IDirect3DDevice9* sm = DiD3D9Driver::Device;

        if (table)
        {
#define SET_CONST(_name) Driver->SetShaderConsts(type,_name,shaderEnv._name)

            if (shaderEnv.numModelMatrices > 1 && modelMatrices != -1)
            {
                // 4x3
                sm->SetVertexShaderConstantF(modelMatrices,
                    (float*)(&shaderEnv.modelMatrices), shaderEnv.numModelMatrices * 3);
            }
            else
            {
                // 4x4
                SET_CONST(modelMatrix);
            }

            SET_CONST(viewMatrix);
            SET_CONST(projMatrix);
            SET_CONST(modelViewMatrix);
            SET_CONST(modelViewProjMatrix);
            SET_CONST(viewProjMatrix);
            SET_CONST(texViewProjMatrix);

            SET_CONST(eyePosition);
            SET_CONST(eyeDirection);
            SET_CONST(eyePositionObjSpace);
            SET_CONST(farnearPlane);

            SET_CONST(time);
            SET_CONST(viewportSize);
            SET_CONST(texelOffsets);

            SET_CONST(numDirLights);
            if (shaderEnv.numDirLights > 0 && dirLightsColor != -1)
            {
                if (type == SHADER_VERTEX)
                {
                    sm->SetVertexShaderConstantF(dirLightsColor,
                        (float*)(&shaderEnv.dirLightsColor[0].r), shaderEnv.numDirLights);
                    sm->SetVertexShaderConstantF(dirLightsDir,
                        (float*)(&shaderEnv.dirLightsDir[0].x), shaderEnv.numDirLights);
                }
                else
                {
                    sm->SetPixelShaderConstantF(dirLightsColor,
                        (float*)(&shaderEnv.dirLightsColor[0].r), shaderEnv.numDirLights);
                    sm->SetPixelShaderConstantF(dirLightsDir,
                        (float*)(&shaderEnv.dirLightsDir[0].x), shaderEnv.numDirLights);
                }
            }

            SET_CONST(numPointLights);
            if (shaderEnv.numPointLights > 0 && pointLightsColor != -1)
            {
                if (type == SHADER_VERTEX)
                {
                    sm->SetVertexShaderConstantF(pointLightsColor,
                        (float*)(&shaderEnv.pointLightsColor[0].r), shaderEnv.numPointLights);
                    sm->SetVertexShaderConstantF(pointLightsPosition,
                        (float*)(&shaderEnv.pointLightsPosition[0].x), shaderEnv.numPointLights);
                    sm->SetVertexShaderConstantF(pointLightsAttenuation,
                        (float*)(&shaderEnv.pointLightsAttenuation[0].x), shaderEnv.numPointLights);
                }
                else
                {
                    sm->SetPixelShaderConstantF(pointLightsColor,
                        (float*)(&shaderEnv.pointLightsColor[0].r), shaderEnv.numPointLights);
                    sm->SetPixelShaderConstantF(pointLightsPosition,
                        (float*)(&shaderEnv.pointLightsPosition[0].x), shaderEnv.numPointLights);
                    sm->SetPixelShaderConstantF(pointLightsAttenuation,
                        (float*)(&shaderEnv.pointLightsAttenuation[0].x), shaderEnv.numPointLights);
                }
            }

            SET_CONST(globalAmbient);
            SET_CONST(ambientColor);
            SET_CONST(diffuseColor);
            SET_CONST(specularColor);
            SET_CONST(opacity);
            SET_CONST(shininess);

            if (boneMatrices != -1)
            {
                sm->SetVertexShaderConstantF(boneMatrices,
                    (float*)(&shaderEnv.boneMatrices), shaderEnv.numBones * 3);
            }

#undef SET_CONST
        }
    }

    int DiShaderConstants::GetRegisterID(const char* name)
    {
        DI_ASSERT(table);
        D3DXHANDLE handle = table->GetConstantByName(0, name);
        if (!handle)
        {
            return -1;
        }
        D3DXCONSTANT_DESC desc;
        unsigned int numParams = 1;
        HRESULT hr = table->GetConstantDesc(handle, &desc, &numParams);
        DX9_CHKERR(hr);
        return desc.RegisterIndex;
    }

    STDMETHODIMP DiShaderIncluder::Open(THIS_ D3DXINCLUDE_TYPE includeType,
        LPCSTR fileName, LPCVOID,
        LPCVOID *data, uint32 *dataSize)
    {
        HRESULT result = D3DERR_NOTFOUND;

        char fullpath[1024];
        strcpy_s(fullpath, 1024, DiAssetManager::GetInstance().GetBasePath().c_str());
        if (includeType == D3DXINC_SYSTEM)
        {
            strcat_s(fullpath, 1024, "shaders/include/");
        }
        strcat_s(fullpath, 1024, fileName);

        FILE *file = 0;
        fopen_s(&file, fullpath, "r");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            size_t fileLen = ftell(file);
            if (fileLen > 1)
            {
                fseek(file, 0, SEEK_SET);
                char *fileData = DI_NEW char[fileLen + 1];
                fileLen = fread(fileData, 1, fileLen, file);
                fileData[fileLen] = 0;
                *data = fileData;
                *dataSize = (uint32)fileLen;
            }
            fclose(file);
            result = D3D_OK;
        }
        DI_ASSERT(result == D3D_OK);
        return result;
    }

    STDMETHODIMP DiShaderIncluder::Close(THIS_ LPCVOID data)
    {
        delete[]((char*)data);
        return D3D_OK;
    }
}