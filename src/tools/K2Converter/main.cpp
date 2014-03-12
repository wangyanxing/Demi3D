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

#pragma warning(disable:4267)
#include "Demi.h"

using namespace Demi;

#include "LogManager.h"
#include "AssetManager.h"
#include "Model.h"
#include "MeshSerial.h"
#include "SubModel.h"
#include "SubMesh.h"
#include "Motion.h"
#include "MotionSerial.h"
#include "Bone.h"
#include "ClipController.h"
#include "AnimationClip.h"
#include "Animation.h"
#include "KeyFrame.h"

#pragma warning(disable : 4996)

#include <stdio.h>
#include <string.h>
#include <fstream>

#include <windows.h>

DiAssetManager* g_assetMgr = 0;
DiMesh*		    g_currentModel = 0;
DiSubMesh*		g_currentSub = 0;
bool			g_trans_orit = false;
DiString		g_prefix;

struct Vert
{
    DiVec3 pos;
    DiVec2 uv;
    DiVec3 normal;
    DiVec3 tangent;
};

typedef DiVector<Vert>	VertList;
VertList g_currentVerts;

struct matrix4
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};

struct matrix34
{
    float m00, m01, m02;
    float m10, m11, m12;
    float m20, m21, m22;
    float m30, m31, m32;
};

struct bone
{
    matrix34 inv_matrix;
    matrix34 matrix;
    DiString name;
    int id;
    int parent;
};

DiVector<bone> g_curBones;

DiMat4 converMat(const matrix34& m)
{
    DiMat4 ret(
        m.m00, m.m01, m.m02, 0,
        m.m10, m.m11, m.m12, 0,
        m.m20, m.m21, m.m22, 0,
        m.m30, m.m31, m.m32, 1);
    ret = ret.transpose();
    return ret;
}

void init_engine()
{
    printf("init engine\n");
    DiLogManager* logmgr = new DiLogManager;
    logmgr->Init("K2Converter.log");

    g_assetMgr = new DiAssetManager;
    g_assetMgr->SetBasePath("E:\\msg2\\Engine\\Media\\");
}

void close_engine()
{
    printf("closing engine\n");
    delete DiLogManager::GetInstancePtr();
    delete g_assetMgr;
    g_assetMgr = NULL;
}

void print_matrix34(const matrix34& mat34)
{
    return;
    printf("%.2f,%.2f,%.2f,0.00\n%.2f,%.2f,%.2f,0.00\n%.2f,%.2f,%.2f,0.00\n%.2f,%.2f,%.2f,1.00\n",
        mat34.m00, mat34.m01, mat34.m02, mat34.m10, mat34.m11, mat34.m12, mat34.m20, mat34.m21, mat34.m22, mat34.m30, mat34.m31, mat34.m32);
}

bool check_fourcc_chunk(FILE* fp, char* sig)
{
    char hed[4];
    fread(hed, sizeof(char), 4, fp);
    if (sig[0] != hed[0] ||
        sig[1] != hed[1] ||
        sig[2] != hed[2] ||
        sig[3] != hed[3])
    {
        return false;
    }
    return true;
}

bool check_fourcc_chunk(char* hed, char* sig)
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

int read_int(FILE* fp)
{
    int num = 0;
    fread(&num, sizeof(int), 1, fp);
    return num;
}

char read_char(FILE* fp)
{
    char num = 0;
    fread(&num, sizeof(char), 1, fp);
    return num;
}

unsigned char read_uchar(FILE* fp)
{
    unsigned char num = 0;
    fread(&num, sizeof(unsigned char), 1, fp);
    return num;
}

float read_float(FILE* fp)
{
    float num = 0;
    fread(&num, sizeof(float), 1, fp);
    return num;
}

void read_matrix34(FILE* fp, matrix34* mat)
{
    fread(mat, sizeof(matrix34), 1, fp);
}

void read_string(FILE* fp, char* are, int length)
{
    fread(are, length, 1, fp);
    are[length] = '\0';
}

void convert_bones(DiSkeleton* skeleton)
{
#if 1
    FILE* fp = fopen("bones.txt","w+");
    fprintf(fp, "DiVector<DiVec3> bones;\n");
    for (size_t i = 0; i < g_curBones.size(); ++i)
    {
        DiVec3 pos, scale;
        DiQuat rot;
        DiMat4 mat = converMat(g_curBones[i].matrix);
        mat.decomposition(pos, scale, rot);
        fprintf(fp, "bones.push_back(DiVec3(%f, %f, %f));\n", pos.x, pos.y, pos.z);
    }
    fclose(fp);
#endif

    for (size_t i = 0; i < g_curBones.size(); ++i)
        skeleton->CreateBone(g_curBones[i].name, g_curBones[i].id);

    DiBone* root = nullptr;

    for (size_t i = 0; i < g_curBones.size(); ++i)
    {
        DiBone* b = skeleton->GetBone(g_curBones[i].id);
        if (g_curBones[i].parent >= 0)
        {
#if 1
            DiBone* p = skeleton->GetBone(g_curBones[i].parent);
            p->AddChild(b);
#endif
        }
        else
            root = b;
    }

    DiVector<DiBone*> level;
    level.push_back(root);

    while (!level.empty())
    {
        DiVector<DiBone*> next;
        for (auto i = level.begin(); i != level.end(); ++i)
        {
            DiBone* bone = (*i);
            int id = bone->GetHandle();

            DiVec3 pos, scale;
            DiQuat rot;

            DiMat4 mat = converMat(g_curBones[id].matrix);
            mat.decomposition(pos, scale, rot);

            DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_X);
            if (g_trans_orit)
            {
                pos = q*pos;
                rot = q*rot;
            }

            if (g_curBones[id].parent >= 0)
            {
                DiMat4 parentmat = DiMat4::IDENTITY;
                parentmat = converMat(g_curBones[g_curBones[id].parent].matrix);

                DiVec3 parent_pos, parent_scale;
                DiQuat parent_rot;
                parentmat.decomposition(parent_pos, parent_scale, parent_rot);

                if (g_trans_orit)
                {
                    parent_pos = q*parent_pos;
                    parent_rot = q*parent_rot;
                }

                pos = parent_rot.Inverse() * (pos - parent_pos) / parent_scale;
                rot = parent_rot.Inverse() * rot;
            }

#if 0
            if (bone->GetParent())
#else
            if (false)
#endif
            {
                bone->SetDerivedPosition(pos);
                bone->SetDerivedOrientation(rot);
            }
            else
            {
                bone->SetPosition(pos);
                bone->SetOrientation(rot);
            }

            bone->SetScale(scale);

            size_t cn = bone->GetChildrenNum();
            for (size_t c = 0; c < cn; c++)
                next.push_back((DiBone*)bone->GetChild(c));
        }

        level = next;
    }
}

void read_bone(int id, FILE* fp)
{
    int parent_bone_index = read_int(fp);
    //printf("parent bone index = %d\n", parent_bone_index);

    matrix34 inv_matrix;
    matrix34 matrix;
    read_matrix34(fp, &inv_matrix);
    read_matrix34(fp, &matrix);
    print_matrix34(inv_matrix);
    //printf("\n");
    print_matrix34(matrix);

    char length = read_char(fp);
    char name[255];
    read_string(fp, name, length);
    //printf("name : %s\n", name);

    g_curBones[id].id = id;
    g_curBones[id].parent = parent_bone_index;
    g_curBones[id].matrix = matrix;
    g_curBones[id].inv_matrix = inv_matrix;
    g_curBones[id].name = name;



    char zero = read_char(fp);
}

void read_verts(FILE* fp)
{
    printf("------------verts-----------\n");
    int chunkSize = read_int(fp);
    int numverts = (chunkSize - 4) / 12;
    int meshindex = read_int(fp);
    printf("mesh index:%d\n", meshindex);
    float* vertics = new float[3 * numverts];
    fread(vertics, sizeof(float)* 3 * numverts, 1, fp);

    DI_ASSERT(g_currentVerts.size() == numverts);
    for (int i = 0; i < numverts; i++)
    {
        if (g_trans_orit)
        {
            g_currentVerts[i].pos.x = -vertics[i * 3 + 1];
            g_currentVerts[i].pos.y = vertics[i * 3 + 2];
            g_currentVerts[i].pos.z = -vertics[i * 3 + 0];
            DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_Y);
            g_currentVerts[i].pos = q*g_currentVerts[i].pos;
        }
        else
        {
            g_currentVerts[i].pos.x = vertics[i * 3 + 0];
            g_currentVerts[i].pos.y = vertics[i * 3 + 1];
            g_currentVerts[i].pos.z = vertics[i * 3 + 2];
        }
    }

    delete[] vertics;
}

void read_face(FILE* fp)
{
    printf("-----------face------------\n");
    int chunkSize = read_int(fp);
    int meshindex = read_int(fp);
    int numfaces = read_int(fp);
    printf("face nums:%d\n", numfaces);
    g_currentSub->SetPrimitiveCount(numfaces);
    g_currentSub->SetPrimitiveType(PT_TRIANGLELIST);

    char indexsize = read_char(fp);
    printf("index size:%d\n", indexsize);

    void* id = g_currentSub->CreateIndexData(3 * numfaces, indexsize == 4);

    if (indexsize == 2)
    {
        fread(id, sizeof(unsigned short)* 3 * numfaces, 1, fp);
    }
    else if (indexsize == 1)
    {
        USHORT* id16 = (USHORT*)(id);
        unsigned char* indices = new unsigned char[numfaces * 3];
        fread(indices, sizeof(unsigned char)* 3 * numfaces, 1, fp);

        for (int i = 0; i < numfaces * 3; i++)
        {
            id16[i] = (USHORT)indices[i];
        }

        delete[]indices;
    }
    else if (indexsize == 4)//32bits index
    {
        fread(id, sizeof(unsigned int)* 3 * numfaces, 1, fp);
    }
    else
    {
        printf("wtf??!!!\n");
    }
}

void read_nrml(FILE* fp)
{
    printf("-----------nrml------------\n");
    int chunkSize = read_int(fp);

    int numverts = (chunkSize - 4) / 12;
    printf("normal nums:%d\n", numverts);

    if (!numverts)
    {
        return;
    }
    int meshindex = read_int(fp);

    float* vertics = new float[3 * numverts];
    fread(vertics, sizeof(float)* 3 * numverts, 1, fp);

    DI_ASSERT(g_currentVerts.size() == numverts);
    for (int i = 0; i < numverts; i++)
    {
        if (g_trans_orit)
        {
            g_currentVerts[i].normal.x = -vertics[i * 3 + 1];
            g_currentVerts[i].normal.y = vertics[i * 3 + 2];
            g_currentVerts[i].normal.z = -vertics[i * 3 + 0];
            DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_Y);
            g_currentVerts[i].normal = q*g_currentVerts[i].normal;
        }
        else
        {
            g_currentVerts[i].normal.x = vertics[i * 3 + 0];
            g_currentVerts[i].normal.y = vertics[i * 3 + 1];
            g_currentVerts[i].normal.z = vertics[i * 3 + 2];
        }
    }

    delete[] vertics;
}

void read_texc(FILE* fp)
{
    printf("-----------texc------------\n");
    int chunkSize = read_int(fp);

    int numverts = (chunkSize - 4) / 8;
    printf("texc nums:%d\n", numverts);

    int meshindex = read_int(fp);
    float temp = read_float(fp);

    float* vertics = new float[2 * numverts];
    fread(vertics, sizeof(float)* 2 * numverts, 1, fp);

    DI_ASSERT(g_currentVerts.size() == numverts);
    //fprintf(f,"%.5f, %.5f\n",vertics[i*2+0],vertics[i*2+1]);
    for (int i = 0; i < numverts; i++)
    {
        g_currentVerts[i].uv.x = vertics[i * 2 + 0];
        g_currentVerts[i].uv.y = vertics[i * 2 + 1];
    }
    delete[] vertics;

    //float temp = read_float(fp);
}

void read_colr(FILE* fp)
{
    printf("-----------colr------------\n");
    int chunkSize = read_int(fp);

    int numverts = (chunkSize - 4) / 4;
    printf("colr nums:%d\n", numverts);
    int meshindex = read_int(fp);

    unsigned int* vertics = new unsigned int[numverts];
    fread(vertics, sizeof(unsigned int)*numverts, 1, fp);
    delete[] vertics;
}

void read_lnk(FILE* fp)
{
    printf("-----------lnk------------\n");
    int chunkSize = read_int(fp);

    int meshIndex = read_int(fp);
    int vertNums = read_int(fp);

    for (int i = 0; i < vertNums; i++)
    {
        int num_weights = read_int(fp);

        float* weights = new float[num_weights];
        int* indexes = new int[num_weights];

        fread(weights, sizeof(float)*num_weights, 1, fp);
        fread(indexes, sizeof(int)*num_weights, 1, fp);

        delete[] weights;
        delete[] indexes;
    }
}

void read_sign(FILE* fp)
{
    printf("-----------sign------------\n");
    int chunkSize = read_int(fp);

    unsigned char* vertics = new unsigned char[chunkSize];
    fread(vertics, chunkSize, 1, fp);
    delete[] vertics;


    // 	int numverts = (chunkSize - 8);
    // 	printf("sign nums:%d\n",numverts);
    // 
    // 	unsigned char* vertics = new unsigned char[numverts];
    // 	fread(vertics,sizeof(unsigned char)*numverts,1,fp);
    // 	delete[] vertics;
}

void read_tang(FILE* fp)
{
    printf("-----------tang------------\n");
    int chunkSize = read_int(fp);

    int numverts = (chunkSize - 4) / 12;
    printf("tang nums:%d\n", numverts);
    int meshindex = read_int(fp);
    int what = read_int(fp);

    float* vertics = new float[3 * numverts];
    fread(vertics, sizeof(float)* 3 * numverts, 1, fp);

    DI_ASSERT(g_currentVerts.size() == numverts);
    for (int i = 0; i < numverts; i++)
    {
        if (g_trans_orit)
        {
            g_currentVerts[i].tangent.x = -vertics[i * 3 + 1];
            g_currentVerts[i].tangent.y = vertics[i * 3 + 2];
            g_currentVerts[i].tangent.z = -vertics[i * 3 + 0];
            DiQuat q(DiRadian(DiDegree(-90)), DiVec3::UNIT_Y);
            g_currentVerts[i].tangent = q*g_currentVerts[i].tangent;
        }
        else
        {
            g_currentVerts[i].tangent.x = vertics[i * 3 + 0];
            g_currentVerts[i].tangent.y = vertics[i * 3 + 1];
            g_currentVerts[i].tangent.z = vertics[i * 3 + 2];
        }
    }

    delete[] vertics;
}

void read_mesh(FILE* fp)
{
    float minx, miny, minz;
    float maxx, maxy, maxz;

    printf("\n##################################\n\n");

    g_currentSub = g_currentModel->CreateSubMesh();
    g_currentVerts.clear();

    g_currentSub->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_POSITION);
    g_currentSub->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT2, VERT_USAGE_TEXCOORD);
    g_currentSub->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_NORMAL);
    g_currentSub->GetVertexElements().AddElement(0, VERT_TYPE_FLOAT3, VERT_USAGE_TANGENT);

    // mesh trunkµÄ´óÐ¡
    read_int(fp);

    int mesh_index = read_int(fp);
    printf("mesh index:%d\n", mesh_index);

    int mesh_mod = read_int(fp);
    printf("mesh mod:%d\n", mesh_mod);


    int vertics_count = read_int(fp);
    printf("vertics count:%d\n", vertics_count);
    g_currentSub->SetVerticeNum(vertics_count);
    g_currentVerts.resize(vertics_count);

    minx = read_float(fp);
    miny = read_float(fp);
    minz = read_float(fp);

    maxx = read_float(fp);
    maxy = read_float(fp);
    maxz = read_float(fp);

    printf("bbox : (%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f)\n", minx, miny, minz, maxx, maxy, maxz);

    int bone_link = read_int(fp);
    printf("bone link:%d\n", bone_link);

    char mesh_name_length = read_char(fp);
    char material_name_length = read_char(fp);
    char name[255];
    read_string(fp, name, mesh_name_length);
    printf("mesh name : %s\n", name);
    read_char(fp);

    read_string(fp, name, material_name_length);
    printf("material name : %s\n", name);
    read_char(fp);
    DiString matname = name;
    if (matname == "material")
    {
        g_currentSub->SetMaterialName(g_prefix + "_mat.mtl");
    }
    else if (matname == "256material")
    {
        g_currentSub->SetMaterialName(g_prefix + "_256_mat.mtl");
    }
    else if (matname == "ramp_1")
    {
        g_currentSub->SetMaterialName(g_prefix + "_ramp_1.mtl");
    }
    else
    {
        g_currentSub->SetMaterialName(g_prefix + "_mat.mtl");
    }

    while (!feof(fp))
    {
        char hed[4];
        fread(hed, sizeof(char), 4, fp);

        if (check_fourcc_chunk(hed, "vrts"))
        {
            read_verts(fp);
        }
        else if (check_fourcc_chunk(hed, "face"))
        {
            read_face(fp);
        }
        else if (check_fourcc_chunk(hed, "nrml"))
        {
            read_nrml(fp);
        }
        else if (check_fourcc_chunk(hed, "texc"))
        {
            read_texc(fp);
        }
        else if (check_fourcc_chunk(hed, "colr"))
        {
            read_colr(fp);
        }
        else if (check_fourcc_chunk(hed, "lnk1"))
        {
            read_lnk(fp);
        }
        else if (check_fourcc_chunk(hed, "lnk3"))
        {
            read_lnk(fp);
        }
        else if (check_fourcc_chunk(hed, "sign"))
        {
            read_sign(fp);
        }
        else if (check_fourcc_chunk(hed, "tang"))
        {
            read_tang(fp);
        }
        else if (check_fourcc_chunk(hed, "mesh"))
        {
            return;
        }
        else
        {
            printf("!!!!!wrong way : %c%c%c%c\n", hed[0], hed[1], hed[2], hed[3]);
            return;
        }
    }
}

void set_mesh_verts()
{
    void* data = g_currentSub->CreateSourceData(0, g_currentVerts.size(), sizeof(float)* 11);
    memcpy(data, &g_currentVerts[0], g_currentVerts.size()*sizeof(float)* 11);
}

DiMotionPtr convertMesh(const char* name, const char* outpath)
{
    FILE* fp = fopen(name, "rb");
    static int id = 0;
    DiString asname;
    asname.Format("conver_md_%d", ++id);

    DiMeshPtr model = DiAssetManager::GetInstancePtr()->CreateManualAsset<DiMesh>(asname);
    g_currentModel = model.get();

    if (!check_fourcc_chunk(fp, "SMDL"))
    {
        printf("unknow file format\n");
        return nullptr;
    }

    if (!check_fourcc_chunk(fp, "head"))
    {
        printf("file does not start with head chunk\n");
        return nullptr;
    }

    // head size
    int temp = read_int(fp);

    int version = read_int(fp);
    int num_meshes = read_int(fp);
    int num_sprites = read_int(fp);
    int num_surfs = read_int(fp);
    int num_bones = read_int(fp);

    printf("version : %d\n", version);
    printf("%d meshes\n", num_meshes);
    printf("%d sprites\n", num_sprites);
    printf("%d surfaces\n", num_surfs);
    printf("%d bones\n", num_bones);

    float minx, miny, minz;
    float maxx, maxy, maxz;

    minx = read_float(fp);
    miny = read_float(fp);
    minz = read_float(fp);

    maxx = read_float(fp);
    maxy = read_float(fp);
    maxz = read_float(fp);

    printf("bbox : (%.2f,%.2f,%.2f),(%.2f,%.2f,%.2f)\n", minx, miny, minz, maxx, maxy, maxz);
    g_currentModel->SetBounds(DiAABB(minx, miny, minz, maxx, maxy, maxz));

    if (!check_fourcc_chunk(fp, "bone"))
    {
        printf("no bone chunk\n");
        return nullptr;
    }

    // bone trunk size
    read_int(fp);

    // create motion 
    DiString motionname = g_prefix;
    DiMotionPtr mt = Demi::DiAssetManager::GetInstancePtr()->CreateOrReplaceAsset<DiMotion>(motionname);
    DiSkeleton* skeleton = mt->CreateSkeleton();

    g_curBones.resize(num_bones);

    printf("------------bone-----------\n");
    for (int i = 0; i < num_bones; i++)
    {
        read_bone(i, fp);
    }

    convert_bones(skeleton);

    printf("------------mesh-----------\n");
    if (!check_fourcc_chunk(fp, "mesh"))
    {
        printf("no mesh chunk\n");
        return nullptr;
    }

    while (!feof(fp))
    {
        read_mesh(fp);
        set_mesh_verts();
    }

    DiMeshSerializer ms;
    DiString exfile = name;
    exfile = exfile.ExtractFileName();
    DiString out = outpath;
    out += exfile;
    ms.ExportMesh(model, out);

    fclose(fp);
    return mt;
}

void copyRelMats(const char* texPath, const char* name, const char* outpath)
{
    DiVector<DiString> files;
    files.push_back("color.dds");
    files.push_back("normal_rxgb.dds");
    files.push_back("spec_color.dds");

    for (size_t i = 0; i < files.size(); i++)
    {
        DiString src = texPath + DiString(name);
        src += "\\";
        src += files[i];

        DiString dest = outpath;
        dest += files[i];

        CopyFileA(src.c_str(), dest.c_str(), FALSE);
    }
}

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

struct Clip
{
    Clip(int b = 0)
    :bone(b)
    {
        //vis.resize(frames, 0);
        //pos.resize(frames,DiVec3::ZERO);
        //rot.resize(frames,DiVec3::ZERO);
        //scale.resize(frames,DiVec3::UNIT_SCALE);
    }
    int bone;
    DiVector<int> vis;
    DiVector<DiVec3> pos;
    DiVector<DiVec3> rot;
    DiVector<DiVec3> scale;
};

size_t maxClipKeys(Clip& c)
{
    return std::max(c.pos.size(), std::max(c.rot.size(), c.scale.size()));
}

void updateClipSize(Clip& c, int keytype, int size)
{
    switch (keytype)
    {
    case MKEY_X:
    case MKEY_Y:
    case MKEY_Z:
        if (c.pos.size() != size)
            c.pos.resize(size, DiVec3::ZERO);
        return;
    case MKEY_PITCH:
    case MKEY_ROLL:
    case MKEY_YAW:
        if (c.rot.size() != size)
            c.rot.resize(size, DiVec3::ZERO);
        return;
    case MKEY_VISIBILITY:
        if (c.vis.size() != size)
            c.vis.resize(size, 0);
        return;
    case MKEY_SCALE_X:
    case MKEY_SCALE_Y:
    case MKEY_SCALE_Z:
        if (c.scale.size() != size)
            c.scale.resize(size, DiVec3::UNIT_SCALE);
        return;
    default:
        break;
    }
}

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

void readClip(FILE* fp, DiVector<Clip>& clips)
{
    if (!check_fourcc_chunk(fp, "bmtn"))
    {
        printf("unknow clip chunk\n");
        return;
    }

    int chunkSize = read_int(fp);

    int boneindex = read_int(fp);
    int keytype = read_int(fp);
    int numkeys = read_int(fp);

    unsigned char nameLen = read_uchar(fp);
    char* name = new char[nameLen + 1];
    read_string(fp, name, nameLen);
    read_uchar(fp);

    //printf("boneindex: %d, keytype: %d, numkeys: %d\n", boneindex, keytype, numkeys);
    //printf("name: %s\n", name);

    Clip& c = clips[boneindex];
    updateClipSize(c, keytype, numkeys);
    
    if (keytype == MKEY_VISIBILITY)
    {
        unsigned char* data = new unsigned char[numkeys];
        fread(data, numkeys, 1, fp);
        for (int i = 0; i < numkeys; i++)
            updateClip(c, keytype, i, 0, data[i]);
        delete[] data;
    }
    else
    {
        float* data = new float[numkeys];
        fread(data, numkeys * sizeof(float), 1, fp);
        for (int i = 0; i < numkeys; i++)
            updateClip(c, keytype, i, data[i], 0);
        delete[] data;
    }

    delete[] name;
}

void convertToDemiAnim(DiVector<Clip>& clips, DiAnimation* anim, DiSkeleton* skeleton, int numFrames)
{
    DiVector<DiVector<DiVec3> > vs;
    DiVector<DiVector<DiVec3> > rs;
    for (size_t i = 0; i < clips.size(); ++i)
    {
        Clip& c = clips[i];
        DiBone* bone = skeleton->GetBone(i);

        DiMat4 boneLocal;
        boneLocal.makeTransform(bone->GetPosition(),bone->GetScale(),bone->GetOrientation());
        boneLocal = boneLocal.inverse();

        DiVector<DiVec3> mod;
        DiVector<DiVec3> modr;

        DiNodeClip* clip = anim->CreateNodeClip(i, bone);
        size_t maxkeys = maxClipKeys(c);
        for (size_t k = 0; k < maxkeys; ++k)
        {
            DiTransformKeyFrame* key = clip->CreateNodeKeyFrame(float(k) / float(numFrames));
            DiVec3 trans = DiVec3::ZERO;
            DiVec3 scale = DiVec3::UNIT_SCALE;
            DiQuat rot = DiQuat::IDENTITY;
            
            if (k < c.pos.size())
                trans = c.pos[k];
            else
                trans = c.pos.back();

            if (k < c.scale.size())
                scale = c.scale[k];
            else
                scale = c.scale.back();

            DiVec3 euler;
            if (k < c.rot.size())
                euler = c.rot[k];
            else
                euler = c.rot.back();

            DiMat3 rotmat;
            rotmat.FromEulerAnglesYXZ(DiDegree(euler.z), DiDegree(euler.y), DiDegree(euler.x));
            rot.FromRotationMatrix(rotmat);

            DiMat4 m;
            m.makeTransform(trans,scale,rot);
            m = boneLocal * m;
            DiVec3 _p, _s;
            DiQuat _q;
            m.decomposition(_p, _s, _q);

            mod.push_back(trans);
            modr.push_back(euler);

            key->SetTranslate(_p);
            key->SetScale(_s);
            key->SetRotation(_q);
        }

        vs.push_back(mod);
        rs.push_back(modr);
    }

#if 1
    FILE* fp = fopen("clips.txt", "w+");
    fprintf(fp, "std::vector<std::vector<DiVec3> > clips; \n");
    fprintf(fp, "std::vector<std::vector<DiVec3> > rotclips; \n");
    fprintf(fp, "void initClips(){ \n");
    fprintf(fp, "std::vector<DiVec3> t; \n");

    for (size_t i = 0; i < vs.size(); ++i)
    {
        fprintf(fp, "t = {");
        for (auto pi = vs[i].begin(); pi != vs[i].end(); ++pi)
        {
            fprintf(fp, "%sDiVec3(%f,%f,%f)", pi == vs[i].begin() ? "" : ",", pi->x, pi->y, pi->z);
        }
        fprintf(fp, "};\n");
        fprintf(fp, "clips.push_back(t);\n");
    }
    for (size_t i = 0; i < rs.size(); ++i)
    {
        fprintf(fp, "t = {");
        for (auto pi = rs[i].begin(); pi != rs[i].end(); ++pi)
        {
            fprintf(fp, "%sDiVec3(%f,%f,%f)", pi == rs[i].begin() ? "" : ",", pi->x, pi->y, pi->z);
        }
        fprintf(fp, "};\n");
        fprintf(fp, "rotclips.push_back(t);\n");
    }
    fprintf(fp, "}\n");
    fclose(fp);
#endif
}

void convertClip(const char* name, const char* outpath, DiMotionPtr motion)
{
    FILE* fp = fopen(name, "rb");

    if (!check_fourcc_chunk(fp, "CLIP"))
    {
        printf("unknow file format\n");
        return;
    }

    if (!check_fourcc_chunk(fp, "head"))
    {
        printf("file does not start with head chunk\n");
        return;
    }

    int what = read_int(fp);
    int version = read_int(fp); // should be 2?
    int num_bones = read_int(fp);
    int num_frames = read_int(fp);

    printf("version: %d\n", version);
    printf("num bones: %d\n", num_bones);
    printf("num frames: %d\n", num_frames);


    DiVector<Clip> clips;
    for (int i = 0; i < num_bones; i++)
    {
        clips.push_back(Clip(i));
    }

    while (!feof(fp))
    {
        readClip(fp,clips);
    }

    DiString animName = name;
    animName = animName.ExtractFileName();
    animName = animName.ExtractBaseName();
    float length = num_frames / 30.0f; // fixed fps for now
    DiAnimation* anim = motion->CreateAnimation(animName, length);
    DiSkeleton* ske = motion->GetSkeleton();

    convertToDemiAnim(clips, anim, ske, num_frames);

    DiMotionSerializer ser;
    DiString out = outpath;
    out += animName;
    out += ".motion";
    ser.ExportMotion(motion, out);

    fclose(fp);
}

int main(int numargs, char** args)
{
    init_engine();

    //DiString quality = "high.model";
    //DiString quality = "pumkinward.model";
    DiString quality = "model.model";
    g_prefix = "";
    //DiString inpath = "L:\\Games\\HON_res\\heroes\\";
    DiString inpath = "L:\\Games\\HON_res\\world\\props\\";
    DiString texturepath = "L:\\Games\\HON_tex\\00000000\\heroes\\";
    DiString output = "C:\\Demi\\media\\models\\hon\\";


    DiVector<DiString> files;
    //files.push_back("halloween_props");
    files.push_back("cauldron");
    //files.push_back("aluna");
    //files.push_back("rally");
    //files.push_back("andromeda");
    //files.push_back("arachna");
    //files.push_back("armadon");

    DiVector<DiMotionPtr> motions;
    motions.resize(files.size());

    for (size_t i = 0; i < files.size(); i++)
    {
        DiString out = output;
        g_prefix = files[i];
        out += g_prefix + "_";
        DiString filename = inpath + files[i] + "\\" + quality;
        auto motion = convertMesh(filename.c_str(), out.c_str());
        motions[i] = motion;

        copyRelMats(texturepath.c_str(), files[i].c_str(), out.c_str());
    }
    //g_prefix = "teapot";
    //convertMesh("L:\\Games\\HON_res\\core\\null\\null.model", "C:\\Demi\\media\\models\\hon\\");

    g_prefix = "default_1";
    //convertClip("L:\\Games\\HON_res\\heroes\\aluna\\clips\\default_1.clip", "C:\\Demi\\media\\models\\hon\\", motions[0]);
    //convertClip("L:\\Games\\HON_res\\world\\props\\halloween_props\\pumkinward.clip", "C:\\Demi\\media\\models\\hon\\", motions[0]);
    convertClip("L:\\Games\\HON_res\\world\\props\\cauldron\\clips\\default_1.clip", "C:\\Demi\\media\\models\\hon\\", motions[0]);

    close_engine();

    return 0;
}
