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

#include "DrvGLES2Pch.h"
#include "ES2Shader.h"
#include "ES2ShaderParam.h"
#include "GLES2Driver.h"
#include "ES2Util.h"
#include "ES2TypeMappings.h"
#include "ShaderProgram.h"
#include "AssetManager.h"
#include "ES2ShaderPipeline.h"

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "EAGL2Util.h"
#endif

namespace Demi
{
    DiGLES2ShaderLinker::CustomAttribute DiGLES2ShaderLinker::msCustomAttributes[] = 
    {
        CustomAttribute("Position",     DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_POSITION,        0)),
        CustomAttribute("BlendWeights", DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_BLENDWEIGHT,     0)),
        CustomAttribute("Normal",       DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_NORMAL,          0)),
        CustomAttribute("Color",        DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_COLOR,           0)),
        CustomAttribute("SecondColor",  DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_SECONDARY_COLOR, 0)),
        CustomAttribute("BlendIndices", DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_BLENDINDICES,    0)),
        CustomAttribute("Texcoord0",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        0)),
        CustomAttribute("Texcoord1",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        1)),
        CustomAttribute("Texcoord2",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        2)),
        CustomAttribute("Texcoord3",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        3)),
        CustomAttribute("Texcoord4",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        4)),
        CustomAttribute("Texcoord5",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        5)),
        CustomAttribute("Texcoord6",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        6)),
        CustomAttribute("Texcoord7",    DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        7)),
        CustomAttribute("Tangent",      DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TANGENT,         0)),
        CustomAttribute("Binormal",     DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_BINORMAL,        0)),
    };

    DiGLES2ShaderInstance::DiGLES2ShaderInstance(DiShaderType type, DiShaderProgram* prog) : DiShaderInstance(type)
        , mShaderProgram(prog)
        , mShaderHandle(0)
        , mGLProgramHandle(0)
        , mType(type)
        , mCompiled(0)
        , mLinked(0)
    {
    }

    DiGLES2ShaderInstance::~DiGLES2ShaderInstance()
    {
    }

    bool DiGLES2ShaderInstance::Compile(const DiString& code)
    {
        if (code.empty())
            return false;

        // check the includes firstly
        PorcessShaders(code);

        GLenum shaderType = 0;
        switch (mType)
        {
        case SHADER_VERTEX:
            shaderType = GL_VERTEX_SHADER;
            break;
        case SHADER_PIXEL:
            shaderType = GL_FRAGMENT_SHADER;
            break;
        default:
            DI_WARNING("Invalid shader type");
        }
        CHECK_GL_ERROR( mShaderHandle = glCreateShader(shaderType) );

        if (DiGLES2Driver::GLUtil->CheckExtension("GL_EXT_debug_label"))
        {
            IF_IOS_VERSION_IS_GREATER_THAN(5.0)
                glLabelObjectEXT(GL_SHADER_OBJECT_EXT, mShaderHandle, 0, mShaderProgram->GetShaderFileName().c_str());
        }

        if (Driver->GetGfxCaps()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
        {
            CHECK_GL_ERROR(mGLProgramHandle = glCreateProgram());

            if (DiGLES2Driver::GLUtil->CheckExtension("GL_EXT_debug_label"))
            {
                IF_IOS_VERSION_IS_GREATER_THAN(5.0)
                    glLabelObjectEXT(GL_PROGRAM_OBJECT_EXT, mGLProgramHandle, 0, mShaderProgram->GetShaderFileName().c_str());
            }
        }

        const char *source = mProcessedShader.c_str();
        glShaderSource(mShaderHandle, 1, &source, NULL);

        glCompileShader(mShaderHandle);

        CHECK_GL_ERROR(glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &mCompiled));

        if (!mCompiled)
        {
            DiString msg;
            msg.Format("GLSL compile info: %s\n", mShaderProgram->GetShaderFileName().c_str());
            LogObjectInfo(msg, mShaderHandle);
            return false;
        }
        return mCompiled == 1;
    }

    void DiGLES2ShaderInstance::Bind(const DiShaderEnvironment& shaderEnv)
    {
    }

    void DiGLES2ShaderInstance::Release()
    {
        CHECK_GL_ERROR(glDeleteShader(mShaderHandle));

        if (Driver->GetGfxCaps()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
        {
            CHECK_GL_ERROR(glDeleteProgram(mGLProgramHandle));
        }

        mShaderHandle = 0;
        mGLProgramHandle = 0;
        mCompiled = 0;
    }

    DiString DiGLES2ShaderInstance::LogObjectInfo(const DiString& msg, const GLuint obj)
    {
        DiString logMessage = msg;

        if (obj > 0)
        {
            GLint infologLength = 0;

            if (glIsShader(obj))
            {
                CHECK_GL_ERROR(glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength));
            }
            else if (glIsProgram(obj))
            {
                CHECK_GL_ERROR(glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength));
            }
            else if (Driver->GetGfxCaps()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
            {
                IF_IOS_VERSION_IS_GREATER_THAN(5.0)
                {
                    if (glIsProgramPipelineEXT(obj))
                        CHECK_GL_ERROR(glGetProgramPipelineivEXT(obj, GL_INFO_LOG_LENGTH, &infologLength));
                }
            }

            if (infologLength > 1)
            {
                GLint charsWritten = 0;

                char * infoLog = new char[infologLength];
                infoLog[0] = 0;

                if (glIsShader(obj))
                {
                    CHECK_GL_ERROR(glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog));
                }
                else if (glIsProgram(obj))
                {
                    CHECK_GL_ERROR(glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog));
                }
                else if (Driver->GetGfxCaps()->hasCapability(RSC_SEPARATE_SHADER_OBJECTS))
                {
                    IF_IOS_VERSION_IS_GREATER_THAN(5.0)
                    {
                        if (glIsProgramPipelineEXT(obj))
                            CHECK_GL_ERROR(glGetProgramPipelineInfoLogEXT(obj, infologLength, &charsWritten, infoLog));
                    }
                }

                if (strlen(infoLog) > 0)
                    logMessage += "\n" + DiString(infoLog);

                delete[] infoLog;

                if (logMessage.size() > 0)
                {
                    DI_LOG(logMessage.c_str());
                }
            }
        }
        
        return logMessage;
    }

    void DiGLES2ShaderInstance::LinkToProgramObject(const GLuint programObject)
    {
        if (!mShaderHandle)
        {
            DI_WARNING("Shader should be compiled before linking.");
            return;
        }

        CHECK_GL_ERROR(glAttachShader(programObject, mShaderHandle));
    }

    void DiGLES2ShaderInstance::UnlinkToProgramObject(const GLuint programObject)
    {
        CHECK_GL_ERROR(glDetachShader(programObject, mShaderHandle));
    }

    void DiGLES2ShaderInstance::PorcessShaders(const DiString& code)
    {
        mProcessedShader.clear();

        // version
        // the #version should be the first statement on some platform
        mProcessedShader += "#version ";
        mProcessedShader += DiGLES2Driver::GetGlslVersion();
        mProcessedShader += "\n";

        mProcessedShader += "#define DI_GLES2\n";

        // precious
        mProcessedShader += "precision highp int;\n";
        mProcessedShader += "precision highp float;\n";

        // defines
        for (auto i = mCompileDesc.marcos.begin(); i != mCompileDesc.marcos.end(); ++i)
        {
            DiString line;
            line.Format("#define %s %s\n", i->first.c_str(), i->second.c_str());
            mProcessedShader += line;
        }
        mProcessedShader += "\n";
        mProcessedShader += code;

        // includes
        for (;;)
        {
            DiString::size_type pos = mProcessedShader.find("#include");
            if (pos != DiString::npos)
            {
                DiString::size_type first = 0, second = 0;
                for (auto i = pos + 7; i < mProcessedShader.size() && mProcessedShader[i] != '\n'; ++i)
                {
                    if (mProcessedShader[i] == '\"')
                    {
                        if (first == 0)
                            first = i;
                        else
                            second = i;
                    }
                    if (second != 0)
                        break;
                }
                DiString filename = mProcessedShader.substr(first + 1, second - first - 1);
                DiString fullinclude = mProcessedShader.substr(pos, second - pos + 1);

                DiDataStreamPtr buf = DiAssetManager::GetInstance().OpenArchive(filename);
                if (!buf)
                {
                    DI_WARNING("Cannot locate the include glsl file : %s", filename.c_str());
                    return;
                }
                mProcessedShader.Replace(fullinclude.c_str(), buf->GetAsString().c_str());
            }
            else
            {
                break;
            }
        }
    }

    DiGLES2ShaderLinker::DiGLES2ShaderLinker(DiGLES2ShaderInstance* vs, DiGLES2ShaderInstance* ps)
        : mGLHandle(0)
        , mVS(vs)
        , mPS(ps)
        , mLinked(0)
    {
    }

    DiGLES2ShaderLinker::~DiGLES2ShaderLinker()
    {
        CHECK_GL_ERROR(glDeleteProgram(mGLHandle));

        for (auto i = mConsts.begin(); i != mConsts.end(); ++i)
        {
            DI_DELETE i->second;
        }
        for (auto i = mSamplers.begin(); i != mSamplers.end(); ++i)
        {
            DI_DELETE i->second;
        }
    }

    void DiGLES2ShaderLinker::Bind()
    {
        if (mLinked)
        {
            CHECK_GL_ERROR(glUseProgram(mGLHandle));
        }
    }

    void DiGLES2ShaderLinker::LoadConstants(DiGLES2ShaderParam* params)
    {
        if (DiGLUniforms::msUniformFuncs.empty())
            DiGLUniforms::InitUniformFuncs();

        char* uniformName = NULL;
        GLint maxLength = 0;
        CHECK_GL_ERROR(glGetProgramiv(mGLHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength));

        // If the max length of active uniforms is 0, then there are 0 active.
        // There won't be any to extract so we can return.
        if (maxLength == 0)
            return;

        uniformName = new char[maxLength + 1];

        mSamplers.clear();
        GLint uniformCount = 0;
        CHECK_GL_ERROR(glUseProgram(mGLHandle));
        CHECK_GL_ERROR(glGetProgramiv(mGLHandle, GL_ACTIVE_UNIFORMS, &uniformCount));
        
        for (int index = 0; index < uniformCount; index++)
        {
            GLint arraySize = 0;
            GLenum glType = 0;
            CHECK_GL_ERROR(glGetActiveUniform(mGLHandle, index, 256, NULL,
                &arraySize, &glType, uniformName));

            GLint location = glGetUniformLocation(mGLHandle, uniformName);
            if (location >= 0)
            {
                // Check for array index included in the name and strip it
                DiString name(uniformName);
                int pos = name.find('[');
                if (pos != DiString::npos)
                {
                    // If not the first index, skip
                    if (name.find("[0]", pos) == DiString::npos)
                        continue;

                    name = name.substr(0, pos);
                }

                DiGLES2ShaderConstant* constant = DI_NEW DiGLES2ShaderConstant();
                constant->program = 0; // we don't need it here
                constant->location = location;
                constant->type = glType;

                // built-in uniforms
                if (!strncmp(uniformName, "g_", 2))
                {
                    // check if we already have this global uniform
                    if (DiGLUniforms::msUniformFuncs.find(name) != DiGLUniforms::msUniformFuncs.end())
                    {
                        params->AddBuiltinParam(uniformName,DiGLUniforms::msUniformFuncs[name], constant);
                    }
                    else
                    {
                        DI_WARNING("The global uniform: %s cannot be located", uniformName);
                    }
                    continue;
                }

                
                if (glType == GL_SAMPLER_2D || glType == GL_SAMPLER_CUBE)
                {
                    constant->unit = (uint32)mSamplers.size();
                    mSamplers[name] = constant;

                    CHECK_GL_ERROR(glUniform1i(location, constant->unit));
                }
                else
                {
                    constant->unit = 0;
                    mConsts[name] = constant;
                }

                auto demitype = DiGLTypeMappings::ConvertGLShaderConstType(glType);
                if (demitype != DiShaderParameter::NUM_VARIABLE_TYPES)
                {
                    params->AddParameter(demitype, name);
                }
                else
                {
                    DI_WARNING("Unsupported gl shader constant type: %d", glType);
                }
            }
        }

        delete[] uniformName;
    }

    DiGLES2ShaderConstant* DiGLES2ShaderLinker::GetConstant(const DiString& constname)
    {
        auto i = mConsts.find(constname);
        if (i != mConsts.end())
            return i->second;
        else
            return nullptr;
    }

    DiGLES2ShaderConstant*  DiGLES2ShaderLinker::GetSampler(const DiString& constname)
    {
        auto i = mSamplers.find(constname);
        if (i != mSamplers.end())
            return i->second;
        else
            return nullptr;
    }

    bool DiGLES2ShaderLinker::HasConstant(const DiString& constname)
    {
        return mConsts.find(constname) != mConsts.end();
    }

    bool DiGLES2ShaderLinker::HasSampler(const DiString& samplername)
    {
        return mSamplers.find(samplername) != mSamplers.end();
    }

    void DiGLES2ShaderLinker::Link()
    {
        if (!mLinked)
        {
            glGetError();

            CHECK_GL_ERROR(mGLHandle = glCreateProgram());

            // bind attribute
            size_t numAttribs = sizeof(msCustomAttributes) / sizeof(CustomAttribute);
            for (size_t i = 0; i < numAttribs; ++i)
                CHECK_GL_ERROR(glBindAttribLocation(mGLHandle, msCustomAttributes[i].attrib, msCustomAttributes[i].name.c_str()));

            // all shaders should be compiled before linking to program objects
            if (mVS)
                mVS->LinkToProgramObject(mGLHandle);

            if (mPS)
                mPS->LinkToProgramObject(mGLHandle);

            CHECK_GL_ERROR(glLinkProgram(mGLHandle));
            CHECK_GL_ERROR(glGetProgramiv(mGLHandle, GL_LINK_STATUS, &mLinked));
           
            DiGLES2ShaderInstance::LogObjectInfo("GLSL linking result: ", mGLHandle);

            if (glIsProgram(mGLHandle))
            {
                glValidateProgram(mGLHandle);
            }

            DiGLES2ShaderInstance::LogObjectInfo("GLSL validation result: ", mGLHandle);

            // TODO from binary codes
        }
    }

    DiGLUniforms::UniformFuncs DiGLUniforms::msUniformFuncs;

    void DiGLUniforms::InitUniformFuncs()
    {
        msUniformFuncs["g_modelMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->modelMatrix.transpose()[0]);
        };

        msUniformFuncs["g_viewMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->viewMatrix.transpose()[0]);
        };

        msUniformFuncs["g_projMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->projMatrix.transpose()[0]);
        };

        msUniformFuncs["g_modelViewMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->modelViewMatrix.transpose()[0]);
        };

        msUniformFuncs["g_modelViewProjMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->modelViewProjMatrix.transpose()[0]);
        };

        msUniformFuncs["g_viewProjMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->viewProjMatrix.transpose()[0]);
        };

        msUniformFuncs["g_texMatrix"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_MAT4FV(c, 1, env->texMatrix.transpose()[0]);
        };

        msUniformFuncs["g_boneMatrices"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numBones*3, (float*)(&env->boneMatrices));
        };

        msUniformFuncs["g_modelMatrices"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numModelMatrices*3, (float*)(&env->modelMatrices));
        };

        msUniformFuncs["g_eyePosition"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_3FV(c, 1, env->eyePosition.ptr());
        };

        msUniformFuncs["g_eyeDirection"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_3FV(c, 1, env->eyeDirection.ptr());
        };

        msUniformFuncs["g_farnearPlane"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_2FV(c, 1, env->farnearPlane.ptr());
        };

        msUniformFuncs["g_time"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_1FV(c, 1, &env->time);
        };

        msUniformFuncs["g_viewportSize"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, 1, env->viewportSize.ptr());
        };

        msUniformFuncs["g_globalAmbient"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, 1, env->globalAmbient.Ptr());
        };

        msUniformFuncs["g_ambientColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, 1, env->ambientColor.Ptr());
        };

        msUniformFuncs["g_diffuseColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_3FV(c, 1, env->diffuseColor.Ptr());
        };

        msUniformFuncs["g_specularColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_3FV(c, 1, env->specularColor.Ptr());
        };

        msUniformFuncs["g_opacity"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_1FV(c, 1, &env->opacity);
        };

        msUniformFuncs["g_shininess"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_1FV(c, 1, &env->shininess);
        };

        msUniformFuncs["g_texelOffsets"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, 1, env->texelOffsets.ptr());
        };

        msUniformFuncs["g_numDirLights"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_1I(c, env->numDirLights);
        };

        msUniformFuncs["g_dirLightsColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numDirLights, env->dirLightsColor[0].Ptr());
        };

        msUniformFuncs["g_dirLightsDir"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numDirLights, env->dirLightsDir[0].ptr());
        };

        msUniformFuncs["g_numPointLights"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_1I(c, env->numPointLights);
        };

        msUniformFuncs["g_pointLightsColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numPointLights, env->pointLightsColor[0].Ptr());
        };

        msUniformFuncs["g_pointLightsPosition"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numPointLights, env->pointLightsPosition[0].ptr());
        };

        msUniformFuncs["g_pointLightsAttenuation"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, env->numPointLights, env->pointLightsAttenuation[0].ptr());
        };

        msUniformFuncs["g_hasSkyLight"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_1I(c, (env->hasSkyLight ? 1 : 0));
        };

        msUniformFuncs["g_skyLightColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, 1, env->skyLightColor.Ptr());
        };

        msUniformFuncs["g_groundColor"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_4FV(c, 1, env->groundColor.Ptr());
        };

        msUniformFuncs["g_skyLightDir"] = [](const DiShaderEnvironment* env, DiGLES2ShaderConstant* c) {
            GL_UNIFORM_3FV(c, 1, env->skyLightDir.ptr());
        };
    }
}