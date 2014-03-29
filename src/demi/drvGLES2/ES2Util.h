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

#ifndef GLUtil_h__
#define GLUtil_h__

namespace Demi
{
    /** OpenGL utilities class which is responsible for
        the initialization of the GL environment and contexts
        as well as the extension managements
     */
    class DI_GLES2_API DiGLUtil
    {
    public:

        DiGLUtil(){}

        virtual             ~DiGLUtil(){}

    public:

        const DiString&     GetGLVendor() const { return mVendor; }

        const DiString&     GetGLVersion() const { return mVersion; }

        bool                CheckMinGLVersion(const DiString& v) const;

        /** Check if a specific extension can be supported
         */
        bool                CheckExtension(const DiString& ext) const;

        virtual void*       GetProcAddress(const DiString& procname) = 0;

        virtual void        InitExtensions();

        /** For multi-monitors supporting
         */
        virtual uint8       GetDisplayMonitorCount() const
        {
            //TODO
            return 1;
        }

        /** Depends on the specific platform
         */
        virtual DiWindow*   CreateNewWindow() = 0;

    protected:

        /// OpengGL version
        DiString            mVersion;
        
        /// Major graphics device vendor
        DiString            mVendor;

        /// Platform independently extension list
        DiSet<DiString>     mExtensionList;
    };
}

#endif // GLUtil_h__