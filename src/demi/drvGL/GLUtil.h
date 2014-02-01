
/********************************************************************
    File:       GLUtil.h
    Creator:    demiwangya
*********************************************************************/

#ifndef GLUtil_h__
#define GLUtil_h__

namespace Demi
{
    class DI_GLDRV_API DiGLUtil
    {
    public:

        DiGLUtil(){}

        virtual ~DiGLUtil(){}

    public:

        const DiString&     GetGLVendor() const
        {
            return mVendor;
        }

        const DiString&     GetGLVersion() const
        {
            return mVersion;
        }

        bool                CheckMinGLVersion(const DiString& v) const;

        bool                CheckExtension(const DiString& ext) const;

        virtual void*       GetProcAddress(const DiString& procname) = 0;

        virtual void        InitExtensions();

        virtual uint8       GetDisplayMonitorCount() const
        {
            //TODO
            return 1;
        }

        virtual DiWindow*   CreateNewWindow() = 0;

    protected:

        DiString            mVersion;

        DiString            mVendor;

        DiSet<DiString>     mExtensionList;
    };
}

#endif // GLUtil_h__