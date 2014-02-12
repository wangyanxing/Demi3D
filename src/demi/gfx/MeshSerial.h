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

#ifndef DiMeshSerial_h__
#define DiMeshSerial_h__


#include "Mesh.h"
#include "Serializer.h"

namespace Demi 
{
    class DiMeshSerializerImpl;

    enum DiMeshSerialVersion
    {
        MODEL_SERIAL_VERSION_0 = 0,

        MODEL_SERIAL_VERSION_NUM
    };

    class DI_GFX_API DiMeshSerializer : public DiBase
    {
    public:
        
        DiMeshSerializer();

        virtual                 ~DiMeshSerializer();

    public:

        void                     ExportMesh(const DiMeshPtr pMesh, DiDataStreamPtr stream);

        void                     ExportMesh(const DiMeshPtr pMesh, const DiString& filename);

        void                     ExportMesh(const DiString& meshName, const DiString& filename);

        void                     ImportMesh(DiDataStreamPtr& stream, DiMesh* pDest);

    protected:

        DiMeshSerializerImpl*    GetImplemention(DiMeshSerialVersion ver);

        bool                     CheckHead(char* h0);
    };

    class DiMeshSerializerImpl : public DiSerializer
    {
    public:

        DiMeshSerializerImpl();

        virtual                 ~DiMeshSerializerImpl();

    public:

        void                    ExportMesh(const DiMesh* pMesh, DiDataStreamPtr stream);

        void                    ImportMesh(DiDataStreamPtr& stream, DiMesh* pDest);

    protected:

        virtual void            ReadFileHeader(DiDataStreamPtr& stream);

        virtual void            WriteFileHeader(void);

    protected:

        virtual void            ReadSubMesh(DiDataStreamPtr& stream, DiMesh* pMesh);

        virtual void            ReadBoundsInfo(DiDataStreamPtr& stream, DiMesh* pMesh);

        virtual void            ReadGeometry(DiDataStreamPtr& stream, DiSubMesh* pMesh);

        virtual void            ReadGeometryVertexDeclaration(DiDataStreamPtr& stream, DiSubMesh* pMesh);

        virtual void            ReadGeometryVertexBuffer(DiDataStreamPtr& stream, DiSubMesh* pMesh);

        virtual void            ReadSubMeshBoneWeights(DiDataStreamPtr& stream, DiSubMesh* pMesh);

    protected:

        virtual void            WriteSubMesh(DiSubMesh* pMesh);

        virtual void            WriteBoundsInfo(const DiMesh* pMesh);

        virtual void            WriteGeometry(DiSubMesh* pMesh);

        virtual void            WriteGeometryVertexDeclaration(DiSubMesh* pMesh);

        virtual void            WriteGeometryVertexBuffer(DiSubMesh* pMesh);

        virtual void            WriteSubMeshBoneWeights(DiSubMesh* pMesh);

    protected:

        virtual size_t          CalcSubMeshSize(const DiSubMesh* pSub);

        virtual size_t          CalcGeometrySize(const DiSubMesh* pSub);

        virtual size_t          CalcStreamDataSize(const DiSubMesh* pSub);
    };
}

#endif
