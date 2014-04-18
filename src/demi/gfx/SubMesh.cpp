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
#include "SubMesh.h"
#include "Mesh.h"
#include "VertexDeclaration.h"

#define MAX_HARDWARE_BLEND_WEIGHTS 4
#define MAX_SOFTWARE_BLEND_WEIGHTS 8

namespace Demi 
{
    DiSubMesh::DiSubMesh(DiMesh* parent):
        mMesh(parent),
        mVerticesNum(0),
        mIndexData(NULL),
        mIndicsNum(0),
        m32BitIndex(false),
        mPrimitiveType(PT_TRIANGLELIST),
        mPrimitiveCount(0),
        mMaxWeights(0),
        mIndex(0),
        mVFElements(0)
    {
    }

    DiSubMesh::~DiSubMesh()
    {
        ReleaseVertexData();
        ReleaseIndexData();
    }

    void DiSubMesh::SetVerticeNum( uint32 vnum )
    {
        mVerticesNum = vnum;
    }

    void DiSubMesh::ComputePrimitiveCount()
    {
        switch(mPrimitiveType)
        {
        case PT_POINTLIST: 
            {
                mPrimitiveCount = mIndicsNum;
                break;
            }
        case PT_LINELIST:
            {
                mPrimitiveCount = mIndicsNum / 2;
                break;
            }
        case PT_LINESTRIP:
            {
                mPrimitiveCount = mIndicsNum >= 2 ? mIndicsNum - 1 : 0;
                break;
            }
        case PT_TRIANGLELIST:
            {
                mPrimitiveCount = mIndicsNum / 3;
                break;
            }
        case PT_TRIANGLESTRIP:
            {
                mPrimitiveCount = mIndicsNum >= 3 ? mIndicsNum - 2 : 0;
                break;
            }
        case PT_TRIANGLEFAN:
            {
                mPrimitiveCount = mIndicsNum >= 3 ? mIndicsNum - 2 : 0;
                break;
            }
        default:
            mPrimitiveCount = 0;
        }
    }

    void DiSubMesh::SetMaterialName( const DiString& material )
    {
        mMaterialName = material;
    }

    void* DiSubMesh::CreateSourceData( uint16 streamid, uint32 num, uint16 stride )
    {
        if (mVertexData.find(streamid) != mVertexData.end())
        {
            return mVertexData[streamid].data;
        }
        void* data = malloc(num*stride);
        mVertexData[streamid] = SourceData(data,num,stride,streamid);
        return data;
    }

    void DiSubMesh::CreateSourceData( uint16 streamid, uint32 num, uint16 stride, void* data )
    {
        if (mVertexData.find(streamid) != mVertexData.end())
        {
            return;
        }
        mVertexData[streamid] = SourceData(data,num,stride,streamid);
    }

    DiSubMesh::SourceData DiSubMesh::GetSourceData( size_t ID )
    {
        if (mVertexData.find(ID) == mVertexData.end())
        {
            return SourceData(0,0,0,0);
        }
        
        return mVertexData[ID];
    }

    DiSubMesh::SourceDataIterator DiSubMesh::GetSourceDataIterator() const
    {
        return SourceDataIterator(mVertexData.begin(),mVertexData.end());
    }

    void* DiSubMesh::CreateIndexData( int indexNum,bool use32Bit )
    {
        ReleaseIndexData();

        m32BitIndex = use32Bit;
        mIndexData = malloc(indexNum*(use32Bit?32:16));
        mIndicsNum = indexNum;
        return mIndexData;
    }

    uint32 DiSubMesh::GetIndexSize() const
    {
        return m32BitIndex ? 4 : 2;
    }

    void DiSubMesh::ReleaseVertexData()
    {
        for (auto it = mVertexData.begin(); it != mVertexData.end(); ++it)
        {
            free(it->second.data);
        }
        mVertexData.clear();

        GetVertexElements().ClearElements();

        mVerticesNum = 0;
    }

    void DiSubMesh::ReleaseIndexData()
    {
        if (mIndexData)
        {
            free(mIndexData);
            mIndexData = NULL;
        }

        mIndicsNum = 0;
        m32BitIndex = false;
        mPrimitiveType = PT_TRIANGLELIST;
    }

    uint16 DiSubMesh::RationaliseBoneWeights()
    {
        auto MAX_WEIGHT = MAX_HARDWARE_BLEND_WEIGHTS;
        if (mMesh->NeedSoftSkinning())
        {
            MAX_WEIGHT = MAX_SOFTWARE_BLEND_WEIGHTS;
        }

        typedef std::multimap<float, BoneWeightList::iterator> WeightIteratorMap;

        unsigned short maxWeights = 0;
        bool existsNonSkinnedVertices = false;
        BoneWeightList::iterator i;

        for (size_t v = 0; v < mVerticesNum; ++v)
        {
            unsigned short currWeights = static_cast<unsigned short>(mBoneWeights.count(v));
            if (currWeights <= 0)
                existsNonSkinnedVertices = true;

            if (maxWeights < currWeights)
                maxWeights = currWeights;
            
            if (currWeights > MAX_WEIGHT)
            {
                std::pair<BoneWeightList::iterator, BoneWeightList::iterator> range;
                
                WeightIteratorMap weightToAssignmentMap;
                range = mBoneWeights.equal_range(v);
                
                for (i = range.first; i != range.second; ++i)
                {
                    weightToAssignmentMap.insert(
                        WeightIteratorMap::value_type(i->second.weight, i));
                }
                unsigned short numToRemove = currWeights - MAX_WEIGHT;
                WeightIteratorMap::iterator remIt = weightToAssignmentMap.begin();

                while (numToRemove--)
                {
                    mBoneWeights.erase(remIt->second);
                    ++remIt;
                }
            }

            std::pair<BoneWeightList::iterator, BoneWeightList::iterator> normalise_range = mBoneWeights.equal_range(v);
            float totalWeight = 0;
            
            for (i = normalise_range.first; i != normalise_range.second; ++i)
                totalWeight += i->second.weight;
            
            if (!DiMath::RealEqual(totalWeight, 1.0f))
            {
                for (i = normalise_range.first; i != normalise_range.second; ++i)
                {
                    i->second.weight = i->second.weight / totalWeight;
                }
            }
        }

        if (maxWeights > MAX_WEIGHT)
        {
            DI_WARNING("The mesh %s has more than %d blend weights!", MAX_WEIGHT, mMesh->GetName().c_str());
            maxWeights = MAX_WEIGHT;
        }

        if (existsNonSkinnedVertices)
        {
            DI_WARNING("The mesh %s has non-skinned vertices!", mMesh->GetName().c_str());
        }

        mMaxWeights = maxWeights;

        return maxWeights;
    }

    void DiSubMesh::SetupBoneWeightsData(uint16 numBlendWeightsPerVertex)
    {
        DI_ASSERT(numBlendWeightsPerVertex > 0);

        //numBlendWeightsPerVertex = 4;

        IndexMap boneToBlendIndex;
        BuildIndexMap(boneToBlendIndex, mBlendIndexToBoneIndexMap);

        if (!mMesh->NeedSoftSkinning())
        {
            uint16 stream = 0;

            bool needAddElements = true;
            if (mVertexElems.Contains(VERT_USAGE_BLENDWEIGHT) ||
                mVertexElems.Contains(VERT_USAGE_BLENDINDICES))
            {
                needAddElements = false;
                stream = mVertexElems.GetUsageAtStream(VERT_USAGE_BLENDINDICES);
            }

            if (needAddElements)
            {
                stream = mVertexElems.GetStreams();
                mVertexElems.AddElement(stream, VERT_TYPE_UBYTE4, VERT_USAGE_BLENDINDICES, 0);
                mVertexElems.AddElement(stream, DiVertexType(numBlendWeightsPerVertex - 1), VERT_USAGE_BLENDWEIGHT, 0);
            }

            uint16 vertSize = sizeof(uint8)* 4 + sizeof(float)*numBlendWeightsPerVertex;
            uint8* pBase = (uint8*)CreateSourceData(stream, mVerticesNum, vertSize);
            memset(pBase, 0, mVerticesNum*vertSize);

            auto i = mBoneWeights.begin();
            auto iend = mBoneWeights.end();
            float* pWeight;
            uint8* pIndex;
            for (size_t v = 0; v < mVerticesNum; ++v)
            {
                pIndex = (uint8*)pBase;
                pWeight = (float*)(pBase + sizeof(uint8)* 4);
                for (uint16 bone = 0; bone < numBlendWeightsPerVertex; ++bone)
                {
                    if (i != iend && i->second.vertexIndex == v)
                    {
                        *pWeight++ = i->second.weight;
                        *pIndex++ = static_cast<uint8>(/*boneToBlendIndex[*/i->second.boneIndex/*]*/);
                        ++i;
                    }
                    else
                    {
                        *pWeight++ = (bone == 0) ? 1.0f : 0.0f;
                        *pIndex++ = 0;
                    }
                }
                pBase += vertSize;
            }
        }
    }

    void DiSubMesh::SetupBoneWeights()
    {
        unsigned short maxBones = RationaliseBoneWeights();

        if (maxBones != 0)
            SetupBoneWeightsData(maxBones);
    }

    void DiSubMesh::AddWeight( uint32 vertexId, uint16 boneId, float weight )
    {
        DiBoneWeight bw;
        bw.vertexIndex = vertexId;
        bw.boneIndex = boneId;
        bw.weight = weight;
        mBoneWeights.insert(DiSubMesh::BoneWeightList::value_type(bw.vertexIndex, bw));
    }

    void DiSubMesh::ReleaseSourceData( uint16 streamid )
    {
        auto it = mVertexData.find(streamid);
        if (it != mVertexData.end())
        {
            free(it->second.data);
            mVertexData.erase(it);
        }
    }

    void DiSubMesh::BuildIndexMap( IndexMap& boneIndexToBlendIndexMap, IndexMap& blendIndexToBoneIndexMap )
    {
        if (mBoneWeights.empty())
        {
            boneIndexToBlendIndexMap.clear();
            blendIndexToBoneIndexMap.clear();
            return;
        }

        typedef DiSet<uint16> BoneIndexSet;
        BoneIndexSet usedBoneIndices;

        auto itendVBA = mBoneWeights.end();
        for (auto itVBA = mBoneWeights.begin(); itVBA != itendVBA; ++itVBA)
        {
            usedBoneIndices.insert(itVBA->second.boneIndex);
        }

        blendIndexToBoneIndexMap.resize(usedBoneIndices.size());
        boneIndexToBlendIndexMap.resize(*usedBoneIndices.rbegin() + 1);

        unsigned short blendIndex = 0;
        auto itendBoneIndex = usedBoneIndices.end();
        for (auto itBoneIndex = usedBoneIndices.begin(); itBoneIndex != itendBoneIndex; ++itBoneIndex, ++blendIndex)
        {
            boneIndexToBlendIndexMap[*itBoneIndex] = blendIndex;
            blendIndexToBoneIndexMap[blendIndex] = *itBoneIndex;
        }
    }

    DiSubMesh::SoftBlendData* DiSubMesh::GenerateBlendData()
    {
        SoftBlendData* data = DI_NEW SoftBlendData();
        int floatNums = 0;
        
        if (mVertexElems.Contains(VERT_USAGE_POSITION))
        {
            data->hasPos = true;
            floatNums+=3;
        }
        auto elementPos = mVertexElems.FindElement(VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
        
        if (mVertexElems.Contains(VERT_USAGE_NORMAL))
        {
            data->hasNormal = true;
            floatNums+=3;
        }
        auto elementNormal = mVertexElems.FindElement(VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
        
        if (mVertexElems.Contains(VERT_USAGE_TANGENT))
        {
            data->hasTangent = true;
            floatNums+=3;
        }
        auto elementTang = mVertexElems.FindElement(VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);
        
        DI_ASSERT(floatNums>0);
        data->data = DI_NEW float[mVerticesNum*floatNums];
        float* buf = data->data;
        
        for (uint32 i = 0; i < mVerticesNum; ++i)
        {
            if (data->hasPos)
            {
                auto& vertData = mVertexData[elementPos.Stream];
                uint8* buffer = (uint8*)vertData.data;
                float* pSrc = (float*)(buffer + i * vertData.stride + elementPos.Offset);
                memcpy(buf,pSrc,sizeof(float)*3);
                buf += 3;
            }
            if (data->hasNormal)
            {
                auto& vertData = mVertexData[elementNormal.Stream];
                uint8* buffer = (uint8*)vertData.data;
                float* pSrc = (float*)(buffer + i * vertData.stride + elementNormal.Offset);
                memcpy(buf,pSrc,sizeof(float)*3);
                buf += 3;
            }
            if (data->hasTangent)
            {
                auto& vertData = mVertexData[elementTang.Stream];
                uint8* buffer = (uint8*)vertData.data;
                float* pSrc = (float*)(buffer + i * vertData.stride + elementTang.Offset);
                memcpy(buf,pSrc,sizeof(float)*3);
                buf += 3;
            }
        }
        return data;
    }

}