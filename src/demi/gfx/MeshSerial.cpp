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
#include "MeshSerial.h"
#include "MeshFormat.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexDeclaration.h"
#include "AssetManager.h"

namespace Demi 
{

    DiMeshSerializer::DiMeshSerializer()
    {

    }

    DiMeshSerializer::~DiMeshSerializer()
    {

    }

    void DiMeshSerializer::ExportMesh( const DiMeshPtr pMesh, DiDataStreamPtr stream )
    {
        static char head[4] = { 'D', 'e', 'm', 'i' };
        stream->Write(head, 4);

        uint16 version = MODEL_SERIAL_VERSION_NUM - 1;
        stream->Write(&version,sizeof(uint16));

        DiMeshSerializerImpl* impl = GetImplemention((DiMeshSerialVersion)version);
        if (impl)
        {
            impl->ExportMesh(pMesh.get(),stream);
        }

        DI_DELETE impl;
    }

    void DiMeshSerializer::ExportMesh( const DiMeshPtr pMesh, const DiString& filename )
    {
        FILE* fp = fopen(filename.c_str(),"wb");
        DiDataStreamPtr ds(DI_NEW DiFileHandleDataStream(fp,DiDataStream::WRITE));
        ExportMesh(pMesh,ds);
    }

    void DiMeshSerializer::ExportMesh( const DiString& meshName, const DiString& filename )
    {
        DiMeshPtr md = DiAssetManager::GetInstance().GetAsset<DiMesh>(meshName);
        ExportMesh(md,filename);
    }

    bool DiMeshSerializer::CheckHead(char* h0)
    {
        static char head[4] = { 'D', 'e', 'm', 'i' };

        if (h0[0] == head[0] &&
            h0[1] == head[1] &&
            h0[2] == head[2] &&
            h0[3] == head[3])
        {
            return true;
        }
        
        DI_WARNING("Cannot load the mesh file, bad file header");
        return false;
    }

    void DiMeshSerializer::ImportMesh( DiDataStreamPtr& stream, DiMesh* pDest )
    {
        DI_SERIAL_LOG("Loading mesh : %s", stream->GetName().c_str());
        char sign[4];
        stream->Read(sign,4);

        if (CheckHead(sign))
        {
            uint16 version;
            stream->Read(&version,sizeof(uint16));

            DI_SERIAL_LOG("Mesh version: %d", version);

            if (version >= MODEL_SERIAL_VERSION_NUM)
            {
                DI_ERROR("Unknow mesh version.");
                return;
            }

            DiMeshSerializerImpl* impl = GetImplemention((DiMeshSerialVersion)version);
            if (impl)
            {
                impl->ImportMesh(stream,pDest);
            }

            DI_DELETE impl;
            return;
        }

        DI_ERROR("Invalid mesh file head.");
        return;
    }

    DiMeshSerializerImpl* DiMeshSerializer::GetImplemention( DiMeshSerialVersion ver )
    {
        if (ver == MODEL_SERIAL_VERSION_0)
            return DI_NEW DiMeshSerializerImpl();
        else 
            return NULL;
    }

    //////////////////////////////////////////////////////////////////////////

    DiMeshSerializerImpl::DiMeshSerializerImpl()
    {

    }

    DiMeshSerializerImpl::~DiMeshSerializerImpl()
    {

    }

    void DiMeshSerializerImpl::ExportMesh( const DiMesh* pMesh, DiDataStreamPtr stream )
    {
        mStream = stream;

        WriteFileHeader();

        DiMesh::ConstSubMeshIterator it = pMesh->GetSubMeshs();
        while (it.HasMoreElements())
        {
            DiSubMesh* sm = it.GetNext();
            WriteSubMesh(sm);
        }

        WriteBoundsInfo(pMesh);
    }

    void DiMeshSerializerImpl::ImportMesh( DiDataStreamPtr& stream, DiMesh* pMesh )
    {
        ReadFileHeader(stream);

        if (!stream->Eof())
        {
            uint16 streamID = ReadChunk(stream);
            while(!stream->Eof() &&
                (streamID == DI_SUBMESH || streamID == DI_MESH_BOUNDS))
            {
                switch(streamID)
                {
                case DI_SUBMESH:
                    ReadSubMesh(stream, pMesh);
                    break;

                case DI_MESH_BOUNDS:
                    ReadBoundsInfo(stream, pMesh);
                    break;
                }

                if (!stream->Eof())
                    streamID = ReadChunk(stream);

            }
            if (!stream->Eof())
                stream->Skip(-MSTREAM_OVERHEAD_SIZE);
        }
    }

    void DiMeshSerializerImpl::ReadFileHeader( DiDataStreamPtr& /*stream*/ )
    {
    }

    void DiMeshSerializerImpl::ReadSubMesh( DiDataStreamPtr& stream, DiMesh* pMesh )
    {
        DI_SERIAL_LOG("Reading submesh..");

        DiSubMesh* sm = pMesh->CreateSubMesh();

        unsigned short streamID = 0;

        DiString material = ReadString(stream);
        sm->SetMaterialName(material);

        DI_SERIAL_LOG("Liking material: %s", material.c_str());

        unsigned int indexCount = 0;
        ReadInts(stream, &indexCount, 1);

        DI_SERIAL_LOG("Indeices count: %d", indexCount);

        bool idx32bit;
        ReadBools(stream, &idx32bit, 1);

        DI_SERIAL_LOG("Index size: %d", idx32bit?32:16);

        uint16 primitive;
        ReadShorts(stream,&primitive,1);
        sm->SetPrimitiveType((DiPrimitiveType)primitive);

        DI_SERIAL_LOG("Primitive type: %d", primitive);

        if (indexCount > 0)
        {
            void* indexdata = sm->CreateIndexData(indexCount,idx32bit?true:false);
            int size = indexCount * sm->GetIndexSize();
            stream->Read(indexdata, size);
            DI_SERIAL_LOG("%d bytes readed", size);
        }

        streamID = ReadChunk(stream);
        if (streamID != DI_GEOMETRY)
        {
            DI_ERROR("Bad stream ID");
            return;
        }

        ReadGeometry(stream, sm);

        if (!stream->Eof())
        {
            streamID = ReadChunk(stream);
            if (streamID == DI_MESH_WEIGHTS)
            {
                ReadSubMeshBoneWeights(stream,sm);
            }
            else
            {
                if (!stream->Eof())
                    stream->Skip(-MSTREAM_OVERHEAD_SIZE);
            }
        }
    }

    void DiMeshSerializerImpl::ReadGeometry( DiDataStreamPtr& stream, DiSubMesh* pMesh )
    {
        DI_SERIAL_LOG("Reading geometry data...");

        unsigned int vertexCount = 0;
        ReadInts(stream, &vertexCount, 1);
        pMesh->SetVerticeNum(vertexCount);

        DI_SERIAL_LOG("Vertices count: %d", vertexCount);

        unsigned int faceCount = 0;
        ReadInts(stream, &faceCount, 1);
        pMesh->SetPrimitiveCount(faceCount);

        DI_SERIAL_LOG("Face count: %d", faceCount);

        if (!stream->Eof())
        {
            unsigned short streamID = ReadChunk(stream);
            while(!stream->Eof() &&
                (streamID == DI_GEOMETRY_VERTEX_BUFFER ||
                streamID == DI_GEOMETRY_VERTEX_DECLARATION))
            {
                switch (streamID)
                {
                case DI_GEOMETRY_VERTEX_DECLARATION:
                    ReadGeometryVertexDeclaration(stream, pMesh);
                    break;
                case DI_GEOMETRY_VERTEX_BUFFER:
                    ReadGeometryVertexBuffer(stream, pMesh);
                    break;
                }

                if (!stream->Eof())
                {
                    streamID = ReadChunk(stream);
                }
            }
            if (!stream->Eof())
            {
                stream->Skip(-MSTREAM_OVERHEAD_SIZE);
            }
        }
    }

    void DiMeshSerializerImpl::ReadGeometryVertexDeclaration( 
        DiDataStreamPtr& stream, DiSubMesh* pMesh)
    {
        DI_SERIAL_LOG("Reading vertex declaration info...");

        uint32  elementsNum = (mCurChunkSize - MSTREAM_OVERHEAD_SIZE)/(sizeof(uint16)*4);

        DI_SERIAL_LOG("Number of elements: %d", elementsNum);

        if (elementsNum > 0 && !stream->Eof())
        {
            pMesh->GetVertexElements().ClearElements();

            for (uint32 i=0; i<elementsNum; i++)
            {
                unsigned short source, tmp;
                DiVertexType vType;
                DiVertexUsage vSemantic;
                ReadShorts(stream, &source, 1);
                ReadShorts(stream, &tmp, 1);
                vType = static_cast<DiVertexType>(tmp);
                ReadShorts(stream, &tmp, 1);
                vSemantic = static_cast<DiVertexUsage>(tmp);
                ReadShorts(stream, &tmp, 1);
                uint8 usageID = static_cast<uint8>(tmp);
                pMesh->GetVertexElements().AddElement(source,vType,vSemantic,usageID);
            }
        }
    }

    void DiMeshSerializerImpl::ReadGeometryVertexBuffer( 
        DiDataStreamPtr& stream, DiSubMesh* pMesh )
    {
        DI_SERIAL_LOG("Reading vertex buffer...");

        uint16 bindIndex, vertexSize;
        
        ReadShorts(stream, &bindIndex, 1);
        ReadShorts(stream, &vertexSize, 1);

        uint32 size = vertexSize * pMesh->GetVerticeNum();
        void* buf = pMesh->CreateSourceData(bindIndex,pMesh->GetVerticeNum(),vertexSize);

        stream->Read(buf, size);
        DI_SERIAL_LOG("%d bytes readed", size);
    }

    void DiMeshSerializerImpl::ReadBoundsInfo( DiDataStreamPtr& stream, DiMesh* pMesh )
    {
        DiVec3 min, max;
        ReadFloats(stream, &min.x, 1);
        ReadFloats(stream, &min.y, 1);
        ReadFloats(stream, &min.z, 1);
        ReadFloats(stream, &max.x, 1);
        ReadFloats(stream, &max.y, 1);
        ReadFloats(stream, &max.z, 1);
        DiAABB box(min, max);
        pMesh->SetBounds(box);

        DI_SERIAL_LOG("AABB: min(%.3f,%.3f,%.3f), max(%.3f,%.3f,%.3f)", min.x, min.y, min.z,max.x,max.y,max.z);
    }

    void DiMeshSerializerImpl::WriteFileHeader( void )
    {
    }

    void DiMeshSerializerImpl::WriteSubMesh( DiSubMesh* pMesh )
    {
        WriteChunkHeader(DI_SUBMESH, CalcSubMeshSize(pMesh));

        WriteString(pMesh->GetMaterialName());

        unsigned int indexCount = pMesh->GetIndexNum();
        WriteInts(&indexCount,1);

        bool u32 = pMesh->GetIndexSize() == 4;
        WriteBools(&u32,1);

        uint16 primitive = (uint16)pMesh->GetPrimitiveType();
        WriteShorts(&primitive,1);

        WriteData(pMesh->GetIndexData(),
            pMesh->GetIndexSize() * pMesh->GetIndexNum(), 1);

        WriteGeometry(pMesh);

        if (!pMesh->mBoneWeights.empty())
        {
            WriteSubMeshBoneWeights(pMesh);
        }
    }

    void DiMeshSerializerImpl::WriteBoundsInfo( const DiMesh* pMesh )
    {
        unsigned long size = MSTREAM_OVERHEAD_SIZE;

        size += sizeof(float) * 6;
        WriteChunkHeader(DI_MESH_BOUNDS, size);

        const DiVec3& min = pMesh->GetBounds().GetMinimum();
        const DiVec3& max = pMesh->GetBounds().GetMaximum();

        WriteFloats(&min.x, 1);
        WriteFloats(&min.y, 1);
        WriteFloats(&min.z, 1);

        WriteFloats(&max.x, 1);
        WriteFloats(&max.y, 1);
        WriteFloats(&max.z, 1);
    }

    void DiMeshSerializerImpl::WriteGeometry( DiSubMesh* pMesh )
    {
        WriteChunkHeader(DI_GEOMETRY, CalcGeometrySize(pMesh));

        uint32 vnum = pMesh->GetVerticeNum();
        WriteInts(&vnum,1);

        uint32 faces = pMesh->GetPrimitiveCount();
        WriteInts(&faces,1);

        WriteGeometryVertexDeclaration(pMesh);

        WriteGeometryVertexBuffer(pMesh);
    }

    void DiMeshSerializerImpl::WriteGeometryVertexDeclaration( DiSubMesh* pMesh )
    {
        DiVertexElements& ele = pMesh->GetVertexElements();

        uint32 elementNums = ele.GetNumVertexElements();

        WriteChunkHeader(DI_GEOMETRY_VERTEX_DECLARATION, MSTREAM_OVERHEAD_SIZE +
            4 * sizeof(uint16) * elementNums);

        for (uint32 i=0; i<elementNums; i++)
        {
            DiVertexElements::Element ve = ele.GetElement(i);
            uint16 temp = ve.Stream;
            WriteShorts(&temp,1);
            temp = ve.Type;
            WriteShorts(&temp,1);
            temp = ve.Usage;
            WriteShorts(&temp,1);
            temp = ve.UsageIndex;
            WriteShorts(&temp,1);
        }
    }

    void DiMeshSerializerImpl::WriteGeometryVertexBuffer( DiSubMesh* pMesh )
    {
        DiSubMesh::SourceDataIterator it = pMesh->GetSourceDataIterator();
        while (it.HasMoreElements())
        {
            DiSubMesh::SourceData sd = it.GetNext();

            size_t size = MSTREAM_OVERHEAD_SIZE;
            size += sizeof(uint16);
            size += sizeof(uint16);
            size += sd.GetSize();    

            WriteChunkHeader(DI_GEOMETRY_VERTEX_BUFFER,size);
            WriteShorts(&sd.stream,1);
            WriteShorts(&sd.stride,1);
            WriteData(sd.data,sd.GetSize(),1);
        }
    }

    size_t DiMeshSerializerImpl::CalcSubMeshSize( const DiSubMesh* pSub )
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;
        
        size += pSub->GetMaterialName().length() + 1;
        size += sizeof(bool);
        size += sizeof(unsigned int);
        size += pSub->GetIndexSize() * pSub->GetIndexNum();
        size += sizeof(uint16);
        size += CalcGeometrySize(pSub);

        return size;
    }

    size_t DiMeshSerializerImpl::CalcGeometrySize( const DiSubMesh* pSub )
    {
        size_t size = MSTREAM_OVERHEAD_SIZE;
        
        size += sizeof(uint32);
        size += (MSTREAM_OVERHEAD_SIZE + 3 * sizeof(uint16)) * pSub->GetVertexElements().GetNumVertexElements();
        size += CalcStreamDataSize(pSub);

        return size;
    }

    size_t DiMeshSerializerImpl::CalcStreamDataSize( const DiSubMesh* pSub )
    {
        size_t size = 0;
        DiSubMesh::SourceDataIterator it = pSub->GetSourceDataIterator();
        while (it.HasMoreElements())
        {
            DiSubMesh::SourceData sd = it.GetNext();

            size += MSTREAM_OVERHEAD_SIZE;
            size += sizeof(uint16);    
            size += sizeof(uint16);    
            size += sd.GetSize();    
        }

        return size;
    }

    void DiMeshSerializerImpl::ReadSubMeshBoneWeights( DiDataStreamPtr& stream, DiSubMesh* pMesh )
    {
        DI_SERIAL_LOG("Reading bone weights...");

        uint32 size = 0;
        ReadInts(stream,&size,1);
        DI_SERIAL_LOG("Size: %d",size);

        for (uint16 i=0; i<size; i++)
        {
            DiBoneWeight weight;
            ReadInts(stream,&weight.vertexIndex,1);
            ReadShorts(stream,&weight.boneIndex,1);
            ReadFloats(stream,&weight.weight,1);
            pMesh->mBoneWeights.insert(DiSubMesh::BoneWeightList::value_type(weight.vertexIndex, weight));
        }

        pMesh->SetupBoneWeights();
    }

    void DiMeshSerializerImpl::WriteSubMeshBoneWeights( DiSubMesh* pMesh )
    {
        const DiSubMesh::BoneWeightList& weights = pMesh->GetBoneWeights();

        uint32 weightsSize = weights.size();
        size_t chunkSize = MSTREAM_OVERHEAD_SIZE + sizeof(uint32) + weightsSize * sizeof(DiBoneWeight);
        
        WriteChunkHeader(DI_MESH_WEIGHTS, chunkSize);

        WriteInts(&weightsSize,1);

        DiSubMesh::BoneWeightList::const_iterator it;
        DiSubMesh::BoneWeightList::const_iterator itEnd = weights.end();

        for (it = weights.begin(); it != itEnd; ++it)
        {
            WriteInts(&it->second.vertexIndex,1);
            WriteShorts(&it->second.boneIndex,1);
            WriteFloats(&it->second.weight,1);
        }
    }
}