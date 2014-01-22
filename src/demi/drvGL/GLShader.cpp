
#include "GLShader.h"
#include "GLShaderVariable.h"
#include "GLDriver.h"
#include "GLTypeMappings.h"
#include "ShaderProgram.h"

namespace Demi
{
    DiGLShaderInstance::DiGLShaderInstance(DiShaderType type, DiShaderProgram* prog) : DiShaderInstance(type)
        , mShaderProgram(prog)
        , mShaderHandle(0)
        , mType(type)
        , mCompiled(0)
    {
        mGLHandle = glCreateProgramObjectARB();
        if (!mGLHandle)
        {
            DI_WARNING("Could not create shader program");
        }
    }

    DiGLShaderInstance::~DiGLShaderInstance()
    {
        glDeleteObjectARB(mGLHandle);
    }

    void DiGLShaderInstance::Compile(const DiString& code)
    {
        DI_ASSERT(mGLHandle);

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
        }
    }

    void DiGLShaderInstance::Bind(const DiShaderEnvironment& shaderEnv)
    {
        glUseProgramObjectARB(mGLHandle);
        
    }

    void DiGLShaderInstance::LoadVariables(std::function<void(DiGpuVariable*)> func)
    {

    }

    void DiGLShaderInstance::Release()
    {
        glDeleteObjectARB(mShaderHandle);
    }

    void DiGLShaderInstance::LogGLSLError(GLenum glErr, 
        const DiString& errorTextPrefix, const GLhandleARB obj, 
        const bool forceInfoLog, const bool forceException)
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

            if (forceException)
            {
                DI_WARNING("GLSL error:");
                DI_WARNING("%s", msg.c_str());
            }
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

}