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
#include "Q3BspLevel.h"
#include "DataStream.h"
#include "AssetManager.h"
#include "Texture.h"
#include "Image.h"
#include "Q3BspPatch.h"

namespace Demi
{
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    // byte swapping functions
    static void SwapFourBytes(uint32* dw)
    {
        uint32 tmp;
        tmp =  (*dw & 0x000000FF);
        tmp = ((*dw & 0x0000FF00) >> 0x08) | (tmp << 0x08);
        tmp = ((*dw & 0x00FF0000) >> 0x10) | (tmp << 0x08);
        tmp = ((*dw & 0xFF000000) >> 0x18) | (tmp << 0x08);
        memcpy (dw, &tmp, sizeof(uint32));
    }

    static void SwapFourBytesGrup (uint32* src, int size)
    {
        uint32* ptr = (uint32*)src;
        int i;
        for (i = 0; i < size/4; ++i) {
            SwapFourBytes (&ptr[i]);
        }
    }
#endif
    
    DiQ3BspLevel::DiQ3BspLevel()
    {
    }
    
    DiQ3BspLevel::~DiQ3BspLevel()
    {
        
    }
    
    void DiQ3BspLevel::LoadHeader(DiDataStreamPtr& inStream)
    {
        // Load just the header
        bsp_header_t* pHeader =  DI_NEW bsp_header_t;
        inStream->Read(pHeader, sizeof(bsp_header_t));
        mChunk = DiMemoryDataStreamPtr(DI_NEW DiMemoryDataStream(pHeader, sizeof(bsp_header_t), false));
        // Grab all the counts, header only
        Initialise(true);
		// Delete manually since delete and delete[] (as used by MemoryDataStream)
		// are not compatible
        DI_DELETE pHeader;
    }
    
    void DiQ3BspLevel::LoadLevel(DiDataStreamPtr& inStream)
    {
        mChunk = DiMemoryDataStreamPtr(DI_NEW DiMemoryDataStream(inStream));
        Initialise();
        
        DumpContents();
    }
    
    void DiQ3BspLevel::ExtractLightmaps(void)
    {
        mLightMaps.clear();
        
        // Lightmaps are always 128x128x24 (RGB)
        unsigned char* pLightmap = mLightmaps;
        for (int i = 0; i < mNumLightmaps; ++i)
        {
            // Load, no mipmaps, brighten by factor 2.5
			DiDataStreamPtr stream(DI_NEW DiMemoryDataStream(pLightmap, 128 * 128 * 3, false));
            
            DiString lmName;
            lmName.Format("_q3bsp_lm_%d", i);
            auto lightTexture = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(lmName);
            //lightTexture->Load(stream);
            
            DiImage img(stream);
            img.LoadRawDataToTexture(lightTexture.get(), PF_R8G8B8, 128, 128);
            
            mLightMaps.push_back(lightTexture);
            
            pLightmap += BSP_LIGHTMAP_BANKSIZE;
        }
    }
    
    void DiQ3BspLevel::Initialise(bool headerOnly)
    {
        mHeader = (bsp_header_t*)mChunk->GetPtr();
        
        // Header counts
        InitialiseCounts();
        
        // Data pointers
        if (headerOnly)
            mLumpStart = 0;
        else
        {
            mLumpStart = ((unsigned char*)mHeader) + sizeof(mHeader);
			InitialisePointers();
        }
        
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
		// swap header
        SwapFourBytes ((uint32*)&mHeader->version);
#endif

    }
    
    void DiQ3BspLevel::InitialiseCounts(void)
    {
        mNumEntities = GetLumpSize(BSP_ENTITIES_LUMP);
        mNumElements = GetLumpSize(BSP_ELEMENTS_LUMP) / sizeof(int);
        mNumFaces = GetLumpSize(BSP_FACES_LUMP) / sizeof(bsp_face_t);
        mNumLeafFaces = GetLumpSize(BSP_LFACES_LUMP) / sizeof(int);
        mNumLeaves = GetLumpSize(BSP_LEAVES_LUMP) / sizeof(bsp_leaf_t);
        mNumLightmaps = GetLumpSize(BSP_LIGHTMAPS_LUMP)/BSP_LIGHTMAP_BANKSIZE;
        mNumModels = GetLumpSize(BSP_MODELS_LUMP) / sizeof(bsp_model_t);
        mNumNodes = GetLumpSize(BSP_NODES_LUMP) / sizeof(bsp_node_t);
        mNumPlanes = GetLumpSize(BSP_PLANES_LUMP)/sizeof(bsp_plane_t);
        mNumShaders = GetLumpSize(BSP_SHADERS_LUMP)/sizeof(bsp_shader_t);
        mNumVertices = GetLumpSize(BSP_VERTICES_LUMP)/sizeof(bsp_vertex_t);
        mNumLeafBrushes = GetLumpSize(BSP_LBRUSHES_LUMP)/sizeof(int);
        mNumBrushes = GetLumpSize(BSP_BRUSH_LUMP)/sizeof(bsp_brush_t);
        mNumBrushSides = GetLumpSize(BSP_BRUSHSIDES_LUMP)/sizeof(bsp_brushside_t);
    }
    
    void DiQ3BspLevel::InitialisePointers(void)
    {
        mEntities = (unsigned char*)GetLump(BSP_ENTITIES_LUMP);
        mElements = (int*)GetLump(BSP_ELEMENTS_LUMP);
        mFaces = (bsp_face_t*)GetLump(BSP_FACES_LUMP);
        mLeafFaces = (int*)GetLump(BSP_LFACES_LUMP);
        mLeaves = (bsp_leaf_t*)GetLump(BSP_LEAVES_LUMP);
        mLightmaps = (unsigned char*)GetLump(BSP_LIGHTMAPS_LUMP);
        mModels = (bsp_model_t*)GetLump(BSP_MODELS_LUMP);
        mNodes = (bsp_node_t*)GetLump(BSP_NODES_LUMP);
        mPlanes = (bsp_plane_t*) GetLump(BSP_PLANES_LUMP);
        mShaders = (bsp_shader_t*) GetLump(BSP_SHADERS_LUMP);
        mVis = (bsp_vis_t*)GetLump(BSP_VISIBILITY_LUMP);
        mVertices = (bsp_vertex_t*) GetLump(BSP_VERTICES_LUMP);
        mLeafBrushes = (int*)GetLump(BSP_LBRUSHES_LUMP);
        mBrushes = (bsp_brush_t*) GetLump(BSP_BRUSH_LUMP);
        mBrushSides = (bsp_brushside_t*) GetLump(BSP_BRUSHSIDES_LUMP);
        
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
        SwapFourBytesGrup ((uint32*)mElements, mNumElements*sizeof(int));
        SwapFourBytesGrup ((uint32*)mFaces, mNumFaces*sizeof(bsp_face_t));
        SwapFourBytesGrup ((uint32*)mLeafFaces, mNumLeafFaces*sizeof(int));
        SwapFourBytesGrup ((uint32*)mLeaves, mNumLeaves*sizeof(bsp_leaf_t));
        SwapFourBytesGrup ((uint32*)mModels, mNumModels*sizeof(bsp_model_t));
        SwapFourBytesGrup ((uint32*)mNodes, mNumNodes*sizeof(bsp_node_t));
        SwapFourBytesGrup ((uint32*)mPlanes, mNumPlanes*sizeof(bsp_plane_t));
        
        for (int i=0; i < mNumShaders; ++i)
        {
            SwapFourBytes((uint32*)&mShaders[i].surface_flags);
            SwapFourBytes((uint32*)&mShaders[i].content_flags);
        }
        
        SwapFourBytes((uint32*)&mVis->cluster_count);
        SwapFourBytes((uint32*)&mVis->row_size);
        SwapFourBytesGrup ((uint32*)mVertices, mNumVertices*sizeof(bsp_vertex_t));
        SwapFourBytesGrup ((uint32*)mLeafBrushes, mNumLeafBrushes*sizeof(int));
        SwapFourBytesGrup ((uint32*)mBrushes,  mNumBrushes*sizeof(bsp_brush_t));
        SwapFourBytesGrup ((uint32*)mBrushSides, mNumBrushSides*sizeof(bsp_brushside_t));
#endif
    }
    
    void* DiQ3BspLevel::GetLump(int lumpType)
    {
        if (mLumpStart)
        {
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
            // swap lump offset
            SwapFourBytes ((uint32*)&mHeader->lumps[lumpType].offset);
#endif
            return (unsigned char*)mHeader + mHeader->lumps[lumpType].offset;
        }
        else
            return 0;
    }
    
    int DiQ3BspLevel::GetLumpSize(int lumpType)
    {
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
        // swap lump size
        SwapFourBytes ((uint32*)&mHeader->lumps[lumpType].size);
#endif
        return mHeader->lumps[lumpType].size;
    }
    
    void DiQ3BspLevel::DumpContents(void)
    {
        DI_INFO("----------------------------" );
        DI_INFO("Quake3 BSP level statistics" );
        DI_INFO("Entities     : %d", mNumEntities );
        DI_INFO("Faces        : %d", mNumFaces );
        DI_INFO("Leaf Faces   : %d", mNumLeafFaces );
        DI_INFO("Leaves       : %d", mNumLeaves );
        DI_INFO("Lightmaps    : %d", mNumLightmaps );
        DI_INFO("Elements     : %d", mNumElements );
        DI_INFO("Models       : %d", mNumModels );
        DI_INFO("Nodes        : %d", mNumNodes );
        DI_INFO("Planes       : %d", mNumPlanes );
        DI_INFO("Shaders      : %d", mNumShaders );
        DI_INFO("Vertices     : %d", mNumVertices );
        DI_INFO("Vis Clusters : %d", mVis->cluster_count );
        
        DI_INFO("\nShaders:");
        for (int i = 0; i < mNumShaders; ++i)
        {
            DI_INFO("Shader %d: %s", i, mShaders[i].name );
        }
        
        DI_INFO("\nEntities:");
        char* strEnt = strtok((char*)mEntities, "\0");
        while (strEnt != 0)
        {
            DI_INFO(strEnt);
            strEnt = strtok(0, "\0");
        }
    }

    Q3BspPatch* DiQ3BspLevel::HandlePatch(int faceIndex) const
    {
        Q3BspPatch *q3patch;
        q3patch = new Q3BspPatch;

        int patch_size_x = (mFaces[faceIndex].mesh_cp[0] - 1) / 2;
        int patch_size_y = (mFaces[faceIndex].mesh_cp[1] - 1) / 2;
        int num_bezier_patches = patch_size_y * patch_size_x;

        q3patch->size = num_bezier_patches;
        q3patch->bezier = DI_NEW DiQ3Bezier[q3patch->size];

        int patchIndex = 0;
        int ii, n, j, nn;
        for (ii = 0, n = 0; n < patch_size_x; n++, ii = 2 * n)
        {
            for (j = 0, nn = 0; nn < patch_size_y; nn++, j = 2 * nn)
            {
                int index = 0;
                for (int ctr = 0; ctr < 3; ctr++)
                {
                    int pos = ctr * mFaces[faceIndex].mesh_cp[0];

                    q3patch->bezier[patchIndex].mControls[index++] =
                        DiQ3BspVertex(
                        // position
                        mVertices[mFaces[faceIndex].vert_start +
                        ii +
                        mFaces[faceIndex].mesh_cp[0] * j +
                        pos].point,
                        // uv
                        mVertices[mFaces[faceIndex].vert_start +
                        ii +
                        mFaces[faceIndex].mesh_cp[0] * j +
                        pos].texcoord,
                        // normal
                        mVertices[mFaces[faceIndex].vert_start +
                        ii +
                        mFaces[faceIndex].mesh_cp[0] * j +
                        pos].normal);

                    q3patch->bezier[patchIndex].mControls[index++] =
                        DiQ3BspVertex(
                        mVertices[mFaces[faceIndex].vert_start + ii + mFaces[faceIndex].mesh_cp[0] * j + pos + 1].point,
                        mVertices[mFaces[faceIndex].vert_start + ii + mFaces[faceIndex].mesh_cp[0] * j + pos + 1].texcoord,
                        mVertices[mFaces[faceIndex].vert_start + ii + mFaces[faceIndex].mesh_cp[0] * j + pos + 1].normal);

                    q3patch->bezier[patchIndex].mControls[index++] =
                        DiQ3BspVertex(
                        mVertices[mFaces[faceIndex].vert_start + ii + mFaces[faceIndex].mesh_cp[0] * j + pos + 2].point,
                        mVertices[mFaces[faceIndex].vert_start + ii + mFaces[faceIndex].mesh_cp[0] * j + pos + 2].texcoord,
                        mVertices[mFaces[faceIndex].vert_start + ii + mFaces[faceIndex].mesh_cp[0] * j + pos + 2].normal);
                }
                q3patch->bezier[patchIndex].Tessellate(5);
                patchIndex++;
            }
        }

        return q3patch;
    }

}