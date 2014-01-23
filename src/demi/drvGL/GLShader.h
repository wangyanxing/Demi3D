
/********************************************************************
    File:       GLShader.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "GpuProgram.h"
#include "VertexFormat.h"

namespace Demi
{
    class DiGLShaderInstance : public DiShaderInstance
    {
    public:

        DiGLShaderInstance(DiShaderType type, DiShaderProgram* prog);

        ~DiGLShaderInstance();

    public:

        bool                Compile(const DiString& code);

        void                Bind(const DiShaderEnvironment& shaderEnv);

        void                LoadVariables(std::function<void(DiGpuVariable*)> func);

        void                LinkToProgramObject(const GLhandleARB programObject);

        void                UnlinkToProgramObject(const GLhandleARB programObject);

        void                Release();

        DiShaderType        GetType()
        {
            return mType;
        }

    public:

        static void         LogGLSLError(GLenum glErr, const DiString& errorTextPrefix,
                                const GLhandleARB obj, const bool forceInfoLog = false);

        static DiString     LogObjectInfo(const DiString& msg, const GLhandleARB obj);

    private:

        DiShaderProgram*    mShaderProgram;

        GLhandleARB         mShaderHandle;

        DiShaderType        mType;

        GLint               mCompiled;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiGLShaderLinker
    {
    public:

        DiGLShaderLinker(DiGLShaderInstance* vs, DiGLShaderInstance* ps);

        ~DiGLShaderLinker();

    public:

        void                Bind();

        void                Link();

        void                ExtractAttributes();

        bool                IsAttributeValid(DiVertexUsage semantic, uint8 index);

    private:

        struct CustomAttribute
        {
            DiString name;
            GLuint   attrib;
            CustomAttribute(const DiString& _name, GLuint _attrib)
                :name(_name), attrib(_attrib) {}
        };

        static CustomAttribute msCustomAttributes[];

    private:

        GLhandleARB         mGLHandle;

        DiGLShaderInstance* mVS;

        DiGLShaderInstance* mPS;

        GLint               mLinked;

        DiSet<GLuint>       mValidAttributes;
    };
}