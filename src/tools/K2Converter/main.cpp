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

#pragma warning(disable : 4996)

#include <stdio.h>
#include <string.h>
#include <fstream>

#include <windows.h>

DiAssetManager* g_assetMgr = 0;
DiMesh*		    g_currentModel = 0;
DiSubMesh*		g_currentSub = 0;
bool			g_trans_orit = true;
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

void read_bone(int id, FILE* fp, DiSkeleton* skeleton, DiVector<DiPair<int, int>>& parentTable)
{
    int parent_bone_index = read_int(fp);
    printf("parent bone index = %d\n", parent_bone_index);

    matrix34 inv_matrix;
    matrix34 matrix;
    read_matrix34(fp, &inv_matrix);
    read_matrix34(fp, &matrix);
    print_matrix34(inv_matrix);
    //printf("\n");
    print_matrix34(matrix);

    DiMat4 mat = converMat(matrix);

    char length = read_char(fp);
    char name[255];
    read_string(fp, name, length);
    printf("name : %s\n", name);

    DiBone* bone = skeleton->CreateBone(name, id);
    DiVec3 pos, scale;
    DiQuat rot;
    mat.decomposition(pos, scale, rot);
    bone->SetPosition(pos);
    bone->SetScale(scale);
    bone->SetOrientation(rot);

    char zero = read_char(fp);

    parentTable[id].first = id;
    parentTable[id].second = parent_bone_index;
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

    DiVector<DiPair<int, int>> parentTable(num_bones);

    printf("------------bone-----------\n");
    for (int i = 0; i < num_bones; i++)
    {
        read_bone(i, fp, skeleton, parentTable);
    }

    for (auto i = parentTable.begin(); i != parentTable.end(); ++i)
    {
        DiBone* b = skeleton->GetBone(i->first);
        if (i->second >= 0)
        {
            DiBone* parent = skeleton->GetBone(i->second);
            parent->AddChild(b);
        }
    }

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
    Clip(int b)
        :bone(b)
    {
        vis = 0;
        pos = DiVec3::ZERO;
        rot = DiVec3::ZERO;
        scale = DiVec3::UNIT_SCALE;
    }
    int bone;
    int vis;
    DiVec3 pos;
    DiVec3 rot;
    DiVec3 scale;
};

void updateClip(Clip& c, int keytype, float val, int vis)
{
    switch (keytype)
    {
    case MKEY_X:
        c.pos.x = val; return;
    case MKEY_Y:
        c.pos.y = val; return;
    case MKEY_Z:
        c.pos.z = val; return;
    case MKEY_PITCH:
        c.rot.x = val; return;
    case MKEY_ROLL:
        c.rot.y = val; return;
    case MKEY_YAW:
        c.rot.z = val; return;
    case MKEY_VISIBILITY:
        c.vis = vis; return;
    case MKEY_SCALE_X:
        c.scale.x = val; return;
    case MKEY_SCALE_Y:
        c.scale.x = val; return;
    case MKEY_SCALE_Z:
        c.scale.x = val; return;
    default:
        break;
    }
}

void readClip(FILE* fp)
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

    printf("boneindex: %d, keytype: %d, numkeys: %d\n", boneindex, keytype, numkeys);
    //printf("keytype: %d\n", keytype);
    //printf("numkeys: %d\n", numkeys);
    printf("name: %s\n", name);
    
    if (keytype == MKEY_VISIBILITY)
    {
        unsigned char* data = new unsigned char[numkeys];
        fread(data, numkeys, 1, fp);
        delete[] data;
    }
    else
    {
        float* data = new float[numkeys];
        fread(data, numkeys * sizeof(float), 1, fp);
        //for (int i = 0; i < numkeys; i++)
            //printf("%f ", data[i]);
        //printf("\n");
        delete[] data;
    }

    delete[] name;
}

void convertClip(const char* name, const char* outpath)
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

    while (!feof(fp))
    {
        readClip(fp);
    }

    fclose(fp);
}

int main(int numargs, char** args)
{
    init_engine();

    DiString quality = "high.model";
    g_prefix = "";
    DiString inpath = "L:\\Games\\HON_res\\heroes\\";
    DiString texturepath = "L:\\Games\\HON_tex\\00000000\\heroes\\";
    DiString output = "C:\\Demi\\media\\models\\hon\\";


    DiVector<DiString> files;
    //files.push_back("aluna");
    //files.push_back("rally");
    files.push_back("andromeda");
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

    g_prefix = "armadon";
    convertClip("L:\\Games\\HON_res\\heroes\\armadon\\clips\\walk_1.clip", "C:\\Demi\\media\\models\\hon\\");

    close_engine();

    return 0;
}
