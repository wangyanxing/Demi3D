
#include "GLShader.h"
#include "GLShaderParam.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "ShaderProgram.h"

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

        if (!code.empty())
        {
            const char *source = code.c_str();
            glShaderSourceARB(mShaderHandle, 1, &source, NULL);
        }

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

    void DiGLShaderLinker::LoadConstants()
    {
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

                DiGLShaderConstant p;
                p.location = location;
                p.type = glType;
                mConsts[name] = p;
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
        if (!mLinked)
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

            LoadConstants();
            LoadAttributes();

            glErr = glGetError();
            if (glErr != GL_NO_ERROR)
                DiGLShaderInstance::LogGLSLError(glErr, "Binding GLShaderLinker: ", mGLHandle);
        }
    }

}