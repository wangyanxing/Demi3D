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
#include "RenderWindow.h"
#include "RenderTarget.h"

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
                for(int i = 0; i < 3; ++i)
                    mRotateRingNode[i]->SetVisible(true);
                break;
                
            default:
                break;
        }
    }
    
    void DiTransGizmo::Update()
    {
        auto camera = DiBase::Driver->GetSceneManager()->GetCamera();
        auto pos = mBaseNode->GetDerivedPosition();
        
        DiQuat defaultRot[3];
        defaultRot[0].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_Y);
        defaultRot[1].FromAngleAxis(DiRadian(DiDegree(90)), DiVec3::UNIT_X);
        defaultRot[2] = DiQuat::IDENTITY;
        
        DiVec3 planeNormal[3] = {DiVec3::UNIT_X, DiVec3::UNIT_Y, DiVec3::UNIT_Z};
        DiVec3 dirOrig[3] = {DiVec3::UNIT_Y, DiVec3::UNIT_Z, DiVec3::UNIT_Y};
        
        for(int i = 0; i < 3; ++i)
        {
            auto dir = camera->GetDerivedPosition() - pos;
            DiPlane plane(planeNormal[i], 0);
            dir = plane.projectVector(dir);
            dir.normalise();
            
            auto rotation = dirOrig[i].getRotationTo(dir);
            mRotateRingNode[i]->SetOrientation(rotation * defaultRot[i]);
        }
        
		DiVec4 rect;
		int iLeft,iTop,iWidth,iHeight;
        
        float width = DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->GetWidth();
        float height = DiBase::Driver->GetMainRenderWindow()->GetSceneCanvas()->GetHeight();

		float minsize = std::min(width, height);
		float distance = (mBaseNode->GetDerivedPosition() - camera->GetDerivedPosition()).length();
        
		distance /= (minsize / 20.0f);
		distance *= 1;

        mBaseNode->SetScale(distance,distance,distance);
    }
    
    void DiTransGizmo::HideAll()
    {
        mAxesNode->SetVisible(false);
        
        mRotateCircleNode->SetVisible(false);
        
        for(int i = 0; i < 3; ++i)
            mRotateRingNode[i]->SetVisible(false);
    }
    
    void DiTransGizmo::RayPick(const DiRay& ray)
    {
        if(mMode == GIZMO_MOVE || mMode == GIZMO_SCALE)
        {
            DiTransAxes::PickResult ret = mAxes->Pick(ray);
            if(ret != DiTransAxes::PICK_NONE)
            {
                DI_DEBUG("Pick: %d", ret);
            }
        }
        else if(mMode == GIZMO_ROTATE)
        {
            
        }
    }
    
    void DiTransGizmo::OnMouseMove(const OIS::MouseEvent& event)
    {
        
    }
    
    void DiTransGizmo::OnMouseDown(const OIS::MouseEvent& event)
    {
        
    }
    
    void DiTransGizmo::OnMouseUp(const OIS::MouseEvent& event)
    {
        
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
        mCircles->AddCircle(DiVec3::ZERO, 4.3f, DiColor());
        mCircles->AddCircle(DiVec3::ZERO, 3.5f, DiColor(0.2f,0.2f,0.2f));
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
        }
        
        HideAll();
    }
}