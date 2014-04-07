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

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
#   include "EAGL2Util.h"
#endif

namespace Demi
{
    DiGLES2ShaderPipeline::SemanticToStringMap DiGLES2ShaderPipeline::mSemanticTypeMap;
    
    DiGLES2ShaderPipeline::DiGLES2ShaderPipeline(DiGLES2ShaderInstance* vs, 
        DiGLES2ShaderInstance* ps)
        : mVS(vs)
        , mPS(ps)
        , mLinked(0)
    {
        // init mCustomAttributesIndexes
		for(size_t i = 0 ; i < MAX_VERT_USAGE; i++)
        {
			for(size_t j = 0 ; j < MAX_UV_SETS; j++)
            {
                mCustomAttributesIndexes[i][j] = NULL_CUSTOM_ATTRIBUTES_INDEX;
            }
        }
        
        if(mSemanticTypeMap.empty())
        {
            // Initialize the attribute to semantic map
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("Position", VERT_USAGE_POSITION));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("BlendWeights", VERT_USAGE_BLENDWEIGHT));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("Normal", VERT_USAGE_NORMAL));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("Color", VERT_USAGE_COLOR));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("SecondColor", VERT_USAGE_SECONDARY_COLOR));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("BlendIndices", VERT_USAGE_BLENDINDICES));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("Tangent", VERT_USAGE_TANGENT));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("Binormal", VERT_USAGE_BINORMAL));
            mSemanticTypeMap.insert(SemanticToStringMap::value_type("Texcoord", VERT_USAGE_TEXCOORD));
        }
    }

    DiGLES2ShaderPipeline::~DiGLES2ShaderPipeline()
    {
        IF_IOS_VERSION_IS_GREATER_THAN(5.0)
            CHECK_GL_ERROR(glDeleteProgramPipelinesEXT(1, &mGLProgramPipelineHandle));
        
        for(auto i = mConsts.begin(); i != mConsts.end(); ++i)
        {
            DI_DELETE i->second;
        }
        for(auto i = mSamplers.begin(); i != mSamplers.end(); ++i)
        {
            DI_DELETE i->second;            
        }
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
                GLuint programHandle = mVS->GetProgramHandle();

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
                GLuint programHandle = mPS->GetProgramHandle();

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
                    CHECK_GL_ERROR(glUseProgramStagesEXT(mGLProgramPipelineHandle, GL_VERTEX_SHADER_BIT_EXT, mVS->GetProgramHandle()));
                }
                if (mPS && mPS->IsLinked())
                {
                    CHECK_GL_ERROR(glUseProgramStagesEXT(mGLProgramPipelineHandle, GL_FRAGMENT_SHADER_BIT_EXT, mPS->GetProgramHandle()));
                }

                DiGLES2ShaderInstance::LogObjectInfo("GLSL program pipeline result : ", mGLProgramPipelineHandle);

                if (mVS && mPS)
                {
                    DiString label = mVS->GetShaderProgram()->GetShaderFileName() + "|" + mPS->GetShaderProgram()->GetShaderFileName();
                    IF_IOS_VERSION_IS_GREATER_THAN(5.0)
                    {
                        glLabelObjectEXT(GL_PROGRAM_PIPELINE_OBJECT_EXT, mGLProgramPipelineHandle, 0, label.c_str());
                    }
                }
                
                if (mVS)
                {
                    GLint activeAttrbCount = 0;
                    CHECK_GL_ERROR(glGetProgramiv(mVS->GetProgramHandle(), GL_ACTIVE_ATTRIBUTES, &activeAttrbCount));
                    
                    if(activeAttrbCount)
                    {
                        GLint maxActiveattrbLen = 0;
                        CHECK_GL_ERROR(glGetProgramiv(mVS->GetProgramHandle(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxActiveattrbLen));
                        if(maxActiveattrbLen)
                        {
                            char* name = DI_NEW char[maxActiveattrbLen+1];
                            name[maxActiveattrbLen] = 0;
                            
                            for(GLint attributeIndex = 0; attributeIndex < activeAttrbCount; ++attributeIndex)
                            {
                                GLsizei nameLen = 0;
                                GLint size = 0;
                                GLenum type = 0;
                                CHECK_GL_ERROR(glGetActiveAttrib(mVS->GetProgramHandle(), attributeIndex, maxActiveattrbLen+1, &nameLen, &size, &type, name));
                                GLint attrLocation = glGetAttribLocation(mVS->GetProgramHandle(), name);
                                DiVertexUsage usage = GetAttributeSemanticEnum(name);
                                uint32 texid = 0;
                                if (usage == VERT_USAGE_TEXCOORD)
                                    texid = GetAttributeTecoordID(name);
                                mCustomAttributesIndexes[usage][texid] = attrLocation;
                            }
                            
                            DI_DELETE[] name;
                        }
                    }
                }
            }
        }
    }

    GLint DiGLES2ShaderPipeline::GetAttributeIndex(DiVertexUsage semantic, uint32 index)
    {
        GLint res = mCustomAttributesIndexes[semantic][index];
		if (res == NULL_CUSTOM_ATTRIBUTES_INDEX)
		{
            GLuint handle = mVS->GetProgramHandle();
			const char * attString = GetAttributeSemanticString(semantic);
			GLint attrib;
            CHECK_GL_ERROR(attrib = glGetAttribLocation(handle, attString));
            
			// For uv and other case the index is a part of the name
			if (attrib == NOT_FOUND_CUSTOM_ATTRIBUTES_INDEX)
			{
                DiString attStringWithSemantic;
                attStringWithSemantic.Format("%s%d", attString, index);
                CHECK_GL_ERROR(attrib = glGetAttribLocation(handle, attStringWithSemantic.c_str()));
			}
            
			// Update mCustomAttributesIndexes with the index we found (or didn't find)
			mCustomAttributesIndexes[semantic-1][index] = attrib;
			res = attrib;
		}
        
		return res;
    }

    DiVertexUsage DiGLES2ShaderPipeline::GetAttributeSemanticEnum(const DiString& type)
    {
        DiVertexUsage semantic = mSemanticTypeMap[type];
        if (semantic >= 0)
        {
            return semantic;
        }
        else
        {
            if (DiString::StartsWith(type, "Texcoord"))
            {
                return VERT_USAGE_TEXCOORD;
            }
            else
            {
                DI_WARNING("Missing attribute!");
                DI_ASSERT_FAIL;
                return (DiVertexUsage)0;
            }
        }
    }
    
    uint32 DiGLES2ShaderPipeline::GetAttributeTecoordID(const DiString& type)
    {
        uint32 id = type[type.length()-1] - '0';
        DI_ASSERT(id >=0 && id < MAX_UV_SETS);
        return id;
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
        GLuint programObject = shader->GetProgramHandle();

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
                
                DiGLES2ShaderConstant* constant = DI_NEW DiGLES2ShaderConstant();
                constant->program = programObject;
                constant->location = location;
                constant->type = glType;

                // built-in uniforms
                if (!strncmp(uniformName, "g_", 2))
                {
                    // check if we already have this global uniform
                    if (DiGLUniforms::msUniformFuncs.find(name) != DiGLUniforms::msUniformFuncs.end())
                    {
                        params->AddBuiltinParam(DiGLUniforms::msUniformFuncs[name], constant);
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

                    CHECK_GL_ERROR(glProgramUniform1iEXT(programObject, location, constant->unit));
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

    DiGLES2ShaderConstant* DiGLES2ShaderPipeline::GetConstant(const DiString& constname)
    {
        auto i = mConsts.find(constname);
        if (i != mConsts.end())
            return i->second;
        else
            return nullptr;
    }

    DiGLES2ShaderConstant* DiGLES2ShaderPipeline::GetSampler(const DiString& constname)
    {
        auto i = mSamplers.find(constname);
        if (i != mSamplers.end())
            return i->second;
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