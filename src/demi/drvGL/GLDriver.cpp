
#include "GLDriver.h"
#include "RenderTarget.h"
#include "RenderUnit.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "GfxDriver.h"
#include "RenderWindow.h"
#include "GLContext.h"

#ifdef WIN32
#   include "Win32Window.h"
#   include "Win32GLContext.h"
#   include "Win32GLUtil.h"
#endif

namespace Demi
{
    DiGLDriver::DiGLDriver()
        :mMainContext(nullptr)
    {
    }
    
    DiGLDriver::~DiGLDriver()
    {
    }

    bool DiGLDriver::InitGfx(DiWndHandle wnd)
    {
        DI_LOG("OpenGL driver is intializing.");

        mMainContext = _CreateContext(wnd);

        return true;
    }

    bool DiGLDriver::InitGfx(uint16 width, uint16 height, bool fullscreen)
    {
        DI_LOG("Creating window: %d x %d, %s mode.", width, height, fullscreen?"full screen":"window");
        mWidth = width;
        mHeight = height;

        return true;
    }

    bool DiGLDriver::IsDeviceLost()
    {
        return false;
    }

    void DiGLDriver::BeginFrame()
    {
    }
    
    void DiGLDriver::EndFrame()
    {
    }

    void DiGLDriver::ReleaseGfx()
    {
        if (mMainContext)
        {
            DI_DELETE mMainContext;
            mMainContext = nullptr;
        }

        DI_LOG("OpenGL stuff successfully released.");
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiMat4& mat)
    {
        if (regID < 0)
            return;

        static DiMat4 m;
        mat.transpose(m);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec2& vec)
    {
        static DiVec4 t;

        if (regID < 0)
            return;

        t.x = vec.x;
        t.y = vec.y;
        t.z = 0;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec3& vec)
    {
        static DiVec4 t;

        if (regID < 0)
            return;

        t.x = vec.x;
        t.y = vec.y;
        t.z = vec.z;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiVec4& vec)
    {
        if (regID < 0)
            return;
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, float val)
    {
        if (regID < 0)
            return;

        static DiVec4 t;
        t.x = val;
        t.y = 0;
        t.z = 0;
        t.w = 0;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, int val)
    {
        if (regID < 0)
            return;
    }

    void DiGLDriver::SetShaderConsts(DiShaderType shaderType, int regID, const DiColor& col)
    {
        if (regID < 0)
            return;

        static DiVec4 t;
        t.x = col.r;
        t.y = col.g;
        t.z = col.b;
        t.w = col.a;

        SetShaderConsts(shaderType, regID, t);
    }

    void DiGLDriver::ConvertProjectionMatrix(const DiMat4& matrix, DiMat4& dest)
    {
        dest = matrix;
        dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
        dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
        dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
        dest[2][3] = (dest[2][3] + dest[3][3]) / 2;
    }

    bool DiGLDriver::IsDeviceLost() const
    {
        return false;
    }

    bool DiGLDriver::RenderGeometry(DiRenderUnit* unit)
    {
        return true;
    }

    const DiVec2& DiGLDriver::GetTexelOffset() const
    {
        static DiVec2 texelOffset(-0.5f, -0.5f);
        return texelOffset;
    }

    bool DiGLDriver::ResetDevice(uint16 w, uint16 h)
    {
        return true;
    }

    void DiGLDriver::SetViewport(int x, int y, int w, int h, float minz, float maxz)
    {
    }

    void DiGLDriver::SetFillMode(DiFillMode mode)
    {
    }

    DiTextureDrv* DiGLDriver::CreateTextureDriver(DiTexture* texture)
    {
        return nullptr;
    }

    DiIndexBuffer* DiGLDriver::CreateIndexBuffer()
    {
        return nullptr;
    }

    DiVertexBuffer* DiGLDriver::CreateVertexBuffer()
    {
        return nullptr;
    }

    DiVertexDeclaration* DiGLDriver::CreateVertexDeclaration()
    {
        return nullptr;
    }

    DiShaderInstance* DiGLDriver::CreateVSInstance(DiShaderProgram* prog)
    {
        return nullptr;
    }

    DiShaderInstance* DiGLDriver::CreatePSInstance(DiShaderProgram* prog)
    {
        return nullptr;
    }

    DiRenderTarget* DiGLDriver::CreateRenderTarget()
    {
        return nullptr;
    }

    void DiGLDriver::CreateWindowTarget(DiRenderTarget*& outRT, DiWndHandle wnd)
    {
        
    }

    void DiGLDriver::GetWindowDimension(DiWndHandle wnd, uint32& w, uint32& h)
    {
        RECT rect;
        GetClientRect((HWND)wnd, &rect);
        w = rect.right - rect.left;
        h = rect.bottom - rect.top;
    }

    void DiGLDriver::BindMaterialStates(const DiMaterial* mat)
    {
        
    }

    void DiGLDriver::Clear(uint32 flag, const DiColor& col, float depth, unsigned short stencil /*= 0*/)
    {

    }

    DiWindow* DiGLDriver::CreateWnd()
    {
        return DI_NEW DiWin32Window();
    }

    DiGLContext* DiGLDriver::_CreateContext(DiWndHandle wnd)
    {
        DiGLContext* ret = nullptr;
#ifdef WIN32
        ret = DI_NEW DiWin32GLContext(wnd);
        DI_LOG("Win32 GL context created.");
#endif
        return ret;
    }
}