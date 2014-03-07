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

#include "GfxPch.h"
#include "Q3BspScene.h"
#include "AssetManager.h"

namespace Demi
{
#define NUM_FACES_PER_PROGRESS_REPORT 100
#define NUM_NODES_PER_PROGRESS_REPORT 50
#define NUM_LEAVES_PER_PROGRESS_REPORT 50
#define NUM_BRUSHES_PER_PROGRESS_REPORT 50

    DiBspScene::DiBspScene(const DiString& name)
        : mRootNode(nullptr)
        , mBrushes(nullptr)
        , mSkyEnabled(false)
        , mName(name)
    {

    }

    DiBspScene::~DiBspScene()
    {
        Unload();
    }

    bool DiBspScene::isLeafVisible(const DiBspNode* from, const DiBspNode* to) const
    {
        if (to->mVisCluster == -1)
            return false;

        if (from->mVisCluster == -1)
            // Camera outside world?
            return true;

        if (!from->IsLeaf() || !to->IsLeaf())
        {
            DI_WARNING("Both nodes must be leaf nodes for visibility testing.");
            return false;
        }

        // Use PVS to determine visibility
        return (*(mVisData.tableData + from->mVisCluster*mVisData.rowLength +
            ((to->mVisCluster) >> 3)) & (1 << ((to->mVisCluster) & 7))) != 0;
    }

    DiBspNode* DiBspScene::findLeaf(const DiVec3& point) const
    {
        DiBspNode* node = mRootNode;

        while (!node->IsLeaf())
            node = node->GetNextNode(point);

        return node;
    }

    void DiBspScene::_notifyObjectMoved(const DiTransUnitPtr mov, const DiVec3& pos)
    {
        // Locate any current nodes the object is supposed to be attached to
        MovableToNodeMap::iterator i = mMovableToNodeMap.find(mov);
        if (i != mMovableToNodeMap.end())
        {
            DiList<DiBspNode*>::iterator nodeit, nodeitend;
            nodeitend = i->second.end();
            for (nodeit = i->second.begin(); nodeit != nodeitend; ++nodeit)
            {
                // Tell each node
                (*nodeit)->RemoveTransUnit(mov);
            }
            // Clear the existing list of nodes because we'll reevaluate it
            i->second.clear();
        }

        tagNodesWithMovable(mRootNode, mov, pos);
    }

    void DiBspScene::_notifyObjectDetached(const DiTransUnitPtr mov)
    {
        // Locate any current nodes the object is supposed to be attached to
        MovableToNodeMap::iterator i = mMovableToNodeMap.find(mov);
        if (i != mMovableToNodeMap.end())
        {
            DiList<DiBspNode*>::iterator nodeit, nodeitend;
            nodeitend = i->second.end();
            for (nodeit = i->second.begin(); nodeit != nodeitend; ++nodeit)
            {
                // Tell each node
                (*nodeit)->RemoveTransUnit(mov);
            }
            // delete the entry for this MovableObject
            mMovableToNodeMap.erase(i);
        }
    }

    size_t DiBspScene::calculateLoadingStages(const DiString& levelName)
    {
        auto data = DiAssetManager::GetInstance().OpenArchive(levelName);
        return calculateLoadingStages(data);
    }

    size_t DiBspScene::calculateLoadingStages(DiDataStreamPtr& stream)
    {
        DiQ3BspLevel q3;

        // Load header only
        q3.LoadHeader(stream);

        // Ok, count up the things that we will report
        size_t stages = 0;

        // loadEntities (1 stage)
        ++stages;
        // extractLightmaps (external, 1 stage)
        ++stages;
        // initQuake3Patches
        ++stages;
        // vertex setup
        ++stages;
        // face setup
        ++stages;
        // patch building
        ++stages;
        // material setup
        // this is not strictly based on load, since we only know the number
        // of faces, not the number of materials
        // raise one event for every 50 faces, plus one at the end
        stages += (q3.mNumFaces / NUM_FACES_PER_PROGRESS_REPORT) + 1;
        // node setup
        stages += (q3.mNumNodes / NUM_NODES_PER_PROGRESS_REPORT) + 1;
        // brush setup
        stages += (q3.mNumBrushes / NUM_BRUSHES_PER_PROGRESS_REPORT) + 1;
        // leaf setup
        stages += (q3.mNumLeaves / NUM_LEAVES_PER_PROGRESS_REPORT) + 1;
        // vis
        ++stages;

        return stages;
    }

    void DiBspScene::load(DiDataStreamPtr& stream)
    {
        // Use Quake3 file loader
        DiQ3BspLevel q3;
        q3.LoadLevel(stream);

        loadQuake3Level(q3);
    }

    void DiBspScene::Load(void)
    {
        mSkyEnabled = false;
        auto data = DiAssetManager::GetInstance().OpenArchive(mName);
        load(data);
    }

    void DiBspScene::Unload(void)
    {
//         if (mVertexData)
//             OGRE_DELETE mVertexData;
//         mIndexes.setNull();
//         if (mFaceGroups)
//             OGRE_DELETE_ARRAY_T(mFaceGroups, StaticFaceGroup, (size_t)mNumFaceGroups, MEMCATEGORY_GEOMETRY);
//         if (mLeafFaceGroups)
//             OGRE_FREE(mLeafFaceGroups, MEMCATEGORY_GEOMETRY);

        if (mRootNode)
            DI_DELETE[] mRootNode;

        if (mVisData.tableData)
            DI_DELETE[] mVisData.tableData;

        if (mBrushes)
            DI_DELETE[] mBrushes;

        //mVertexData = 0;
        mRootNode = 0;
        //mFaceGroups = 0;
        mLeafFaceGroups = 0;
        mBrushes = 0;
        mVisData.tableData = 0;
//         for (auto pi = mPatches.begin(); pi != mPatches.end(); ++pi)
//         {
//             DI_DELETE pi->second;
//         }
        //mPatches.clear();
    }

    void DiBspScene::loadQuake3Level(DiQ3BspLevel& q3lvl)
    {
        loadEntities(q3lvl);

        q3lvl.ExtractLightmaps();

    }

    void DiBspScene::tagNodesWithMovable(DiBspNode* node, const DiTransUnitPtr mov, const DiVec3& pos)
    {
        if (node->IsLeaf())
        {
            // Add to movable->node map
            // Insert all the time, will get current if already there
            std::pair<MovableToNodeMap::iterator, bool> p =
                mMovableToNodeMap.insert(
                MovableToNodeMap::value_type(mov, DiList<DiBspNode*>()));

            p.first->second.push_back(node);

            // Add movable to node
            node->AddTransUnit(mov);
        }
        else
        {
            // Find distance to dividing plane
            float dist = node->GetDistance(pos);

            // TODO
            if (DiMath::Abs(dist) < mov->GetBoundingBox().GetRadius())
            {
                // Bounding sphere crosses the plane, do both
                tagNodesWithMovable(node->GetBack(), mov, pos);
                tagNodesWithMovable(node->GetFront(), mov, pos);
            }
            else if (dist < 0)
            { 
                tagNodesWithMovable(node->GetBack(), mov, pos);
            }
            else
            {
                tagNodesWithMovable(node->GetFront(), mov, pos);
            }
        }
    }

    void DiBspScene::loadEntities(const DiQ3BspLevel& q3lvl)
    {
        char* strEnt;
        DiString line;
        DiVector<DiString> vecparams;
        DiVec3 origin = DiVec3::ZERO;
        DiRadian angle(0);
        size_t pos;
        char* lineend;
        bool isPlayerStart;

        isPlayerStart = false;
        strEnt = (char*)q3lvl.mEntities;

        lineend = strchr(strEnt, '\n');
        while (lineend != 0)
        {
            *lineend = '\0';
            line = strEnt;
            strEnt = lineend + 1;
            line.Trim("\t\n ");
            if (line.length() > 0)
            {
                line.ToLower();
                // Remove quotes
                while ((pos = line.find("\"", 0)) != DiString::npos)
                {
                    line = line.substr(0, pos) + line.substr(pos + 1, line.length() - (pos + 1));
                }
                vecparams = line.Tokenize("\t\n ");
                auto params = vecparams.begin();

                if (params[0] == "origin")
                {
                    origin.x = atof(params[1].c_str());
                    origin.y = atof(params[2].c_str());
                    origin.z = atof(params[3].c_str());
                }
                if (params[0] == "angle")
                {
                    angle = DiDegree(atof(params[1].c_str()));
                }
                if (params[0] == "classname" && params[1] == "info_player_deathmatch")
                {
                    isPlayerStart = true;
                }
                if (params[0] == "}")
                {
                    if (isPlayerStart)
                    {
                        // Save player start
                        DiVec3 position = origin;
                        DiQuat orientation;
                        orientation.FromAngleAxis(angle, DiVec3::UNIT_Z);

                        //mPlayerStarts.push_back(vp);
                    }
                    isPlayerStart = false;
                }
            }

            lineend = strchr(strEnt, '\n');
        }
    }
}