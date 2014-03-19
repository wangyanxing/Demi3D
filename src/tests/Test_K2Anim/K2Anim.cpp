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
#include "euler.h"
#include "MathMisc.h"

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
        printf("false fourcc chunk:%c%c%c%c\n",hed[0],hed[1],hed[2],hed[3]);
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
            if (c.pos.size() < size)
                c.pos.resize(size, DiVec3::ZERO);
            return;
        case MKEY_PITCH:
        case MKEY_ROLL:
        case MKEY_YAW:
            if (c.rot.size() < size)
                c.rot.resize(size, DiVec3::ZERO);
            return;
        case MKEY_VISIBILITY:
            if (c.vis.size() < size)
                c.vis.resize(size, 0);
            return;
        case MKEY_SCALE_X:
        case MKEY_SCALE_Y:
        case MKEY_SCALE_Z:
            if (c.scale.size() < size)
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

void readClip(FILE* fp, DiMap<DiString,Clip>& clips, int frameNum)
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

    int i = 0;
    if (DiString(name) == "Bip01")
    {
        i++;
    }

    Clip& c = clips[DiString(name)];
    c.resize(frameNum);
    
    printf("id=%d, name=%s, ktype=%d, ks=%d\n",boneindex,name,keytype,numkeys);

    if (keytype == MKEY_VISIBILITY)
    {
        unsigned char* data = new unsigned char[numkeys];
        fread(data, numkeys, 1, fp);
        for (int i = 0; i < numkeys; i++)
            updateClip(c, keytype, i, 0, data[i]);
        for (int i = numkeys; i < frameNum; ++i)
            updateClip(c, keytype, i, 0, data[numkeys - 1]);
        delete[] data;
    }
    else
    {
        float* data = new float[numkeys];
        fread(data, numkeys * sizeof(float), 1, fp);
        for (int i = 0; i < numkeys; i++)
            updateClip(c, keytype, i, data[i], 0);
        for (int i = numkeys; i < frameNum; ++i)
            updateClip(c, keytype, i, data[numkeys - 1], 0);
        delete[] data;
    }
    
    //skip some space if needed
    int kfsize = keytype == MKEY_VISIBILITY?1:4;
    int rest = chunkSize - (sizeof(int)*3 + 1 + nameLen + 1 + numkeys*kfsize);
    for(int i = 0; i < rest; ++i) read_uchar(fp);
    delete[] name;
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

    if (R->parity) q[j + 1] = -q[j + 1];
}

DiQuat convertEuler(const DiVec3& eulerrot)
{
    float qout[] = { 1, 0, 0, 0 };
    float ein[] = { DiDegree(eulerrot.x).valueRadians(), DiDegree(eulerrot.y).valueRadians(), DiDegree(eulerrot.z).valueRadians() };
    eulO_to_quat(qout, ein, EULER_ORDER_YXZ);
    DiQuat q(qout);
    return q;
}

////////////////////////////////////////////////////////////////////////

K2Anim::K2Anim(const DiString& baseDir, DiSceneManager* sm)
  : mBaseDir(baseDir)
  , mSm(sm)
  , mNumFrames(0)
  , mRootBone(nullptr)
{
    mBonesHelper = make_shared<DiDebugHelper>();
    DiMaterialPtr helpermat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
    helpermat->SetDepthCheck(false);
    mBonesHelper->SetMaterial(helpermat);
    sm->GetRootNode()->CreateChild()->AttachObject(mBonesHelper);

    mClipsHelper = make_shared<DiDebugHelper>();
    mClipsHelper->SetMaterial(helpermat);
    sm->GetRootNode()->CreateChild()->AttachObject(mClipsHelper);
}

K2Anim::~K2Anim()
{
}

void K2Anim::_LoadBones(const DiString& model)
{
    DiString realfile = mBaseDir + model;
    printf("Loading bones\n");
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
    
    printf("model version: %d\n",version);
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
    mParents.clear();
    mBoneNodes.clear();
    mNameTable.clear();
    for (int id = 0; id < num_bones; id++)
    {
        int parent_bone_index = read_int(fp);
        mParents.push_back(parent_bone_index);
        
        matrix34 inv_matrix;
        matrix34 matrix;
        read_matrix34(fp, &inv_matrix);
        read_matrix34(fp, &matrix);
        
        char length = read_char(fp);
        char name[255];
        read_string(fp, name, length);
        
        printf("bone:%s[%2d], parent:%2d\n", name, id, parent_bone_index);
        
        read_char(fp);
        
        mNameTable[name] = id;
        
        DiMat4 mat = converMat(matrix);
        Trans t;
        mat.decomposition(t.pos, t.scale, t.rot);
        mBones.push_back(t);

        mBoneNodes.push_back(new DiNode(name));
        mBoneIds[mBoneNodes.back()] = id;
    }

    for (size_t i = 0; i < mBoneNodes.size(); ++i)
    {
        DiNode* b = mBoneNodes[i];
        if (mParents[i] >= 0)
        {
            DiNode* parent = mBoneNodes[mParents[i]];
            parent->AddChild(b);
        }
        else
            mRootBone = b;
    }

    fclose(fp);
}

void K2Anim::_LoadClips(const DiString& clip)
{
    DiString realfile = mBaseDir + clip;
    FILE* fp = fopen(realfile.c_str(), "rb");

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
    mNumFrames = read_int(fp);

    printf("clip version: %d\n", version);
    printf("num bones: %d\n", num_bones);
    printf("num frames: %d\n", mNumFrames);

    mClips.clear();

    while (!feof(fp))
    {
        readClip(fp, mClips, mNumFrames);
    }

    fclose(fp);
}

void K2Anim::Load(const DiString& model, const DiString& clip)
{
    _LoadBones(model);
    _LoadClips(clip);

    // create visualizers
    for (auto i = mBoneNodes.begin(); i != mBoneNodes.end(); ++i)
    {
        DiCullNode* terNode = mSm->GetRootNode()->CreateChild();
        DiSimpleShapePtr termBox = make_shared<DiSimpleShape>();
        termBox->CreateBox(0.5);
        DiMaterialPtr matbox = DiMaterial::QuickCreate("lambert_v", "lambert_p");
        matbox->SetAmbient(DiColor(0.3f, 0.3f, 0.3f));
        termBox->SetMaterial(matbox);
        terNode->AttachObject(termBox);
        mBoneVisuals.push_back(terNode);
    }

    DiEuler e;
    e.fromQuat(DiQuat(0.092f, 0.701f, -0.092f, 0.701f));
    e.normalise();

    DiEuler e2(0, 0, DiDegree(-104).valueRadians());
    DiQuat q = e2;
    q.normalise();
    q.normalise();

    DiVec3 bip01Pos(0.000000, 1.129078, 14.300982);
    DiVec3 bip01PelvisPos(0.000000, 1.12907827, 14.3009815);

    DiQuat bip01Rot(0.608761966, 0, 0, -0.793352902);
    DiQuat bip01PelvisRot(0.0922950432, 0.701057017, -0.0922949165, 0.701058030);

    DiEuler bip01eu;
    bip01eu.fromQuat(bip01Rot);
    bip01eu.normalise();
    float b01yaw = bip01eu.yaw().valueDegrees();
    float b01pitch = bip01eu.pitch().valueDegrees();
    float b01roll = bip01eu.roll().valueDegrees();

    DiEuler bip01Pelviseu;
    bip01Pelviseu.fromQuat(bip01PelvisRot);
    bip01Pelviseu.normalise();
    float b01Pelvisyaw = bip01Pelviseu.yaw().valueDegrees();
    float b01Pelvispitch = bip01Pelviseu.pitch().valueDegrees();
    float b01Pelvisroll = bip01Pelviseu.roll().valueDegrees();

    DiMat4 parent;
    parent.makeInverseTransform(bip01Pos, DiVec3::UNIT_SCALE, bip01Rot);

    DiMat4 child;
    child.makeTransform(bip01PelvisPos, DiVec3::UNIT_SCALE, bip01PelvisRot);
    child = parent * child;
    child = child.inverse();

    DiMat3 t;
    child.extract3x3Matrix(t);
    DiQuat qq;
    qq.FromRotationMatrix(t);

    DiEuler bip01PelviseuKF(DiDegree(0), DiDegree(-90), DiDegree(-90));
    DiQuat kfquat = bip01PelviseuKF;
    kfquat = qq * kfquat;

    int x = 0; x++;


    _UpdateClips();
}

void K2Anim::_UpdateBonesHelper()
{
    mBonesHelper->Clear();

    uint32 numBons = mBones.size();
    for (uint32 i = 0; i < numBons; ++i)
    {
        DiVec3 pos = mBones[i].pos;
        DiVec3 posParent = mParents[i] >= 0 ? mBones[mParents[i]].pos : pos;

        mBonesHelper->AddLine(pos, posParent, DiColor::Yellow);
        
        DiAABB b;
        DiVec3 minv = pos - DiVec3(0.1, 0.1, 0.1);
        DiVec3 maxv = pos + DiVec3(0.1, 0.1, 0.1);
        b.SetExtents(minv, maxv);
        
#if 0
        if (mNameTable["Scene Root"] == (int)i ||
            mNameTable["Bip01"] == (int)i ||
            mNameTable["Bip01 Pelvis"] == (int)i ||
            mNameTable["Bip01 Spine"] == (int)i ||
            mNameTable["Bip01 L Thigh"] == (int)i ||
            mNameTable["Bip01 L Calf"] == (int)i)
#endif
        {
            mBonesHelper->AddBoundingBox(b, DiColor::Blue);
        }
    }
}

void K2Anim::_UpdateClipsHelper()
{
    mClipsHelper->Clear();
    static float key = 0;
    key += 0.1f;
    if (key >= mNumFrames)
        key = 0;
    
    DiVector<DiNode*> level;
    level.push_back(mRootBone);
    while (!level.empty())
    {
        DiVector<DiNode*> next;
        for (auto i = level.begin(); i != level.end(); ++i)
        {
            DiNode* bone = *i;
            DiString boneName = bone->GetName();

            auto it = mClips.find(boneName);
            if (it != mClips.end())
            {
                //if (bone->GetName() == "Box01")
                {
                    DiVec3 eulerrot = it->second.getRot(key);
                    DiQuat rot = convertEuler(eulerrot);
                    bone->SetOrientation(rot);
                    //DI_DEBUG("%f,%f,%f\n", eulerrot.x, eulerrot.y, eulerrot.z);
                }
                
                bone->SetPosition(it->second.getPos(key));
                
                bone->SetScale(it->second.getScale(key));
            }

            size_t cn = bone->GetChildrenNum();
            for (size_t c = 0; c < cn; c++)
                next.push_back((DiBone*)bone->GetChild(c));

            if (bone->GetName() == "Bip01 Pelvis")
            {
                DiQuat qpel = bone->GetDerivedOrientation();
                DiVec3 pos = bone->GetDerivedPosition();
            }
        }
        level = next;
    }
    
    mRootBone->_Update(true, false);

    uint32 numBons = mBoneNodes.size();
    for (uint32 i = 0; i < numBons; ++i)
    {
        DiNode* b = mBoneNodes[i];
        
        DiNode* p = b->GetParent();
        DiVec3 pos = b->GetDerivedPosition();
        DiVec3 posParent = p ? (p->GetDerivedPosition()) : pos;
        //
        
#if 0
        if (b->GetName() == "Scene Root" ||
            b->GetName() == "Bip01" ||
            b->GetName() == "Bip01 Pelvis" ||
            b->GetName() == "Bip01 Spine" ||
            b->GetName() == "Bip01 L Thigh" || 
            b->GetName() == "Bip01 L Calf")
        {
            mClipsHelper->AddLine(pos, posParent, DiColor::Red);
            mBoneVisuals[i]->SetVisible(true);
        }
        else
#endif
        {
#if 0
            mBoneVisuals[i]->SetVisible(false);
#else
            mClipsHelper->AddLine(pos, posParent, DiColor::Red);
            mBoneVisuals[i]->SetVisible(true);
#endif
        }
        mBoneVisuals[i]->SetPosition(pos);
        mBoneVisuals[i]->SetOrientation(b->GetDerivedOrientation());
        mBoneVisuals[i]->SetScale(b->GetDerivedScale());
    }
}

void K2Anim::_UpdateClips()
{
    mClipsHelper->Clear();
    static float key = 0;
    key += 0.1f;
    if (key >= mNumFrames)
        key = 0;

    int count = 0;
    for (auto i = mClips.begin(); i != mClips.end(); ++i)
    {
        DiString name = i->first;

        if (name == "Bip01 Pelvis")
        {
            int x = 0; x++;
        }

        int boneID = mNameTable[name];
        int parentID = mParents[boneID];
        Trans& t = mBones[boneID];

        DiMat4 child;
        child.makeTransform(t.pos, t.scale, t.rot);

        if (parentID >= 0)
        {
            Trans& tpar = mBones[parentID];
            DiMat4 parent;
            parent.makeInverseTransform(tpar.pos, tpar.scale, tpar.rot);

            child = parent * child;
        }
        child = child.inverse();

        DiVec3 kpos = i->second.getPos(key);
        DiVec3 eulerrot = i->second.getRot(key);
        DiEuler e(DiDegree(eulerrot.x), DiDegree(eulerrot.y), DiDegree(eulerrot.z));
        e.normalise();
        DiQuat krot = e;
        DiQuat krot2 = DiEuler(DiDegree(eulerrot.y), DiDegree(eulerrot.x), DiDegree(eulerrot.z));

        float qout[] = { 1, 0, 0, 0 };
        float ein[] = { DiDegree(eulerrot.x).valueRadians(), DiDegree(eulerrot.y).valueRadians(), DiDegree(eulerrot.z).valueRadians() };
        eulO_to_quat(qout, ein, EULER_ORDER_YXZ);

        DiVec3 kscale = i->second.getScale(key);

        DiMat4 keyframe;
        keyframe.makeTransform(kpos, kscale, krot2);

        keyframe = child * keyframe;

        DiVec3 p, s;
        DiQuat r;
        keyframe.decomposition(p, s, r);
       
        DiVec3 worldPos = t.pos + p;
        mBoneVisuals[count++]->SetPosition(worldPos);

        if (name == "Bip01" || name == "Bip01 Pelvis" || name == "Bip01 Spine")
        {
            DiString sp;
            sp.SetVector3(p);
            DiString sr;
            sr.SetQuaternion(r);
            DI_DEBUG("%s",name.c_str());
            DI_DEBUG("%s", sp.c_str());
            DI_DEBUG("%s", sr.c_str());
        }
    }
}
