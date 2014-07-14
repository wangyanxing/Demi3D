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

#include "FxerPch.h"
#include "TransGizmo.h"
#include "TransAxes.h"
#include "SimpleShape.h"
#include "DebugHelper.h"
#include "CullNode.h"
#include "SceneManager.h"
#include "RenderWindow.h"
#include "RenderTarget.h"
#include "EditorManager.h"
#include "BaseEditorObject.h"

namespace Demi
{
    DiTransGizmo::DiTransGizmo(DiBaseEditorObj* parent)
        :mObject(parent)
    {
        for(int i = 0; i < 3; ++i)
        {
            mRotateRingNode[i] = 0;
        }
        Create();
    }

    DiTransGizmo::~DiTransGizmo(void)
    {
        DiSceneManager* sm = DiBase::Driver->GetSceneManager();
        sm->DestroyNode(mBaseNode);
        sm->DestroyNode(mAxesNode);
        sm->DestroyNode(mRotateCircleNode);
        for(int i = 0; i < 3; ++i)
        {
            sm->DestroyNode(mRotateRingNode[i]);
        }
    }
    
    void DiTransGizmo::SetGizmoMode(GizmoMode mode)
    {
        mMode = mode;
     
        HideAll();
        
        switch (mode)
        {
        case GIZMO_SELECT:
            mAxesNode->SetVisible(true);
            mAxes->SetShowArraw(false);
            mAxes->SetShowScale(false);
            mAxes->SetShowScalePlane(false);
            break;
                
        case GIZMO_MOVE:
            mAxesNode->SetVisible(true);
            mAxes->SetShowArraw(true);
            mAxes->SetShowScale(false);
            mAxes->SetShowScalePlane(false);
            break;
            
        case GIZMO_ROTATE:
            mRotateCircleNode->SetVisible(true);
            for(int i = 0; i < 3; ++i)
                mRotateRingNode[i]->SetVisible(true);
            break;
                
        case GIZMO_SCALE:
            mAxesNode->SetVisible(true);
            mAxes->SetShowArraw(false);
            mAxes->SetShowScale(true);
            break;
            
        default:
            break;
        }
    }
    
    void DiTransGizmo::Update()
    {
        if(!mActive)
            return;
        
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
    
    DiRay DiTransGizmo::GetMouseRay(int _left, int _top)
    {
        DiRenderWindow* rw = DiBase::Driver->GetMainRenderWindow();
        
        auto vp = rw->GetSceneCanvas()->GetViewport();
        
        float screenPosX = float(_left) / float(rw->GetSceneCanvas()->GetWidth());
        float screenPosY = float(_top) / float(rw->GetSceneCanvas()->GetHeight());
        
        screenPosX = DiMath::Clamp<float>(screenPosX, 0, 1);
        screenPosY = DiMath::Clamp<float>(screenPosY, 0, 1);
        
        auto camera = DiBase::Driver->GetSceneManager()->GetCamera();
        return camera->GetCameraToViewportRay(screenPosX, screenPosY);
    }
    
    DiAABB DiTransGizmo::GetWorldAABB()
    {
        DiAABB aabb = mAxesNode->GetWorldAABB();
        aabb.Merge(mRotateRingNode[0]->GetWorldAABB());
        aabb.Merge(mRotateRingNode[1]->GetWorldAABB());
        aabb.Merge(mRotateRingNode[2]->GetWorldAABB());
        return aabb;
    }
    
    void DiTransGizmo::OnMouseDrag(int _left, int _top, MyGUI::MouseButton _id)
    {
        if(_id != MyGUI::MouseButton::Left || !mPicking)
        {
            return;
        }
        
        auto ray = GetMouseRay(_left, _top);
        
        if(mMode == GIZMO_MOVE)
        {
            auto vNewPos = CalculateDeltaTrans(ray, mLastGizmoPlane, mLastPickResult, mLastDerivedPos);
            vNewPos = vNewPos - mLast3DDelta + mLastDerivedPos;
            mObject->SetPosition(vNewPos);
        }
        else if(mMode == GIZMO_SCALE)
        {
            auto vNewDist = CalculateDeltaTrans(ray, mLastGizmoPlane, mLastPickResult, mLastDerivedPos);
            float fNewDist = vNewDist.length();
            float fLength = mLast3DDelta.length();
            auto vScale = mLastDerivedScale;
            
            if (mLastPickResult == DiTransAxes::PICK_SCALE_X)
            {
                vScale.x *= (fNewDist / fLength);
            }
            if (mLastPickResult == DiTransAxes::PICK_SCALE_Y)
            {
                vScale.y *= (fNewDist / fLength);
            }
            if (mLastPickResult == DiTransAxes::PICK_SCALE_Z)
            {
                vScale.z *= (fNewDist / fLength);
            }
            if (mLastPickResult == DiTransAxes::PICK_SCALE_XY)
            {
                vScale.x *= (fNewDist / fLength);
                vScale.y *= (fNewDist / fLength);
            }
            if (mLastPickResult == DiTransAxes::PICK_SCALE_XZ)
            {
                vScale.x *= (fNewDist / fLength);
                vScale.z *= (fNewDist / fLength);
            }
            if (mLastPickResult == DiTransAxes::PICK_SCALE_YZ)
            {
                vScale.y *= (fNewDist / fLength);
                vScale.z *= (fNewDist / fLength);
            }
            if (mLastPickResult == DiTransAxes::PICK_SCALE_XYZ)
            {
                vScale.x *= (fNewDist / fLength);
                vScale.y *= (fNewDist / fLength);
                vScale.z *= (fNewDist / fLength);
            }
            // SET SCALE
            mObject->SetScale(vScale);
        }
        else if(mMode == GIZMO_ROTATE)
        {
            //auto ret = PickRotRings(ray, out);
        }
    }
    
    void DiTransGizmo::OnMouseMove(int _left, int _top)
    {
        auto ray = GetMouseRay(_left, _top);
        
        if(!DiMath::intersects(ray, GetWorldAABB()).first)
        {
            return;
        }
        
        DiVec3 out(0,0,0);
        if(mMode == GIZMO_MOVE || mMode == GIZMO_SCALE)
        {
            auto ret = mAxes->Pick(ray,out);
            mAxes->Highlight(ret);
        }
        else if(mMode == GIZMO_ROTATE)
        {
            auto ret = PickRotRings(ray, out);
            HightlightRotRings(ret);
        }
    }
    
    void DiTransGizmo::OnMouseDown(int _left, int _top, MyGUI::MouseButton _id)
    {
        if(_id != MyGUI::MouseButton::Left)
            return;
        
        auto ray = GetMouseRay(_left, _top);
        
        if(!DiMath::intersects(ray, GetWorldAABB()).first)
        {
            return;
        }

        DiVec3 out(0,0,0);
        int ret = 0;
        if(mMode == GIZMO_MOVE || mMode == GIZMO_SCALE)
        {
            ret = mAxes->Pick(ray,out);
            if(ret != DiTransAxes::PICK_NONE)
            {
                mLastPos = out;
            }
        }
        else if(mMode == GIZMO_ROTATE)
        {
            ret = PickRotRings(ray, out);
            if(ret != PICK_NONE)
            {
                mLastPos = out;
            }
        }
        
        if (ret != 0)
			mPicking = true;
		else
			mPicking = false;
        
        mLastPickResult = ret;
        mLastDerivedPos = mObject->GetPosition();
        mLastDerivedScale = mObject->GetScale();
        mLastGizmoPlane = FindGizmoPlane(ray, mLastPickResult);
        
        mLast3DDelta = CalculateDeltaTrans(ray, mLastGizmoPlane, mLastPickResult, mLastDerivedPos);
    }
    
    void DiTransGizmo::OnMouseUp(int _left, int _top, MyGUI::MouseButton _id)
    {
        mPicking = false;
        mLastPickResult = 0;
    }
    
    DiPlane DiTransGizmo::FindGizmoPlane(DiRay &pickRay, int TranslationAxis)
    {
        DiQuat qOrient;
		if(DiEditorManager::Get()->IsGizmoInWorldSpace())
			qOrient = DiQuat::IDENTITY;
        else
            qOrient = mBaseNode->GetDerivedOrientation();
        
        auto vPos = mBaseNode->GetDerivedPosition();
        auto camera = DiBase::Driver->GetSceneManager()->GetCamera();
        auto vCamBack = -camera->GetDirection();

        if(!TranslationAxis)
            return DiPlane(vCamBack,vPos);
        
        DiPlane planeX(qOrient.xAxis(), vPos);
        DiPlane planeY(qOrient.yAxis(), vPos);
        DiPlane planeZ(qOrient.zAxis(), vPos);
        
        float vX = planeX.projectVector(pickRay.getDirection()).length();
        float vY = planeY.projectVector(pickRay.getDirection()).length();
        float vZ = planeZ.projectVector(pickRay.getDirection()).length();
        
        if(TranslationAxis & DiTransAxes::PICK_MOVE_X)
            vX = 10000.0f;
        
        if(TranslationAxis & DiTransAxes::PICK_MOVE_Y)
            vY = 10000.0f;
        
        if(TranslationAxis & DiTransAxes::PICK_MOVE_Z)
            vZ = 10000.0f;
        
        if (vX < vY && vX < vZ)
            return planeX;
        else
        {
            if (vY < vX && vY < vZ )
                return planeY;
            else
                return planeZ;
        }
    }
    
    DiVec3 DiTransGizmo::CalculateDeltaTrans(const DiRay& pickRay, const DiPlane& plane, int pickResult, const DiVec3& curPos)
    {
        auto result = pickRay.intersects(plane);
        
        if (result.first)
        {
            DiVec3 AxisX = DiVec3::ZERO;
            DiVec3 AxisY = DiVec3::ZERO;
            DiVec3 AxisZ = DiVec3::ZERO;
            
            DiQuat rot;
            if(DiEditorManager::Get()->IsGizmoInWorldSpace())
                rot = DiQuat::IDENTITY;
            else
                rot = mBaseNode->GetDerivedOrientation();
            
            if(pickResult)
            {
                if((pickResult & DiTransAxes::PICK_MOVE_X) || (pickResult & DiTransAxes::PICK_SCALE_X))
                    AxisX = rot.xAxis();
                if((pickResult & DiTransAxes::PICK_MOVE_Y) || (pickResult & DiTransAxes::PICK_SCALE_Y))
                    AxisY = rot.yAxis();
                if((pickResult & DiTransAxes::PICK_MOVE_Z) || (pickResult & DiTransAxes::PICK_SCALE_Z))
                    AxisZ = rot.zAxis();
            }
            else
            {
                AxisX = rot.xAxis();
                AxisY = rot.yAxis();
                AxisZ = rot.zAxis();
            }
            
            auto Proj = pickRay.getPoint(result.second) - curPos;
            auto vPos1 = (AxisX.dotProduct(Proj) * AxisX);
            auto vPos2 = (AxisY.dotProduct(Proj) * AxisY);
            auto vPos3 = (AxisZ.dotProduct(Proj) * AxisZ);
            auto vPos = vPos1 + vPos2 + vPos3;
            
            mLastTranslationDelta = vPos;
        }
        return mLastTranslationDelta;
    }
    
    void DiTransGizmo::HightlightRotRings(RotatePick pickret)
    {
        mRotatingRingsMat[0]->SetDiffuse(DiColor::Red);
        mRotatingRingsMat[1]->SetDiffuse(DiColor::Green);
        mRotatingRingsMat[2]->SetDiffuse(DiColor::Blue);
        
        if(pickret == PICK_ROT_X)
        {
            mRotatingRingsMat[0]->SetDiffuse(DiColor::Yellow);
        }
        else if(pickret == PICK_ROT_Y)
        {
            mRotatingRingsMat[1]->SetDiffuse(DiColor::Yellow);
        }
        else if(pickret == PICK_ROT_Z)
        {
            mRotatingRingsMat[2]->SetDiffuse(DiColor::Yellow);
        }
        else if(pickret == PICK_ROT_XYZ)
        {
            mRotatingRingsMat[0]->SetDiffuse(DiColor::Yellow);
            mRotatingRingsMat[1]->SetDiffuse(DiColor::Yellow);
            mRotatingRingsMat[2]->SetDiffuse(DiColor::Yellow);
        }
    }
    
    DiTransGizmo::RotatePick DiTransGizmo::PickRotRings(const DiRay& ray, DiVec3& out)
    {
        DiSphere center(mBaseNode->GetDerivedPosition(), 0.6f);
        auto ret = DiMath::intersects(ray, center);
        if(ret.first)
        {
            out = ray.getPoint(ret.second);
            return PICK_ROT_XYZ;
        }
        
        RotatePick pickret[3] = {PICK_ROT_X,PICK_ROT_Y,PICK_ROT_Z};
        for(int i = 0; i < 3; ++i)
        {
            auto mat = mRotateRingNode[i]->GetFullTransform();
            
            DiVector<DiVec3> verts;
            verts.reserve(mRotatingRingsVerts.size());
            
            for (auto& v : mRotatingRingsVerts)
            {
                verts.push_back(mat.transformAffine(v));
            }
            
            for (size_t j = 0; j < verts.size(); j+=3)
            {
                ret = DiMath::intersects(ray,verts[j],verts[j+1],verts[j+2], true, true);
                if(ret.first)
                {
                    out = ray.getPoint(ret.second);
                    return pickret[i];
                }
            }
        }
        
        return PICK_NONE;
    }
    
    void DiTransGizmo::Show(bool visible)
    {
        if(mActive != visible)
        {
            mBaseNode->SetVisible(visible);
            mActive = visible;
            
            if(mActive)
            {
                HideAll();
                SetGizmoMode(mMode);
            }
        }
    }
    
    void DiTransGizmo::GenerateRotRingVerts()
    {
        mRotatingRingsVerts.clear();
        
        float radius = 3.5f;
        float tube = 0.03f;
        int radialSegments = 6;
        int tubularSegments = 32;
        float arc = DiMath::PI;

        DiVector<DiVec3> vbs;
        DiVec3 center(0,0,0);

        for ( int j = 0; j <= radialSegments; j++ )
        {
            for ( int i = 0; i <= tubularSegments; i++ )
            {
                float u = i / (float)tubularSegments * arc;
                float v = j / (float)radialSegments * DiMath::PI * 2;
                
                center.x = radius * DiMath::Cos( u );
                center.y = radius * DiMath::Sin( u );
                
                DiVec3 vertex;
                vertex.x = ( radius + tube * DiMath::Cos( v ) ) * DiMath::Cos( u );
                vertex.y = ( radius + tube * DiMath::Cos( v ) ) * DiMath::Sin( u );
                vertex.z = tube * DiMath::Sin( v );
                
                vbs.push_back(vertex);
            }
        }
        
        for ( uint16 j = 1; j <= radialSegments; j++ )
        {
            for ( uint16 i = 1; i <= tubularSegments; i++ )
            {
                
                uint16 a = ( tubularSegments + 1 ) * j + i - 1;
                uint16 b = ( tubularSegments + 1 ) * ( j - 1 ) + i - 1;
                uint16 c = ( tubularSegments + 1 ) * ( j - 1 ) + i;
                uint16 d = ( tubularSegments + 1 ) * j + i;
                
                mRotatingRingsVerts.push_back(vbs[a]);
                mRotatingRingsVerts.push_back(vbs[b]);
                mRotatingRingsVerts.push_back(vbs[d]);
                mRotatingRingsVerts.push_back(vbs[b]);
                mRotatingRingsVerts.push_back(vbs[c]);
                mRotatingRingsVerts.push_back(vbs[d]);
            }
        }
    }
    
    void DiTransGizmo::Create()
    {
        mBaseNode = mObject->GetSceneNode()->CreateChild();
        
        mAxes = make_shared<DiTransAxes>();
        mAxesNode = mBaseNode->CreateChild();
        mAxesNode->AttachObject(mAxes);
        
        mCircles = make_shared<DiDebugHelper>();
        mCircles->SetFaceToCamera(true);
        mCircles->AddCircle(DiVec3::ZERO, 0.4f, DiColor());
        mCircles->AddCircle(DiVec3::ZERO, 3.5f, DiColor(0.8f,0.8f,0.8f));
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
            mRotatingRingsMat[i] = DiMaterial::QuickCreate("basic_v", "basic_p");
            mRotatingRingsMat[i]->SetDiffuse(cols[i]);
            mRotateRings[i]->SetMaterial(mRotatingRingsMat[i]);
            mRotateRingNode[i] = mBaseNode->CreateChild();
            mRotateRingNode[i]->AttachObject(mRotateRings[i]);
        }
        
        GenerateRotRingVerts();
        HideAll();
    }
}