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

#ifndef DiK2Configs_h__
#define DiK2Configs_h__

#include "K2Prerequisites.h"

namespace Demi
{
    /** Global configuration manager
     */
    class DEMI_K2_API DiK2Configs : public DiBase
    {
    public:

        static void Init();

        static void Shutdown();

        static DiString GetK2MediaPath(const DiString& relativePath);

        /** Get the real data stream by a relative path
            e.g. we can use "heroes/aluna/model.mdf" as the argument
         */
        static DiDataStreamPtr GetDataStream(const DiString& relPath, bool asBinary);
    };
}

#endif