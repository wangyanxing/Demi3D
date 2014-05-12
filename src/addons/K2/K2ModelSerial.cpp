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

#include "K2Pch.h"
#include "K2ModelSerial.h"
#include "XMLFile.h"
#include "K2Model.h"
#include "Mesh.h"
#include "Model.h"
#include "SubMesh.h"
#include "AssetManager.h"
#include "K2Clip.h"
#include "Material.h"
#include "ShaderManager.h"
#include "ShaderParam.h"
#include "Texture.h"
#include "K2Configs.h"
#include "Command.h"
#include "ConsoleVariable.h"
#include "K2Asset.h"
#include "VertexFormat.h"

namespace Demi
{
    /// temp structure for model loading
    struct matrix34
    {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
        float m30, m31, m32;

        DiMat4 convert()
        {
            DiMat4 ret(
                m00, m01, m02, 0,
                m10, m11, m12, 0,
                m20, m21, m22, 0,
                m30, m31, m32, 1);
            ret = ret.transpose();
            return ret;
        }
    };

    struct K2Vert
    {
        DiVec3 pos;
        DiVec3 normal;
        DiVec2 uv;
    };

#define MAX_K2_BONEWEIGHTS 8
    struct K2VertSkin
    {
        K2VertSkin()
        {
            for (int i = 0; i < MAX_K2_BONEWEIGHTS; i++)
            {
                weights[i] = 0;
                indices[i] = 0;
            }
        }
        float weights[MAX_K2_BONEWEIGHTS];
        uint8 indices[MAX_K2_BONEWEIGHTS];
    };

    bool g_trans_orit = true;
    bool g_hardSkin = true;
    bool g_hasAnim = false;
    
    DiVector<K2Vert> gCurrentVerts;
    DiVector<K2VertSkin> gCurrentVertSkins;
    DiVector<uint32> gCurrentVertColors;
    DiVector<DiVec3> gCurrentTangents;

    DiQuat g_trans_orit_quat(DiRadian(DiDegree(-90)), DiVec3::UNIT_X);
    
    enum ClipTags
    {
        MKEY_X,
        MKEY_Y,
        MKEY_Z,
        MKEY_PITCH,
        MKEY_ROLL,
        MKEY_YAW,
        MKEY_VISIBILITY,
        MKEY_SCALE_X,
        MKEY_SCALE_Y,
        MKEY_SCALE_Z
    };

    /** A raw clip data structure, using Euler angle
     */
    struct Clip
    {
        Clip(int b = 0)
        :bone(b)
        {
        }
        int bone;
        DiVector<int> vis;
        DiVector<DiVec3> pos;
        DiVector<DiVec3> rot;
        DiVector<DiVec3> scale;
        
        void resize(int frames)
        {
            if (pos.empty())
                pos.resize(frames, DiVec3::ZERO);
            if (vis.empty())
                vis.resize(frames, 255);
            if (rot.empty())
                rot.resize(frames, DiVec3::ZERO);
            if (scale.empty())
                scale.resize(frames, DiVec3::UNIT_SCALE);
        }
    };

    void updateClip(Clip& c, int keytype, int keyID, float val, int vis)
    {
        switch (keytype)
        {
            case MKEY_X:
                c.pos[keyID].x = val; return;
            case MKEY_Y:
                c.pos[keyID].y = val; return;
            case MKEY_Z:
                c.pos[keyID].z = val; return;
            case MKEY_PITCH:
                c.rot[keyID].x = val; return;
            case MKEY_ROLL:
                c.rot[keyID].y = val; return;
            case MKEY_YAW:
                c.rot[keyID].z = val; return;
            case MKEY_VISIBILITY:
                c.vis[keyID] = vis; return;
            case MKEY_SCALE_X:
                c.scale[keyID].x = val; return;
            case MKEY_SCALE_Y:
                c.scale[keyID].x = val; return;
            case MKEY_SCALE_Z:
                c.scale[keyID].x = val; return;
            default:
                break;
        }
    }

    //////////////////////////////////////////////////////////////////////////

    DiK2MdfSerial::DiK2MdfSerial()
        :mIgnoreSubMesh(false)
    {
    }

    DiK2MdfSerial::~DiK2MdfSerial()
    {
    }

    bool DiK2MdfSerial::ParseMdf(const DiString& file, DiK2ModelAsset* target)
    {
        if (file.empty())
            return false;
        
        auto data = DiK2Configs::GetDataStream(file, K2_RES_XML);
        if (!data)
        {
            DI_WARNING("Cannot open k2 mdf: %s", file.c_str());
            return false;
        }
        
        DI_LOG("Loading k2 mdf: %s", file.c_str());
        
        shared_ptr<DiXMLFile> xmlfile(DI_NEW DiXMLFile());
        xmlfile->Load(data->GetAsString());
        data->Close();

        DiXMLElement root = xmlfile->GetRoot();
        return ParseMdf(root, target);
    }

    bool DiK2MdfSerial::ParseMdf(DiXMLElement rootNode, DiK2ModelAsset* target)
    {
        if (rootNode.GetName() != "model")
        {
            DI_WARNING("Invalid K2 MDF script, can't find the model tag");
            return false;
        }
        
        // xxx.model
#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
        target->mModelFile = rootNode.GetAttribute("med");
#else
        target->mModelFile = rootNode.GetAttribute("high");
#endif
        if( target->mModelFile.empty() )
            target->mModelFile = rootNode.GetAttribute("file");
        
        target->mModelFile.ToLower();
        
        // animation clips
        DiXMLElement child = rootNode.GetChild();
        while (child)
        {
            if (child.CheckName("anim"))
                ParseAnim(child, target);
            child = child.GetNext();
        }

        return true;
    }

    void DiK2MdfSerial::ParseAnim(DiXMLElement data, DiK2ModelAsset* target)
    {
        DiString animName = data.GetAttribute("name");
        DiString clipName = data.GetAttribute("clip");

        K2Anim& anim = target->AddAnim();
        anim.name = animName;
        anim.clip = clipName;
        anim.loop = false;
        anim.fps  = 25;
        anim.numframes = 0;
        anim.loopbackframe = 0;

        if (data.HasAttribute("loop"))
            anim.loop = data.GetBool("loop");
        if (data.HasAttribute("fps"))
            anim.fps = data.GetFloat("fps");
        if (data.HasAttribute("numframes"))
            anim.numframes = data.GetInt("numframes");
        if (data.HasAttribute("loopbackframe"))
            anim.loopbackframe = data.GetInt("loopbackframe");
    }
    
    bool DiK2MdfSerial::LoadModel(const DiString& file, DiK2ModelAsset* target)
    {
        auto data = DiK2Configs::GetDataStream(file, K2_RES_BINARY);
        if (!data)
        {
            DI_WARNING("Cannot open k2 model: %s", file.c_str());
            return false;
        }

        DI_LOG("Loading k2 model: %s",file.c_str());

        mStream = data;
        
        DI_SERIAL_LOG("Loading k2 model: %s", file.c_str());
        
        if (!CheckFourcc("SMDL"))
        {
            DI_WARNING("Invalid k2 model file: unknow file format");
            return false;
        }
        
        if (!CheckFourcc("head"))
        {
            DI_WARNING("invalid k2 model file: file does not start with head chunk");
            return false;
        }
        
        // head size
        ReadInt(mStream);
        
        int version = ReadInt(mStream);
        int num_meshes = ReadInt(mStream);
        int num_sprites = ReadInt(mStream);
        int num_surfs = ReadInt(mStream);
        int num_bones = ReadInt(mStream);

        if (version == 1)
        {
            DI_WARNING("It's an old version of k2 model: %s", file.c_str());
        }
        
        DI_SERIAL_LOG("version : %d", version);
        DI_SERIAL_LOG("%d meshes", num_meshes);
        DI_SERIAL_LOG("%d sprites", num_sprites);
        DI_SERIAL_LOG("%d surfaces", num_surfs);
        DI_SERIAL_LOG("%d bones", num_bones);
        
        DiVec3 minBound, maxBound;
        minBound.x = ReadFloat(mStream);
        minBound.y = ReadFloat(mStream);
        minBound.z = ReadFloat(mStream);
        maxBound.x = ReadFloat(mStream);
        maxBound.y = ReadFloat(mStream);
        maxBound.z = ReadFloat(mStream);
        
        DiString meshname = target->GetBaseFolder() + "/";
        meshname += mStream->GetName();
        meshname += file.ExtractFileName();
        DiMeshPtr mesh = DiAssetManager::GetInstancePtr()->CreateOrReplaceAsset<DiMesh>(meshname);

        if (g_trans_orit)
        {
            minBound = g_trans_orit_quat * minBound;
            maxBound = g_trans_orit_quat * maxBound;
        }
        
        DiAABB bounds;
        bounds.Merge(minBound);
        bounds.Merge(maxBound);

        mesh->SetBounds(bounds);

        if (!LoadBones(target->GetBoneData(), num_bones))
            return false;

        if (!CheckFourcc("mesh"))
        {
            DI_WARNING("invalid k2 model file: it should be mesh chunk");
            return false;
        }

        
        g_hasAnim = target->GetAnimNums() > 0;
        // for now, we don't need animation on trees
        if (target->mIsTree)
            g_hasAnim = false;

        while (!mStream->Eof())
        {
            DiSubMesh* sub = LoadMeshes(target,mesh);

            if (sub)
            {
                bool hasTangent = !gCurrentTangents.empty();
                bool hasVertColors = !gCurrentVertColors.empty();

                size_t vertnum = gCurrentVerts.size();
                int geoStride = sizeof(DiVec3)+sizeof(DiVec3)+sizeof(DiVec2);//pos+norm+uv
                int softGeoStride = sizeof(DiVec3)+sizeof(DiVec3);//pos+norm+uv
                int uvStride = sizeof(DiVec2);
                int colorStride = sizeof(uint32);
                int tangStride = sizeof(DiVec3);
                int weightsStride = sizeof(float)* 4;
                int indicesStride = sizeof(uint8)* 4;

                if (g_hasAnim && g_hardSkin)
                {
                    // hardware skinning, max # of bone weights is 4
                    int stride = sub->GetVertexElements().GetStreamElementsSize(0);
                    uint8* buf = (uint8*)sub->CreateSourceData(0, vertnum, stride);
                    
                    for (size_t i = 0; i < vertnum; ++i)
                    {
                        memcpy(buf, &gCurrentVerts[i], geoStride);
                        buf += geoStride;
                        if (hasTangent)
                        {
                            memcpy(buf, &gCurrentTangents[i], tangStride);
                            buf += tangStride;
                        }
                        if (hasVertColors)
                        {
                            memcpy(buf, &gCurrentVertColors[i], colorStride);
                            buf += colorStride;
                        }
                        memcpy(buf, &gCurrentVertSkins[i].weights, weightsStride);
                        buf += weightsStride;
                        memcpy(buf, &gCurrentVertSkins[i].indices, indicesStride);
                        buf += indicesStride;
                    }
                }
                else if (!g_hasAnim)
                {
                    // no animation
                    int stride = sub->GetVertexElements().GetStreamElementsSize(0);
                    uint8* buf = (uint8*)sub->CreateSourceData(0, vertnum, stride);
                    for (size_t i = 0; i < vertnum; ++i)
                    {
                        memcpy(buf, &gCurrentVerts[i], geoStride);
                        buf += geoStride;
                        if (hasTangent)
                        {
                            memcpy(buf, &gCurrentTangents[i], tangStride);
                            buf += tangStride;
                        }
                        if (hasVertColors)
                        {
                            memcpy(buf, &gCurrentVertColors[i], colorStride);
                            buf += colorStride;
                        }
                    }
                }
                else
                {
                    // software skinning
                    int stride = sub->GetVertexElements().GetStreamElementsSize(0);
                    uint8* buf = (uint8*)sub->CreateSourceData(0, vertnum, stride);
                    for (size_t i = 0; i < vertnum; ++i)
                    {
                        memcpy(buf, &gCurrentVerts[i], softGeoStride);
                        buf += softGeoStride;
                        if (hasTangent)
                        {
                            memcpy(buf, &gCurrentTangents[i], tangStride);
                            buf += tangStride;
                        }
                    }
                   
                    stride = sub->GetVertexElements().GetStreamElementsSize(1);
                    buf = (uint8*)sub->CreateSourceData(1, vertnum, stride);
                    for (size_t i = 0; i < vertnum; ++i)
                    {
                        memcpy(buf, &gCurrentVerts[i].uv, uvStride);
                        buf += uvStride;
                        if (hasVertColors)
                        {
                            memcpy(buf, &gCurrentVertColors[i], colorStride);
                            buf += colorStride;
                        }
                    }
                }
            }
        }

        target->SetMesh(mesh);

        mStream->Close();
        mStream = nullptr;

        return true;
    }

    bool DiK2MdfSerial::LoadBones(DiK2BonesDataPtr target, int numBones)
    {
        if (!CheckFourcc("bone"))
        {
            DI_WARNING("It should be a bone chunk");
            return false;
        }

        // chunk size
        ReadInt(mStream);

        for (int i = 0; i < numBones; ++i)
        {
            int parentID = ReadInt(mStream);

            matrix34 inv_matrix;
            matrix34 matrix;
            mStream->Read(&inv_matrix, sizeof(matrix34));
            mStream->Read(&matrix, sizeof(matrix34));

            uint8 length = ReadByte(mStream);
            DiString name = ReadString(mStream, length);
            ReadByte(mStream);  // should be zero, end of the string

            DI_SERIAL_LOG("Bone ID: %d, name : %s", i, name.c_str());

            DiMat4 mat = matrix.convert();
            DiMat4 invMat = inv_matrix.convert();

            if (g_trans_orit)
            {
                DiMat3 m;
                m.FromAngleAxis(DiVec3::UNIT_X, DiRadian(DiDegree(-90)));
                DiMat4 mm(m);
                mat = mm*mat;
                invMat = mat.inverse();
            }

            target->names.push_back(name);
            target->parents.push_back(parentID);
            target->trans.push_back(mat);
            target->invtrans.push_back(invMat);
            target->nameMap[name] = i;
        }
        return true;
    }

    bool DiK2MdfSerial::CheckFourcc(char* sig)
    {
        char hed[4];
        mStream->Read(hed, 4);
        if (sig[0] != hed[0] ||
            sig[1] != hed[1] ||
            sig[2] != hed[2] ||
            sig[3] != hed[3])
        {
            return false;
        }
        return true;
    }
    
    bool DiK2MdfSerial::CheckFourcc(char* hed, char* sig)
    {
        if (sig[0] != hed[0] ||
            sig[1] != hed[1] ||
            sig[2] != hed[2] ||
            sig[3] != hed[3])
        {
            return false;
        }
        return true;
    }

    DiSubMesh* DiK2MdfSerial::LoadMeshes(DiK2ModelAsset* target, DiMeshPtr mesh)
    {
        gCurrentVerts.clear();
        gCurrentVertColors.clear();
        gCurrentTangents.clear();
        gCurrentVertSkins.clear();

        // chunk size
        int chunksize = ReadInt(mStream);

        int meshid = ReadInt(mStream); // mesh index
        int meshmod = ReadInt(mStream); // mesh mod
        int vertNum = ReadInt(mStream);

        gCurrentVerts.resize(vertNum);

        DiVec3 minBound, maxBound;
        minBound.x = ReadFloat(mStream);
        minBound.y = ReadFloat(mStream);
        minBound.z = ReadFloat(mStream);
        maxBound.x = ReadFloat(mStream);
        maxBound.y = ReadFloat(mStream);
        maxBound.z = ReadFloat(mStream);

        if (g_trans_orit)
        {
            minBound = g_trans_orit_quat * minBound;
            maxBound = g_trans_orit_quat * maxBound;
        }
        DiAABB bounds;
        bounds.Merge(minBound);
        bounds.Merge(maxBound);

        int bonelink = ReadInt(mStream); //bone link

        uint8 mesh_name_length = ReadByte(mStream);
        uint8 material_name_length = ReadByte(mStream);

        DiString meshName = ReadString(mStream, mesh_name_length);
        ReadByte(mStream);

        mIgnoreSubMesh = false;
        if (vertNum == 0 || DiString::StartsWith(meshName, "_trisurf_") ||
            DiString::StartsWith(meshName, "_invis_"))
        {
            mIgnoreSubMesh = true;
        }

        // software skinning
        auto numBones = target->GetBoneData()->names.size();
        bool forcesoft = CommandMgr->GetIntVar("force_softskin") == 1;
        g_hardSkin = !forcesoft && numBones <= MAX_BONE_NUM;

        //////////////////////////////////////////////////////////////////////////

        DiSubMesh* submesh = nullptr;
        if (!mIgnoreSubMesh)
        {
            submesh = mesh->CreateSubMesh();
            submesh->SetVerticeNum(vertNum);
        }
        
        // manually setup the skinning weights
        if (bonelink >= 0)
        {
            if(gCurrentVertSkins.empty())
                gCurrentVertSkins.resize(vertNum);
            for (int i = 0; i < vertNum; i++)
            {
                gCurrentVertSkins[i].weights[0] = 1.0f;
                gCurrentVertSkins[i].indices[0] = bonelink;
                if (submesh)
                {
                    submesh->AddWeight(i, bonelink,1.0f);
                }
            }
        }
        
        DiString materialName = ReadString(mStream, material_name_length);
        ReadByte(mStream);

        DI_SERIAL_LOG("Mesh name: %s", meshName.c_str());
        DI_SERIAL_LOG("Material name: %s", materialName.c_str());

        DI_LOG("<K2 Submesh>: %s [%d], mod[%d], bonelnk[%d], mat[%s]", meshName.c_str(), meshid, meshmod, bonelink, materialName.c_str());
        
        if(materialName.empty())
        {
            materialName = mesh->GetName();
            materialName = materialName.ExtractFileName();
            materialName = materialName.ExtractBaseName();
            materialName += "material";
        }

        while (!mStream->Eof())
        {
            char hed[4];
            mStream->Read(hed, sizeof(char) * 4);

            if (CheckFourcc(hed, "vrts"))
                ReadVertices();
            else if (CheckFourcc(hed, "face"))
                ReadFaces(submesh);
            else if (CheckFourcc(hed, "nrml"))
                ReadNormals();
            else if (CheckFourcc(hed, "texc"))
                ReadUVs();
            else if (CheckFourcc(hed, "colr"))
                ReadVertColors();
            else if (CheckFourcc(hed, "lnk1"))
            {
                ReadBoneLinks(submesh);
            }
            else if (CheckFourcc(hed, "lnk3"))
            {
                // hardware skinning
                ReadBoneLinks(submesh);
            }
            else if (CheckFourcc(hed, "sign"))
                ReadSigns();
            else if (CheckFourcc(hed, "tang"))
                ReadTangents();
            else if (CheckFourcc(hed, "mesh"))
                // start another turn
                break;
            else if (CheckFourcc(hed, "surf"))
            {
                // start another turn
                ReadSurfaces();
            }
            else
            {
                DI_WARNING("K2 Model:Unknown data chunk tag: %c%c%c%c", hed[0], hed[1], hed[2], hed[3]);
                break;
            }
        }

        if (submesh)
        {
            DiString materialFile = TryMaterialFile(materialName, target);
            if (materialFile.empty())
            {
                DI_WARNING("No material founded: %s", target->GetBaseFolder().c_str());
                submesh->SetMaterialName("defaultMat.mtl");
            }
            else
            {
                DI_LOG("Loading k2 material:%s", materialFile.c_str());
                auto mat = ParseMaterial(materialFile, target->GetBaseFolder(), g_hasAnim && g_hardSkin);
                submesh->SetMaterialName(mat->GetName());
            }

            //////////////////////////////////////////////////////////////////////////
            uint64 vfelements = 0;
            DiVertexElements vertElements;
            vertElements.AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
            vertElements.AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);

            if (g_hasAnim && g_hardSkin)
            {
                vertElements.AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
                if (!gCurrentTangents.empty())
                    vertElements.AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);
                if (!gCurrentVertColors.empty())
                    vertElements.AddElement(0, VERT_TYPE_COLOR, VERT_USAGE_COLOR);
                vertElements.AddElement(0, VERT_TYPE_FLOAT4, VERT_USAGE_BLENDWEIGHT);
                vertElements.AddElement(0, VERT_TYPE_UBYTE4, VERT_USAGE_BLENDINDICES);
            }
            else if (g_hasAnim && !g_hardSkin)
            {
                if (!gCurrentTangents.empty())
                    vertElements.AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);

                vertElements.AddElement(1, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
                if (!gCurrentVertColors.empty())
                    vertElements.AddElement(1, VERT_TYPE_COLOR, VERT_USAGE_COLOR);
                vfelements = DiVFElement::VF_ELEMENT_POS3F | DiVFElement::VF_ELEMENT_NORMAL3F
                    | DiVFElement::VF_ELEMENT_TANGENT3F | DiVFElement::VF_ELEMENT_UV02F;
            }
            else if (!g_hasAnim)
            {
                vertElements.AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
                if (!gCurrentTangents.empty())
                    vertElements.AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);
                if (!gCurrentVertColors.empty())
                    vertElements.AddElement(0, VERT_TYPE_COLOR, VERT_USAGE_COLOR);
            }
            submesh->GetVertexElements().Clone(vertElements);
            submesh->SetVFElements(vfelements);
        }

        //if (g_hasAnim && submesh)
            //submesh->RationaliseBoneWeights(!g_hardSkin);

        return submesh;
    }

    void DiK2MdfSerial::ReadVertices()
    {
        DI_SERIAL_LOG("------------verts-----------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        int numverts = (chunkSize - 4) / 12;
        int meshindex = ReadInt(mStream);
        DI_SERIAL_LOG("mesh index:%d", meshindex);
        float* vertics = new float[3 * numverts];
        mStream->Read(vertics, sizeof(float)* 3 * numverts);

        DI_ASSERT(gCurrentVerts.size() == numverts);
        for (int i = 0; i < numverts; i++)
        {
            gCurrentVerts[i].pos.x = vertics[i * 3 + 0];
            gCurrentVerts[i].pos.y = vertics[i * 3 + 1];
            gCurrentVerts[i].pos.z = vertics[i * 3 + 2];

            if (g_trans_orit)
                gCurrentVerts[i].pos = g_trans_orit_quat * gCurrentVerts[i].pos;
        }

        delete[] vertics;
    }

    void DiK2MdfSerial::ReadFaces(DiSubMesh* sub)
    {
        DI_SERIAL_LOG("-----------face------------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        int meshindex = ReadInt(mStream);
        int numfaces = ReadInt(mStream);
        DI_SERIAL_LOG("face nums:%d", numfaces);
        sub->SetPrimitiveCount(numfaces);
        sub->SetPrimitiveType(PT_TRIANGLELIST);

        uint8 indexsize = ReadByte(mStream);
        DI_SERIAL_LOG("index size:%d", indexsize);

        void* id = sub->CreateIndexData(3 * numfaces, indexsize == 4);

        if (indexsize == 2)
        {
            mStream->Read(id, sizeof(unsigned short)* 3 * numfaces);
        }
        else if (indexsize == 1)
        {
            uint16* id16 = (uint16*)(id);
            unsigned char* indices = new unsigned char[numfaces * 3];
            mStream->Read(indices, sizeof(unsigned char)* 3 * numfaces);

            for (int i = 0; i < numfaces * 3; i++)
            {
                id16[i] = (uint16)indices[i];
            }

            delete[]indices;
        }
        else if (indexsize == 4)//32bits index
        {
            mStream->Read(id, sizeof(unsigned int)* 3 * numfaces);
        }
        else
        {
            DI_SERIAL_LOG("Unknown error when reading face data");
        }
    }

    void DiK2MdfSerial::ReadNormals()
    {
        DI_SERIAL_LOG("-----------nrml------------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        int numverts = (chunkSize - 4) / 12;
        DI_SERIAL_LOG("normal nums:%d", numverts);

        if (!numverts)
            return;
        int meshindex = ReadInt(mStream);

        float* vertics = new float[3 * numverts];
        mStream->Read(vertics, sizeof(float)* 3 * numverts);

        DI_ASSERT(gCurrentVerts.size() == numverts);
        for (int i = 0; i < numverts; i++)
        {
            gCurrentVerts[i].normal.x = vertics[i * 3 + 0];
            gCurrentVerts[i].normal.y = vertics[i * 3 + 1];
            gCurrentVerts[i].normal.z = vertics[i * 3 + 2];

            if (g_trans_orit)
                gCurrentVerts[i].normal = g_trans_orit_quat * gCurrentVerts[i].normal;
        }

        delete[] vertics;
    }

    void DiK2MdfSerial::ReadUVs()
    {
        DI_SERIAL_LOG("-----------texc------------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        int numverts = (chunkSize - 4) / 8;
        DI_SERIAL_LOG("texc nums:%d", numverts);

        int meshindex = ReadInt(mStream);
        float temp = ReadFloat(mStream);

        float* vertics = new float[2 * numverts];
        mStream->Read(vertics, sizeof(float)* 2 * numverts);

        DI_ASSERT(gCurrentVerts.size() == numverts);
        for (int i = 0; i < numverts; i++)
        {
            gCurrentVerts[i].uv.x = vertics[i * 2 + 0];
            gCurrentVerts[i].uv.y = vertics[i * 2 + 1];
        }
        delete[] vertics;
    }

    void DiK2MdfSerial::ReadVertColors()
    {
        DI_SERIAL_LOG("-----------colr------------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        int numverts = (chunkSize - 8) / 4;
        DI_SERIAL_LOG("colr nums:%d", numverts);

        if (gCurrentVertColors.empty())
            gCurrentVertColors.resize(numverts);

        int meshindex = ReadInt(mStream);
        int meshindex2 = ReadInt(mStream); // anybody can tell me what the hell is it?
        unsigned int* vertics = new unsigned int[numverts];
        mStream->Read(vertics, sizeof(unsigned int)*numverts);
        for (int i = 0; i < numverts; ++i)
        {
            DiColor c;
            c.SetAsRGBA(vertics[i]);
            gCurrentVertColors[i] = vertics[i];
        }
        delete[] vertics;
    }

    void DiK2MdfSerial::ReadBoneLinks(DiSubMesh* sm)
    {
        DI_SERIAL_LOG("-----------lnk------------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        ReadInt(mStream); // mesh index
        int vertNums = ReadInt(mStream);

        if (gCurrentVertSkins.empty())
            gCurrentVertSkins.resize(vertNums);

        int maxWeights = 0;

        for (int i = 0; i < vertNums; i++)
        {
            int numWeights = ReadInt(mStream);

            float* weights = new float[numWeights];
            int* indexes = new int[numWeights];

            mStream->Read(weights, sizeof(float)*numWeights);
            mStream->Read(indexes, sizeof(int)*numWeights);

            int numCount = 0;
            for (int w = 0; w < numWeights; ++w)
            {
                if (numCount > MAX_K2_BONEWEIGHTS)
                    break;

                if (!DiMath::RealEqual(weights[w], 0))
                {
                    gCurrentVertSkins[i].weights[numCount] = weights[w];
                    gCurrentVertSkins[i].indices[numCount++] = indexes[w];

                    if (sm)
                    {
                        sm->AddWeight(i, indexes[w], weights[w]);
                    }
                }
            }
            maxWeights = DiMath::Max(maxWeights, numCount);
            delete[] weights;
            delete[] indexes;
        }
        //DI_ASSERT(maxWeights <= MAX_K2_BONEWEIGHTS);
        sm->SetMaxWeights(maxWeights);
        if(maxWeights > 4)
        {
            DI_INFO("K2 Submesh: %s, The number of bone weights is greater than 4, use software skinning",sm->GetParentMesh()->GetName().c_str());
            g_hardSkin = false;
        }
    }

    void DiK2MdfSerial::ReadSigns()
    {
        // what's this??
        DI_SERIAL_LOG("-----------sign------------");
        int chunkSize = ReadInt(mStream);
        mStream->Skip(chunkSize);
    }

    void DiK2MdfSerial::ReadTangents()
    {
        DI_SERIAL_LOG("-----------tang------------");
        int chunkSize = ReadInt(mStream);

        if (mIgnoreSubMesh && chunkSize)
        {
            mStream->Skip(chunkSize);
            return;
        }

        int numverts = (chunkSize - 4) / 12;
        DI_SERIAL_LOG("tang nums:%d", numverts);
        int meshindex = ReadInt(mStream);
        int what = ReadInt(mStream);

        if (gCurrentTangents.empty())
            gCurrentTangents.resize(numverts);

        float* vertics = new float[3 * numverts];
        mStream->Read(vertics, sizeof(float)* 3 * numverts);

        DI_ASSERT(gCurrentVerts.size() == numverts);
        for (int i = 0; i < numverts; i++)
        {
            gCurrentTangents[i].x = vertics[i * 3 + 0];
            gCurrentTangents[i].y = vertics[i * 3 + 1];
            gCurrentTangents[i].z = vertics[i * 3 + 2];

            if (g_trans_orit)
                gCurrentTangents[i] = g_trans_orit_quat * gCurrentTangents[i];
        }

        delete[] vertics;
    }

    void DiK2MdfSerial::ReadSurfaces()
    {
        DI_SERIAL_LOG("-----------surf------------");

        int chunk_size = ReadInt(mStream);

#if 1
        int surfindex = ReadInt(mStream);
        int num_planes = ReadInt(mStream);
        int num_points = ReadInt(mStream);
        int num_edges = ReadInt(mStream);
        int num_tris = ReadInt(mStream);

        float minx, miny, minz;
        float maxx, maxy, maxz;
        minx = ReadFloat(mStream);
        miny = ReadFloat(mStream);
        minz = ReadFloat(mStream);
        maxx = ReadFloat(mStream);
        maxy = ReadFloat(mStream);
        maxz = ReadFloat(mStream);

        int flag = ReadInt(mStream);

        for (int i = 0; i < num_planes; ++i)
        {
            float p1 = ReadFloat(mStream);
            float p2 = ReadFloat(mStream);
            float p3 = ReadFloat(mStream);
            float p4 = ReadFloat(mStream);
        }

        for (int i = 0; i < num_points; ++i)
        {
            float p1 = ReadFloat(mStream);
            float p2 = ReadFloat(mStream);
            float p3 = ReadFloat(mStream);
        }

        for (int i = 0; i < num_edges; ++i)
        {
            float p1 = ReadFloat(mStream);
            float p2 = ReadFloat(mStream);
            float p3 = ReadFloat(mStream);
            float p4 = ReadFloat(mStream);
            float p5 = ReadFloat(mStream);
            float p6 = ReadFloat(mStream);
        }

        for (int i = 0; i < num_tris; ++i)
        {
            int p1 = ReadInt(mStream);
            int p2 = ReadInt(mStream);
            int p3 = ReadInt(mStream);
        }

#else
        mStream->Skip(chunk_size);
#endif
    }

    DiMaterialPtr DiK2MdfSerial::ParseMaterial(const DiString& matFile,
        const DiString& basePath, bool needSkinning)
    {
        DiDataStreamPtr data = DiK2Configs::GetDataStream(matFile, K2_RES_XML);

        uint64 shaderFlag = 0;
        shared_ptr<DiXMLFile> xmlfile(new DiXMLFile());
        xmlfile->Load(data->GetAsString());

        DiXMLElement root = xmlfile->GetRoot();
        if (root.GetName() != "material")
        {
            DI_WARNING("Invalid k2 material script.");
            return nullptr;
        }

        bool translucent = false;
        bool writeDepth = true;

        DiString diffuseTex;
        DiString normalTex;
        
        // for water
        DiString normal1Tex;
        DiString normal2Tex;

        DiString shaderVs, shaderPs;

        DiVec3 diffuseColor = DiVec3(1,1,1);
        float specularLevel = 1.0f;
        float opacity = 1.0f;
        float shiness = 16.0f;
        DiMap<DiString, float> floatParams;
        DiMap<DiString, DiVec3> vec3Params;
        DiString blendSrc = "BLEND_ONE";
        DiString blendDest = "BLEND_ZERO";

        DiXMLElement child = root.GetChild();
        while (child)
        {
            if (child.CheckName("phase"))
            {
                // we just care about this
                if (child.GetAttribute("name") == "color")
                {

                    if (child.HasAttribute("translucent"))
                        translucent = child.GetBool("translucent");
                    if (child.HasAttribute("alphatest") && child.GetBool("alphatest"))
                        shaderFlag |= SHADER_FLAG_ALPHA_TEST;
                    if (child.HasAttribute("depthwrite"))
                        writeDepth = child.GetBool("depthwrite");
                    if (child.HasAttribute("srcblend"))
                        blendSrc = child.GetAttribute("srcblend");
                    if (child.HasAttribute("dstblend"))
                        blendDest = child.GetAttribute("dstblend");

                    shaderVs = child.GetAttribute("vs");
                    shaderPs = child.GetAttribute("ps");

                    // samplers
                    DiXMLElement samplers = child.GetChild();
                    while (samplers)
                    {
                        if (samplers.CheckName("sampler"))
                        {
                            if (samplers.GetAttribute("name") == "diffuse")
                            {
                                diffuseTex = samplers.GetAttribute("texture").ExtractBaseName();
                                shaderFlag |= SHADER_FLAG_USE_MAP;
                            }
                            else if (samplers.GetAttribute("name") == "normalmap")
                            {
                                // for ordinary material
                                normalTex = samplers.GetAttribute("texture").ExtractBaseName();
                                shaderFlag |= SHADER_FLAG_USE_NORMALMAP;
                            }
                            else if (samplers.GetAttribute("name") == "normalmap1")
                            {
                                // for water
                                normal1Tex = samplers.GetAttribute("texture").ExtractBaseName();
                            }
                            else if (samplers.GetAttribute("name") == "normalmap2")
                            {
                                // for water
                                normal2Tex = samplers.GetAttribute("texture").ExtractBaseName();
                            }
                            // we don't care about their team map for now
                        }

                        samplers = samplers.GetNext();
                    }
                }
            }
            else if (child.CheckName("parameters"))
            {
                if (child.HasAttribute("vDiffuseColor"))
                    diffuseColor = child.GetVector3("vDiffuseColor");
                if (child.HasAttribute("fSpecularLevel"))
                    specularLevel = child.GetFloat("fSpecularLevel");
                if (child.HasAttribute("fGlossiness"))
                    shiness = child.GetFloat("fGlossiness");
                if (child.HasAttribute("fOpacity"))
                    opacity = child.GetFloat("fOpacity");
                
                DiXMLElement parametes = child.GetChild();
                while (parametes)
                {
                    if (parametes.CheckName("float"))
                    {
                        float var = parametes.GetFloat("value");
                        DiString name = parametes.GetAttribute("name");
                        floatParams[name] = var;
                    }
                    else if (parametes.CheckName("vec3"))
                    {
                        DiVec3 var = parametes.GetVector3("value");
                        DiString name = parametes.GetAttribute("name");
                        vec3Params[name] = var;
                    }
                    else
                    {
                        DI_WARNING("Unsupported k2 parameter type");
                    }
                    parametes = parametes.GetNext();
                }
            }
            child = child.GetNext();
        }

        if (needSkinning)
            shaderFlag |= SHADER_FLAG_SKINNED;

        DiString shader = DiK2Configs::GetShader(shaderVs);

        DiMaterialPtr mat = DiMaterial::QuickCreate(shader+"_v", shader+"_p", shaderFlag);
        if (translucent)
            mat->SetBlendMode(BLEND_ALPHA);
        //mat->SetDepthWrite(writeDepth);
        
        if (blendSrc == "BLEND_ONE" && blendDest == "BLEND_ONE")
            mat->SetBlendMode(BLEND_ADD);

        mat->SetDiffuse(DiColor(diffuseColor.x, diffuseColor.y, diffuseColor.z));
        mat->SetSpecular(DiColor::White*specularLevel);
        mat->SetShininess(shiness);
        mat->SetOpacity(opacity);

        DiShaderParameter* sm = mat->GetShaderParameter();

        // load textures
        if (shaderFlag & SHADER_FLAG_USE_MAP)
        {
            DiString colorPath;
            if (!diffuseTex.empty() && diffuseTex[0] == '/')
                colorPath = diffuseTex;
            else
                colorPath = basePath + "/" + diffuseTex;
            
            DiTexturePtr tex = DiK2Configs::GetTexture(colorPath);
            if (tex)
            {
                sm->WriteTexture2D("map", tex);
            }
        }
        if (shaderFlag & SHADER_FLAG_USE_NORMALMAP)
        {
            DiString colorPath;
            if (!normalTex.empty() && normalTex[0] == '/')
                colorPath = normalTex;
            else
                colorPath = basePath + "/" + normalTex;

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
            DiTexturePtr normtex = DiK2Configs::GetTexture(colorPath);
            if (normtex)
                sm->WriteTexture2D("normalMap", normtex);
#else
            DiTexturePtr normtex = DiK2Configs::GetTexture(colorPath + "_rxgb");
            if (normtex)
                sm->WriteTexture2D("normalMap", normtex);
            
            DiTexturePtr spectex = DiK2Configs::GetTexture(colorPath + "_s");
            if (spectex)
                sm->WriteTexture2D("specularMap", spectex);
#endif
        }
        if (shader == DiK2Configs::WATER_SHADER)
        {
            DiString path1, path2;

            if (!normal1Tex.empty() && normal1Tex[0] == '/')
                path1 = normal1Tex;
            else
                path1 = basePath + "/" + normal1Tex;
            if (!normal2Tex.empty() && normal2Tex[0] == '/')
                path2 = normal2Tex;
            else
                path2 = basePath + "/" + normal2Tex;

#if DEMI_PLATFORM == DEMI_PLATFORM_IOS
            DiTexturePtr normtex1 = DiK2Configs::GetTexture(path1);
            if (normtex1)
                sm->WriteTexture2D("normalmap1", normtex1);
            DiTexturePtr normtex2 = DiK2Configs::GetTexture(path2);
            if (normtex2)
                sm->WriteTexture2D("normalmap2", normtex2);
#else
            DiTexturePtr normtex1 = DiK2Configs::GetTexture(path1 + "_rxgb");
            if (normtex1)
                sm->WriteTexture2D("normalmap1", normtex1);
            DiTexturePtr normtex2 = DiK2Configs::GetTexture(path2 + "_rxgb");
            if (normtex2)
                sm->WriteTexture2D("normalmap2", normtex2);
#endif
        }

        for (auto i = floatParams.begin(); i != floatParams.end(); ++i)
        {
            sm->WriteFloat(i->first, i->second);
        }
        for (auto i = vec3Params.begin(); i != vec3Params.end(); ++i)
        {
            sm->WriteFloat3(i->first, i->second);
        }

        return mat;
    }
    
    bool DiK2MdfSerial::LoadClip(const DiString& file, K2Anim* anim, DiK2ModelAsset* target)
    {
        auto data = DiK2Configs::GetDataStream(file, K2_RES_BINARY);
        if (!data)
        {
            DI_WARNING("Cannot open k2 clip: %s", file.c_str());
            return false;
        }
        mStream = data;

        if (!CheckFourcc("CLIP"))
        {
            DI_WARNING("Unknow clip file format");
            return false;
        }
        
        if (!CheckFourcc("head"))
        {
            DI_WARNING("Clip file does not start with head chunk");
            return false;
        }
        
        ReadInt(mStream);//chunk size
        
        int version = ReadInt(mStream);
        int numBones = ReadInt(mStream);
        int numFrames = ReadInt(mStream);

        if (anim->numframes == 0 || anim->numframes > numFrames)
            anim->numframes = numFrames;

        K2KeyFrames* kf = DI_NEW K2KeyFrames();
        target->AddKeyFrame(anim->name, kf);
        
        if (version != 2)
        {
            DI_WARNING("The version(%d) of the clip file: %s may not be supported now", version, file.c_str());
        }
        
        DI_SERIAL_LOG("Frame number: %d", numFrames);
        
        DiStrHash<Clip> rawClips;

        // temp data chunks
        float* dataFloats = DI_NEW float[numFrames];

        while(!mStream->Eof())
        {
            if (!CheckFourcc("bmtn"))
                break;
            
            int chunkSize = ReadInt(mStream);
            
            ReadInt(mStream); //bone index, unused here
            int keytype = ReadInt(mStream);
            int numkeys = ReadInt(mStream);
            
            uint8 nameLen = ReadByte(mStream);
            DiString bonename = ReadString(mStream,nameLen);
            ReadByte(mStream);

            Clip& c = rawClips[bonename];
            c.resize(numFrames);

            if (keytype == MKEY_VISIBILITY)
            {
                // we don't care about the visibilities since they are always 255
                mStream->Skip(numkeys);
            }
            else
            {
                mStream->Read(dataFloats, numkeys * sizeof(float));
                for (int i = 0; i < numkeys; i++)
                    updateClip(c, keytype, i, dataFloats[i], 0);
                for (int i = numkeys; i < numFrames; ++i)
                    updateClip(c, keytype, i, dataFloats[numkeys - 1], 0);
            }

            //skip some space if needed
            int kfsize = keytype == MKEY_VISIBILITY ? 1 : 4;
            int rest = chunkSize - (sizeof(int)* 3 + 1 + nameLen + 1 + numkeys*kfsize);
            if (rest > 0)
                mStream->Skip(rest);
        }

        // convert the raw data to our data structure
        for (auto i = rawClips.begin(); i != rawClips.end(); ++i)
        {
            Clip& c = i->second;
            DiString boneName = i->first;
            bool rootbone = boneName == "Scene Root";
            for (int f = 0; f < numFrames; ++f)
            {
                auto& frames = kf->boneFrames[boneName];
                Trans trans;
                trans.pos = c.pos[f];
                trans.scale = c.scale[f];
                trans.rot = DiK2Configs::ConvertAngles(c.rot[f]);

                if (g_trans_orit && rootbone)
                {
                    // rotate the root bone (usually "Scene Root")
                    
                    DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_X);
                    trans.pos = q * trans.pos;
                    trans.rot = q * trans.rot;
                }

                frames.push_back(trans);
            }
        }
        
        delete[] dataFloats;

        return true;
    }

    DiString DiK2MdfSerial::TryMaterialFile(const DiString& materialName, DiK2ModelAsset* target)
    {
        DiString mateiralFile = target->GetBaseFolder() + "/" + materialName;
        mateiralFile += ".material";

        if (!DiK2Configs::K2ArchiveExists(mateiralFile, false))
        {
            mateiralFile = target->GetBaseFolder() + "/material.material";
            if (!DiK2Configs::K2ArchiveExists(mateiralFile, false))
            {
                mateiralFile.clear();
            }
        }

        return mateiralFile;
    }

}