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
#include "ES2ShaderPipeline.h"
#include "GLES2Driver.h"
#include "ES2Shader.h"
#include "ES2ShaderParam.h"
#include "ES2TypeMappings.h"

namespace Demi
{
    DiGLES2ShaderPipeline::DiGLES2ShaderPipeline(DiGLES2ShaderInstance* vs, 
        DiGLES2ShaderInstance* ps)
        : mVS(vs)
        , mPS(ps)
        , mLinked(0)
    {

    }

    DiGLES2ShaderPipeline::~DiGLES2ShaderPipeline()
    {
        IF_IOS_VERSION_IS_GREATER_THAN(5.0)
            CHECK_GL_ERROR(glDeleteProgramPipelinesEXT(1, &mGLProgramPipelineHandle));
    }

    void DiGLES2ShaderPipeline::Bind()
    {
        if (mLinked)
        {
            CHECK_GL_ERROR(glBindProgramPipelineEXT(mGLProgramPipelineHandle));
        }
    }

    void DiGLES2ShaderPipeline::Link()
    {
        GLint linkStatus = 0;
        if (!mLinked)
        {
            glGetError();

            CHECK_GL_ERROR(glGenProgramPipelinesEXT(1, &mGLProgramPipelineHandle));
            CHECK_GL_ERROR(glBindProgramPipelineEXT(mGLProgramPipelineHandle));

            if (mVS && !mVS->IsLinked())
            {
                GLuint programHandle = mVS->GetShaderHandle();

                CHECK_GL_ERROR(glProgramParameteriEXT(programHandle, GL_PROGRAM_SEPARABLE_EXT, GL_TRUE));
                mVS->LinkToProgramObject(programHandle);
                CHECK_GL_ERROR(glLinkProgram(programHandle));
                CHECK_GL_ERROR(glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus));

                if (linkStatus)
                {
                    mVS->SetLinked(linkStatus);
                    mLinked |= VERTEX_PROGRAM_LINKED;
                }

                DiString msg;
                msg.Format("GLSL [VS] compile info: %s\n", mVS->GetShaderProgram()->GetShaderFileName().c_str());
                DiGLES2ShaderInstance::LogObjectInfo(msg, programHandle);
            }

            if (mPS && !mPS->IsLinked())
            {
                GLuint programHandle = mPS->GetShaderHandle();

                CHECK_GL_ERROR(glProgramParameteriEXT(programHandle, GL_PROGRAM_SEPARABLE_EXT, GL_TRUE));
                mPS->LinkToProgramObject(programHandle);
                CHECK_GL_ERROR(glLinkProgram(programHandle));
                CHECK_GL_ERROR(glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus));

                if (linkStatus)
                {
                    mPS->SetLinked(linkStatus);
                    mLinked |= FRAGMENT_PROGRAM_LINKED;
                }

                DiString msg;
                msg.Format("GLSL [PS] compile info: %s\n", mPS->GetShaderProgram()->GetShaderFileName().c_str());
                DiGLES2ShaderInstance::LogObjectInfo(msg, programHandle);
            }

            if (mLinked)
            {
                if (mVS && mVS->IsLinked())
                {
                    CHECK_GL_ERROR(glUseProgramStagesEXT(mGLProgramPipelineHandle, GL_VERTEX_SHADER_BIT_EXT, mVS->GetShaderHandle()));
                }
                if (mPS && mPS->IsLinked())
                {
                    CHECK_GL_ERROR(glUseProgramStagesEXT(mGLProgramPipelineHandle, GL_FRAGMENT_SHADER_BIT_EXT, mPS->GetShaderHandle()));
                }

                DiGLES2ShaderInstance::LogObjectInfo("GLSL program pipeline result : ", mGLProgramPipelineHandle);

                if (mVS && mPS)
                {
                    DiString label = mVS->GetShaderProgram()->GetShaderFileName() + "/" + mPS->GetShaderProgram()->GetShaderFileName();
                    IF_IOS_VERSION_IS_GREATER_THAN(5.0)
                    {
                        glLabelObjectEXT(GL_PROGRAM_PIPELINE_OBJECT_EXT, mGLProgramPipelineHandle, 0, label.c_str());
                    }
                }
            }
        }
    }

    GLint DiGLES2ShaderPipeline::GetAttributeIndex(DiVertexUsage semantic, uint32 index)
    {
#if 0
        GLint res = mCustomAttributesIndexes[semantic - 1][index];
        if (res == NULL_CUSTOM_ATTRIBUTES_INDEX)
        {
            const char * attString = GetAttributeSemanticString(semantic);
            GLint attrib;
            CHECK_GL_ERROR(attrib = glGetAttribLocation(mGLProgramHandle, attString));

            // sadly position is a special case 
            if (attrib == NOT_FOUND_CUSTOM_ATTRIBUTES_INDEX && semantic == VERT_USAGE_POSITION)
            {
                CHECK_GL_ERROR(attrib = glGetAttribLocation(mGLProgramHandle, "position"));
            }

            // for uv and other case the index is a part of the name
            if (attrib == NOT_FOUND_CUSTOM_ATTRIBUTES_INDEX)
            {
                DiString attStringWithSemantic;
                attStringWithSemantic.Format("%s%d", attString, index);
                CHECK_GL_ERROR(attrib = glGetAttribLocation(mGLProgramHandle, attStringWithSemantic.c_str()));
            }

            // update mCustomAttributesIndexes with the index we found (or didn't find) 
            mCustomAttributesIndexes[semantic - 1][index] = attrib;
            res = attrib;
        }
        return res;
#endif
        return 0;
    }

    DiVertexUsage DiGLES2ShaderPipeline::GetAttributeSemanticEnum(DiString type)
    {
        DiVertexUsage semantic = mSemanticTypeMap[type];
        if (semantic > 0)
        {
            return semantic;
        }
        else
        {
            DI_WARNING("Missing attribute!");
            DI_ASSERT_FAIL;
            return (DiVertexUsage)0;
        }
    }

    const char* DiGLES2ShaderPipeline::GetAttributeSemanticString(DiVertexUsage semantic)
    {
        for (auto i = mSemanticTypeMap.begin(); i != mSemanticTypeMap.end(); ++i)
        {
            if ((*i).second == semantic)
                return (*i).first.c_str();
        }

        DI_WARNING("Missing attribute!");
        DI_ASSERT_FAIL;
        return 0;
    }

    void DiGLES2ShaderPipeline::LoadConstants(DiGLES2ShaderParam* params)
    {
        if (DiGLUniforms::msUniformFuncs.empty())
            DiGLUniforms::InitUniformFuncs();

        mSamplers.clear();

        _LoadContant(mVS, params);
        _LoadContant(mPS, params);
    }

    void DiGLES2ShaderPipeline::_LoadContant(DiGLES2ShaderInstance* shader, DiGLES2ShaderParam* params)
    {
        GLuint programObject = shader->GetShaderHandle();

        char* uniformName = NULL;
        GLint maxLength = 0;
        CHECK_GL_ERROR(glGetProgramiv(programObject, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength));

        // If the max length of active uniforms is 0, then there are 0 active.
        // There won't be any to extract so we can return.
        if (maxLength == 0)
            return;

        uniformName = DI_NEW char[maxLength + 1];

        GLint uniformCount = 0;
        CHECK_GL_ERROR(glGetProgramiv(programObject, GL_ACTIVE_UNIFORMS, &uniformCount));

        for (int index = 0; index < uniformCount; index++)
        {
            GLint arraySize = 0;
            GLenum glType = 0;
            CHECK_GL_ERROR(glGetActiveUniform(programObject, index, 256, NULL,
                &arraySize, &glType, uniformName));

            GLint location = glGetUniformLocation(programObject, uniformName);
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

                // built-in uniforms
                if (!strncmp(uniformName, "g_", 2))
                {
                    // check if we already have this global uniform
                    if (DiGLUniforms::msUniformFuncs.find(name) != DiGLUniforms::msUniformFuncs.end())
                    {
                        params->AddBuiltinParam(location, DiGLUniforms::msUniformFuncs[name]);
                    }
                    else
                    {
                        DI_WARNING("The global uniform: %s cannot be located", uniformName);
                    }
                    continue;
                }

                if (glType == GL_SAMPLER_2D || glType == GL_SAMPLER_CUBE)
                {
                    DiGLES2ShaderSampler s;
                    s.location = location;
                    s.type = glType;
                    s.unit = (uint32)mSamplers.size();
                    mSamplers[name] = s;

                    CHECK_GL_ERROR(glUniform1i(location, s.unit));
                }
                else
                {
                    DiGLES2ShaderConstant p;
                    p.location = location;
                    p.type = glType;
                    mConsts[name] = p;
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

    DiGLES2ShaderConstant* DiGLES2ShaderPipeline::GetConstant(const DiString& constname)
    {
        auto i = mConsts.find(constname);
        if (i != mConsts.end())
            return &(i->second);
        else
            return nullptr;
    }

    DiGLES2ShaderSampler* DiGLES2ShaderPipeline::GetSampler(const DiString& constname)
    {
        auto i = mSamplers.find(constname);
        if (i != mSamplers.end())
            return &(i->second);
        else
            return nullptr;
    }

    bool DiGLES2ShaderPipeline::HasConstant(const DiString& constname)
    {
        return mConsts.find(constname) != mConsts.end();
    }

    bool DiGLES2ShaderPipeline::HasSampler(const DiString& samplername)
    {
        return mSamplers.find(samplername) != mSamplers.end();
    }
}