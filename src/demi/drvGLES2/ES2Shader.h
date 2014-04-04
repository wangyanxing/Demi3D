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

#ifndef GLES2Shader_h__
#define GLES2Shader_h__

#include "ES2Prerequisites.h"
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

    struct DiGLShaderSampler
    {
        GLint  location;
        GLenum type;
        uint32 unit;
    };

    //////////////////////////////////////////////////////////////////////////

    class DiGLES2ShaderInstance : public DiShaderInstance
    {
    public:

        DiGLES2ShaderInstance(DiShaderType type, DiShaderProgram* prog);

        ~DiGLES2ShaderInstance();

    public:

        bool                Compile(const DiString& code);

        void                Bind(const DiShaderEnvironment& shaderEnv);

        void                LinkToProgramObject(const GLuint programObject);

        void                UnlinkToProgramObject(const GLuint programObject);

        void                Release();

        DiShaderType        GetType()
        {
            return mType;
        }

    public:

        static DiString     LogObjectInfo(const DiString& msg, const GLuint obj);

    private:

        void                PorcessShaders(const DiString& code);

        DiShaderProgram*    mShaderProgram;

        GLuint              mShaderHandle;

        GLuint              mGLProgramHandle;

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

    class DiGLES2ShaderLinker : public DiBase
    {
    public:

        DiGLES2ShaderLinker(DiGLES2ShaderInstance* vs, DiGLES2ShaderInstance* ps);

        ~DiGLES2ShaderLinker();

    public:

        void                Link();

        void                Bind();

        void                LoadConstants(DiGLES2ShaderParam* params);

        DiGLShaderConstant* GetConstant(const DiString& constname);

        DiGLShaderSampler*  GetSampler(const DiString& constname);

        bool                HasConstant(const DiString& constname);

        bool                HasSampler(const DiString& samplername);

        GLuint              GetGLHandle() const { return mGLHandle; }

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
        
        GLuint              mGLProgramPipelineHandle;

        GLuint              mGLHandle;

        DiGLES2ShaderInstance* mVS;

        DiGLES2ShaderInstance* mPS;

        GLint               mLinked;

        typedef DiStrHash<DiGLShaderConstant> Consts;
        typedef DiStrHash<DiGLShaderSampler> Samplers;

        Consts              mConsts;

        Samplers            mSamplers;
    };
}

#endif // GLShader_h__