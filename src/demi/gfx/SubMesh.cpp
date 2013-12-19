
#include "SubMesh.h"
#include "Mesh.h"
#include "VertexDeclaration.h"

#define MAX_BLEND_WEIGHTS 4

namespace Demi 
{
    DiSubMesh::DiSubMesh(DiMesh* parent):
        mMesh(parent),
        mVerticesNum(0),
        mIndexData(NULL),
        mIndicsNum(0),
        m32BitIndex(FALSE),
        mPrimitiveType(PT_TRIANGLELIST),
        mPrimitiveCount(0),
        mMaxWeights(0),
        mIndex(0)
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

    void* DiSubMesh::CreateIndexData( int indexNum,BOOL use32Bit )
    {
        ReleaseIndexData();

        m32BitIndex = use32Bit;
        mIndexData = malloc(indexNum*(use32Bit?32:16));
        mIndicsNum = indexNum;
        return mIndexData;
    }

    uint32 DiSubMesh::GetIndexSize() const
    {
        return m32BitIndex?32:16;
    }

    void DiSubMesh::ReleaseVertexData()
    {
        SourceDataList::iterator it;
        SourceDataList::iterator itEnd = mVertexData.end();
        for (it = mVertexData.begin(); it != itEnd; ++it)
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
        m32BitIndex = FALSE;
        mPrimitiveType = PT_TRIANGLELIST;
    }

    uint16 DiSubMesh::RationaliseBoneWeights()
    {
        typedef std::multimap<float, BoneWeightList::iterator> WeightIteratorMap;

        unsigned short maxBones = 0;
        bool existsNonSkinnedVertices = false;
        BoneWeightList::iterator i;

        for (size_t v = 0; v < mVerticesNum; ++v)
        {
            unsigned short currBones = static_cast<unsigned short>(mBoneWeights.count(v));
            if (currBones <= 0)
            {
                existsNonSkinnedVertices = true;
            }

            if (maxBones < currBones)
            {
                maxBones = currBones;
            }
            
            if (currBones > MAX_BLEND_WEIGHTS)
            {
                std::pair<BoneWeightList::iterator, BoneWeightList::iterator> range;
                
                WeightIteratorMap weightToAssignmentMap;
                range = mBoneWeights.equal_range(v);
                
                for (i = range.first; i != range.second; ++i)
                {
                    weightToAssignmentMap.insert(
                        WeightIteratorMap::value_type(i->second.weight, i));
                }
                unsigned short numToRemove = currBones - MAX_BLEND_WEIGHTS;
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
            {
                totalWeight += i->second.weight;
            }
            
            if (!DiMath::RealEqual(totalWeight, 1.0f))
            {
                for (i = normalise_range.first; i != normalise_range.second; ++i)
                {
                    i->second.weight = i->second.weight / totalWeight;
                }
            }
        }

        if (maxBones > MAX_BLEND_WEIGHTS)
        {
            DI_WARNING("The mesh %s has more than 4 blend weights!", mMesh->GetName().c_str());
            maxBones = MAX_BLEND_WEIGHTS;
        }

        if (existsNonSkinnedVertices)
        {
            DI_WARNING("The mesh %s has non-skinned vertices!", mMesh->GetName().c_str());
        }

        mMaxWeights = maxBones;

        return maxBones;
    }

    void DiSubMesh::SetupBoneWeightsData(uint16 numBlendWeightsPerVertex)
    {
        DI_ASSERT(numBlendWeightsPerVertex > 0 && numBlendWeightsPerVertex <= MAX_BLEND_WEIGHTS);

        numBlendWeightsPerVertex = 4;


         IndexMap boneToBlendIndex;
         BuildIndexMap(boneToBlendIndex, mBlendIndexToBoneIndexMap);

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
            mVertexElems.AddElement(stream, VERT_TYPE_UBYTE4, VERT_USAGE_BLENDINDICES,0);
            mVertexElems.AddElement(stream, DiVertexType(numBlendWeightsPerVertex - 1), VERT_USAGE_BLENDWEIGHT, 0);
        }

        uint16 vertSize = sizeof(UCHAR)*4 + sizeof(float)*numBlendWeightsPerVertex;
        UCHAR* pBase = (UCHAR*)CreateSourceData(stream,mVerticesNum,vertSize);
        UCHAR* base = pBase;
        base;
        memset(pBase,0,mVerticesNum*vertSize);

        BoneWeightList::const_iterator i, iend;
        i = mBoneWeights.begin();
        iend = mBoneWeights.end();
        float* pWeight;
        UCHAR* pIndex;
        for (size_t v = 0; v < mVerticesNum; ++v)
        {
            pIndex = (UCHAR*)pBase;
            pWeight = (float*)(pBase + sizeof(UCHAR)*4);
            for (uint16 bone = 0; bone < numBlendWeightsPerVertex; ++bone)
            {
                if (i != iend && i->second.vertexIndex == v)
                {
                    *pWeight++ = i->second.weight;
                    *pIndex++ = static_cast<UCHAR>(/*boneToBlendIndex[*/i->second.boneIndex/*]*/);
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

    void DiSubMesh::SetupBoneWeights()
    {
        unsigned short maxBones = RationaliseBoneWeights();

        if (maxBones != 0)
        {
            SetupBoneWeightsData(maxBones);
        }
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
        SourceDataList::iterator it = mVertexData.find(streamid);
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

        BoneWeightList::const_iterator itVBA, itendVBA;
        itendVBA = mBoneWeights.end();
        for (itVBA = mBoneWeights.begin(); itVBA != itendVBA; ++itVBA)
        {
            usedBoneIndices.insert(itVBA->second.boneIndex);
        }

        blendIndexToBoneIndexMap.resize(usedBoneIndices.size());
        boneIndexToBlendIndexMap.resize(*usedBoneIndices.rbegin() + 1);

        BoneIndexSet::const_iterator itBoneIndex, itendBoneIndex;
        unsigned short blendIndex = 0;
        itendBoneIndex = usedBoneIndices.end();
        for (itBoneIndex = usedBoneIndices.begin(); itBoneIndex != itendBoneIndex; ++itBoneIndex, ++blendIndex)
        {
            boneIndexToBlendIndexMap[*itBoneIndex] = blendIndex;
            blendIndexToBoneIndexMap[blendIndex] = *itBoneIndex;
        }
    }
}