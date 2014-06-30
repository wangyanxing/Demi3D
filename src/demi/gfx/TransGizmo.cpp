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
#include "TransGizmo.h"
#include "TransAxes.h"
#include "SimpleShape.h"
#include "DebugHelper.h"
#include "CullNode.h"
#include "SceneManager.h"

namespace Demi
{
    DiTransGizmo::DiTransGizmo(void)
    {
        Create();
    }

    DiTransGizmo::~DiTransGizmo(void)
    {
    }
    
    void DiTransGizmo::SetGizmoMode(GizmoMode mode)
    {
        if(mMode == mode)
            return;
        
        mMode = mode;
     
        HideAll();
        
        switch (mode)
        {
            case GIZMO_MOVE:
                mAxesNode->SetVisible(true);
                break;
                
            case GIZMO_ROTATE:
                mRotateCircleNode->SetVisible(true);
                break;
                
            default:
                break;
        }
    }
    
    void DiTransGizmo::Update()
    {
    }
    
    void DiTransGizmo::HideAll()
    {
        mAxesNode->SetVisible(false);
        mRotateCircleNode->SetVisible(false);
    }
    
    void DiTransGizmo::Create()
    {
        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        mBaseNode = sm->GetRootNode()->CreateChild();
        
        mAxes = make_shared<DiTransAxes>();
        mAxesNode = mBaseNode->CreateChild();
        mAxesNode->AttachObject(mAxes);
        
        mCircles = make_shared<DiDebugHelper>();
        mCircles->SetFaceToCamera(true);
        mCircles->AddCircle(DiVec3::ZERO, 4, DiColor());
        mCircles->AddCircle(DiVec3::ZERO, 3, DiColor(0.2f,0.2f,0.2f));
        mRotateCircleNode = mBaseNode->CreateChild();
        mRotateCircleNode->AttachObject(mCircles);
        
        DiColor cols[3] = {DiColor::Red, DiColor::Green, DiColor::Blue};
        DiQuat rot[3];
        rot[0].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_Y);
        rot[1].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_X);
        rot[2] = DiQuat::IDENTITY;
        
        for(int i = 0; i < 3; ++i)
        {
            mRotateRings[i] = make_shared<DiSimpleShape>();
            mRotateRings[i]->CreateTorus(3.5f, 0.03f, 6, 32, DiMath::PI);
            auto mat = DiMaterial::QuickCreate("basic_v", "basic_p");
            mat->SetDiffuse(cols[i]);
            mRotateRings[i]->SetMaterial(mat);
            mRotateRingNode[i] = mBaseNode->CreateChild();
            mRotateRingNode[i]->AttachObject(mRotateRings[i]);
            mRotateRingNode[i]->SetOrientation(rot[i]);
        }
        
        HideAll();
    }
}