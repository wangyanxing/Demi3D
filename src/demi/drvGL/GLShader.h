
/********************************************************************
    File:       GLShader.h
    Creator:    demiwangya
*********************************************************************/

#ifndef GLShader_h__
#define GLShader_h__

#include "GpuProgram.h"
#include "VertexFormat.h"
#include <functional>

namespace Demi
{
    struct DiGLShaderConstant
    {
        GLint  location;
        GLenum type;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiGLShaderInstance : public DiShaderInstance
    {
    public:

        DiGLShaderInstance(DiShaderType type, DiShaderProgram* prog);

        ~DiGLShaderInstance();

    public:

        bool                Compile(const DiString& code);

        void                Bind(const DiShaderEnvironment& shaderEnv);

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

        void                PorcessShaders(const DiString& code);

        DiShaderProgram*    mShaderProgram;

        GLhandleARB         mShaderHandle;

        DiShaderType        mType;

        GLint               mCompiled;

        DiString            mProcessedShader;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiGLUniforms
    {
    public:

        typedef std::function<void(const DiShaderEnvironment*, GLuint)> BindingFunc;

        typedef DiStrHash<BindingFunc> UniformFuncs;

        static void InitUniformFuncs();

        static UniformFuncs msUniformFuncs;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiGLShaderLinker
    {
    public:

        DiGLShaderLinker(DiGLShaderInstance* vs, DiGLShaderInstance* ps);

        ~DiGLShaderLinker();

    public:

        void                Link();

        void                Bind();

        void                LoadConstants(DiGLShaderParam* params);

        void                LoadAttributes();

        bool                IsAttributeValid(DiVertexUsage semantic, uint8 index);

        DiGLShaderConstant* GetConstant(const DiString& constname);

        bool                HasConstant(const DiString& constname);

        GLhandleARB         GetGLHandle() const { return mGLHandle; }

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

        typedef DiStrHash<DiGLShaderConstant> Consts;

        Consts              mConsts;
    };
}

#endif // GLShader_h__