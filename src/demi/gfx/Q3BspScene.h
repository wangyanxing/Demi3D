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

#ifndef DiQ3BspScene_h__
#define DiQ3BspScene_h__

#include "Q3BspLevel.h"
#include "Q3BspNode.h"

namespace Demi
{
    class DiBspScene
    {
    public:
        DiBspScene(const DiString& name);

        ~DiBspScene();

        /** Determines if one leaf node is visible from another. */
        bool isLeafVisible(const DiBspNode* from, const DiBspNode* to) const;

        /** Returns a pointer to the root node (BspNode) of the BSP tree. */
        const DiBspNode* getRootNode(void)
        {
            return mRootNode;
        }

        /** Walks the entire BSP tree and returns the leaf
        which contains the given point.
        */
        DiBspNode* findLeaf(const DiVec3& point) const;

        /** Ensures that the MovableObject is attached to the right leaves of the
        BSP tree.
        */
        void _notifyObjectMoved(const DiTransUnitPtr mov, const DiVec3& pos);

        /** Internal method, makes sure an object is removed from the leaves when detached from a node. */
        void _notifyObjectDetached(const DiTransUnitPtr mov);

        /** Gets a pointer to the start of the leaf nodes. */
        DiBspNode* getLeafStart(void) { return &mRootNode[mLeafStart]; }

        /** Gets the number of leaf nodes */
        int getNumLeaves(void) const { return mNumLeaves; }

        /** Calculate the number of loading stages required for a given level */
        static size_t calculateLoadingStages(const DiString& levelName);

        /** Calculate the number of loading stages required for a given level */
        static size_t calculateLoadingStages(DiDataStreamPtr& stream);

        /** Load direct from stream */
        void load(DiDataStreamPtr& stream);

        /** Is sky enabled? */
        bool isSkyEnabled(void) const
        {
            return mSkyEnabled;
        }

        /** Get Sky material name */
        const DiString& getSkyMaterialName(void) const
        {
            return mSkyMaterial;
        }

        /** Get sky curvature */
        float getSkyCurvature(void) const
        {
            return mSkyCurvature;
        }

        /** Utility class just to enable queueing of patches */
    protected:

        void Load(void);

        void Unload(void);
        
        /** Pointer to the root node of the BSP tree;
        This pointer actually has a dual purpose; to avoid allocating lots of small chunks of
        memory, the BspLevel actually allocates all nodes required through this pointer. So this
        pointer is the handle for the allocation of memory for all nodes. It also happens to point
        to the root node, since the first one in the memory chunk is the root node.
        */
        DiBspNode* mRootNode;
        int mNumNodes;
        int mNumLeaves;
        int mNumBrushes;
        int mLeafStart; /// The index at which leaf nodes begin

        /** Vertex format for fixed geometry.
        Note that in this case vertex components (position, normal, texture coords etc)
        are held interleaved in the same buffer. However, the format here is different from
        the format used by Quake because older Direct3d drivers like the vertex elements
        to be in a particular order within the buffer. See VertexDeclaration for full
        details of this marvellous(not) feature.
        */
        struct BspVertex
        {
            float position[3];
            float normal[3];
            int colour;
            float texcoords[2];
            float lightmap[2];
        };

        /// Vertex data holding all the data for the level, but able to render parts of it
        //VertexData* mVertexData;

        /** Array of indexes into the mFaceGroups array. This buffer is organised
        by leaf node so leaves can just use contiguous chunks of it and
        get repointed to the actual entries in mFaceGroups. */
        int* mLeafFaceGroups;
        int mNumLeafFaceGroups;

        /** Array of face groups, indexed into by contents of mLeafFaceGroups. */
        //StaticFaceGroup* mFaceGroups;
        int mNumFaceGroups;

        /// Indexes for the whole level, will be copied to the real indexdata per frame
        size_t mNumIndexes;
        /// System-memory buffer
        //HardwareIndexBufferSharedPtr mIndexes;

        /// Brushes as used for collision, main memory is here
        DiBspNode::Brush *mBrushes;

        /** Vector of player start points */
        //vector<ViewPoint>::type mPlayerStarts;

        /** Internal utility function for loading data from Quake3. */
        void loadQuake3Level( DiQ3BspLevel& q3lvl);
        /** Internal lookup table to determine visibility between leaves.
        Leaf nodes are assigned to 'clusters' of nodes, which are used to group nodes together for
        visibility testing. This data holds a lookup table which is used to determine if one cluster of leaves
        is visible from another cluster. Whilst it would be possible to expand all this out so that
        each node had a list of pointers to other visible nodes, this would be very expensive in terms
        of storage (using the cluster method there is a table which is 1-bit squared per cluster, rounded
        up to the nearest byte obviously, which uses far less space than 4-bytes per linked node per source
        node). Of course the limitation here is that you have to each leaf in turn to determine if it is visible
        rather than just following a list, but since this is only done once per frame this is not such a big
        overhead.
        Each row in the table is a 'from' cluster, with each bit in the row corresponding to a 'to' cluster,
        both ordered based on cluster index. A 0 in the bit indicates the 'to' cluster is not visible from the
        'from' cluster, whilst a 1 indicates it is.
        As many will notice, this is lifted directly from the Quake implementation of PVS.
        */
        struct VisData
        {
            unsigned char *tableData;
            int numClusters;            /// Number of clusters, therefore number of rows
            int rowLength;                /// Length in bytes of each row (num clusters / 8 rounded up)
        };

        VisData mVisData;


        /** Internal method for parsing chosen entities. */
        void loadEntities(const DiQ3BspLevel& q3lvl);

        typedef DiMap<const DiTransUnitPtr, DiList<DiBspNode*> > MovableToNodeMap;
        /// Map for locating the nodes a movable is currently a member of
        MovableToNodeMap mMovableToNodeMap;

        void tagNodesWithMovable(DiBspNode* node, const DiTransUnitPtr mov, const DiVec3& pos);

        /// Storage of patches
        //typedef map<int, PatchSurface*>::type PatchMap;
        //PatchMap mPatches;
        /// Total number of vertices required for all patches
        size_t mPatchVertexCount;
        /// Total number of indexes required for all patches
        size_t mPatchIndexCount;
        /// Sky enabled?
        bool mSkyEnabled;
        /// Sky material
        DiString mSkyMaterial;
        /// Sky details
        float mSkyCurvature;

        DiString mName;

        //void initQuake3Patches(const DiQ3BspLevel & q3lvl, VertexDeclaration* decl);

        void buildQuake3Patches(size_t vertOffset, size_t indexOffset);

        void quakeVertexToBspVertex(const bsp_vertex_t* src, BspVertex* dest);
    };
}

#endif
