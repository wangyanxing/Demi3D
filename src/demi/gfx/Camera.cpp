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
#include "Camera.h"
#include "CullNode.h"
#include "SceneManager.h"

namespace Demi 
{

    DiCamera::DiCamera( const DiString& name,DiSceneManager* sm)
        : DiFrustum(name),
        mOrientation(DiQuat::IDENTITY),
        mPosition(DiVec3::ZERO),
        mFillMode(FM_SOLID),
        mWindowSet(false),
        mAutoAspectRatio(false),
        mCullFrustum(0),
        mSceneManager(sm)
    {
        mFOVy = DiRadian(DiMath::PI/4.0f);
        mNearDist = 1.0f;
        mFarDist = 100000.0f;
        mAspect = 1.33333333333333f;
        mProjType = PT_PERSPECTIVE;
        SetFixedYawAxis(true);

        InvalidateFrustum();
        InvalidateView();

        mViewMatrix = DiMat4::ZERO;
        DiMat4 mat;
        mat.setIdentity();
        mParentNode = nullptr;
        mReflect = false;
    }

    DiCamera::DiCamera(DiSceneManager* sm)
        :DiFrustum(),
        mOrientation(DiQuat::IDENTITY),
        mPosition(DiVec3::ZERO),
        mFillMode(FM_SOLID),
        mWindowSet(false),
        mAutoAspectRatio(false),
        mCullFrustum(0),
        mSceneManager(sm)
    {
        mFOVy = DiRadian(DiMath::PI/4.0f);
        mNearDist = 100.0f;
        mFarDist = 100000.0f;
        mAspect = 1.33333333333333f;
        mProjType = PT_PERSPECTIVE;
        SetFixedYawAxis(true);

        InvalidateFrustum();
        InvalidateView();

        mViewMatrix = DiMat4::ZERO;
        DiMat4 mat;
        mat.setIdentity();
        mParentNode = nullptr;
        mReflect = false;
    }

    DiCamera::~DiCamera()
    {
    }

    void DiCamera::SetFillMode(DiFillMode sd)
    {
        mFillMode = sd;
    }

    DiFillMode DiCamera::GetFillMode(void) const
    {
        return mFillMode;
    }
    
    DiCamera* DiCamera::GetSplitFrustum(float nearClip, float farClip) const
    {
        nearClip = DiMath::Max(nearClip, GetNearClipDistance());
        farClip = DiMath::Min(farClip, GetFarClipDistance());
        if (farClip < nearClip)
            farClip = nearClip;

        DiCamera* cam = DI_NEW DiCamera(nullptr);
        cam->CopySettingFrom(this);
        cam->SetNearClipDistance(nearClip);
        cam->SetFarClipDistance(farClip);

        return cam;
    }

    void DiCamera::SetPosition(float x, float y, float z)
    {
        mPosition.x = x;
        mPosition.y = y;
        mPosition.z = z;
        InvalidateView();
    }

    void DiCamera::SetPosition(const DiVec3& vec)
    {
        mPosition = vec;
        InvalidateView();
    }

    const DiVec3& DiCamera::GetPosition(void) const
    {
        return mPosition;
    }

    void DiCamera::Move(const DiVec3& vec)
    {
        mPosition = mPosition + vec;
        InvalidateView();
    }

    void DiCamera::MoveRelative(const DiVec3& vec)
    {
        DiVec3 trans = mOrientation * vec;

        mPosition = mPosition + trans;
        InvalidateView();
    }

    void DiCamera::SetDirection(float x, float y, float z)
    {
        SetDirection(DiVec3(x,y,z));
    }

    void DiCamera::SetDirection(const DiVec3& vec)
    {
        if (vec == DiVec3::ZERO) 
            return;

        DiVec3 zAdjustVec = -vec;
        zAdjustVec.normalise();

        DiQuat targetWorldOrientation;

        if( mYawFixed )
        {
            DiVec3 xVec = mYawFixedAxis.crossProduct( zAdjustVec );
            xVec.normalise();

            DiVec3 yVec = zAdjustVec.crossProduct( xVec );
            yVec.normalise();

            targetWorldOrientation.FromAxes( xVec, yVec, zAdjustVec );
        }
        else
        {

            DiVec3 axes[3];
            updateView();
            mRealOrientation.ToAxes(axes);
            DiQuat rotQuat;
            if ( (axes[2]+zAdjustVec).squaredLength() <  0.00005f) 
            {
                rotQuat.FromAngleAxis(DiRadian(DiMath::PI), axes[1]);
            }
            else
            {
                // Derive shortest arc to DI_NEW direction
                rotQuat = axes[2].getRotationTo(zAdjustVec);

            }
            targetWorldOrientation = rotQuat * mRealOrientation;
        }

        if (mParentNode)
        {
            mOrientation = mParentNode->GetDerivedOrientation().Inverse(
                ) * targetWorldOrientation;
        }
        else
        {
            mOrientation = targetWorldOrientation;
        }

        InvalidateView();
    }

    DiVec3 DiCamera::GetDirection(void) const
    {
        return mOrientation * -DiVec3::UNIT_Z;
    }


    DiVec3 DiCamera::GetUp(void) const
    {
        return mOrientation * DiVec3::UNIT_Y;
    }


    DiVec3 DiCamera::GetRight(void) const
    {
        return mOrientation * DiVec3::UNIT_X;
    }


    void DiCamera::LookAt(const DiVec3& targetPoint)
    {
        updateView();
        this->SetDirection(targetPoint - mRealPosition);
    }


    void DiCamera::LookAt( float x, float y, float z )
    {
        DiVec3 vTemp( x, y, z );
        this->LookAt(vTemp);
    }


    void DiCamera::Roll(const DiRadian& angle)
    {
        DiVec3 zAxis = mOrientation * DiVec3::UNIT_Z;
        Rotate(zAxis, angle);

        InvalidateView();
    }


    void DiCamera::Yaw(const DiRadian& angle)
    {
        DiVec3 yAxis;

        if (mYawFixed)
        {
            yAxis = mYawFixedAxis;
        }
        else
        {
            yAxis = mOrientation * DiVec3::UNIT_Y;
        }

        Rotate(yAxis, angle);

        InvalidateView();
    }

    void DiCamera::Pitch(const DiRadian& angle)
    {
        DiVec3 xAxis = mOrientation * DiVec3::UNIT_X;
        Rotate(xAxis, angle);

        InvalidateView();

    }

    void DiCamera::Rotate(const DiVec3& axis, const DiRadian& angle)
    {
        DiQuat q;
        q.FromAngleAxis(angle,axis);
        Rotate(q);
    }

    void DiCamera::Rotate(const DiQuat& q)
    {
        DiQuat qnorm = q;
        qnorm.normalise();
        mOrientation = qnorm * mOrientation;

        InvalidateView();
    }

    bool DiCamera::IsViewOutOfDate(void) const
    {
        if (mParentNode)
        {
            if (mRecalcView ||
                mParentNode->GetDerivedOrientation() != mLastParentOrientation ||
                mParentNode->GetDerivedPosition() != mLastParentPosition)
            {
                mLastParentOrientation = mParentNode->GetDerivedOrientation();
                mLastParentPosition = mParentNode->GetDerivedPosition();
                mRealOrientation = mLastParentOrientation * mOrientation;
                mRealPosition = (mLastParentOrientation * mPosition) + mLastParentPosition;
                mRecalcView = true;
                mRecalcWindow = true;
            }
        }
        else
        {
            mRealOrientation = mOrientation;
            mRealPosition = mPosition;
        }

        // Deriving reflected orientation / position
        if (mRecalcView)
        {
            if (mReflect)
            {
                // Calculate reflected orientation, use up-vector as fallback axis. 
                DiVec3 dir = mRealOrientation * DiVec3::NEGATIVE_UNIT_Z;
                DiVec3 rdir = dir.reflect(mReflectPlane.normal);
                DiVec3 up = mRealOrientation * DiVec3::UNIT_Y;
                mDerivedOrientation = dir.getRotationTo(rdir, up) * mRealOrientation;

                // Calculate reflected position.
                mDerivedPosition = mReflectMatrix.transformAffine(mRealPosition);
            }
            else
            {
                mDerivedOrientation = mRealOrientation;
                mDerivedPosition = mRealPosition;
            }
        }

        return mRecalcView;

    }

    void DiCamera::InvalidateView() const
    {
        mRecalcWindow = true;
        DiFrustum::InvalidateView();
    }

    void DiCamera::InvalidateFrustum(void) const
    {
        mRecalcWindow = true;
        DiFrustum::InvalidateFrustum();
    }

    void DiCamera::SetFixedYawAxis(bool useFixed, const DiVec3& fixedAxis)
    {
        mYawFixed = useFixed;
        mYawFixedAxis = fixedAxis;
    }

    const DiQuat& DiCamera::GetOrientation(void) const
    {
        return mOrientation;
    }

    void DiCamera::SetOrientation(const DiQuat& q)
    {
        mOrientation = q;
        mOrientation.normalise();
        InvalidateView();
    }

    const DiQuat& DiCamera::GetDerivedOrientation(void) const
    {
        updateView();
        return mDerivedOrientation;
    }

    const DiVec3& DiCamera::GetDerivedPosition(void) const
    {
        updateView();
        return mDerivedPosition;
    }

    DiVec3 DiCamera::GetDerivedDirection(void) const
    {
        updateView();
        return mDerivedOrientation * DiVec3::NEGATIVE_UNIT_Z;
    }

    DiVec3 DiCamera::GetDerivedUp(void) const
    {
        updateView();
        return mDerivedOrientation * DiVec3::UNIT_Y;
    }

    DiVec3 DiCamera::GetDerivedRight(void) const
    {
        updateView();
        return mDerivedOrientation * DiVec3::UNIT_X;
    }

    const DiQuat& DiCamera::GetRealOrientation(void) const
    {
        updateView();
        return mRealOrientation;
    }

    const DiVec3& DiCamera::GetRealPosition(void) const
    {
        updateView();
        return mRealPosition;
    }

    DiVec3 DiCamera::GetRealDirection(void) const
    {
        updateView();
        return mRealOrientation * DiVec3::NEGATIVE_UNIT_Z;
    }

    DiVec3 DiCamera::GetRealUp(void) const
    {
        updateView();
        return mRealOrientation * DiVec3::UNIT_Y;
    }

    DiVec3 DiCamera::GetRealRight(void) const
    {
        updateView();
        return mRealOrientation * DiVec3::UNIT_X;
    }

    void DiCamera::GetWorldTransforms(DiMat4* mat) const 
    {
        updateView();

        DiVec3 scale(1.0, 1.0, 1.0);
        if (mParentNode)
        {
            scale = mParentNode->GetDerivedScale();
        }

        mat->makeTransform(
            mDerivedPosition,
            scale,
            mDerivedOrientation);
    }

    DiRay DiCamera::GetCameraToViewportRay(float screenX, float screenY) const
    {
        DiRay ret;
        GetCameraToViewportRay(screenX, screenY, &ret);
        return ret;
    }

    void DiCamera::GetCameraToViewportRay(float screenX, float screenY, DiRay* outRay) const
    {
        DiMat4 inverseVP = (GetProjectionMatrix() * GetViewMatrix()).inverse();

        float nx = (2.0f * screenX) - 1.0f;
        float ny = 1.0f - (2.0f * screenY);
        DiVec3 nearPoint(nx, ny, -1.f);
        DiVec3 midPoint (nx, ny,  0.0f);

        DiVec3 rayOrigin, rayTarget;

        rayOrigin = inverseVP * nearPoint;
        rayTarget = inverseVP * midPoint;

        DiVec3 rayDirection = rayTarget - rayOrigin;
        rayDirection.normalise();

        outRay->setOrigin(rayOrigin);
        outRay->setDirection(rayDirection);
    } 

    DiPlaneBoundedVol DiCamera::GetCameraToViewportBoxVolume(float screenLeft, 
        float screenTop, float screenRight, float screenBottom, bool includeFarPlane)
    {
        DiPlaneBoundedVol vol;
        GetCameraToViewportBoxVolume(screenLeft, screenTop, screenRight, screenBottom, 
            &vol, includeFarPlane);
        return vol;
    }

    void DiCamera::GetCameraToViewportBoxVolume(float screenLeft, 
        float screenTop, float screenRight, float screenBottom, 
        DiPlaneBoundedVol* outVolume, bool includeFarPlane)
    {
        outVolume->planes.clear();

        if (mProjType == PT_PERSPECTIVE)
        {
            DiRay ul = GetCameraToViewportRay(screenLeft, screenTop);
            DiRay ur = GetCameraToViewportRay(screenRight, screenTop);
            DiRay bl = GetCameraToViewportRay(screenLeft, screenBottom);
            DiRay br = GetCameraToViewportRay(screenRight, screenBottom);

            DiVec3 normal;
            // top plane
            normal = ul.getDirection().crossProduct(ur.getDirection());
            normal.normalise();
            outVolume->planes.push_back(
                DiPlane(normal, GetDerivedPosition()));

            // right plane
            normal = ur.getDirection().crossProduct(br.getDirection());
            normal.normalise();
            outVolume->planes.push_back(
                DiPlane(normal, GetDerivedPosition()));

            // bottom plane
            normal = br.getDirection().crossProduct(bl.getDirection());
            normal.normalise();
            outVolume->planes.push_back(
                DiPlane(normal, GetDerivedPosition()));

            // left plane
            normal = bl.getDirection().crossProduct(ul.getDirection());
            normal.normalise();
            outVolume->planes.push_back(
                DiPlane(normal, GetDerivedPosition()));

        }
        else
        {
            DiRay ul = GetCameraToViewportRay(screenLeft, screenTop);
            DiRay br = GetCameraToViewportRay(screenRight, screenBottom);

            updateFrustumPlanes();
            outVolume->planes.push_back(
                DiPlane(mFrustumPlanes[FRUSTUM_PLANE_TOP].normal, ul.getOrigin()));
            outVolume->planes.push_back(
                DiPlane(mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal, br.getOrigin()));
            outVolume->planes.push_back(
                DiPlane(mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal, br.getOrigin()));
            outVolume->planes.push_back(
                DiPlane(mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal, ul.getOrigin()));


        }

        outVolume->planes.push_back(GetFrustumPlane(FRUSTUM_PLANE_NEAR));
        if (includeFarPlane)
        {
            outVolume->planes.push_back(GetFrustumPlane(FRUSTUM_PLANE_FAR));
        }
    }

    void DiCamera::SetWindow (float Left, float Top, float Right, float Bottom)
    {
        mWLeft = Left;
        mWTop = Top;
        mWRight = Right;
        mWBottom = Bottom;

        mWindowSet = true;
        mRecalcWindow = true;
    }

    void DiCamera::ResetWindow ()
    {
        mWindowSet = false;
    }

    void DiCamera::SetWindowImpl() const
    {
        if (!mWindowSet || !mRecalcWindow)
        {
            return;
        }

        float vpLeft, vpRight, vpBottom, vpTop;
        calcProjectionParameters(vpLeft, vpRight, vpBottom, vpTop);

        float vpWidth = vpRight - vpLeft;
        float vpHeight = vpTop - vpBottom;

        float wvpLeft   = vpLeft + mWLeft * vpWidth;
        float wvpRight  = vpLeft + mWRight * vpWidth;
        float wvpTop    = vpTop - mWTop * vpHeight;
        float wvpBottom = vpTop - mWBottom * vpHeight;

        DiVec3 vp_ul (wvpLeft, wvpTop, -mNearDist);
        DiVec3 vp_ur (wvpRight, wvpTop, -mNearDist);
        DiVec3 vp_bl (wvpLeft, wvpBottom, -mNearDist);
        DiVec3 vp_br (wvpRight, wvpBottom, -mNearDist);

        DiMat4 inv = mViewMatrix.inverseAffine();

        DiVec3 vw_ul = inv.transformAffine(vp_ul);
        DiVec3 vw_ur = inv.transformAffine(vp_ur);
        DiVec3 vw_bl = inv.transformAffine(vp_bl);
        DiVec3 vw_br = inv.transformAffine(vp_br);

        mWindowClipPlanes.clear();
        if (mProjType == PT_PERSPECTIVE)
        {
            DiVec3 position = GetPositionForViewUpdate();
            mWindowClipPlanes.push_back(DiPlane(position, vw_bl, vw_ul));
            mWindowClipPlanes.push_back(DiPlane(position, vw_ul, vw_ur));
            mWindowClipPlanes.push_back(DiPlane(position, vw_ur, vw_br));
            mWindowClipPlanes.push_back(DiPlane(position, vw_br, vw_bl));
        }
        else
        {
            DiVec3 x_axis(inv[0][0], inv[0][1], inv[0][2]);
            DiVec3 y_axis(inv[1][0], inv[1][1], inv[1][2]);
            x_axis.normalise();
            y_axis.normalise();
            mWindowClipPlanes.push_back(DiPlane( x_axis, vw_bl));
            mWindowClipPlanes.push_back(DiPlane(-x_axis, vw_ur));
            mWindowClipPlanes.push_back(DiPlane( y_axis, vw_bl));
            mWindowClipPlanes.push_back(DiPlane(-y_axis, vw_ur));
        }

        mRecalcWindow = false;

    }

    const DiVector<DiPlane>& DiCamera::GetWindowPlanes(void) const
    {
        updateView();
        SetWindowImpl();
        return mWindowClipPlanes;
    }

    const DiVec3& DiCamera::GetPositionForViewUpdate(void) const
    {
        return mRealPosition;
    }

    const DiQuat& DiCamera::GetOrientationForViewUpdate(void) const
    {
        return mRealOrientation;
    }

    bool DiCamera::GetAutoAspectRatio(void) const
    {
        return mAutoAspectRatio;
    }

    void DiCamera::SetAutoAspectRatio(bool autoratio)
    {
        mAutoAspectRatio = autoratio;
    }

    bool DiCamera::IsVisible(const DiAABB& bound, DiFrustumPlane* culledBy) const
    {
        if (mCullFrustum)
            return mCullFrustum->IsVisible(bound, culledBy);
        else
            return DiFrustum::IsVisible(bound, culledBy);
    }

    bool DiCamera::IsVisible(const DiSphere& bound, DiFrustumPlane* culledBy) const
    {
        if (mCullFrustum)
            return mCullFrustum->IsVisible(bound, culledBy);
        else
            return DiFrustum::IsVisible(bound, culledBy);
    }

    bool DiCamera::IsVisible(const DiVec3& vert, DiFrustumPlane* culledBy) const
    {
        if (mCullFrustum)
            return mCullFrustum->IsVisible(vert, culledBy);
        else
            return DiFrustum::IsVisible(vert, culledBy);
    }

    const DiVec3* DiCamera::GetWorldSpaceCorners(void) const
    {
        if (mCullFrustum)
            return mCullFrustum->GetWorldSpaceCorners();
        else
            return DiFrustum::GetWorldSpaceCorners();
    }

    const DiPlane& DiCamera::GetFrustumPlane( unsigned short plane ) const
    {
        if (mCullFrustum)
            return mCullFrustum->GetFrustumPlane(plane);
        else
            return DiFrustum::GetFrustumPlane(plane);
    }

    bool DiCamera::ProjectSphere(const DiSphere& sphere, 
        float* left, float* top, float* right, float* bottom) const
    {
        if (mCullFrustum)
            return mCullFrustum->ProjectSphere(sphere, left, top, right, bottom);
        else
            return DiFrustum::ProjectSphere(sphere, left, top, right, bottom);
    }

    float DiCamera::GetNearClipDistance(void) const
    {
        if (mCullFrustum)
            return mCullFrustum->GetNearClipDistance();
        else
            return DiFrustum::GetNearClipDistance();
    }

    float DiCamera::GetFarClipDistance(void) const
    {
        if (mCullFrustum)
            return mCullFrustum->GetFarClipDistance();
        else
            return DiFrustum::GetFarClipDistance();
    }

    const DiMat4& DiCamera::GetViewMatrix(void) const
    {
        if (mCullFrustum)
            return mCullFrustum->GetViewMatrix();
        else
            return DiFrustum::GetViewMatrix();
    }

    const DiMat4& DiCamera::GetViewMatrix(bool ownFrustumOnly) const
    {
        if (ownFrustumOnly)
            return DiFrustum::GetViewMatrix();
        else
            return GetViewMatrix();
    }

    void DiCamera::CopySettingFrom(const DiCamera* cam)
    {
        DiFrustum::CopySettingFrom(this);
        this->SetPosition(cam->GetPosition());
        this->SetOrientation(cam->GetOrientation());
    }

    DiCamera::Visibility DiCamera::GetVisibility( const DiAABB &bound )
    {
        if ( bound.IsNull() )
            return NONE;

        DiVec3 centre = bound.GetCenter();
        DiVec3 halfSize = bound.GetHalfSize();

        bool all_inside = true;

        for ( int plane = 0; plane < 6; ++plane )
        {
            if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
                continue;

            DiPlane::Side side = GetFrustumPlane(plane).getSide(centre, halfSize);
            if(side == DiPlane::NEGATIVE_SIDE)
                return NONE;
            if(side == DiPlane::BOTH_SIDE) 
                all_inside = false;
        }

        if ( all_inside )
            return FULL;
        else
            return PARTIAL;
    }

    void DiCamera::ForwardIntersect( const DiPlane& worldPlane, DiVector<DiVec4>* intersect3d ) const
    {
        if(!intersect3d)
        {
            return;
        }

        DiVec3 trCorner = GetWorldSpaceCorners()[0];
        DiVec3 tlCorner = GetWorldSpaceCorners()[1];
        DiVec3 blCorner = GetWorldSpaceCorners()[2];
        DiVec3 brCorner = GetWorldSpaceCorners()[3];

        DiPlane pval = worldPlane;
        if(pval.normal.z < 0.0)
        {
            pval.normal *= -1.0;
            pval.d *= -1.0;
        }
        DiQuat invPlaneRot = pval.normal.getRotationTo(DiVec3::UNIT_Z);

        DiVec3 lPos = invPlaneRot * GetDerivedPosition();
        DiVec3 vec[4];
        vec[0] = invPlaneRot * trCorner - lPos;
        vec[1] = invPlaneRot * tlCorner - lPos; 
        vec[2] = invPlaneRot * blCorner - lPos; 
        vec[3] = invPlaneRot * brCorner - lPos; 

        DiVector<DiVec4> iPnt = GetRayForwardIntersect(lPos, vec, -pval.d);

        if(intersect3d) 
        {
            DiQuat planeRot = invPlaneRot.Inverse();
            (*intersect3d).clear();
            for(unsigned int i=0; i<iPnt.size(); i++)
            {
                DiVec3 intersection = planeRot * DiVec3(iPnt[i].x, iPnt[i].y, iPnt[i].z);
                (*intersect3d).push_back(DiVec4(intersection.x, intersection.y, intersection.z, iPnt[i].w));
            }
        }
    }

    DiVector<DiVec4> DiCamera::GetRayForwardIntersect( const DiVec3& anchor, const DiVec3 *dir, float planeOffset ) const
    {
        DiVector<DiVec4> res;

        if(!dir)
        {
            return res;
        }

        int infpt[4] = {0, 0, 0, 0}; // 0=finite, 1=infinite, 2=straddles infinity
        DiVec3 vec[4];

        // find how much the anchor point must be displaced in the plane's
        // constant variable
        float delta = planeOffset - anchor.z;

        // now set the intersection point and note whether it is a 
        // point at infinity or straddles infinity
        unsigned int i;
        for (i=0; i<4; i++)
        {
            float test = dir[i].z * delta;
            if (test == 0.0) 
            {
                vec[i] = dir[i];
                infpt[i] = 1;
            }
            else 
            {
                float lambda = delta / dir[i].z;
                vec[i] = anchor + (lambda * dir[i]);
                if(test < 0.0)
                {
                    infpt[i] = 2;
                }
            }
        }

        for (i=0; i<4; i++)
        {
            // store the finite intersection points
            if (infpt[i] == 0)
                res.push_back(DiVec4(vec[i].x, vec[i].y, vec[i].z, 1.0));
            else
            {
                // handle the infinite points of intersection;
                // cases split up into the possible frustum planes 
                // pieces which may contain a finite intersection point
                int nextind = (i+1) % 4;
                int prevind = (i+3) % 4;
                if ((infpt[prevind] == 0) || (infpt[nextind] == 0))
                {
                    if (infpt[i] == 1)
                        res.push_back(DiVec4(vec[i].x, vec[i].y, vec[i].z, 0.0));
                    else
                    {
                        // handle the intersection points that straddle infinity (back-project)
                        if(infpt[prevind] == 0) 
                        {
                            DiVec3 temp = vec[prevind] - vec[i];
                            res.push_back(DiVec4(temp.x, temp.y, temp.z, 0.0));
                        }
                        if(infpt[nextind] == 0)
                        {
                            DiVec3 temp = vec[nextind] - vec[i];
                            res.push_back(DiVec4(temp.x, temp.y, temp.z, 0.0));
                        }
                    }
                } // end if we need to add an intersection point to the list
            } // end if infinite point needs to be considered
        } // end loop over frustun corners

        // we end up with either 0, 3, 4, or 5 intersection points

        return res;
    }
}