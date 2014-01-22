
/********************************************************************
    File:       GLShader.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuProgram.h"

namespace Demi
{
    class DiGLShaderInstance : public DiShaderInstance
    {
    public:

        DiGLShaderInstance(DiShaderType type, DiShaderProgram* prog);

        ~DiGLShaderInstance();

        void            Compile(const DiString& code);

        void            Bind(const DiShaderEnvironment& shaderEnv);

        void            LoadVariables(std::function<void(DiGpuVariable*)> func);

        void            Release();

        DiShaderType    GetType()
        {
            return mType;
        }

    public:

        static void     LogGLSLError(GLenum glErr, const DiString& errorTextPrefix,
                            const GLhandleARB obj, const bool forceInfoLog, const bool forceException);

        static DiString LogObjectInfo(const DiString& msg, const GLhandleARB obj);

    private:

        DiShaderProgram*    mShaderProgram;

        GLhandleARB         mShaderHandle;

        GLhandleARB         mGLHandle;

        DiShaderType        mType;

        GLint               mCompiled;
    };
}