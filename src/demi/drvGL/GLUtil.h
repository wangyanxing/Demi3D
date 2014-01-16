
/********************************************************************
    File:       GLUtil.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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

        virtual bool        CheckExtension(const DiString& ext) const;

        virtual void*       GetProcAddress(const DiString& procname) = 0;

        virtual void        InitExtensions();

        virtual uint8       GetDisplayMonitorCount() const
        {
            return 1;
        }

    protected:

        DiString            mVersion;

        DiString            mVendor;

        DiSet<DiString>     mExtensionList;
    };
}