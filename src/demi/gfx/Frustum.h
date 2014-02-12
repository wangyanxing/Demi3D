


#ifndef DiFrustum_h__
#define DiFrustum_h__


#include "GfxPrerequisites.h"
#include "TransformUnit.h"
#include "vector2.h"
#include "vector3.h"
#include "plane.h"
#include "matrix4.h"
#include "sphere.h"
#include "AxisAlignedBox.h"
#include "planeBoundedVolume.h"

namespace Demi 
{
    enum DiOrientationMode
    {
        OR_DEGREE_0       = 0,
        OR_DEGREE_90      = 1,
        OR_DEGREE_180     = 2,
        OR_DEGREE_270     = 3,

        OR_PORTRAIT       = OR_DEGREE_0,
        OR_LANDSCAPERIGHT = OR_DEGREE_90,
        OR_LANDSCAPELEFT  = OR_DEGREE_270
    };

    enum DiProjectionType
    {
        PT_ORTHOGRAPHIC,
        PT_PERSPECTIVE
    };

    enum DiFrustumPlane
    {
        FRUSTUM_PLANE_NEAR   = 0,
        FRUSTUM_PLANE_FAR    = 1,
        FRUSTUM_PLANE_LEFT   = 2,
        FRUSTUM_PLANE_RIGHT  = 3,
        FRUSTUM_PLANE_TOP    = 4,
        FRUSTUM_PLANE_BOTTOM = 5
    };

    class DI_GFX_API DiFrustum : public DiTransformUnit
    {
    public:
        DiFrustum(const DiString& name);

        DiFrustum();
        
        virtual                     ~DiFrustum(void);

    public:

        virtual void                SetFOVy(const DiRadian& fovy);

        virtual const DiRadian&        GetFOVy(void) const;

        virtual void                SetNearClipDistance(float nearDist);

        virtual float                GetNearClipDistance(void) const;

        virtual void                SetFarClipDistance(float farDist);

        virtual float                GetFarClipDistance(void) const;

        virtual void                SetAspectRatio(float ratio);

        virtual float                GetAspectRatio(void) const;

        virtual void                SetFrustumOffset(const DiVec2& offset);

        virtual void                SetFrustumOffset(float horizontal = 0.0, float vertical = 0.0);

        virtual const DiVec2&        GetFrustumOffset() const;

        virtual void                SetFocalLength(float focalLength = 1.0);

        virtual float                GetFocalLength() const;

        virtual void                SetFrustumExtents(float left, float right, float top, float bottom);

        virtual void                 ResetFrustumExtents(); 

        virtual void                 GetFrustumExtents(float& outleft, float& outright, float& outtop, float& outbottom) const;

        virtual const DiMat4&         GetProjectionMatrix(void) const;

        void                        GetProjectionMatrixRs(DiMat4& out) const;

        virtual const DiMat4&         GetViewMatrix(void) const;

        virtual void                 CalcViewMatrixRelative(const DiVec3& relPos, DiMat4& matToUpdate) const;

        virtual void                 SetCustomViewMatrix(bool enable, 
            const DiMat4& viewMatrix = DiMat4::IDENTITY);
        
        virtual bool                IsCustomViewMatrixEnabled(void) const 
        { return mCustomViewMatrix; }

        virtual void                SetCustomProjectionMatrix(bool enable, 
            const DiMat4& projectionMatrix = DiMat4::IDENTITY);
        
        virtual bool                IsCustomProjectionMatrixEnabled(void) const
        { return mCustomProjMatrix; }

        virtual const DiPlane*         GetFrustumPlanes(void) const;

        virtual const DiPlane&         GetFrustumPlane( unsigned short plane ) const;

        virtual bool                 IsVisible(const DiAABB& bound, DiFrustumPlane* culledBy = 0) const;

        virtual bool                 IsVisible(const DiSphere& bound, DiFrustumPlane* culledBy = 0) const;

        virtual bool                 IsVisible(const DiVec3& vert, DiFrustumPlane* culledBy = 0) const;

        const DiAABB&                GetBoundingBox(void) const;

        virtual const DiVec3*        GetWorldSpaceCorners(void) const;

        virtual void                SetProjectionType(DiProjectionType pt);

        virtual DiProjectionType    GetProjectionType(void) const;

        virtual void                 SetOrthoWindow(float w, float h);

        virtual void                 SetOrthoWindowHeight(float h);

        virtual void                 SetOrthoWindowWidth(float w);

        virtual float                 GetOrthoWindowHeight() const;

        virtual float                 GetOrthoWindowWidth() const;

        virtual void                 EnableReflection(const DiPlane& p);

        virtual void                 DisableReflection(void);

        virtual bool                 IsReflected(void) const { return mReflect; }
        
        virtual const DiMat4&        GetReflectionMatrix(void) const { return mReflectMatrix; }
        
        virtual const DiPlane&        GetReflectionPlane(void) const { return mReflectPlane; }

        virtual bool                ProjectSphere(const DiSphere& sphere, 
            float* left, float* top, float* right, float* bottom) const;

        virtual void                 EnableCustomNearClipPlane(const DiPlane& plane);

        virtual void                 DisableCustomNearClipPlane(void);

        virtual bool                 IsCustomNearClipPlaneEnabled(void) const 
        { return mObliqueDepthProjection; }

        static const float INFINITE_FAR_PLANE_ADJUST;

        virtual const DiVec3&        GetPositionForViewUpdate(void) const;

        virtual const DiQuat&        GetOrientationForViewUpdate(void) const;

        DiPlaneBoundedVol            GetPlaneBoundedVolume();

        void                        SetOrientationMode(DiOrientationMode orientationMode);

        DiOrientationMode            GetOrientationMode() const;
        
        virtual void                VisitRenderUnit(DiRenderVisitor*) {}

        float                        GetAspect() const { return mAspect; }

        virtual DiString&            GetType();

    protected:

        virtual void                 calcProjectionParameters(float& left, float& right,
                                                        float& bottom, float& top) const;
        virtual void                 updateFrustum(void) const;
        virtual void                 updateView(void) const;
        virtual void                 updateFrustumImpl(void) const;
        virtual void                 updateViewImpl(void) const;
        virtual void                 updateFrustumPlanes(void) const;
        virtual void                 updateFrustumPlanesImpl(void) const;
        virtual void                 updateWorldSpaceCorners(void) const;
        virtual void                 updateWorldSpaceCornersImpl(void) const;
        virtual bool                 IsViewOutOfDate(void) const;
        virtual bool                 isFrustumOutOfDate(void) const;
        virtual void                 InvalidateFrustum(void) const;
        virtual void                 InvalidateView(void) const;

    protected:
        DiProjectionType            mProjType;

        DiRadian                    mFOVy;
        float                         mFarDist;
        float                         mNearDist;
        float                         mAspect;
        float                         mOrthoHeight;
        DiVec2                        mFrustumOffset;
        float                        mFocalLength;

        mutable DiPlane                mFrustumPlanes[6];

        mutable DiQuat                mLastParentOrientation;
        mutable DiVec3                mLastParentPosition;

        mutable DiMat4                 mProjMatrix;
        mutable DiMat4                 mViewMatrix;
        mutable bool                 mRecalcFrustum;
        mutable bool                 mRecalcView;
        mutable bool                 mRecalcFrustumPlanes;
        mutable bool                 mRecalcWorldSpaceCorners;
        mutable bool                 mRecalcVertexData;
        bool                         mCustomViewMatrix;
        bool                         mCustomProjMatrix;
        bool                         mFrustumExtentsManuallySet;
        mutable float                mLeft, mRight, mTop, mBottom;
        mutable DiOrientationMode     mOrientationMode;

        mutable DiAABB                mBoundingBox;
        mutable DiVec3                mWorldSpaceCorners[8];

        bool                        mReflect;
        mutable DiMat4                mReflectMatrix;
        mutable DiPlane                mReflectPlane;

        bool                        mObliqueDepthProjection;
        mutable DiPlane                mObliqueProjPlane;
    };

}

#endif
