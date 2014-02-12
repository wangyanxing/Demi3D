


#ifndef DiGLContext_h__
#define DiGLContext_h__


namespace Demi
{
    class DI_GLDRV_API DiGLContext
    {
    public:

        DiGLContext() {}

        virtual                 ~DiGLContext(){}

    public:

        virtual void            BeginContext() = 0;

        virtual void            EndContext() = 0;

        virtual void            Release() = 0;

        virtual DiGLContext*    Clone() const = 0;
    };
}

#endif
