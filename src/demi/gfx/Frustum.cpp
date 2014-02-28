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
#include "Frustum.h"
#include "sphere.h"
#include "CullNode.h"
#include "GfxDriver.h"

namespace Demi 
{

    const float DiFrustum::INFINITE_FAR_PLANE_ADJUST = 0.00001f;

    DiFrustum::DiFrustum(const DiString& name) : 
        DiTransformUnit(name),
        mProjType(PT_PERSPECTIVE), 
        mFOVy(DiRadian(DiMath::PI/4.0f)), 
        mFarDist(5000.0f), 
        mNearDist(0.1f), 
        mAspect(1.33333333333333f), 
        mOrthoHeight(1000),
        mFrustumOffset(DiVec2::ZERO),
        mFocalLength(1.0f),
        mLastParentOrientation(DiQuat::IDENTITY),
        mLastParentPosition(DiVec3::ZERO),
        mRecalcFrustum(true), 
        mRecalcView(true), 
        mRecalcFrustumPlanes(true),
        mRecalcWorldSpaceCorners(true),
        mRecalcVertexData(true),
        mCustomViewMatrix(false),
        mCustomProjMatrix(false),
        mFrustumExtentsManuallySet(false),
        mOrientationMode(OR_DEGREE_0),
        mReflect(false), 
        mObliqueDepthProjection(false)
    {
        updateView();
        updateFrustum();
    }

    DiFrustum::DiFrustum() :
        DiTransformUnit(),
        mProjType(PT_PERSPECTIVE), 
        mFOVy(DiRadian(DiMath::PI/4.0f)), 
        mFarDist(10000.0f), 
        mNearDist(0.1f), 
        mAspect(1.33333333333333f), 
        mOrthoHeight(1000),
        mFrustumOffset(DiVec2::ZERO),
        mFocalLength(1.0f),
        mLastParentOrientation(DiQuat::IDENTITY),
        mLastParentPosition(DiVec3::ZERO),
        mRecalcFrustum(true), 
        mRecalcView(true), 
        mRecalcFrustumPlanes(true),
        mRecalcWorldSpaceCorners(true),
        mRecalcVertexData(true),
        mCustomViewMatrix(false),
        mCustomProjMatrix(false),
        mFrustumExtentsManuallySet(false),
        mOrientationMode(OR_DEGREE_0),
        mReflect(false), 
        mObliqueDepthProjection(false)
    {
        updateView();
        updateFrustum();
    }

    DiFrustum::~DiFrustum()
    {
    }

    void DiFrustum::SetFOVy(const DiRadian& fov)
    {
        mFOVy = fov;
        InvalidateFrustum();
    }

    const DiRadian& DiFrustum::GetFOVy(void) const
    {
        return mFOVy;
    }

    void DiFrustum::SetFarClipDistance(float farPlane)
    {
        mFarDist = farPlane;
        InvalidateFrustum();
    }

    float DiFrustum::GetFarClipDistance(void) const
    {
        return mFarDist;
    }

    void DiFrustum::SetNearClipDistance(float nearPlane)
    {
//        DI_ASSERT(nearPlane>0);
        mNearDist = nearPlane;
        InvalidateFrustum();
    }

    float DiFrustum::GetNearClipDistance(void) const
    {
        return mNearDist;
    }

    void DiFrustum::SetFrustumOffset(const DiVec2& offset)
    {
        mFrustumOffset = offset;
        InvalidateFrustum();
    }

    void DiFrustum::SetFrustumOffset(float horizontal, float vertical)
    {
        SetFrustumOffset(DiVec2(horizontal, vertical));
    }

    const DiVec2& DiFrustum::GetFrustumOffset() const
    {
        return mFrustumOffset;
    }

    void DiFrustum::SetFocalLength(float focalLength)
    {
        DI_ASSERT(focalLength>0);

        mFocalLength = focalLength;
        InvalidateFrustum();
    }

    float DiFrustum::GetFocalLength() const
    {
        return mFocalLength;
    }

    const DiMat4& DiFrustum::GetProjectionMatrix(void) const
    {
        updateFrustum();

        return mProjMatrix;
    }

    void DiFrustum::GetProjectionMatrixRs(DiMat4& out) const
    {
        updateFrustum();
        Driver->ConvertProjectionMatrix(mProjMatrix,out);
    }

    const DiMat4& DiFrustum::GetViewMatrix(void) const
    {
        updateView();

        return mViewMatrix;
    }

    const DiPlane* DiFrustum::GetFrustumPlanes(void) const
    {
        updateFrustumPlanes();

        return mFrustumPlanes;
    }

    const DiPlane& DiFrustum::GetFrustumPlane(unsigned short plane) const
    {
        updateFrustumPlanes();

        return mFrustumPlanes[plane];
    }

    bool DiFrustum::IsVisible(const DiAABB& bound, DiFrustumPlane* culledBy) const
    {
        if (bound.IsNull()) 
        {
            return false;
        }

        if (bound.IsInfinite()) return true;

        updateFrustumPlanes();

        DiVec3 centre = bound.GetCenter();
        DiVec3 halfSize = bound.GetHalfSize();

        for (int plane = 0; plane < 6; ++plane)
        {
            if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
            {
                continue;
            }

            DiPlane::Side side = mFrustumPlanes[plane].getSide(centre, halfSize);
            if (side == DiPlane::NEGATIVE_SIDE)
            {
                if (culledBy)
                {
                    *culledBy = (DiFrustumPlane)plane;
                }
                return false;
            }
        }

        return true;
    }

    bool DiFrustum::IsVisible(const DiVec3& vert, DiFrustumPlane* culledBy) const
    {
        updateFrustumPlanes();

        for (int plane = 0; plane < 6; ++plane)
        {
            if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
            {
                continue;
            }

            if (mFrustumPlanes[plane].getSide(vert) == DiPlane::NEGATIVE_SIDE)
            {
                if (culledBy)
                {
                    *culledBy = (DiFrustumPlane)plane;
                }
                return false;
            }

        }

        return true;
    }

    bool DiFrustum::IsVisible(const DiSphere& sphere, DiFrustumPlane* culledBy) const
    {
        updateFrustumPlanes();

        for (int plane = 0; plane < 6; ++plane)
        {
            if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
            {
                continue;
            }

            if (mFrustumPlanes[plane].getDistance(sphere.getCenter()) < -sphere.getRadius())
            {
                if (culledBy)
                {
                    *culledBy = (DiFrustumPlane)plane;
                }
                return false;
            }

        }

        return true;
    }

    void DiFrustum::calcProjectionParameters(float& left, float& right, float& bottom, float& top) const
    { 
        if (mCustomProjMatrix)
        {
            DiMat4 invProj = mProjMatrix.inverse();
            DiVec3 topLeft(-0.5f, 0.5f, 0.0f);
            DiVec3 bottomRight(0.5f, -0.5f, 0.0f);

            topLeft = invProj * topLeft;
            bottomRight = invProj * bottomRight;

            left = topLeft.x;
            top = topLeft.y;
            right = bottomRight.x;
            bottom = bottomRight.y;
        }
        else
        {
            if (mFrustumExtentsManuallySet)
            {
                left = mLeft;
                right = mRight;
                top = mTop;
                bottom = mBottom;
            }
            else if (mProjType == PT_PERSPECTIVE)
            {
                DiRadian thetaY (mFOVy * 0.5f);
                float tanThetaY = DiMath::Tan(thetaY);
                float tanThetaX = tanThetaY * mAspect;

                float nearFocal = mNearDist / mFocalLength;
                float nearOffsetX = mFrustumOffset.x * nearFocal;
                float nearOffsetY = mFrustumOffset.y * nearFocal;
                float half_w = tanThetaX * mNearDist;
                float half_h = tanThetaY * mNearDist;

                left   = - half_w + nearOffsetX;
                right  = + half_w + nearOffsetX;
                bottom = - half_h + nearOffsetY;
                top    = + half_h + nearOffsetY;

                mLeft = left;
                mRight = right;
                mTop = top;
                mBottom = bottom;
            }
            else
            {
                left = mLeft;
                right = mRight;
                top = mTop;
                bottom = mBottom;
            }
        }
    }

    void DiFrustum::updateFrustumImpl(void) const
    {
        float left, right, bottom, top;

        calcProjectionParameters(left, right, bottom, top);

        if (!mCustomProjMatrix)
        {

            // The code below will dealing with general projection 
            // parameters, similar glFrustum and glOrtho.
            // Doesn't optimise manually except division operator, so the 
            // code more self-explaining.

            float inv_w = 1 / (right - left);
            float inv_h = 1 / (top - bottom);
            float inv_d = 1 / (mFarDist - mNearDist);

            // Recalc if frustum params changed
            if (mProjType == PT_PERSPECTIVE)
            {
                // Calc matrix elements
                float A = 2 * mNearDist * inv_w;
                float B = 2 * mNearDist * inv_h;
                float C = (right + left) * inv_w;
                float D = (top + bottom) * inv_h;
                float q, qn;
                if (mFarDist == 0)
                {
                    // Infinite far plane
                    q = DiFrustum::INFINITE_FAR_PLANE_ADJUST - 1;
                    qn = mNearDist * (DiFrustum::INFINITE_FAR_PLANE_ADJUST - 2);
                }
                else
                {
                    q = - (mFarDist + mNearDist) * inv_d;
                    qn = -2 * (mFarDist * mNearDist) * inv_d;
                }

                // NB: This creates 'uniform' perspective projection matrix,
                // which depth range [-1,1], right-handed rules
                //
                // [ A   0   C   0  ]
                // [ 0   B   D   0  ]
                // [ 0   0   q   qn ]
                // [ 0   0   -1  0  ]
                //
                // A = 2 * near / (right - left)
                // B = 2 * near / (top - bottom)
                // C = (right + left) / (right - left)
                // D = (top + bottom) / (top - bottom)
                // q = - (far + near) / (far - near)
                // qn = - 2 * (far * near) / (far - near)

                mProjMatrix = DiMat4::ZERO;
                mProjMatrix[0][0] = A;
                mProjMatrix[0][2] = C;
                mProjMatrix[1][1] = B;
                mProjMatrix[1][2] = D;
                mProjMatrix[2][2] = q;
                mProjMatrix[2][3] = qn;
                mProjMatrix[3][2] = -1;

                if (mObliqueDepthProjection)
                {
                    updateView();
                    DiPlane plane = mViewMatrix * mObliqueProjPlane;

                    DiVec4 qVec;
                    qVec.x = (DiMath::Sign(plane.normal.x) + mProjMatrix[0][2]) / mProjMatrix[0][0];
                    qVec.y = (DiMath::Sign(plane.normal.y) + mProjMatrix[1][2]) / mProjMatrix[1][1];
                    qVec.z = -1;
                    qVec.w = (1 + mProjMatrix[2][2]) / mProjMatrix[2][3];

                    // Calculate the scaled plane vector
                    DiVec4 clipPlane4d(plane.normal.x, plane.normal.y, plane.normal.z, plane.d);
                    DiVec4 c = clipPlane4d * (2 / (clipPlane4d.dotProduct(qVec)));

                    // Replace the third row of the projection matrix
                    mProjMatrix[2][0] = c.x;
                    mProjMatrix[2][1] = c.y;
                    mProjMatrix[2][2] = c.z + 1;
                    mProjMatrix[2][3] = c.w; 
                }
            } // perspective
            else if (mProjType == PT_ORTHOGRAPHIC)
            {
                float A = 2 * inv_w;
                float B = 2 * inv_h;
                float C = - (right + left) * inv_w;
                float D = - (top + bottom) * inv_h;
                float q, qn;
                if (mFarDist == 0)
                {
                    // Can not do infinite far plane here, avoid divided zero only
                    q = - DiFrustum::INFINITE_FAR_PLANE_ADJUST / mNearDist;
                    qn = - DiFrustum::INFINITE_FAR_PLANE_ADJUST - 1;
                }
                else
                {
                    q = - 2 * inv_d;
                    qn = - (mFarDist + mNearDist)  * inv_d;
                }

                // NB: This creates 'uniform' orthographic projection matrix,
                // which depth range [-1,1], right-handed rules
                //
                // [ A   0   0   C  ]
                // [ 0   B   0   D  ]
                // [ 0   0   q   qn ]
                // [ 0   0   0   1  ]
                //
                // A = 2 * / (right - left)
                // B = 2 * / (top - bottom)
                // C = - (right + left) / (right - left)
                // D = - (top + bottom) / (top - bottom)
                // q = - 2 / (far - near)
                // qn = - (far + near) / (far - near)

                mProjMatrix = DiMat4::ZERO;
                mProjMatrix[0][0] = A;
                mProjMatrix[0][3] = C;
                mProjMatrix[1][1] = B;
                mProjMatrix[1][3] = D;
                mProjMatrix[2][2] = q;
                mProjMatrix[2][3] = qn;
                mProjMatrix[3][3] = 1;
            } // ortho            
        } // !mCustomProjMatrix

        // Calculate bounding box (local)
        // Box is from 0, down -Z, max dimensions as determined from far plane
        // If infinite view frustum just pick a far value
        float farDist = (mFarDist == 0) ? 100000 : mFarDist;
        // Near plane bounds
        DiVec3 vmin(left, bottom, -farDist);
        DiVec3 vmax(right, top, 0);

        if (mCustomProjMatrix)
        {
            // Some custom projection matrices can have unusual inverted settings
            // So make sure the AABB is the right way around to start with
            DiVec3 tmp = vmin;
            vmin.makeFloor(vmax);
            vmax.makeCeil(tmp);
        }

        if (mProjType == PT_PERSPECTIVE)
        {
            // Merge with far plane bounds
            float radio = farDist / mNearDist;
            vmin.makeFloor(DiVec3(left * radio, bottom * radio, -farDist));
            vmax.makeCeil(DiVec3(right * radio, top * radio, 0));
        }
        mBoundingBox.SetExtents(vmin, vmax);

        mRecalcFrustum = false;

        // Signal to update frustum clipping planes
        mRecalcFrustumPlanes = true;
    }

    void DiFrustum::updateFrustum(void) const
    {
        if (isFrustumOutOfDate())
            updateFrustumImpl();
    }

    bool DiFrustum::IsViewOutOfDate(void) const
    {
        if (mParentNode)
        {
            if (mRecalcView ||
                mParentNode->GetDerivedOrientation() != mLastParentOrientation ||
                mParentNode->GetDerivedPosition() != mLastParentPosition)
            {
                mLastParentOrientation = mParentNode->GetDerivedOrientation();
                mLastParentPosition = mParentNode->GetDerivedPosition();
                mRecalcView = true;
            }
        }
        return mRecalcView;
    }

    bool DiFrustum::isFrustumOutOfDate(void) const
    {
        if (mObliqueDepthProjection)
        {
            if (IsViewOutOfDate())
                mRecalcFrustum = true;
        }

        return mRecalcFrustum;
    }

    void DiFrustum::updateViewImpl(void) const
    {
        if (!mCustomViewMatrix)
        {
            DiMat3 rot;
            const DiQuat& orientation = GetOrientationForViewUpdate();
            const DiVec3& position = GetPositionForViewUpdate();

            mViewMatrix = DiMath::makeViewMatrix(position, orientation, mReflect? &mReflectMatrix : 0);
        }

        mRecalcView = false;
        mRecalcFrustumPlanes = true;
        mRecalcWorldSpaceCorners = true;

        if (mObliqueDepthProjection)
        {
            mRecalcFrustum = true;
        }
    }

    void DiFrustum::CalcViewMatrixRelative(const DiVec3& relPos, DiMat4& matToUpdate) const
    {
        DiMat4 matTrans = DiMat4::IDENTITY;
        matTrans.setTrans(relPos);
        matToUpdate = GetViewMatrix() * matTrans;

    }

    void DiFrustum::updateView(void) const
    {
        if (IsViewOutOfDate())
            updateViewImpl();
    }

    void DiFrustum::updateFrustumPlanesImpl(void) const
    {
        DiMat4 combo = mProjMatrix * mViewMatrix;

        mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal.x = combo[3][0] + combo[0][0];
        mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal.y = combo[3][1] + combo[0][1];
        mFrustumPlanes[FRUSTUM_PLANE_LEFT].normal.z = combo[3][2] + combo[0][2];
        mFrustumPlanes[FRUSTUM_PLANE_LEFT].d = combo[3][3] + combo[0][3];

        mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal.x = combo[3][0] - combo[0][0];
        mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal.y = combo[3][1] - combo[0][1];
        mFrustumPlanes[FRUSTUM_PLANE_RIGHT].normal.z = combo[3][2] - combo[0][2];
        mFrustumPlanes[FRUSTUM_PLANE_RIGHT].d = combo[3][3] - combo[0][3];

        mFrustumPlanes[FRUSTUM_PLANE_TOP].normal.x = combo[3][0] - combo[1][0];
        mFrustumPlanes[FRUSTUM_PLANE_TOP].normal.y = combo[3][1] - combo[1][1];
        mFrustumPlanes[FRUSTUM_PLANE_TOP].normal.z = combo[3][2] - combo[1][2];
        mFrustumPlanes[FRUSTUM_PLANE_TOP].d = combo[3][3] - combo[1][3];

        mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.x = combo[3][0] + combo[1][0];
        mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.y = combo[3][1] + combo[1][1];
        mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].normal.z = combo[3][2] + combo[1][2];
        mFrustumPlanes[FRUSTUM_PLANE_BOTTOM].d = combo[3][3] + combo[1][3];

        mFrustumPlanes[FRUSTUM_PLANE_NEAR].normal.x = combo[3][0] + combo[2][0];
        mFrustumPlanes[FRUSTUM_PLANE_NEAR].normal.y = combo[3][1] + combo[2][1];
        mFrustumPlanes[FRUSTUM_PLANE_NEAR].normal.z = combo[3][2] + combo[2][2];
        mFrustumPlanes[FRUSTUM_PLANE_NEAR].d = combo[3][3] + combo[2][3];

        mFrustumPlanes[FRUSTUM_PLANE_FAR].normal.x = combo[3][0] - combo[2][0];
        mFrustumPlanes[FRUSTUM_PLANE_FAR].normal.y = combo[3][1] - combo[2][1];
        mFrustumPlanes[FRUSTUM_PLANE_FAR].normal.z = combo[3][2] - combo[2][2];
        mFrustumPlanes[FRUSTUM_PLANE_FAR].d = combo[3][3] - combo[2][3];

        for(int i=0; i<6; i++ ) 
        {
            float length = mFrustumPlanes[i].normal.normalise();
            mFrustumPlanes[i].d /= length;
        }

        mRecalcFrustumPlanes = false;
    }

    void DiFrustum::updateFrustumPlanes(void) const
    {
        updateView();
        updateFrustum();

        if (mRecalcFrustumPlanes)
            updateFrustumPlanesImpl();
    }

    void DiFrustum::updateWorldSpaceCornersImpl(void) const
    {
        DiMat4 eyeToWorld = mViewMatrix.inverseAffine();

        float nearLeft, nearRight, nearBottom, nearTop;
        calcProjectionParameters(nearLeft, nearRight, nearBottom, nearTop);

        float farDist = (mFarDist == 0) ? 100000 : mFarDist;

        float radio = mProjType == PT_PERSPECTIVE ? farDist / mNearDist : 1;
        float farLeft = nearLeft * radio;
        float farRight = nearRight * radio;
        float farBottom = nearBottom * radio;
        float farTop = nearTop * radio;

        // near
        mWorldSpaceCorners[0] = eyeToWorld.transformAffine(DiVec3(nearRight, nearTop,    -mNearDist));
        mWorldSpaceCorners[1] = eyeToWorld.transformAffine(DiVec3(nearLeft,  nearTop,    -mNearDist));
        mWorldSpaceCorners[2] = eyeToWorld.transformAffine(DiVec3(nearLeft,  nearBottom, -mNearDist));
        mWorldSpaceCorners[3] = eyeToWorld.transformAffine(DiVec3(nearRight, nearBottom, -mNearDist));

        // far
        mWorldSpaceCorners[4] = eyeToWorld.transformAffine(DiVec3(farRight,  farTop,     -farDist));
        mWorldSpaceCorners[5] = eyeToWorld.transformAffine(DiVec3(farLeft,   farTop,     -farDist));
        mWorldSpaceCorners[6] = eyeToWorld.transformAffine(DiVec3(farLeft,   farBottom,  -farDist));
        mWorldSpaceCorners[7] = eyeToWorld.transformAffine(DiVec3(farRight,  farBottom,  -farDist));

        mRecalcWorldSpaceCorners = false;
    }

    void DiFrustum::updateWorldSpaceCorners(void) const
    {
        updateView();

        if (mRecalcWorldSpaceCorners)
            updateWorldSpaceCornersImpl();
    }

    float DiFrustum::GetAspectRatio(void) const
    {
        return mAspect;
    }

    void DiFrustum::SetAspectRatio(float r)
    {
        mAspect = r;
        InvalidateFrustum();
    }

    const DiAABB& DiFrustum::GetBoundingBox(void) const
    {
        return mBoundingBox;
    }

    void DiFrustum::InvalidateFrustum() const
    {
        mRecalcFrustum = true;
        mRecalcFrustumPlanes = true;
        mRecalcWorldSpaceCorners = true;
        mRecalcVertexData = true;
    }

    void DiFrustum::InvalidateView() const
    {
        mRecalcView = true;
        mRecalcFrustumPlanes = true;
        mRecalcWorldSpaceCorners = true;
    }

    const DiVec3* DiFrustum::GetWorldSpaceCorners(void) const
    {
        updateWorldSpaceCorners();

        return mWorldSpaceCorners;
    }

    void DiFrustum::SetProjectionType(DiProjectionType pt)
    {
        mProjType = pt;
        InvalidateFrustum();
    }

    DiProjectionType DiFrustum::GetProjectionType(void) const
    {
        return mProjType;
    }

    const DiVec3& DiFrustum::GetPositionForViewUpdate(void) const
    {
        return mLastParentPosition;
    }

    const DiQuat& DiFrustum::GetOrientationForViewUpdate(void) const
    {
        return mLastParentOrientation;
    }

    void DiFrustum::EnableReflection(const DiPlane& p)
    {
        mReflect = true;
        mReflectPlane = p;
        mReflectMatrix = DiMath::buildReflectionMatrix(p);
        InvalidateView();

    }

    void DiFrustum::DisableReflection(void)
    {
        mReflect = false;
        InvalidateView();
    }

    bool DiFrustum::ProjectSphere(const DiSphere& sphere, 
                                float* left, float* top, float* right, float* bottom) const
    {
        updateView();
        DiVec3 eyeSpacePos = mViewMatrix.transformAffine(sphere.getCenter());

        // initialise
        *left = *bottom = -1.0f;
        *right = *top = 1.0f;

        if (eyeSpacePos.z < 0)
        {
            updateFrustum();
            const DiMat4& projMatrix = GetProjectionMatrix();
            float r = sphere.getRadius();
            float rsq = r * r;

            // early-exit
            if (eyeSpacePos.squaredLength() <= rsq)
                return false;

            float Lxz = DiMath::Sqr(eyeSpacePos.x) + DiMath::Sqr(eyeSpacePos.z);
            float Lyz = DiMath::Sqr(eyeSpacePos.y) + DiMath::Sqr(eyeSpacePos.z);

            // Find the tangent planes to the sphere
            // XZ first
            // calculate quadratic discriminant: b*b - 4ac
            // x = Nx
            // a = Lx^2 + Lz^2
            // b = -2rLx
            // c = r^2 - Lz^2
            float a = Lxz;
            float b = -2.0f * r * eyeSpacePos.x;
            float c = rsq - DiMath::Sqr(eyeSpacePos.z);
            float D = b*b - 4.0f*a*c;

            // two roots?
            if (D > 0)
            {
                float sqrootD = DiMath::Sqrt(D);
                // solve the quadratic to get the components of the normal
                float Nx0 = (-b + sqrootD) / (2 * a);
                float Nx1 = (-b - sqrootD) / (2 * a);

                // Derive Z from this
                float Nz0 = (r - Nx0 * eyeSpacePos.x) / eyeSpacePos.z;
                float Nz1 = (r - Nx1 * eyeSpacePos.x) / eyeSpacePos.z;

                // Get the point of tangency
                // Only consider points of tangency in front of the camera
                float Pz0 = (Lxz - rsq) / (eyeSpacePos.z - ((Nz0 / Nx0) * eyeSpacePos.x));
                if (Pz0 < 0)
                {
                    // Project point onto near plane in worldspace
                    float nearx0 = (Nz0 * mNearDist) / Nx0;
                    // now we need to map this to viewport coords
                    // use projection matrix since that will take into account all factors
                    DiVec3 relx0 = projMatrix * DiVec3(nearx0, 0, -mNearDist);

                    // find out whether this is a left side or right side
                    float Px0 = -(Pz0 * Nz0) / Nx0;
                    if (Px0 > eyeSpacePos.x)
                    {
                        *right = DiMath::Min(*right, relx0.x);
                    }
                    else
                    {
                        *left = DiMath::Max(*left, relx0.x);
                    }
                }
                float Pz1 = (Lxz - rsq) / (eyeSpacePos.z - ((Nz1 / Nx1) * eyeSpacePos.x));
                if (Pz1 < 0)
                {
                    // Project point onto near plane in worldspace
                    float nearx1 = (Nz1 * mNearDist) / Nx1;
                    // now we need to map this to viewport coords
                    // use projection matrix since that will take into account all factors
                    DiVec3 relx1 = projMatrix * DiVec3(nearx1, 0, -mNearDist);

                    // find out whether this is a left side or right side
                    float Px1 = -(Pz1 * Nz1) / Nx1;
                    if (Px1 > eyeSpacePos.x)
                    {
                        *right = DiMath::Min(*right, relx1.x);
                    }
                    else
                    {
                        *left = DiMath::Max(*left, relx1.x);
                    }
                }
            }

            // Now YZ 
            // calculate quadratic discriminant: b*b - 4ac
            // x = Ny
            // a = Ly^2 + Lz^2
            // b = -2rLy
            // c = r^2 - Lz^2
            a = Lyz;
            b = -2.0f * r * eyeSpacePos.y;
            c = rsq - DiMath::Sqr(eyeSpacePos.z);
            D = b*b - 4.0f*a*c;

            // two roots?
            if (D > 0)
            {
                float sqrootD = DiMath::Sqrt(D);
                // solve the quadratic to get the components of the normal
                float Ny0 = (-b + sqrootD) / (2 * a);
                float Ny1 = (-b - sqrootD) / (2 * a);

                // Derive Z from this
                float Nz0 = (r - Ny0 * eyeSpacePos.y) / eyeSpacePos.z;
                float Nz1 = (r - Ny1 * eyeSpacePos.y) / eyeSpacePos.z;

                // Get the point of tangency
                // Only consider points of tangency in front of the camera
                float Pz0 = (Lyz - rsq) / (eyeSpacePos.z - ((Nz0 / Ny0) * eyeSpacePos.y));
                if (Pz0 < 0)
                {
                    // Project point onto near plane in worldspace
                    float neary0 = (Nz0 * mNearDist) / Ny0;
                    // now we need to map this to viewport coords
                    // use projection matriy since that will take into account all factors
                    DiVec3 rely0 = projMatrix * DiVec3(0, neary0, -mNearDist);

                    // find out whether this is a top side or bottom side
                    float Py0 = -(Pz0 * Nz0) / Ny0;
                    if (Py0 > eyeSpacePos.y)
                    {
                        *top = DiMath::Min(*top, rely0.y);
                    }
                    else
                    {
                        *bottom = DiMath::Max(*bottom, rely0.y);
                    }
                }
                float Pz1 = (Lyz - rsq) / (eyeSpacePos.z - ((Nz1 / Ny1) * eyeSpacePos.y));
                if (Pz1 < 0)
                {
                    // Project point onto near plane in worldspace
                    float neary1 = (Nz1 * mNearDist) / Ny1;
                    // now we need to map this to viewport coords
                    // use projection matriy since that will take into account all factors
                    DiVec3 rely1 = projMatrix * DiVec3(0, neary1, -mNearDist);

                    // find out whether this is a top side or bottom side
                    float Py1 = -(Pz1 * Nz1) / Ny1;
                    if (Py1 > eyeSpacePos.y)
                    {
                        *top = DiMath::Min(*top, rely1.y);
                    }
                    else
                    {
                        *bottom = DiMath::Max(*bottom, rely1.y);
                    }
                }
            }
        }

        return (*left != -1.0f) || (*top != 1.0f) || (*right != 1.0f) || (*bottom != -1.0f);

    }

    void DiFrustum::EnableCustomNearClipPlane(const DiPlane& plane)
    {
        mObliqueDepthProjection = true;
        mObliqueProjPlane = plane;
        InvalidateFrustum();
    }

    void DiFrustum::DisableCustomNearClipPlane(void)
    {
        mObliqueDepthProjection = false;
        InvalidateFrustum();
    }

    void DiFrustum::SetCustomViewMatrix(bool enable, const DiMat4& viewMatrix)
    {
        mCustomViewMatrix = enable;
        if (enable)
        {
            assert(viewMatrix.isAffine());
            mViewMatrix = viewMatrix;
        }
        InvalidateView();
    }

    void DiFrustum::SetCustomProjectionMatrix(bool enable, const DiMat4& projMatrix)
    {
        mCustomProjMatrix = enable;
        if (enable)
        {
            mProjMatrix = projMatrix;
        }
        InvalidateFrustum();
    }

    void DiFrustum::SetOrthoWindow(float w, float h)
    {
        mOrthoHeight = h;
        mAspect = w / h;

        float half_w = w * 0.5f;
        float half_h = h * 0.5f;

        mLeft = -half_w;
        mRight = +half_w;
        mBottom = -half_h;
        mTop = +half_h;

        InvalidateFrustum();
    }

    void DiFrustum::SetOrthoWindow(float left, float top, float right, float bottom)
    {
        mLeft = left;
        mRight = right;
        mTop = top;
        mBottom = bottom;

        mAspect = (right - left) / (bottom - top);
     
        InvalidateFrustum();
    }

    void DiFrustum::SetOrthoWindowHeight(float h)
    {
        mOrthoHeight = h;
        InvalidateFrustum();
    }

    void DiFrustum::SetOrthoWindowWidth(float w)
    {
        SetOrthoWindow(w, w / mAspect);
        InvalidateFrustum();
    }

    float DiFrustum::GetOrthoWindowHeight() const
    {
        return mOrthoHeight;
    }

    float DiFrustum::GetOrthoWindowWidth() const
    {
        return mOrthoHeight * mAspect;    
    }

    void DiFrustum::SetFrustumExtents(float left, float right, float top, float bottom)
    {
        mFrustumExtentsManuallySet = true;
        mLeft = left;
        mRight = right;
        mTop = top;
        mBottom = bottom;

        InvalidateFrustum();
    }

    void DiFrustum::ResetFrustumExtents()
    {
        mFrustumExtentsManuallySet = false;
        InvalidateFrustum();
    }

    void DiFrustum::GetFrustumExtents(float& outleft, float& outright, float& outtop, float& outbottom) const
    {
        updateFrustum();
        outleft = mLeft;
        outright = mRight;
        outtop = mTop;
        outbottom = mBottom;
    }

    DiPlaneBoundedVol DiFrustum::GetPlaneBoundedVolume()
    {
        updateFrustumPlanes();

        DiPlaneBoundedVol volume;
        volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_NEAR]);
        volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_FAR]);
        volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_BOTTOM]);
        volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_TOP]);
        volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_LEFT]);
        volume.planes.push_back(mFrustumPlanes[FRUSTUM_PLANE_RIGHT]);
        return volume;
    }

    void DiFrustum::SetOrientationMode(DiOrientationMode orientationMode)
    {
        mOrientationMode = orientationMode;
        InvalidateFrustum();
    }

    DiOrientationMode DiFrustum::GetOrientationMode() const
    {
        return mOrientationMode;
    }

    DiString& DiFrustum::GetType()
    {
        static DiString type = "Frustom";
        return type;
    }

    void DiFrustum::CopySettingFrom(const DiFrustum* frustum)
    {
        this->SetProjectionType(frustum->GetProjectionType());
        this->SetAspectRatio(frustum->GetAspectRatio());
        this->SetNearClipDistance(frustum->GetNearClipDistance());
        this->SetFarClipDistance(frustum->GetFarClipDistance());
        this->SetFOVy(frustum->GetFOVy());
        this->SetFocalLength(frustum->GetFocalLength());
    }

}