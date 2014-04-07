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

#ifndef DiES2ShaderPipeline_h__
#define DiES2ShaderPipeline_h__

#include "ES2Prerequisites.h"
#include "GpuProgram.h"
#include "VertexFormat.h"

namespace Demi
{
    struct DiGLES2ShaderConstant
    {
        GLuint program;
        GLint  location;
        GLenum type;
        uint32 unit; // for samplers
    };

    class DI_GLES2_API DiGLES2ShaderPipeline : public DiBase
    {
    public:

        DiGLES2ShaderPipeline(DiGLES2ShaderInstance* vs, DiGLES2ShaderInstance* ps);

        ~DiGLES2ShaderPipeline();
        
        enum AttributeResult
        {
            /// A value to define the case we didn't look for the attributes since the contractor
            NULL_CUSTOM_ATTRIBUTES_INDEX = -2,
            /// A value to define the attribute has not been found (this is also the result when glGetAttribLocation fails)
            NOT_FOUND_CUSTOM_ATTRIBUTES_INDEX = -1
            
        };
        
    public:

        GLuint                 GetGLHandle() const { return mGLProgramPipelineHandle; }

        void                   Bind();

        void                   Link();

        GLint                  GetAttributeIndex(DiVertexUsage semantic, uint32 index);
        
        void                   LoadConstants(DiGLES2ShaderParam* params);

        DiGLES2ShaderConstant* GetConstant(const DiString& constname);

        DiGLES2ShaderConstant* GetSampler(const DiString& constname);

        bool                   HasConstant(const DiString& constname);

        bool                   HasSampler(const DiString& samplername);

    protected:

        DiVertexUsage          GetAttributeSemanticEnum(const DiString& type);

        uint32                 GetAttributeTecoordID(const DiString& type);
        
        const char*            GetAttributeSemanticString(DiVertexUsage semantic);

        void                   _LoadContant(DiGLES2ShaderInstance* shader, DiGLES2ShaderParam* params);

    protected:

        enum 
        {
            VERTEX_PROGRAM_LINKED   = 1,
            FRAGMENT_PROGRAM_LINKED = 2,
            ALL_PROGRAMS_LINKED     = 3
        };

        /// GL handle for pipeline object
        GLuint  mGLProgramPipelineHandle;

        GLint   mCustomAttributesIndexes[MAX_VERT_USAGE][MAX_UV_SETS];

        DiGLES2ShaderInstance*  mVS;

        DiGLES2ShaderInstance*  mPS;

        GLint                   mLinked;

        typedef DiStrHash<DiVertexUsage> SemanticToStringMap;
        static SemanticToStringMap mSemanticTypeMap;

        typedef DiStrHash<DiGLES2ShaderConstant*> Consts;

        Consts                  mConsts;

        Consts                  mSamplers;
    };
}

#endif
