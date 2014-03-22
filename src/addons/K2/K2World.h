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

#ifndef DiK2World_h__
#define DiK2World_h__

#include "K2Prerequisites.h"

namespace Demi
{
    class DEMI_K2_API DiK2World : public DiBase
    {
    public:

        DiK2World();

        ~DiK2World();

        friend class DiK2WorldSerial;

    public:

        void            Unload();

        void            Load(const DiString& path);

        DiCullNode*     CreateNode(DiSceneManager* sm);

    private:

        DiTerrainPtr    mTerrain;

        DiCullNode*     mRootNode;
    };
}

#endif