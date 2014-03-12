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

DiDebugHelperPtr debugger;
DiAnimModelPtr model;

void InitScene()
{
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
    model = make_shared<DiAnimModel>("box", "cauldron_model.model", "default_1.motion");
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

    DiColor ctable[] = {
        DiColor::White,
        DiColor::Red,
        DiColor::Green,
        DiColor::Blue,
        DiColor(1, 1, 0)
    };

    for (int i = 0; i < bones.size(); ++i)
    {
        DiAABB b;
        DiVec3 minv = bones[i] - DiVec3(0.3, 0.3, 0.3);
        DiVec3 maxv = bones[i] + DiVec3(0.3, 0.3, 0.3);
        b.SetExtents(minv, maxv);
        debugger->AddBoundingBox(b, ctable[i]);
     }
}

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


void UpdateScene()
{
    debugger->Clear();
    static int time = 0;
    int factor = 10;
    int frames = 151 * factor;

    time++;
    time = time > frames ? 0 : time;
    int f = time / factor;

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
        debugger->AddBoundingBox(b, DiColor());
    }

    debugger->AddSkeleton(model->GetSkeleton(), DiColor::Red);
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