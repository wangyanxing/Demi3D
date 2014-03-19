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
#include "Command.h"
#include "ConsoleVariable.h"
#include "K2Asset.h"

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
        DiVec2 uv;
        DiVec3 normal;
        DiVec3 tangent;
    };

    bool g_trans_orit = false;
    DiVector<K2Vert> gCurrentVerts;
    
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


    typedef struct RotOrderInfo {
        short axis[3];
        short parity; /* parity of axis permutation (even=0, odd=1) - 'n' in original code */
    } RotOrderInfo;

    typedef enum eEulerRotationOrders {
        EULER_ORDER_DEFAULT = 1, /* blender classic = XYZ */
        EULER_ORDER_XYZ = 1,
        EULER_ORDER_XZY,
        EULER_ORDER_YXZ,
        EULER_ORDER_YZX,
        EULER_ORDER_ZXY,
        EULER_ORDER_ZYX
        /* there are 6 more entries with dulpicate entries included */
    } eEulerRotationOrders;

    static const RotOrderInfo rotOrders[] = {
        /* i, j, k, n */
        { { 0, 1, 2 }, 0 }, /* XYZ */
        { { 0, 2, 1 }, 1 }, /* XZY */
        { { 1, 0, 2 }, 1 }, /* YXZ */
        { { 1, 2, 0 }, 0 }, /* YZX */
        { { 2, 0, 1 }, 0 }, /* ZXY */
        { { 2, 1, 0 }, 1 }  /* ZYX */
    };
#define GET_ROTATIONORDER_INFO(order) (assert(order >= 0 && order <= 6), (order < 1) ? &rotOrders[0] : &rotOrders[(order) - 1])

    void eulO_to_quat(float q[4], const float e[3], const short order)
    {
        const RotOrderInfo *R = GET_ROTATIONORDER_INFO(order);
        short i = R->axis[0], j = R->axis[1], k = R->axis[2];
        double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;
        double a[3];

        ti = e[i] * 0.5f;
        tj = e[j] * (R->parity ? -0.5f : 0.5f);
        th = e[k] * 0.5f;

        ci = cos(ti);
        cj = cos(tj);
        ch = cos(th);
        si = sin(ti);
        sj = sin(tj);
        sh = sin(th);

        cc = ci * ch;
        cs = ci * sh;
        sc = si * ch;
        ss = si * sh;

        a[i] = cj * sc - sj * cs;
        a[j] = cj * ss + sj * cc;
        a[k] = cj * cs - sj * sc;

        q[0] = cj * cc + sj * ss;
        q[1] = a[0];
        q[2] = a[1];
        q[3] = a[2];

        if (R->parity) 
            q[j + 1] = -q[j + 1];
    }

    DiQuat convertEuler(const DiVec3& eulerrot)
    {
        float qout[] = { 1, 0, 0, 0 };
        float ein[] = { DiDegree(eulerrot.x).valueRadians(), 
            DiDegree(eulerrot.y).valueRadians(), DiDegree(eulerrot.z).valueRadians() };
        eulO_to_quat(qout, ein, EULER_ORDER_YXZ);
        DiQuat q(qout);
        return q;
    }

    //////////////////////////////////////////////////////////////////////////

    DiK2MdfSerial::DiK2MdfSerial()
    {
    }

    DiK2MdfSerial::~DiK2MdfSerial()
    {
    }

    bool DiK2MdfSerial::ParseMdf(const DiString& file, DiK2ModelAsset* target)
    {
        FILE* fp = fopen(file.c_str(), "r");
        if (!fp)
        {
            DI_WARNING("Cannot open k2 mdf: %s", file.c_str());
            return false;
        }
        
        DI_LOG("Loading k2 mdf: %s",file.c_str());
        
        DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));

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
            DI_WARNING("Invalid material script.");
            return false;
        }
        
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
        FILE* fp = fopen(file.c_str(), "rb");
        if (!fp)
        {
            DI_WARNING("Cannot open k2 model: %s", file.c_str());
            return false;
        }
        
        DI_LOG("Loading k2 model: %s",file.c_str());

        DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));
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
            DI_WARNING("It's an old version of k2 model: %s", target->GetName().c_str());
        }
        
        DI_SERIAL_LOG("version : %d", version);
        DI_SERIAL_LOG("%d meshes", num_meshes);
        DI_SERIAL_LOG("%d sprites", num_sprites);
        DI_SERIAL_LOG("%d surfaces", num_surfs);
        DI_SERIAL_LOG("%d bones", num_bones);
        
        float minx, miny, minz;
        float maxx, maxy, maxz;
        
        minx = ReadFloat(mStream);
        miny = ReadFloat(mStream);
        minz = ReadFloat(mStream);
        
        maxx = ReadFloat(mStream);
        maxy = ReadFloat(mStream);
        maxz = ReadFloat(mStream);
        
        DiString meshname = target->GetName() + "/";
        meshname += mStream->GetName();
        DiMeshPtr mesh = DiAssetManager::GetInstancePtr()->CreateOrReplaceAsset<DiMesh>(meshname);

        DiAABB bounds(minx, miny, minz, maxx, maxy, maxz);
        mesh->SetBounds(bounds);

        if (!LoadBones(target->GetBoneData(), num_bones))
            return false;

        if (!CheckFourcc("mesh"))
        {
            DI_WARNING("invalid k2 model file: it should be mesh chunk");
            return false;
        }

        while (!mStream->Eof())
        {
            DiSubMesh* sub = LoadMeshes(target,mesh);

            if (sub)
            {
                void* data = sub->CreateSourceData(0, gCurrentVerts.size(), sizeof(float)* 11);
                memcpy(data, &gCurrentVerts[0], gCurrentVerts.size()*sizeof(float)* 11);
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

            target->names.push_back(name);
            target->parents.push_back(parentID);
            target->trans.push_back(matrix.convert());
            target->invtrans.push_back(inv_matrix.convert());
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

        // chunk size
        ReadInt(mStream);

        int mesh_index = ReadInt(mStream);
        int mesh_mod = ReadInt(mStream);
        int vertics_count = ReadInt(mStream);

        DiSubMesh* submesh = nullptr;
        if (vertics_count != 0)
        {
            submesh = mesh->CreateSubMesh();
            submesh->SetVerticeNum(vertics_count);
            submesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
            submesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
            submesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
            submesh->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);
        }
       
        gCurrentVerts.resize(vertics_count);

        float minx, miny, minz;
        float maxx, maxy, maxz;

        minx = ReadFloat(mStream);
        miny = ReadFloat(mStream);
        minz = ReadFloat(mStream);

        maxx = ReadFloat(mStream);
        maxy = ReadFloat(mStream);
        maxz = ReadFloat(mStream);

        DiAABB bounds(minx, miny, minz, maxx, maxy, maxz);

        int bone_link = ReadInt(mStream);

        uint8 mesh_name_length = ReadByte(mStream);
        uint8 material_name_length = ReadByte(mStream);

        DiString meshName = ReadString(mStream, mesh_name_length);
        ReadByte(mStream);

        DiString materialName = ReadString(mStream, material_name_length);
        ReadByte(mStream);

        DI_SERIAL_LOG("Mesh name: %s", meshName.c_str());
        DI_SERIAL_LOG("Material name: %s", materialName.c_str());

        if (submesh)
        {
            DiString fullpath = DiK2MdfSerial::GetK2MediaPath(target->GetName());
            DiString mateiralFile = fullpath + "/" + materialName;
            mateiralFile += ".material";

            DiString matName = target->GetName() + "/" + materialName;
            submesh->SetMaterialName(matName);

            ParseMaterial(mateiralFile, matName, fullpath);
        }

        while (!mStream->Eof())
        {
            char hed[4];
            mStream->Read(hed, sizeof(char) * 4);

            if (CheckFourcc(hed, "vrts"))
                read_verts();
            else if (CheckFourcc(hed, "face"))
                read_face(submesh);
            else if (CheckFourcc(hed, "nrml"))
                read_nrml();
            else if (CheckFourcc(hed, "texc"))
                read_texc();
            else if (CheckFourcc(hed, "colr"))
                read_colr();
            else if (CheckFourcc(hed, "lnk1"))
                read_lnk();
            else if (CheckFourcc(hed, "lnk3"))
                read_lnk();
            else if (CheckFourcc(hed, "sign"))
                read_sign();
            else if (CheckFourcc(hed, "tang"))
                read_tang();
            else if (CheckFourcc(hed, "mesh"))
                // start another turn
                return submesh;
            else if (CheckFourcc(hed, "surf"))
                read_surf();
            else
            {
                DI_WARNING("K2 Model:Unknown data chunk tag: %c%c%c%c", hed[0], hed[1], hed[2], hed[3]);
                return submesh;
            }
        }

        return submesh;
    }

    void DiK2MdfSerial::read_verts()
    {
        DI_SERIAL_LOG("------------verts-----------");
        int chunkSize = ReadInt(mStream);
        int numverts = (chunkSize - 4) / 12;
        int meshindex = ReadInt(mStream);
        DI_SERIAL_LOG("mesh index:%d", meshindex);
        float* vertics = new float[3 * numverts];
        mStream->Read(vertics, sizeof(float)* 3 * numverts);

        DI_ASSERT(gCurrentVerts.size() == numverts);
        for (int i = 0; i < numverts; i++)
        {
            if (g_trans_orit)
            {
                gCurrentVerts[i].pos.x = -vertics[i * 3 + 1];
                gCurrentVerts[i].pos.y = vertics[i * 3 + 2];
                gCurrentVerts[i].pos.z = -vertics[i * 3 + 0];
                DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_Y);
                gCurrentVerts[i].pos = q*gCurrentVerts[i].pos;
            }
            else
            {
                gCurrentVerts[i].pos.x = vertics[i * 3 + 0];
                gCurrentVerts[i].pos.y = vertics[i * 3 + 1];
                gCurrentVerts[i].pos.z = vertics[i * 3 + 2];
            }
        }

        delete[] vertics;
    }

    void DiK2MdfSerial::read_face(DiSubMesh* sub)
    {
        DI_SERIAL_LOG("-----------face------------");
        int chunkSize = ReadInt(mStream);
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

    void DiK2MdfSerial::read_nrml()
    {
        DI_SERIAL_LOG("-----------nrml------------");
        int chunkSize = ReadInt(mStream);

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
            if (g_trans_orit)
            {
                gCurrentVerts[i].normal.x = -vertics[i * 3 + 1];
                gCurrentVerts[i].normal.y = vertics[i * 3 + 2];
                gCurrentVerts[i].normal.z = -vertics[i * 3 + 0];
                DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_Y);
                gCurrentVerts[i].normal = q*gCurrentVerts[i].normal;
            }
            else
            {
                gCurrentVerts[i].normal.x = vertics[i * 3 + 0];
                gCurrentVerts[i].normal.y = vertics[i * 3 + 1];
                gCurrentVerts[i].normal.z = vertics[i * 3 + 2];
            }
        }

        delete[] vertics;
    }

    void DiK2MdfSerial::read_texc()
    {
        DI_SERIAL_LOG("-----------texc------------");
        int chunkSize = ReadInt(mStream);

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

    void DiK2MdfSerial::read_colr()
    {
        DI_SERIAL_LOG("-----------colr------------");
        int chunkSize = ReadInt(mStream);

        int numverts = (chunkSize - 4) / 4;
        DI_SERIAL_LOG("colr nums:%d", numverts);
        int meshindex = ReadInt(mStream);

        unsigned int* vertics = new unsigned int[numverts];
        mStream->Read(vertics, sizeof(unsigned int)*numverts);
        delete[] vertics; // unused currently
    }

    void DiK2MdfSerial::read_lnk()
    {
        DI_SERIAL_LOG("-----------lnk------------");
        int chunkSize = ReadInt(mStream);

        int meshIndex = ReadInt(mStream);
        int vertNums = ReadInt(mStream);

        for (int i = 0; i < vertNums; i++)
        {
            int num_weights = ReadInt(mStream);

            float* weights = new float[num_weights];
            int* indexes = new int[num_weights];

            mStream->Read(weights, sizeof(float)*num_weights);
            mStream->Read(indexes, sizeof(int)*num_weights);

            delete[] weights;
            delete[] indexes;
        }
    }

    void DiK2MdfSerial::read_sign()
    {
        // what's this
        DI_SERIAL_LOG("-----------sign------------");
        int chunkSize = ReadInt(mStream);
        mStream->Skip(chunkSize);
    }

    void DiK2MdfSerial::read_tang()
    {
        DI_SERIAL_LOG("-----------tang------------");
        int chunkSize = ReadInt(mStream);

        int numverts = (chunkSize - 4) / 12;
        DI_SERIAL_LOG("tang nums:%d", numverts);
        int meshindex = ReadInt(mStream);
        int what = ReadInt(mStream);

        float* vertics = new float[3 * numverts];
        mStream->Read(vertics, sizeof(float)* 3 * numverts);

        DI_ASSERT(gCurrentVerts.size() == numverts);
        for (int i = 0; i < numverts; i++)
        {
            if (g_trans_orit)
            {
                gCurrentVerts[i].tangent.x = -vertics[i * 3 + 1];
                gCurrentVerts[i].tangent.y = vertics[i * 3 + 2];
                gCurrentVerts[i].tangent.z = -vertics[i * 3 + 0];
                DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_Y);
                gCurrentVerts[i].tangent = q*gCurrentVerts[i].tangent;
            }
            else
            {
                gCurrentVerts[i].tangent.x = vertics[i * 3 + 0];
                gCurrentVerts[i].tangent.y = vertics[i * 3 + 1];
                gCurrentVerts[i].tangent.z = vertics[i * 3 + 2];
            }
        }

        delete[] vertics;
    }

    void DiK2MdfSerial::read_surf()
    {
        DI_SERIAL_LOG("-----------surf------------");

        int chunk_size = ReadInt(mStream);

#if 0
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

    DiMaterialPtr DiK2MdfSerial::ParseMaterial(const DiString& matFile, const DiString& name, const DiString& basePath)
    {
        FILE* fp = fopen(matFile.c_str(), "r");
        if (!fp)
        {
            DI_WARNING("Cannot open k2 material: %s", matFile.c_str());
            return nullptr;
        }

        DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));

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

        DiXMLElement child = root.GetChild();
        while (child)
        {
            if (child.CheckName("phase"))
            {
                // we just care this
                if (child.GetAttribute("name") == "color")
                {
                    if (child.HasAttribute("translucent"))
                        translucent = child.GetBool("translucent");
                    if (child.HasAttribute("alphatest") && child.GetBool("alphatest"))
                        shaderFlag |= SHADER_FLAG_ALPHA_TEST;

                    // samplers
                    DiXMLElement samplers = child.GetChild();
                    while (samplers)
                    {
                        if (samplers.CheckName("sampler"))
                        {
                            if (samplers.GetAttribute("name") == "diffuse")
                                shaderFlag |= SHADER_FLAG_USE_MAP;
                            else if (samplers.GetAttribute("name") == "normalmap")
                                shaderFlag |= SHADER_FLAG_USE_NORMALMAP;

                            // we don't care about their team map for now
                        }

                        samplers = samplers.GetNext();
                    }
                }
            }
            child = child.GetNext();
        }

        DiMaterialPtr mat = DiMaterial::QuickCreate(name, "phong_v", "phong_p", shaderFlag);
        if (translucent)
            mat->SetBlendMode(BLEND_ALPHA);

        DiShaderParameter* sm = mat->GetShaderParameter();

        // load textures
        if (shaderFlag & SHADER_FLAG_USE_MAP)
        {
            DiString colorPath = basePath + "/";
            colorPath += "color.dds";

            FILE* texFp = fopen(colorPath.c_str(), "rb");
            if (!texFp)
            {
                DI_WARNING("Cannot open k2 texture: %s", colorPath.c_str());
            }
            else
            {
                DiTexturePtr tex = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(colorPath);
                DiDataStreamPtr texdata(DI_NEW DiFileHandleDataStream(colorPath,texFp));
                tex->Load(texdata);
                sm->WriteTexture2D("map", colorPath);
            }
        }
        if (shaderFlag & SHADER_FLAG_USE_NORMALMAP)
        {
            DiString colorPath = basePath + "/";
            colorPath += "normal_rxgb.dds";

            FILE* texFp = fopen(colorPath.c_str(), "rb");
            if (!texFp)
            {
                DI_WARNING("Cannot open k2 texture: %s", colorPath.c_str());
            }
            else
            {
                DiTexturePtr tex = DiAssetManager::GetInstance().CreateOrReplaceAsset<DiTexture>(colorPath);
                DiDataStreamPtr texdata(DI_NEW DiFileHandleDataStream(colorPath,texFp));
                tex->Load(texdata);
                sm->WriteTexture2D("normalMap", colorPath);
            }
        }

        return mat;
    }
    
    bool DiK2MdfSerial::LoadClip(const DiString& file, K2Anim* anim, DiK2ModelAsset* target)
    {
        FILE* fp = fopen(file.c_str(), "rb");
        if (!fp)
        {
            DI_WARNING("Cannot open k2 clip: %s", file.c_str());
            return false;
        }
        
        DiDataStreamPtr data(DI_NEW DiFileHandleDataStream(fp));
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
        
        int what = ReadInt(mStream);
        int version = ReadInt(mStream);
        int numBones = ReadInt(mStream);
        int numFrames = ReadInt(mStream);

        if (anim->numframes == 0)
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
            
            int boneindex = ReadInt(mStream);
            int keytype = ReadInt(mStream);
            int numkeys = ReadInt(mStream);
            
            uint8 nameLen = ReadByte(mStream);
            DiString bonename = ReadString(mStream,nameLen);
            ReadByte(mStream);

            Clip& c = rawClips[bonename];
            c.resize(numFrames);

            //DI_SERIAL_LOG("id=%d, name=%s, ktype=%d, ks=%d", boneindex, name.c_str(), keytype, numkeys);
            if (keytype == MKEY_VISIBILITY)
            {
                // we don't care it
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
            for (int f = 0; f < numFrames; ++f)
            {
                auto& frames = kf->boneFrames[boneName];
                Trans trans;
                trans.pos = c.pos[f];
                trans.scale = c.scale[f];
                trans.rot = convertEuler(c.rot[f]);
                frames.push_back(trans);
            }
        }
        
        delete[] dataFloats;

        return true;
    }

    DiString DiK2MdfSerial::GetK2MediaPath(const DiString& relativePath)
    {
        DiString baseFolder = DiBase::CommandMgr->GetConsoleVar("k2_media_folder")->GetString();
        baseFolder += "/";
        baseFolder += relativePath;
        return baseFolder;
    }
}