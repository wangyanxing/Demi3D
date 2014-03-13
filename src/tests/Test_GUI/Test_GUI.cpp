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

#include "Demi.h"
#include "DemoFrame.h"
#include "DebugHelper.h"

#include "testdata.h"
#include "Bone.h"

DiDebugHelperPtr debugger;
DiAnimModelPtr model;
DiCullNode* terNode;

DiColor ctable[] = {
    DiColor::White,
    DiColor::Red,
    DiColor::Green,
    DiColor::Blue,
    DiColor(1, 1, 0.5f),
    DiColor(0, 1, 1),
    DiColor(1, 0, 1)
};

typedef struct RotOrderInfo {
    short axis[3];
    short parity; /* parity of axis permutation (even=0, odd=1) - 'n' in original code */
} RotOrderInfo;

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
void eulO_to_mat3(float M[3][3], const float e[3], const short order)
{
    const RotOrderInfo *R = GET_ROTATIONORDER_INFO(order);
    short i = R->axis[0], j = R->axis[1], k = R->axis[2];
    double ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;

    if (R->parity) {
        ti = -e[i];
        tj = -e[j];
        th = -e[k];
    }
    else {
        ti = e[i];
        tj = e[j];
        th = e[k];
    }

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

    M[i][i] = cj * ch;
    M[j][i] = sj * sc - cs;
    M[k][i] = sj * cc + ss;
    M[i][j] = cj * sh;
    M[j][j] = sj * ss + cc;
    M[k][j] = sj * cs - sc;
    M[i][k] = -sj;
    M[j][k] = cj * si;
    M[k][k] = cj * ci;
}

int order = 0;
DiQuat convEuler(DiVec3& v)
{
#if 1
    DiMat3 rotmat;
    switch (order)
    {
    case 0:
        rotmat.FromEulerAnglesXYZ(DiDegree(v.z), DiDegree(v.y), DiDegree(v.x));
        break;
    case 1:
        rotmat.FromEulerAnglesXZY(DiDegree(v.z), DiDegree(v.y), DiDegree(v.x));
        break;
    case 2:
        rotmat.FromEulerAnglesYXZ(DiDegree(v.z), DiDegree(v.y), DiDegree(v.x));
        break;
    case 3:
        rotmat.FromEulerAnglesYZX(DiDegree(v.z), DiDegree(v.y), DiDegree(v.x));
        break;
    case 4:
        rotmat.FromEulerAnglesZXY(DiDegree(v.z), DiDegree(v.y), DiDegree(v.x));
        break;
    case 5:
        rotmat.FromEulerAnglesZYX(DiDegree(v.z), DiDegree(v.y), DiDegree(v.x));
        break;
    }
    DiQuat q;
    q.FromRotationMatrix(rotmat);
    return q;
#else
    float M[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
    float E[3] = { DiDegree(v.z).valueRadians(), DiDegree(v.y).valueRadians(), DiDegree(v.x).valueRadians() };
    eulO_to_mat3(M, E, order+1);
    DiMat3 dim(M);
    DiQuat q;
    q.FromRotationMatrix(dim);
    return q;
#endif
}

int gNumKeys = 151;
float gCurKey = 0;
DiVec3 getClip(int bone, int key)
{
    return key >= clips[bone].size() ? clips[bone].front() : clips[bone][key];
}
DiVec3 getClipRot(int bone, int key)
{
    return key >= rotclips[bone].size() ? rotclips[bone].front() : rotclips[bone][key];
}

void refreshDebugger()
{
    DiVec3 p1 = getClip(1, gCurKey);
    DiVec3 p2 = getClip(2, gCurKey);
    DiVec3 p3 = getClip(3, gCurKey);
    DiVec3 p4 = getClip(4, gCurKey);

    DiVec3 r1 = getClipRot(1, gCurKey);
    DiVec3 r2 = getClipRot(2, gCurKey);
    DiVec3 r3 = getClipRot(3, gCurKey);
    DiVec3 r4 = getClipRot(4, gCurKey);

    debugger->Clear();
    {
        DiNode nd;
        nd.SetPosition(p1);

        DiQuat curq = convEuler(r1);
        //nd.Pitch(DiRadian(DiDegree(r1.x)));
        //nd.Yaw(DiRadian(DiDegree(r1.y)));
        //nd.Roll(DiRadian(DiDegree(r1.z)));
        nd.SetOrientation(curq);
        //DiVec3 curp = p1;
        debugger->AddBoundingBox(nd.GetDerivedPosition(), 0.3f, DiColor::White);

        //curp = p1 + curq * p2;
        DiNode nd2;
        nd.AddChild(&nd2);
        nd2.Translate(p3);
        curq = convEuler(r3);
        nd2.SetOrientation(curq);
        debugger->AddBoundingBox(nd2.GetDerivedPosition(), 0.3f, DiColor::White);
        terNode->SetPosition(nd2.GetDerivedPosition());
        terNode->SetOrientation(nd2.GetDerivedOrientation());

        terNode;
    }
//     {
//         DiNode nd;
//         nd.SetPosition(p3);
// 
//         DiQuat curq = convEuler(r3);
//         //nd.Pitch(DiRadian(DiDegree(r3.x)));
//         //nd.Roll(DiRadian(DiDegree(r3.y)));
//         //nd.Yaw(DiRadian(DiDegree(r3.z)));
//         nd.SetOrientation(boneRots[3].Inverse()*curq);
//         //DiVec3 curp = p1;
//         debugger->AddBoundingBox(nd.GetDerivedPosition(), 0.3f, DiColor::Green);
// 
//         //curp = p1 + curq * p2;
//         DiNode nd2;
//         nd.AddChild(&nd2);
//         nd2.Translate(p4);
//         curq = convEuler(r4);
//         nd2.SetOrientation(curq);
//         debugger->AddBoundingBox(nd2.GetDerivedPosition(), 0.3f, DiColor::Green);
//     }
    //order = (++order) % 6;
    if (gCurKey > gNumKeys) gCurKey = 0;
}

void InitScene()
{
    DemiDemo::GetApp()->GetInputManager()->registerMousePressEvent("testeuler", [](const OIS::MouseEvent& evt, OIS::MouseButtonID id){
//        if (id == OIS::MB_Left)
//             refreshDebugger();
    });

	DiSceneManager* sm = DiBase::Driver->GetSceneManager();
    
    DiDirLightPtr dirlight;

    sm->SetAmbientColor(DiColor(0.2f, 0.2f, 0.2f));

    dirlight = make_shared<DiDirLight>();
    DiCullNode* dirNode = sm->GetRootNode()->CreateChild();
    dirNode->AttachObject(dirlight);
    dirlight->SetColor(DiColor());
    dirlight->SetDirection(DiVec3(0, -0.3f, -0.4).normalisedCopy());
    //dirlight->SetShadowCastEnable(true); 7

    DiCullNode* node = sm->GetRootNode()->CreateChild();
    //DiModelPtr model = make_shared<DiModel>("box", "arachna_high.model");
    model = make_shared<DiAnimModel>("box", "range_rax_high.model", "default_1.motion");
    model->SetAutoUpdateAnims(true);
    model->GetClipSet()->GetClipController("default_1")->SetEnabled(true);
    //model->SetShadowCastEnable(true);
    //model->SetMaterial(mat);
    node->AttachObject(model);
    //node->Scale(10, 10, 10);

    debugger = make_shared<DiDebugHelper>();
    debugger->AddSkeleton(model->GetSkeleton(), DiColor::Red);
    DiMaterialPtr mat = DiMaterial::QuickCreate("basic_v", "basic_p", SHADER_FLAG_USE_COLOR);
    mat->SetDepthCheck(false);
    debugger->SetMaterial(mat);
    node->AttachObject(debugger);

    terNode = sm->GetRootNode()->CreateChild();
    DiSimpleShapePtr termBox = make_shared<DiSimpleShape>();
    termBox->CreateBox(10);
    DiMaterialPtr matbox = DiMaterial::QuickCreate("lambert_v", "lambert_p");
    matbox->SetAmbient(DiColor(0.3f, 0.3f, 0.3f));
    termBox->SetMaterial(matbox);
    terNode->AttachObject(termBox);

    for (int i = 0; i < bones.size(); ++i)
    {
        DiAABB b;
        DiVec3 minv = bones[i] - DiVec3(0.3, 0.3, 0.3);
        DiVec3 maxv = bones[i] + DiVec3(0.3, 0.3, 0.3);
        b.SetExtents(minv, maxv);
        debugger->AddBoundingBox(b, ctable[i]);
    }

//     DiQuat bindq1 = model->GetSkeleton()->GetBone(2)->GetDerivedOrientation();
//     DiQuat bindq3 = model->GetSkeleton()->GetBone(4)->GetDerivedOrientation();
//     DiMat3 mbind1;
//     bindq1.ToRotationMatrix(mbind1);
//     DiRadian rx1, ry1, rz1;
//     mbind1.ToEulerAnglesXYZ(rx1,ry1,rz1);
//     float dx1 = rx1.valueDegrees();
//     float dy1 = ry1.valueDegrees();
//     float dz1 = rz1.valueDegrees();
//     DiMat3 mbind3;
//     bindq3.ToRotationMatrix(mbind3);
//     DiRadian rx3, ry3, rz3;
//     mbind3.ToEulerAnglesXYZ(rx3,ry3,rz3);
//     float dx3 = rx3.valueDegrees();
//     float dy3 = ry3.valueDegrees();
//     float dz3 = rz3.valueDegrees();
    
    //refreshDebugger();
}

void UpdateScene()
{
    gCurKey += 0.1f;
    //refreshDebugger();
    return;


    debugger->Clear();
    static int time = 0;
    int factor = 10;
    int frames = 151 * factor;

    time++;
    time = time > frames ? 0 : time;
    int f = time / factor;
    f = 0;

    debugger->Clear();
    for (int i = 0; i < bones.size(); ++i)
    {
//         if (i!=2)
//         {
//             continue;
//         }
        auto& c = clips[i];
        auto& cr = rotclips[i];
        DiVec3 pos = f >= c.size() ? c.back() : c[f];
        DiBone* bone_ = model->GetSkeleton()->GetBone(i);
        DiQuat bq = bone_->GetOrientation();
        //pos +=  bone_->GetDerivedPosition();

        DiVec3 euler = f >= cr.size() ? cr.back() : cr[f];

        float ex = DiDegree(euler.x).valueRadians();
        float ey = DiDegree(euler.y).valueRadians();
        float ez = DiDegree(euler.z).valueRadians();

        float M[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
        float E[3] = { ex, ey, ez };
        eulO_to_mat3(M, E, 3);
        DiMat3 dim(M);
        dim = dim.Transpose();

        DiMat3 rotmat;
        rotmat.FromEulerAnglesXYZ(DiDegree(euler.z), DiDegree(euler.y), DiDegree(euler.x));
        DiQuat q;
        q.FromRotationMatrix(rotmat);
        //pos = q*pos;

        DiAABB b;
        DiVec3 minv = pos - DiVec3(0.3, 0.3, 0.3);
        DiVec3 maxv = pos + DiVec3(0.3, 0.3, 0.3);
        b.SetExtents(minv, maxv);
        debugger->AddBoundingBox(b, ctable[i]);
    }

    //debugger->AddSkeleton(model->GetSkeleton(), DiColor::Red);
}

int main(int argc, char *argv[])
{
    initBones();
    initClips();

    DemiDemo app( DemoConfig("Demi test: Gui"));
	app.SetInitCallback(InitScene);
	app.SetUpdateCallback(UpdateScene);
    app.SetShutdownCallback([](){
        debugger.reset();
        model.reset();
    });
	app.Open();

	return 0;
}