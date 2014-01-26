
#include "GLShader.h"
#include "GLShaderParam.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "ShaderProgram.h"
#include "AssetManager.h"

namespace Demi
{
    DiGLShaderLinker::CustomAttribute DiGLShaderLinker::msCustomAttributes[] = 
    {
        CustomAttribute("vertex",           DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_POSITION,        0)),
        CustomAttribute("blendWeights",     DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_BLENDWEIGHT,     0)),
        CustomAttribute("normal",           DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_NORMAL,          0)),
        CustomAttribute("colour",           DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_COLOR,           0)),
        CustomAttribute("secondary_colour", DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_SECONDARY_COLOR, 0)),
        CustomAttribute("blendIndices",     DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_BLENDINDICES,    0)),
        CustomAttribute("uv0",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        0)),
        CustomAttribute("uv1",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        1)),
        CustomAttribute("uv2",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        2)),
        CustomAttribute("uv3",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        3)),
        CustomAttribute("uv4",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        4)),
        CustomAttribute("uv5",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        5)),
        CustomAttribute("uv6",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        6)),
        CustomAttribute("uv7",              DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TEXCOORD,        7)),
        CustomAttribute("tangent",          DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_TANGENT,         0)),
        CustomAttribute("binormal",         DiGLTypeMappings::GetFixedAttributeIndex(VERT_USAGE_BINORMAL,        0)),
    };

    DiGLShaderInstance::DiGLShaderInstance(DiShaderType type, DiShaderProgram* prog) : DiShaderInstance(type)
        , mShaderProgram(prog)
        , mShaderHandle(0)
        , mType(type)
        , mCompiled(0)
    {
    }

    DiGLShaderInstance::~DiGLShaderInstance()
    {
    }

    bool DiGLShaderInstance::Compile(const DiString& code)
    {
        if (code.empty())
            return false;

        // check the includes firstly
        PorcessShaders(code);

        GLenum shaderType = 0;
        switch (mType)
        {
        case SHADER_VERTEX:
            shaderType = GL_VERTEX_SHADER_ARB;
            break;
        case SHADER_PIXEL:
            shaderType = GL_FRAGMENT_SHADER_ARB;
            break;
        }
        mShaderHandle = glCreateShaderObjectARB(shaderType);

        const char *source = mProcessedShader.c_str();
        glShaderSourceARB(mShaderHandle, 1, &source, NULL);

        glCompileShaderARB(mShaderHandle);
        glGetObjectParameterivARB(mShaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &mCompiled);

        if (!mCompiled)
        {
            DiString msg;
            msg.Format("GLSL compile info: %s", mShaderProgram->GetShaderFileName().c_str());
            LogObjectInfo(msg, mShaderHandle);
            return false;
        }
        return true;
    }

    void DiGLShaderInstance::Bind(const DiShaderEnvironment& shaderEnv)
    {
    }

    void DiGLShaderInstance::Release()
    {
        glDeleteObjectARB(mShaderHandle);
    }

    void DiGLShaderInstance::LogGLSLError(GLenum glErr, 
        const DiString& errorTextPrefix, const GLhandleARB obj, 
        const bool forceInfoLog)
    {
        bool errorsFound = false;
        DiString msg = errorTextPrefix;

        while (glErr != GL_NO_ERROR)
        {
            const char* glerrStr = (const char*)gluErrorString(glErr);
            if (glerrStr)
            {
                msg += DiString(glerrStr);
            }
            glErr = glGetError();
            errorsFound = true;
        }

        if (errorsFound || forceInfoLog)
        {
            msg += LogObjectInfo(msg, obj);
            DI_WARNING("GLSL error:");
            DI_WARNING("%s", msg.c_str());
        }
    }

    DiString DiGLShaderInstance::LogObjectInfo(const DiString& msg, const GLhandleARB obj)
    {
        DiString logMessage = msg;

        if (obj > 0)
        {
            GLint infologLength = 0;

            if (glIsProgram(obj))
                glValidateProgram(obj);

            glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength);

            if (infologLength > 0)
            {
                GLint charsWritten = 0;
                GLcharARB* infoLog = new GLcharARB[infologLength];

                glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
                logMessage += DiString(infoLog);
                DI_LOG("%s", logMessage.c_str());

                delete[] infoLog;
            }
        }

        return logMessage;
    }

    void DiGLShaderInstance::LinkToProgramObject(const GLhandleARB programObject)
    {
        if (!mShaderHandle)
        {
            DI_WARNING("Shader should be compiled before linking.");
            return;
        }

        glAttachObjectARB(programObject, mShaderHandle);

        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR)
            LogGLSLError(glErr, "LinkToProgramObject error: ",programObject);
    }

    void DiGLShaderInstance::UnlinkToProgramObject(const GLhandleARB programObject)
    {
        glDetachObjectARB(programObject, mShaderHandle);

        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR)
            LogGLSLError(glErr, "UnlinkToProgramObject error: ", programObject);
    }

    void DiGLShaderInstance::PorcessShaders(const DiString& code)
    {
        mProcessedShader.clear();

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
        DiString::size_type pos = mProcessedShader.find("#include");
        if (pos != DiString::npos)
        {
            DiString::size_type first = 0, second = 0;
            for (auto i = pos + 7; i < mProcessedShader.size() && mProcessedShader[i] != '\n'; ++i)
            {
                char c = mProcessedShader[i];
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
    }

    DiGLShaderLinker::DiGLShaderLinker(DiGLShaderInstance* vs, DiGLShaderInstance* ps)
        : mGLHandle(0)
        , mVS(vs)
        , mPS(ps)
        , mLinked(0)
    {
    }

    DiGLShaderLinker::~DiGLShaderLinker()
    {
        glDeleteObjectARB(mGLHandle);
    }

    void DiGLShaderLinker::Bind()
    {
        if (mLinked)
        {
            glUseProgramObjectARB(mGLHandle);

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR)
                DiGLShaderInstance::LogGLSLError(glErr, "Binding GLShaderLinker: ", mGLHandle);
        }
    }

    void DiGLShaderLinker::LoadAttributes()
    {
        size_t numAttribs = sizeof(msCustomAttributes) / sizeof(CustomAttribute);

        for (size_t i = 0; i < numAttribs; ++i)
        {
            const CustomAttribute& a = msCustomAttributes[i];
            GLint attrib = glGetAttribLocationARB(mGLHandle, a.name.c_str());

            if (attrib != -1)
                mValidAttributes.insert(a.attrib);
        }
    }

    bool DiGLShaderLinker::IsAttributeValid(DiVertexUsage semantic, uint8 index)
    {
        return mValidAttributes.find(DiGLTypeMappings::GetFixedAttributeIndex(semantic, index)) != mValidAttributes.end();
    }

    void DiGLShaderLinker::LoadConstants(DiGLShaderParam* params)
    {
        if (DiGLUniforms::msUniformFuncs.empty())
            DiGLUniforms::InitUniformFuncs();

        GLint uniformCount = 0;
        glGetObjectParameterivARB(mGLHandle, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &uniformCount);
        
        char uniformName[256] = "";
        for (int index = 0; index < uniformCount; index++)
        {
            GLint arraySize = 0;
            GLenum glType = 0;
            glGetActiveUniformARB(mGLHandle, index, 256, NULL,
                &arraySize, &glType, uniformName);

            GLint location = glGetUniformLocationARB(mGLHandle, uniformName);
            if (location >= 0)
            {
                // Check for array index included in the name and strip it
                DiString name(uniformName);
                unsigned index = name.find('[');
                if (index != DiString::npos)
                {
                    // If not the first index, skip
                    if (name.find("[0]", index) == DiString::npos)
                        continue;

                    name = name.substr(0, index);
                }

                // built-in uniforms
                if (!strncmp(uniformName, "g_", 2))
                {
                    params->AddBuiltinParam(location, DiGLUniforms::msUniformFuncs[name]);
                    continue;
                }

                DiGLShaderConstant p;
                p.location = location;
                p.type = glType;
                mConsts[name] = p;

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
    }

    DiGLShaderConstant* DiGLShaderLinker::GetConstant(const DiString& constname)
    {
        auto i = mConsts.find(constname);
        if (i != mConsts.end())
            return &(i->second);
        else
            return nullptr;
    }

    bool DiGLShaderLinker::HasConstant(const DiString& constname)
    {
        return mConsts.find(constname) != mConsts.end();
    }

    void DiGLShaderLinker::Link()
    {
        //if (!mLinked)
        {
            glGetError();
            mGLHandle = glCreateProgramObjectARB();

            GLenum glErr = glGetError();
            if (glErr != GL_NO_ERROR)
                DiGLShaderInstance::LogGLSLError(glErr, "Binding GLShaderLinker: ", 0);

            // all shaders should be compiled before linking to program objects
            if (mVS)
                mVS->LinkToProgramObject(mGLHandle);

            if (mPS)
                mPS->LinkToProgramObject(mGLHandle);

            glLinkProgramARB(mGLHandle);
            glGetObjectParameterivARB(mGLHandle, GL_OBJECT_LINK_STATUS_ARB, &mLinked);

            if (mLinked)
                DiGLShaderInstance::LogObjectInfo(DiString("GLSL linking result : "), mGLHandle);

            LoadAttributes();

            glErr = glGetError();
            if (glErr != GL_NO_ERROR)
                DiGLShaderInstance::LogGLSLError(glErr, "Binding GLShaderLinker: ", mGLHandle);
        }
    }

    DiGLUniforms::UniformFuncs DiGLUniforms::msUniformFuncs;

    void DiGLUniforms::InitUniformFuncs()
    {
        msUniformFuncs["g_modelMatrix"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix4fvARB(location, 1, GL_TRUE, env->modelViewProjMatrix[0]);
        };

        msUniformFuncs["g_viewMatrix"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix4fvARB(location, 1, GL_TRUE, env->viewMatrix[0]);
        };

        msUniformFuncs["g_modelViewMatrix"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix4fvARB(location, 1, GL_TRUE, env->modelViewMatrix[0]);
        };

        msUniformFuncs["g_modelViewProjMatrix"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix4fvARB(location, 1, GL_TRUE, env->modelViewProjMatrix[0]);
        };

        msUniformFuncs["g_viewProjMatrix"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix4fvARB(location, 1, GL_TRUE, env->viewProjMatrix[0]);
        };

        msUniformFuncs["g_texViewProjMatrix"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix4fvARB(location, 1, GL_TRUE, env->texViewProjMatrix[0]);
        };

        msUniformFuncs["g_boneMatrices"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix3x4fv(location, env->numBones, GL_TRUE, (float*)(&env->boneMatrices));
        };

        msUniformFuncs["g_modelMatrices"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniformMatrix3x4fv(location, env->numModelMatrices, GL_TRUE, (float*)(&env->modelMatrices));
        };

        msUniformFuncs["g_eyePosition"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform3fvARB(location, 1, env->eyePosition.ptr());
        };

        msUniformFuncs["g_eyePosition"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform3fvARB(location, 1, env->eyePosition.ptr());
        };

        msUniformFuncs["g_eyeDirection"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform3fvARB(location, 1, env->eyeDirection.ptr());
        };

        msUniformFuncs["g_eyePositionObjSpace"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform3fvARB(location, 1, env->eyePositionObjSpace.ptr());
        };

        msUniformFuncs["g_farnearPlane"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform2fvARB(location, 1, env->farnearPlane.ptr());
        };

        msUniformFuncs["g_time"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform1fvARB(location, 1, &env->time);
        };

        msUniformFuncs["g_globalAmbient"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, 1, env->globalAmbient.Ptr());
        };

        msUniformFuncs["g_ambientColor"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, 1, env->ambientColor.Ptr());
        };

        msUniformFuncs["g_diffuseColor"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform3fvARB(location, 1, env->diffuseColor.Ptr());
        };

        msUniformFuncs["g_specularColor"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform3fvARB(location, 1, env->specularColor.Ptr());
        };

        msUniformFuncs["g_opacity"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform1fvARB(location, 1, &env->opacity);
        };

        msUniformFuncs["g_shininess"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform1fvARB(location, 1, &env->shininess);
        };

        msUniformFuncs["g_texelOffsets"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, 1, env->texelOffsets.ptr());
        };

        msUniformFuncs["g_numDirLights"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform1ivARB(location, 1, &env->numDirLights);
        };

        msUniformFuncs["g_dirLightsColor"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, env->numDirLights, env->dirLightsColor[0].Ptr());
        };

        msUniformFuncs["g_dirLightsDir"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, env->numDirLights, env->dirLightsDir[0].ptr());
        };

        msUniformFuncs["g_numPointLights"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform1ivARB(location, 1, &env->numPointLights);
        };

        msUniformFuncs["g_pointLightsColor"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, env->numPointLights, env->pointLightsColor[0].Ptr());
        };

        msUniformFuncs["g_pointLightsPosition"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, env->numPointLights, env->pointLightsPosition[0].ptr());
        };

        msUniformFuncs["g_pointLightsAttenuation"] = [](const DiShaderEnvironment* env, GLuint location) {
            glUniform4fvARB(location, env->numPointLights, env->pointLightsAttenuation[0].ptr());
        };
    }
}