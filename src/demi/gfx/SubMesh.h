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

#ifndef DiSubMesh_h__
#define DiSubMesh_h__

#include "VertexDeclaration.h"
#include "FastArray.h"

namespace Demi 
{
    struct DiBoneWeight
    {
        uint32   vertexIndex;
        uint16   boneIndex;
        float    weight;
    };

    class DI_GFX_API DiSubMesh : public DiBase
    {
    public:

        DiSubMesh(DiMesh* parent);

        virtual ~DiSubMesh();

        friend class DiMesh;
        friend class DiSubModel;
        friend class DiMeshSerializerImpl;

    public:

        struct SourceData
        {
            SourceData()
            {
                numVertices = 0;
                data    = nullptr;
                stride  = 0;
                stream  = 0;
            }

            SourceData(void* d,uint32 n, uint16 s,uint16 str)
            {
                data        = d;
                numVertices = n;
                stride      = s;
                stream      = str;
            }

            uint32 GetSize()
            {
                return numVertices * stride;
            }

            void*    data;
            uint32   numVertices;
            uint16   stride;
            uint16   stream;
        };

        struct SoftBlendData
        {
            SoftBlendData()
            {
                hasPos = hasNormal = hasTangent = false;
            }
            ~SoftBlendData()
            {
                SAFE_ARRAY_DELETE(data);
            }

            bool hasPos;
            bool hasNormal;
            bool hasTangent;
            float* data;
        };

        typedef DiMap<uint32,SourceData>            SourceDataList;
        typedef DiConstMapIterator<SourceDataList>  SourceDataIterator;
        typedef std::multimap<uint32, DiBoneWeight> BoneWeightList;
        typedef DiVector<uint16>                    IndexMap;

        void                    SetPrimitiveType(DiPrimitiveType pt){ mPrimitiveType = pt; }

        DiPrimitiveType         GetPrimitiveType()const{ return mPrimitiveType; }

        uint32                  GetPrimitiveCount()const {return mPrimitiveCount;}

        void                    SetVerticeNum(uint32 vnum);

        uint32                  GetVerticeNum() const{return mVerticesNum;}

        void                    SetPrimitiveCount(uint32 count){mPrimitiveCount = count;}

        void                    ComputePrimitiveCount();

        void                    SetMaterialName(const DiString& material);

        const DiString&         GetMaterialName()const{return mMaterialName;}

        void*                   CreateSourceData(uint16 streamid, uint32 num, uint16 stride );

        void                    CreateSourceData(uint16 streamid, uint32 num, uint16 stride, void* data );

        void                    ReleaseSourceData(uint16 streamid);

        SourceData              GetSourceData(size_t ID);

        void*                   CreateIndexData(int indexNum,bool use32Bit);

        uint32                  GetIndexSize() const;

        bool                    GetUse32BitIndex() const { return m32BitIndex; }

        uint32                  GetIndexNum()const{return mIndicsNum;}

        void*                   GetIndexData() const{return mIndexData;}

        SourceDataIterator      GetSourceDataIterator() const;

        void                    ReleaseVertexData();

        void                    ReleaseIndexData();

        DiMesh*                 GetParentMesh() const { return mMesh; }

        const BoneWeightList&   GetBoneWeights() { return mBoneWeights; }

        void                    SetupBoneWeights(bool softskin);

        void                    AddWeight(uint32 vertexId, uint16 boneId, float weight);

        const DiVertexElements& GetVertexElements() const { return mVertexElems; }

        DiVertexElements&       GetVertexElements() { return mVertexElems; }

        uint32                  GetMaxWeights() const { return mMaxWeights; }

        uint16                  RationaliseBoneWeights(bool softskin);

        void                    BuildIndexMap(IndexMap& boneIndexToBlendIndexMap, IndexMap& blendIndexToBoneIndexMap);

        IndexMap&               GetBlendIndexToBoneIndexMap() { return mBlendIndexToBoneIndexMap; }

        const IndexMap&         GetBlendIndexToBoneIndexMap() const{ return mBlendIndexToBoneIndexMap; }

        uint32                  GetIndex() const { return mIndex; }

        SoftBlendData*          GenerateBlendData();

        uint64                  GetVFElements() const { return mVFElements; }

        void                    SetVFElements(uint64 val) { mVFElements = val; }

    protected:

        void                    SetupBoneWeightsData(uint16 numBlendWeightsPerVertex, bool softskin);

    protected:

        DiMesh*                 mMesh;

        // the index in this submesh's parent mesh
        uint32                  mIndex;

        SourceDataList          mVertexData;

        uint32                  mVerticesNum;

        void*                   mIndexData;

        uint32                  mIndicsNum;

        bool                    m32BitIndex;

        DiPrimitiveType         mPrimitiveType;

        uint32                  mPrimitiveCount;

        DiString                mMaterialName;

        BoneWeightList          mBoneWeights;

        DiVertexElements        mVertexElems;
        
        uint64                  mVFElements;
        
        IndexMap                mBlendIndexToBoneIndexMap;

        uint32                  mMaxWeights;
    };
}

#endif
