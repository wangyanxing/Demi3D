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

        DiK2World(DiSceneManager* sm);

        ~DiK2World();

        friend class DiK2WorldSerial;

    public:

        void            Unload();

        void            Load(const DiString& path);

        DiString        GetName() { return mName; }

        DiTerrainPtr    GetTerrain() { return mTerrain; }

        /** add a map model (not NPCs or heroes)
         */
        DiK2Model*      AddModel(const DiString& mdf, const DiString& type, const Trans& trans);

        uint32          GetNumModels() const { return mModels.size(); }

        DiK2Model*      GetModel(uint32 id) { return mModels[id]; }

    private:

        void            ProcessTrees(DiK2Model* model);

        void            ProcessCliff(DiK2Model* model);

    private:

        DiTerrainPtr    mTerrain;

        DiCullNode*     mRootNode;

        DiString        mName;

        DiVector<DiK2Model*> mModels;
    };
}

#endif