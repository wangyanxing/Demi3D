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

#ifndef DiQ3BspLevel_h__
#define DiQ3BspLevel_h__

#include "Q3BspFormats.h"

namespace Demi
{
    class DiQ3BspLevel
    {
    public:
        
        DiQ3BspLevel();
        
        ~DiQ3BspLevel();
        
    public:
        
        void    LoadHeader(DiDataStreamPtr& inStream);
        
        void    LoadLevel(DiDataStreamPtr& inStream);
        
        void    ExtractLightmaps(void) const;
        
        
        /** Utility function read the header and set up pointers. */
        void    Initialise(bool headerOnly = false);
        
        /** Utility function read the header and set up counters. */
        void    InitialiseCounts(void);
        
        /** Utility function read the header and set up pointers. */
        void    InitialisePointers(void);

        void*   GetLump(int lumpType);
        
        int     GetLumpSize(int lumpType);

        void    DumpContents(void);
        
    public:
        
        DiMemoryDataStreamPtr mChunk;
        
        // NB no brushes, fog or local lightvolumes yet
        bsp_header_t*   mHeader;
        unsigned char*  mLumpStart;
        
        int* mElements; // vertex indexes for faces
        int mNumElements;
        
        void* mEntities;
        int mNumEntities;
        
        bsp_model_t* mModels;
        int mNumModels;
        
        bsp_node_t* mNodes;
        int mNumNodes;
        
        bsp_leaf_t* mLeaves;
        int mNumLeaves;
        
        int* mLeafFaces;     // Indexes to face groups by leaf
        int mNumLeafFaces;
        
        bsp_plane_t* mPlanes;
        int mNumPlanes;
        
        bsp_face_t* mFaces;      // Groups of faces
        int mNumFaces;
        
        bsp_vertex_t* mVertices;
        int mNumVertices;
        
        bsp_shader_t* mShaders;
        int mNumShaders;
        
        unsigned char* mLightmaps;
        int mNumLightmaps;
        
        bsp_vis_t* mVis;
        
        bsp_brush_t* mBrushes;
        int mNumBrushes;
        
        bsp_brushside_t* mBrushSides;
        int mNumBrushSides;
        
        int* mLeafBrushes;      // Groups of indexes to brushes by leaf
        int mNumLeafBrushes;
    };
}

#endif
