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

//#define _DEBUG_CLIFF_POS

namespace Demi
{
    class DEMI_K2_API DiK2World : public DiBase
    {
    public:

        DiK2World();

        virtual ~DiK2World();

        friend class DiK2WorldSerial;

    public:

        void                Update(float dt);

        void                Unload();

        void                Load(const DiString& path);

        DiString            GetName() { return mName; }

        DiTerrainPtr        GetTerrain() { return mTerrain; }

        /** From world serialization
         */
        void                ProcessWorldEntity(const DiString& mdf, const DiString& type, 
                                const Trans& trans, int id, int team);

        /** add a map model (not NPCs or heroes)
         */
        DiK2RenderObject*   AddRenderObj(const DiString& mdf, K2ObjSubTypes::Type type, const Trans& trans, int id);

        /** Remove and delete
         */
        bool                RemoveRenderObj(DiK2RenderObject*);

        void                RemoveAllRenderObjs();

        DiK2RenderObject*   CreateRenderObject( K2ObjTypes type );

        /** id: usually team id
         */
        bool                GetSpawnPoint(int id, DiVec3& out);

    private:

        void                ProcessTrees(DiK2RenderObject* obj);

        void                ProcessCliff(DiK2RenderObject* obj);

    public:

        DiTerrainPtr        mTerrain;

        DiCullNode*         mRootNode;

        DiString            mName;

        typedef DiSet<DiK2RenderObject*> RenderObjs;
        RenderObjs          mRenderObjs;

#ifdef _DEBUG_CLIFF_POS
        DiDebugHelperPtr    mDebugger;
#endif

        DiMap<int, DiVec3>  mSpawnPoint;

    };
}

#endif