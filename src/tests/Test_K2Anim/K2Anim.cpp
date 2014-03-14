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

#include "K2Anim.h"

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

////////////////////////////////////////////////////////////////////////

K2Anim::K2Anim(const DiString& baseDir)
  : mBaseDir(baseDir)
{
}

K2Anim::~K2Anim()
{
}

void K2Anim::_LoadBones(const DiString& model)
{
    DiString realfile = mBaseDir + model;
    FILE* fp = fopen(realfile.c_str(), "rb");
    
    if (!check_fourcc_chunk(fp, "SMDL"))
    {
        printf("unknow file format\n");
        return;
    }
    
    if (!check_fourcc_chunk(fp, "head"))
    {
        printf("file does not start with head chunk\n");
        return;
    }
    
    // head size
    int temp = read_int(fp);
    
    int version = read_int(fp);
    int num_meshes = read_int(fp);
    int num_sprites = read_int(fp);
    int num_surfs = read_int(fp);
    int num_bones = read_int(fp);
    
    printf("%d bones\n", num_bones);
    
    float minx, miny, minz;
    float maxx, maxy, maxz;
    
    minx = read_float(fp);
    miny = read_float(fp);
    minz = read_float(fp);
    
    maxx = read_float(fp);
    maxy = read_float(fp);
    maxz = read_float(fp);
    
    
    if (!check_fourcc_chunk(fp, "bone"))
    {
        printf("no bone chunk\n");
        return;
    }
    
    // bone trunk size
    read_int(fp);
    
    mBones.clear();
    for (int id = 0; id < num_bones; id++)
    {
        int parent_bone_index = read_int(fp);
        printf("bone:%2d, parent:%2d\n", id, parent_bone_index);
        
        matrix34 inv_matrix;
        matrix34 matrix;
        read_matrix34(fp, &inv_matrix);
        read_matrix34(fp, &matrix);
        
        char length = read_char(fp);
        char name[255];
        read_string(fp, name, length);
        
        read_char(fp);
        
        DiMat4 mat = converMat(matrix);
        Trans t;
        mat.decomposition(t.pos, t.scale, t.rot);
        mBones.push_back(t);
    }
    fclose(fp);
}

void K2Anim::_LoadClips(const DiString& clip)
{
    
}