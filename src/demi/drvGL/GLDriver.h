
/********************************************************************
    File:       GLDriver.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "DrvGLDefines.h"
#include "GLPrerequisites.h"
#include "GfxDriver.h"

namespace Demi
{
    struct MyVertex
    {
        float x, y, z;        //Vertex
        float nx, ny, nz;     //Normal
        float s0, t0;         //Texcoord0
    };

    struct testvbo
    {
        testvbo()
        {
            VBO = 0;
            IBO = 0;
            mVerticesNum = 0;
            mPrimitiveCount = 0;
            mVBSize = 0;
        }

        void init()
        {
            
            float SPHERE_RADIUS = 30; 
            int segments = 32, rings = 32;
            int NUM_SEGMENTS = DiMath::Max(segments, 3);
            int NUM_RINGS = DiMath::Max(rings, 2);
            float phiStart = 0;
            float phiLength = DiMath::PI * 2;
            float thetaStart = 0;
            float thetaLength = DiMath::PI;
            mVerticesNum = (NUM_SEGMENTS + 1) * (NUM_RINGS + 1);
            mPrimitiveCount = 2 * NUM_RINGS * (NUM_SEGMENTS + 1);
            float fDeltaRingAngle = (DiMath::PI / NUM_RINGS);
            float fDeltaSegAngle = (2 * DiMath::PI / NUM_SEGMENTS);

            unsigned short wVerticeIndex = 0;
            int vbsize = mVerticesNum * sizeof(float)* (3 + 3 + 2);
            mVBSize = vbsize;
            float* pVertex = (float*)malloc(vbsize);
            int ibsize = mPrimitiveCount * 3 * sizeof(uint16);
            uint16* pIndices = (uint16*)malloc(ibsize);
            for (int ring = 0; ring <= NUM_RINGS; ring++) {
                float r0 = SPHERE_RADIUS * sinf(ring * fDeltaRingAngle);
                float y0 = SPHERE_RADIUS * cosf(ring * fDeltaRingAngle);
                for (int seg = 0; seg <= NUM_SEGMENTS; seg++) {
                    float x0 = r0 * sinf(seg * fDeltaSegAngle);
                    float z0 = r0 * cosf(seg * fDeltaSegAngle);
                    *pVertex++ = x0;
                    *pVertex++ = y0;
                    *pVertex++ = z0;
                    DiVec3 vNormal = DiVec3(x0, y0, z0).normalisedCopy();
                    *pVertex++ = vNormal.x;
                    *pVertex++ = vNormal.y;
                    *pVertex++ = vNormal.z;
                    *pVertex++ = (float)seg / (float)NUM_SEGMENTS;
                    *pVertex++ = (float)ring / (float)NUM_RINGS;
                    if (ring != NUM_RINGS) {
                        *pIndices++ = wVerticeIndex + NUM_SEGMENTS + 1;
                        *pIndices++ = wVerticeIndex;
                        *pIndices++ = wVerticeIndex + NUM_SEGMENTS;
                        *pIndices++ = wVerticeIndex + NUM_SEGMENTS + 1;
                        *pIndices++ = wVerticeIndex + 1;
                        *pIndices++ = wVerticeIndex;
                        wVerticeIndex++;
                    }
                }
            }
            

            MyVertex pvertex[3];
            //VERTEX 0
            pvertex[0].x = 0.0;
            pvertex[0].y = 0.0;
            pvertex[0].z = 0.0;
            pvertex[0].nx = 0.0;
            pvertex[0].ny = 0.0;
            pvertex[0].nz = 1.0;
            pvertex[0].s0 = 0.0;
            pvertex[0].t0 = 0.0;
            //VERTEX 1
            pvertex[1].x = 1.0;
            pvertex[1].y = 0.0;
            pvertex[1].z = 0.0;
            pvertex[1].nx = 0.0;
            pvertex[1].ny = 0.0;
            pvertex[1].nz = 1.0;
            pvertex[1].s0 = 1.0;
            pvertex[1].t0 = 0.0;
            //VERTEX 2
            pvertex[2].x = 0.0;
            pvertex[2].y = 1.0;
            pvertex[2].z = 0.0;
            pvertex[2].nx = 0.0;
            pvertex[2].ny = 0.0;
            pvertex[2].nz = 1.0;
            pvertex[2].s0 = 0.0;
            pvertex[2].t0 = 1.0;

            unsigned short pindices[3];
            pindices[0] = 0;
            pindices[1] = 1;
            pindices[2] = 2;

            glGenBuffersARB(1, &VBO);
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
            //glBufferDataARB(GL_ARRAY_BUFFER_ARB, vbsize, pVertex, GL_STATIC_DRAW);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(MyVertex)* 3, &pvertex[0].x, GL_STATIC_DRAW);

            glGenBuffersARB(1, &IBO);
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IBO);
            //glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ibsize, pIndices, GL_STATIC_DRAW);
            glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(unsigned short)* 3, pindices, GL_STATIC_DRAW);
            mPrimitiveCount = 1;
        }

        void render()
        {
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
            
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
            
            glEnableVertexAttribArrayARB(0);
            glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0));

            glEnableVertexAttribArrayARB(1);
            glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(12));

            glEnableVertexAttribArrayARB(2);
            glVertexAttribPointerARB(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(24));

            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IBO);
            glDrawElements(GL_TRIANGLES, mPrimitiveCount * 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

            glDisableVertexAttribArrayARB(0);
            glDisableVertexAttribArrayARB(1);
            glDisableVertexAttribArrayARB(2);
            

//             glBindBufferARB(GL_ARRAY_BUFFER_ARB, VBO);
// 
//             glEnableVertexAttribArrayARB(0); 
//             glVertexAttribPointerARB(0, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(0)); 
//             
//             glEnableVertexAttribArrayARB(1); 
//             glVertexAttribPointerARB(1, 3, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(12));
//             
//             glEnableVertexAttribArrayARB(2); 
//             glVertexAttribPointerARB(2, 2, GL_FLOAT, GL_FALSE, sizeof(MyVertex), BUFFER_OFFSET(24));
// 
//             glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, IBO);
//             glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
// 
//             glDisableVertexAttribArrayARB(0);
//             glDisableVertexAttribArrayARB(1);
//             glDisableVertexAttribArrayARB(2);
#undef VBO_BUFFER_OFFSET
        }

        GLuint VBO;
        GLuint IBO;
        int mVerticesNum;
        int mPrimitiveCount;
        int mVBSize;

        static GLfloat vertices[9 * 12];
        static GLfloat normals[9 * 12];
        static GLfloat colors[9 * 12];
    };

    class DI_GLDRV_API DiGLDriver : public DiGfxDriver
    {
    public:

        DiGLDriver();

        virtual                 ~DiGLDriver();

        testvbo                 testdat;

    public:

        DiGfxDriverType         GetDriverType() const { return DRV_OPENGL; }

        bool                    InitGfx(DiWndHandle wnd);

        bool                    InitGfx(uint16 width, uint16 height, bool fullscreen);

        void                    ReleaseGfx();

        bool                    IsDeviceLost();

        void                    BeginFrame();

        void                    EndFrame();

        void                    ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiMat4& mat);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec2& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec3& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiVec4& vec);

        void                    SetShaderConsts(DiShaderType type, int regID, float val);

        void                    SetShaderConsts(DiShaderType type, int regID, int val);

        void                    SetShaderConsts(DiShaderType type, int regID, const DiColor& col);

        void                    BindShaders(DiShaderProgram* vs, DiShaderProgram* ps);

        void                    SetViewport(int x, int y, int w, int h, float minz = 0.0f, float maxz = 1.0f);

        void                    SetFillMode(DiFillMode mode);

        bool                    IsDeviceLost() const;

        bool                    ResetDevice(uint16 w, uint16 h);

        bool                    RenderGeometry(DiRenderUnit* unit);

        const DiVec2&           GetTexelOffset() const;

        DiTextureDrv*           CreateTextureDriver(DiTexture* texture);

        DiIndexBuffer*          CreateIndexBuffer();

        DiVertexBuffer*         CreateVertexBuffer();

        DiVertexDeclaration*    CreateVertexDeclaration();

        DiShaderInstance*       CreateVSInstance(DiShaderProgram* prog);

        DiShaderInstance*       CreatePSInstance(DiShaderProgram* prog);

        DiRenderTarget*         CreateRenderTarget();

        DiShaderParameter*      CreateShaderParams(DiMaterial& mat);

        const DiString&         GetShaderFileExtension() const;

        void                    SetColorBufferWriteEnabled(bool r, bool g, bool b, bool a);

        void                    GetDepthStencilFormatFor(GLenum internalColourFormat, GLenum *depthFormat,
                                    GLenum *stencilFormat);
        
        void                    CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd);

        void                    BindMaterialStates(const DiMaterial* mat);

        void                    GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h);

        void                    Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil = 0);

        // Create a new one if not existed
        DiGLShaderLinker*       GetShaderLinker(DiShaderInstance* vs, DiShaderInstance* ps);

        DiGLContext*            GetContext(DiWndHandle wnd);

        DiGLContext*            GetCurrentContext() { return mCurrentContext; }

        DiWindow*               CreateWnd();

        static DiGLBufferManager* BufferMgr;

        static DiGLFBOManager*    FBOManager;

    private:

        DiGLContext*            _CreateContext(DiWndHandle wnd);

        DiGLUtil*               _CreateGLUtil();

        bool                    _BindSourceData(DiRenderUnit* unit);

        bool                    _BindVertexBuffer(DiRenderUnit* unit);

        void                    _InitMainContext(DiGLContext* context);

    private:

        DiString                mWndClass;

        DiGLContext*            mMainContext;

        DiGLContext*            mCurrentContext;
        
        typedef DiMap<DiWndHandle, DiGLContext*> ContextMap;

        ContextMap              mContextMap;

        DiGLUtil*               mGLUtil;

        bool                    mColourWrite[4];

        bool                    mDepthWrite;

        uint32                  mStencilMask;

        DiGLBufferManager*      mGLBufferManager;

        DiGLFBOManager*         mGLFBOManager;

        typedef DiMap<DiPair<DiShaderInstance*, DiShaderInstance*>, DiGLShaderLinker*> ProgramMap;

        ProgramMap              mProgramMaps;

        DiGLShaderLinker*       mCurrentProgram;
    };
}