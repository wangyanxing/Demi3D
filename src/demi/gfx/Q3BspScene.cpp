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
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "Q3BspPatch.h"
#include "RenderUnit.h"

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
        , mNumPatchVertices(0)
        , mNumBspFaces(0)
        , mBspFaces(nullptr)
        , mRenderUnit(nullptr)
        , mNumPatchIndices(0)
        , mIndexBuffer(nullptr)
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

        if (mIndexBuffer)
            DI_DELETE[] mIndexBuffer;

        if (mLeafFaceGroups)
            DI_DELETE[] mLeafFaceGroups;

        if (mBspFaces)
            DI_DELETE[] mBspFaces;

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

        initQuake3Patches(q3lvl);

        InitBspNodes(q3lvl);

        InitFaces(q3lvl);
        
        InitRenderUnit(q3lvl);
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

    void DiBspScene::initQuake3Patches(const DiQ3BspLevel & q3lvl)
    {
        mBspFaces = new Q3BspFaceRenderer[q3lvl.mNumFaces];
        mNumBspFaces = q3lvl.mNumFaces;
        mNumPatchVertices = 0;
        mNumPatchIndices = 0;

        for (int i = 0; i < mNumBspFaces; i++)
        {
            bsp_face_t* src = &q3lvl.mFaces[i];

            mBspFaces[i].lm_index = src->lm_texture;
            mBspFaces[i].meshvert = src->elem_start;
            mBspFaces[i].n_meshverts = src->elem_count;
            mBspFaces[i].n_vertexes = src->vert_count;
            for (int j = 0; j < 3; j++)
                mBspFaces[i].normal[j] = src->normal[j];
            mBspFaces[i].shader = src->shader;
            mBspFaces[i].type = src->type;
            mBspFaces[i].vertex = src->vert_start;

            mBspFaces[i].n_triangles = mBspFaces[i].n_meshverts / 3;
            if (src->type == BSP_FACETYPE_PATCH)
            {
                mBspFaces[i].patch = q3lvl.HandlePatch(i);
                for (int j = 0; j < mBspFaces[i].patch->size; j++)
                {
                    mNumPatchIndices += mBspFaces[i].patch->bezier[j].mNumIndex;
                    mNumPatchVertices += mBspFaces[i].patch->bezier[j].mNumVertex;
                }
            }
            else
                mBspFaces[i].patch = nullptr;
        }
    }

    void DiBspScene::InitRenderUnit(DiQ3BspLevel & q3lvl)
    {
        DI_ASSERT(!mRenderUnit);

        mRenderUnit = DI_NEW DiBspShape();

        mRenderUnit->mVertexDecl = Driver->CreateVertexDeclaration();
        mRenderUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        mRenderUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
        //mRenderUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_COLOR);
        mRenderUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD, 0);
        mRenderUnit->mVertexDecl->AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD, 1);
        mRenderUnit->mVertexDecl->Create();

        mRenderUnit->mVertexBuffer = Driver->CreateVertexBuffer();
        DiVertexBuffer* vb = mRenderUnit->mVertexBuffer;
        int vertNum = q3lvl.mNumVertices + mNumPatchVertices;
        int stride = sizeof(DiQ3BspVertex);
        //mRenderUnit->mVerticesNum = vertNum;
        vb->SetStride(stride);
        vb->Create(stride * vertNum);

        DiQ3BspVertex* data = (DiQ3BspVertex*)vb->Lock(0, stride * vertNum);
        for (int v = 0; v < q3lvl.mNumVertices; ++v)
            BspVertexToBspVertex(&q3lvl.mVertices[v], data++);

        int idNum = q3lvl.mNumElements + mNumPatchIndices;
        mRenderUnit->mIndexBuffer = Driver->CreateIndexBuffer();
        // Create enough index space to render whole level
        mRenderUnit->mIndexBuffer->Create(idNum * sizeof(uint32), IB_32BITS);

        if (mIndexBuffer)
        {
            DI_DELETE[] mIndexBuffer;
        }
        mIndexBuffer = DI_NEW uint32[idNum];

        //void* ib = mRenderUnit->mIndexBuffer->Lock(0, idNum * sizeof(uint32));
        memcpy(mIndexBuffer, q3lvl.mElements, sizeof(uint32)* q3lvl.mNumElements);
        uint32* patchIB = mIndexBuffer + q3lvl.mNumElements;
        
        // patches

        if (mBspFaces)
        {
            int indexBufferindex = 0;
            int vertexBufferindex = 0;

            for (int faceIndex = 0; faceIndex < mNumBspFaces; faceIndex++)
            {
                if (mBspFaces[faceIndex].type == BSP_FACETYPE_PATCH)
                {
                    Q3BspPatch *patch = mBspFaces[faceIndex].patch;
                    if (!patch)
                        continue;

                    for (int bezierIndex = 0; bezierIndex < patch->size; bezierIndex++)
                    {
                        patch->bezier[bezierIndex].mBaseBufferindex = indexBufferindex + q3lvl.mNumElements;

                        for (unsigned int index = 0; index < patch->bezier[bezierIndex].mNumIndex; index++)
                        {
                            patchIB[indexBufferindex] = (uint32)patch->bezier[bezierIndex].mIndex[index];
                            indexBufferindex++;
                        }

                        patch->bezier[bezierIndex].mBaseVertexIndex = vertexBufferindex + q3lvl.mNumVertices;

                        for (unsigned int vertex = 0; vertex < patch->bezier[bezierIndex].mNumVertex; vertex++)
                        {
                            DiQ3BspVertex *bspVertex = &patch->bezier[bezierIndex].mVertex[vertex];

                            data->mPosition = bspVertex->mPosition;
                            data->mNormal = bspVertex->mNormal;
                            data->mTexcoord[0] = bspVertex->mTexcoord[0];
                            data->mTexcoord[1] = bspVertex->mTexcoord[1];
                            data++;

                            vertexBufferindex++;
                        }
                    }
                }
            }
        }

        //mRenderUnit->mIndexBuffer->Unlock();
        vb->Unlock();
    }

    void DiBspScene::BspVertexToBspVertex(const bsp_vertex_t* src, DiQ3BspVertex* dest)
    {
        memcpy(dest->mPosition.ptr(), src->point, sizeof(float)* 3);
        memcpy(dest->mNormal.ptr(), src->normal, sizeof(float)* 3);
        //dest->color = src->color;
        dest->mTexcoord[0].x = src->texcoord[0][0];
        dest->mTexcoord[0].y = src->texcoord[0][1];
        dest->mTexcoord[1].x = src->texcoord[1][0];
        dest->mTexcoord[1].y = src->texcoord[1][1];
    }

    void DiBspScene::InitFaces(DiQ3BspLevel & q3lvl)
    {
        // faces
        mNumLeafFaceGroups = q3lvl.mNumLeafFaces;
        mLeafFaceGroups = DI_NEW int[mNumLeafFaceGroups];
        memcpy(mLeafFaceGroups, q3lvl.mLeafFaces, sizeof(int)* mNumLeafFaceGroups);
        mNumFaceGroups = q3lvl.mNumFaces;

        int face;
        face = q3lvl.mNumFaces;
        while (face--)
        {
            // Check to see if existing material
            // Format shader#lightmap
            int shadIdx = q3lvl.mFaces[face].shader;

            // materials
        }
    }

    void DiBspScene::InitBspNodes(DiQ3BspLevel & q3lvl)
    {
        // Allocate memory for all nodes (leaves and splitters)
        mNumNodes = q3lvl.mNumNodes + q3lvl.mNumLeaves;
        mNumLeaves = q3lvl.mNumLeaves;
        mLeafStart = q3lvl.mNumNodes;
        mRootNode = DI_NEW DiBspNode[mNumNodes];

        for (int i = 0; i < q3lvl.mNumNodes; ++i)
        {
            DiBspNode* node = &mRootNode[i];
            bsp_node_t* q3node = &q3lvl.mNodes[i];

            // Set non-leaf
            node->mIsLeaf = false;
            // Set owner
            node->mParent = this;
            // Set plane
            node->mSplitPlane.normal.x = q3lvl.mPlanes[q3node->plane].normal[0];
            node->mSplitPlane.normal.y = q3lvl.mPlanes[q3node->plane].normal[1];
            node->mSplitPlane.normal.z = q3lvl.mPlanes[q3node->plane].normal[2];
            node->mSplitPlane.d = -q3lvl.mPlanes[q3node->plane].dist;
            // Set bounding box
            node->mBounds.SetMinimum(DiVec3(&q3node->bbox[0]));
            node->mBounds.SetMaximum(DiVec3(&q3node->bbox[3]));

            if (q3node->back < 0)
            {
                // Points to leaf, offset to leaf start and negate index
                node->mBack = &mRootNode[mLeafStart + (~(q3node->back))];
            }
            else
            {
                // Points to node
                node->mBack = &mRootNode[q3node->back];
            }
            // Set front pointer
            // Negative indexes in Quake3 mean leaves
            if (q3node->front < 0)
            {
                // Points to leaf, offset to leaf start and negate index
                node->mFront = &mRootNode[mLeafStart + (~(q3node->front))];
            }
            else
            {
                // Points to node
                node->mFront = &mRootNode[q3node->front];
            }
        }

        // Brushes
        mNumBrushes = q3lvl.mNumBrushes;
        mBrushes = DI_NEW DiBspNode::Brush[mNumBrushes];

        for (int i = 0; i < q3lvl.mNumBrushes; ++i)
        {
            bsp_brush_t* q3brush = &q3lvl.mBrushes[i];

            // Create a new OGRE brush
            DiBspNode::Brush *pBrush = &(mBrushes[i]);
            int brushSideIdx, numBrushSides;
            numBrushSides = q3brush->numsides;
            brushSideIdx = q3brush->firstside;
            // Iterate over the sides and create plane for each
            while (numBrushSides--)
            {
                bsp_brushside_t* q3brushside = &q3lvl.mBrushSides[brushSideIdx];
                bsp_plane_t* q3brushplane = &q3lvl.mPlanes[q3brushside->planenum];
                // Notice how we normally invert Q3A plane distances, but here we do not
                // Because we want plane normals pointing out of solid brushes, not in
                DiPlane brushSide(
                    DiVec3(
                    q3brushplane->normal[0],
                    q3brushplane->normal[1],
                    q3brushplane->normal[2]),
                    q3brushplane->dist);
                pBrush->planes.push_back(brushSide);
                ++brushSideIdx;
            }
            // Build world fragment
            pBrush->fragment.fragmentType = DiSceneQuery::WFT_PLANE_BOUNDED_REGION;
            pBrush->fragment.planes = &(pBrush->planes);
        }

        // Leaves
        for (int i = 0; i < q3lvl.mNumLeaves; ++i)
        {
            DiBspNode* node = &mRootNode[i + mLeafStart];
            bsp_leaf_t* q3leaf = &q3lvl.mLeaves[i];

            // Set leaf
            node->mIsLeaf = true;
            // Set owner
            node->mParent = this;
            // Set bounding box
            node->mBounds.SetMinimum(DiVec3(&q3leaf->bbox[0]));
            node->mBounds.SetMaximum(DiVec3(&q3leaf->bbox[3]));
            // Set faces
            node->mFaceGroupStart = q3leaf->face_start;
            node->mNumFaceGroups = q3leaf->face_count;

            node->mVisCluster = q3leaf->cluster;

            // Load Brushes for this leaf
            int brushIdx, brushCount, realBrushIdx;
            brushCount = q3leaf->brush_count;
            brushIdx = q3leaf->brush_start;

            while (brushCount--)
            {
                realBrushIdx = q3lvl.mLeafBrushes[brushIdx];
                bsp_brush_t* q3brush = &q3lvl.mBrushes[realBrushIdx];
                // Only load solid ones, we don't care about any other types
                // Shader determines this
                bsp_shader_t* brushShader = &q3lvl.mShaders[q3brush->shaderIndex];
                if (brushShader->content_flags & CONTENTS_SOLID)
                {
                    // Get brush 
                    DiBspNode::Brush *pBrush = &(mBrushes[realBrushIdx]);
                    DI_ASSERT(pBrush->fragment.fragmentType == DiSceneQuery::WFT_PLANE_BOUNDED_REGION);
                    // Assign node pointer
                    node->mSolidBrushes.push_back(pBrush);
                }
                ++brushIdx;
            }
        }

        mVisData.numClusters = q3lvl.mVis->cluster_count;
        mVisData.rowLength = q3lvl.mVis->row_size;
        mVisData.tableData = DI_NEW uint8[q3lvl.mVis->row_size * q3lvl.mVis->cluster_count];
        memcpy(mVisData.tableData, q3lvl.mVis->data, q3lvl.mVis->row_size * q3lvl.mVis->cluster_count);
    }

    uint32 DiBspScene::CacheGeometry(uint32* pIndexes, Q3BspFaceRenderer* faceGroup)
    {
        uint32 idxStart, numIdx, vertexStart;

        if (faceGroup->type == BSP_FACETYPE_MESH || faceGroup->type == BSP_FACETYPE_NORMAL)
        {
            idxStart = faceGroup->meshvert;
            numIdx = faceGroup->n_triangles * 3;
            vertexStart = faceGroup->vertex;
        }
        else if (faceGroup->type == BSP_FACETYPE_PATCH && faceGroup->patch)
        {
//          idxStart = faceGroup->patch->bezier
//          numIdx = faceGroup->patch->getCurrentIndexCount();
//          vertexStart = faceGroup->patch->getVertexOffset();
            return 0;
        }
        else
        {
            // Unsupported face type
            return 0;
        }

        uint32* pSrc = mIndexBuffer + idxStart;
        for (size_t elem = 0; elem < numIdx; ++elem)
            *pIndexes++ = *pSrc++ + static_cast<unsigned int>(vertexStart);

        return numIdx;
    }

    DiBspScene::DiBspShape::DiBspShape()
        : mVertexDecl(nullptr)
        , mVertexBuffer(nullptr)
        , mIndexBuffer(nullptr)
    {
        SetBatchGroup(BATCH_TERRAIN);
    }

    DiBspScene::DiBspShape::~DiBspShape()
    {
        ClearUnits();

        mRenderUnits.clear();
        mVertexBuffer->Release();
        mVertexDecl->Release();
        mIndexBuffer->Release();

        DI_DELETE mVertexBuffer;
        DI_DELETE mVertexDecl;
        DI_DELETE mIndexBuffer;
    }

    DiString& DiBspScene::DiBspShape::GetType()
    {
        static DiString type = "BspLevel";
        return type;
    }

    void DiBspScene::DiBspShape::AddToBatchGroup(DiRenderBatchGroup* bg)
    {
        for (auto it = mRenderUnits.begin(); it != mRenderUnits.end(); ++it)
            bg->AddRenderUnit(*it);
    }

    const DiAABB& DiBspScene::DiBspShape::GetBoundingBox(void) const
    {
        return DiAABB::BOX_INFINITE;
    }

    void DiBspScene::DiBspShape::ClearUnits()
    {
        for (auto it = mRenderUnits.begin(); it != mRenderUnits.end(); ++it)
            DI_DELETE (*it);
        mRenderUnits.clear();
    }
}