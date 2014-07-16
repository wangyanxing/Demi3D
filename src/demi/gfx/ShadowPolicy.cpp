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
#include "ShadowPolicy.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Light.h"
#include "Math/plane.h"
#include "RenderTarget.h"

namespace Demi
{
	/** transform from normal to light space */
	const DiMat4 DiFocusedShadowPolicy::msNormalToLightSpace(
		1,  0,  0,  0,	// x
		0,  0, -1,  0,	// y
		0,  1,  0,  0,	// z
		0,  0,  0,  1);	// w
    
	/** transform  from light to normal space */
	const DiMat4 DiFocusedShadowPolicy::msLightSpaceToNormal(
		1,  0,  0,  0,	// x
		0,  0,  1,  0,	// y
		0, -1,  0,  0,	// z
		0,  0,  0,  1);	// w

	DiFocusedShadowPolicy::DiFocusedShadowPolicy(void)
		: mTempFrustum(DI_NEW DiFrustum())
		, mLightFrustumCamera(DI_NEW DiCamera("TEMP LIGHT INTERSECT CAM", NULL))
		, mLightFrustumCameraCalculated(false)
		, mUseAggressiveRegion(true)
	{
		mTempFrustum->SetProjectionType(PT_PERSPECTIVE);
	}

	DiFocusedShadowPolicy::~DiFocusedShadowPolicy(void)
	{
		DI_DELETE mTempFrustum;
		DI_DELETE mLightFrustumCamera;
	}

	void DiFocusedShadowPolicy::calculateShadowMappingMatrix(const DiSceneManager& sm,
		const DiCamera& cam, const DiLight& light, DiMat4 *out_view, DiMat4 *out_proj, 
		DiCamera *out_cam) const
	{
        DiVec3 lightPos = light.GetDerivedPosition();
        DiVec3 lightDir = light.GetDerivedDirection();
        
		// get the shadow frustum's far distance
		float shadowDist = sm.GetShadowFarDistance();
		if (!shadowDist)
		{
			// need a shadow distance, make one up
			shadowDist = cam.GetNearClipDistance() * 3000;
		}
		float shadowOffset = shadowDist * sm.GetShadowDirLightTextureOffset();

		if (light.GetType() == LIGHT_DIRECTIONAL)
		{
			// generate view matrix if requested
			if (out_view != NULL)
			{
				DiVec3 pos;
#if 0
				if (sm.getCameraRelativeRendering())
				{
					pos = DiVec3::ZERO;
				}
				else
#endif
				{
					pos = cam.GetDerivedPosition();
				}
				*out_view = buildViewMatrix(pos, 
					lightDir,
					cam.GetDerivedUp());
			}

			// generate projection matrix if requested
			if (out_proj != NULL)
			{
				*out_proj = DiMat4::getScale(1, 1, -1);
				//*out_proj = DiMat4::IDENTITY;
			}

			// set up camera if requested
			if (out_cam != NULL)
			{
				out_cam->SetProjectionType(PT_ORTHOGRAPHIC);
				out_cam->SetDirection(lightDir);
				out_cam->SetPosition(cam.GetDerivedPosition());
				out_cam->SetFOVy(DiDegree(90));
				out_cam->SetNearClipDistance(shadowOffset);
			}
		}
#if 0
		else if (light.GetType() == LIGHT_POINT)
		{
			// target analogue to the default shadow textures
			// Calculate look at position
			// We want to look at a spot shadowOffset away from near plane
			// 0.5 is a little too close for angles
			DiVec3 target = cam.GetDerivedPosition() +
				(cam.GetDerivedDirection() * shadowOffset);
			lightDir = target - lightPos;
			lightDir.normalise();

			// generate view matrix if requested
			if (out_view != NULL)
			{
				*out_view = buildViewMatrix(lightPos,
					lightDir, 
					cam.GetDerivedUp());
			}

			// generate projection matrix if requested
			if (out_proj)
			{
				// set FOV to 120 degrees
				mTempFrustum->SetFOVy(DiDegree(120));

				mTempFrustum->SetNearClipDistance(light._deriveShadowNearClipDistance(&cam));
				mTempFrustum->SetFarClipDistance(light._deriveShadowFarClipDistance(&cam));

				*out_proj = mTempFrustum->GetProjectionMatrix();
			}

			// set up camera if requested
			if (out_cam)
			{
				out_cam->SetProjectionType(PT_PERSPECTIVE);
				out_cam->SetDirection(lightDir);
				out_cam->SetPosition(lightPos);
				out_cam->SetFOVy(DiDegree(120));
				out_cam->SetNearClipDistance(light._deriveShadowNearClipDistance(&cam));
				out_cam->SetFarClipDistance(light._deriveShadowFarClipDistance(&cam));
			}
		}
#endif
    }

	void DiFocusedShadowPolicy::calculateB(const DiSceneManager& sm, const DiCamera& cam,
		const DiLight& light, const DiAABB& sceneBB, const DiAABB& receiverBB, 
		PointListBody *out_bodyB) const
	{
		DI_ASSERT_MESSAGE(out_bodyB != NULL, "bodyB vertex list is NULL");

		/// perform convex intersection of the form B = ((V \cap S) + l) \cap S \cap L

		// get V
		mBodyB.define(cam);

		if (light.GetType() != LIGHT_DIRECTIONAL)
		{
			if (mUseAggressiveRegion)
				mBodyB.clip(sceneBB);

			// form a convex hull of bodyB with the light position
			mBodyB.extend(light.GetDerivedPosition());

			// clip bodyB with sceneBB
			mBodyB.clip(sceneBB);

			// clip with the light frustum
			// set up light camera to clip with the resulting frustum planes
			if (!mLightFrustumCameraCalculated)
			{
				calculateShadowMappingMatrix(sm, cam, light, NULL, NULL, mLightFrustumCamera);
				mLightFrustumCameraCalculated = true;
			}
			mBodyB.clip(*mLightFrustumCamera);

			// extract bodyB vertices
			out_bodyB->build(mBodyB);

		}
		else
		{
			// For directional lights, all we care about is projecting the receivers
			// backwards towards the light, clipped by the camera region
			mBodyB.clip(receiverBB);

			// Also clip based on shadow far distance if appropriate
			float farDist = sm.GetShadowFarDistance();
			if (farDist)
			{
				DiVec3 pointOnPlane = cam.GetDerivedPosition() +
					(cam.GetDerivedDirection() * farDist);
				DiPlane p(cam.GetDerivedDirection(), pointOnPlane);
				mBodyB.clip(p);
			}

			// Extrude the intersection bodyB into the inverted light direction and store 
			// the info in the point list.
			// Maximum extrusion extent is to the shadow far distance
			out_bodyB->buildAndIncludeDirection(mBodyB, 
				farDist ? farDist : cam.GetNearClipDistance() * 3000,
				-light.GetDerivedDirection());
		}
	}


	void DiFocusedShadowPolicy::calculateLVS(const DiSceneManager& sm, const DiCamera& cam,
		const DiLight& light, const DiAABB& sceneBB, PointListBody *out_LVS) const
	{
		DiConvexBody bodyLVS;

		// init body with view frustum
		bodyLVS.define(cam);

		// clip the body with the light frustum (point + spot)
		// for a directional light the space of the intersected
		// view frustum and sceneBB is always lighted and in front
		// of the viewer.
		if (light.GetType() != LIGHT_DIRECTIONAL)
		{
			// clip with the light frustum
			// set up light camera to clip the resulting frustum
			if (!mLightFrustumCameraCalculated)
			{
				calculateShadowMappingMatrix(sm, cam, light, NULL, NULL, mLightFrustumCamera);
				mLightFrustumCameraCalculated = true;
			}
			bodyLVS.clip(*mLightFrustumCamera);
		}

		// clip the body with the scene bounding box
		bodyLVS.clip(sceneBB);

		// extract bodyLVS vertices
		out_LVS->build(bodyLVS);
	}

	DiVec3 DiFocusedShadowPolicy::getLSProjViewDir(const DiMat4& lightSpace, 
		const DiCamera& cam, const PointListBody& bodyLVS) const
	{
		// goal is to construct a view direction
		// because parallel lines are not parallel any more after perspective projection we have to transform
		// a ray to point us the viewing direction

		// fetch a point near the camera
		const DiVec3 e_world = getNearCameraPoint_ws(cam.GetViewMatrix(), bodyLVS);

		// plus the direction results in a second point
		const DiVec3 b_world = e_world + cam.GetDerivedDirection();

		// transformation into light space
		const DiVec3 e_ls = lightSpace * e_world;
		const DiVec3 b_ls = lightSpace * b_world;

		// calculate the projection direction, which is the subtraction of
		// b_ls from e_ls. The y component is set to 0 to project the view
		// direction into the shadow map plane.
		DiVec3 projectionDir(b_ls - e_ls);
		projectionDir.y = 0;

		// deal with Y-only vectors
		return DiMath::RealEqual(projectionDir.length(), 0.0f) ?
			DiVec3::NEGATIVE_UNIT_Z : projectionDir.normalisedCopy();
	}

	DiVec3 DiFocusedShadowPolicy::getNearCameraPoint_ws(const DiMat4& viewMatrix, 
		const PointListBody& bodyLVS) const
	{
		if (bodyLVS.getPointCount() == 0)
			return DiVec3(0,0,0);

		DiVec3 nearEye = viewMatrix * bodyLVS.getPoint(0),	// for comparison
			nearWorld = bodyLVS.getPoint(0);				// represents the final point

		// store the vertex with the highest z-value which is the nearest point
		for (size_t i = 1; i < bodyLVS.getPointCount(); ++i)
		{
			const DiVec3& vWorld = bodyLVS.getPoint(i);

			// comparison is done from the viewer
			DiVec3 vEye = viewMatrix * vWorld;

			if (vEye.z > nearEye.z)
			{
				nearEye		= vEye;
				nearWorld	= vWorld;
			}
		}

		return nearWorld;
	}

	DiMat4 DiFocusedShadowPolicy::transformToUnitCube(const DiMat4& m, 
		const PointListBody& body) const
	{
		// map the transformed body AAB points to the unit cube (-1/-1/-1) / (+1/+1/+1) corners
		DiAABB aab_trans;

		for (size_t i = 0; i < body.getPointCount(); ++i)
		{
			aab_trans.Merge(m * body.getPoint(i));
		}

		DiVec3 vMin, vMax;

		vMin = aab_trans.GetMinimum();
		vMax = aab_trans.GetMaximum();

		const DiVec3 trans(-(vMax.x + vMin.x) / (vMax.x - vMin.x),
			-(vMax.y + vMin.y) / (vMax.y - vMin.y),
			-(vMax.z + vMin.z) / (vMax.z - vMin.z));

		const DiVec3 scale(2 / (vMax.x - vMin.x),
			2 / (vMax.y - vMin.y),
			2 / (vMax.z - vMin.z));

		DiMat4 mOut(DiMat4::IDENTITY);
		mOut.setTrans(trans);
		mOut.setScale(scale);

		return mOut;
	}

	DiMat4 DiFocusedShadowPolicy::buildViewMatrix(const DiVec3& pos, const DiVec3& dir, 
		const DiVec3& up) const
	{
		DiVec3 xN = dir.crossProduct(up);
		xN.normalise();
		DiVec3 upN = xN.crossProduct(dir);
		upN.normalise();

		DiMat4 m(xN.x,		xN.y,		xN.z,		-xN.dotProduct(pos),
			upN.x,		upN.y,		upN.z,		-upN.dotProduct(pos),
			-dir.x,		-dir.y,	-dir.z,	dir.dotProduct(pos),
			0.0,			0.0,		0.0,		1.0
			);

		return m;
	}
    
	void DiFocusedShadowPolicy::getShadowCamera (const DiSceneManager *sm, const DiCamera *cam, 
		const DiViewport *vp, const DiLight *light, DiCamera *texCam, size_t iteration) const
	{
		// check availability - viewport not needed
		DI_ASSERT_MESSAGE(sm != NULL, "SceneManager is NULL");
		DI_ASSERT_MESSAGE(cam != NULL, "Camera (viewer) is NULL");
		DI_ASSERT_MESSAGE(light != NULL, "Light is NULL");
		DI_ASSERT_MESSAGE(texCam != NULL, "Camera (texture) is NULL");
		mLightFrustumCameraCalculated = false;

		texCam->SetNearClipDistance(light->DeriveShadowNearClipDistance(cam));
		texCam->SetFarClipDistance(light->DeriveShadowFarClipDistance(cam));

		// calculate standard shadow mapping matrix
		DiMat4 LView, LProj;
		calculateShadowMappingMatrix(*sm, *cam, *light, &LView, &LProj, NULL);

		// build scene bounding box
		auto& visInfo = texCam->GetVisBoundsInfo();
		DiAABB sceneBB = visInfo.aabb;
		DiAABB receiverAABB = cam->GetVisBoundsInfo().receiverAabb;
		sceneBB.Merge(receiverAABB);
		sceneBB.Merge(cam->GetDerivedPosition());

		// in case the sceneBB is empty (e.g. nothing visible to the cam) simply
		// return the standard shadow mapping matrix
		if (sceneBB.IsNull())
		{
			texCam->SetCustomViewMatrix(true, LView);
			texCam->SetCustomProjectionMatrix(true, LProj);
			return;
		}

		// calculate the intersection body B
		mPointListBodyB.reset();
		calculateB(*sm, *cam, *light, sceneBB, receiverAABB, &mPointListBodyB);

		// in case the bodyB is empty (e.g. nothing visible to the light or the cam)
		// simply return the standard shadow mapping matrix
		if (mPointListBodyB.getPointCount() == 0)
		{
			texCam->SetCustomViewMatrix(true, LView);
			texCam->SetCustomProjectionMatrix(true, LProj);
			return;
		}

		// transform to light space: y -> -z, z -> y
		LProj = msNormalToLightSpace * LProj;

		// calculate LVS so it does not need to be calculated twice
		// calculate the body L \cap V \cap S to make sure all returned points are in 
		// front of the camera
		mPointListBodyLVS.reset();
		calculateLVS(*sm, *cam, *light, sceneBB, &mPointListBodyLVS);

		// fetch the viewing direction
		const DiVec3 viewDir = getLSProjViewDir(LProj * LView, *cam, mPointListBodyLVS);

		// The light space will be rotated in such a way, that the projected light view 
		// always points upwards, so the up-vector is the y-axis (we already prepared the
		// light space for this usage).The transformation matrix is set up with the
		// following parameters:
		// - position is the origin
		// - the view direction is the calculated viewDir
		// - the up vector is the y-axis
		LProj = buildViewMatrix(DiVec3::ZERO, viewDir, DiVec3::UNIT_Y) * LProj;

		// map bodyB to unit cube
		LProj = transformToUnitCube(LProj * LView, mPointListBodyB) * LProj;

		// transform from light space to normal space: y -> z, z -> -y
		LProj = msLightSpaceToNormal * LProj;

		// set the two custom matrices
		texCam->SetCustomViewMatrix(true, LView);
		texCam->SetCustomProjectionMatrix(true, LProj);
	}

	DiFocusedShadowPolicy::PointListBody::PointListBody()
	{
		// Preallocate some space
		mBodyPoints.reserve(12);
	}

	DiFocusedShadowPolicy::PointListBody::PointListBody(const DiConvexBody& body)
	{
		build(body);
	}

	DiFocusedShadowPolicy::PointListBody::~PointListBody()
	{
	}

	void DiFocusedShadowPolicy::PointListBody::merge(const PointListBody& plb)
	{
		size_t size = plb.getPointCount();
		for (size_t i = 0; i < size; ++i)
		{
			this->addPoint(plb.getPoint(i));
		}
	}

	void DiFocusedShadowPolicy::PointListBody::build(const DiConvexBody& body, bool filterDuplicates)
	{
		// erase list
		mBodyPoints.clear();

		// Try to reserve a representative amount of memory
		mBodyPoints.reserve(body.getPolygonCount() * 6);

		// build new list
		for (size_t i = 0; i < body.getPolygonCount(); ++i)
		{
			for (size_t j = 0; j < body.getVertexCount(i); ++j)
			{
				const DiVec3 &vInsert = body.getVertex(i, j);

				// duplicates allowed?
				if (filterDuplicates)
				{
					bool bPresent = false;

					for(auto vit = mBodyPoints.begin();
						vit != mBodyPoints.end(); ++vit)
					{
						const DiVec3& v = *vit;

						if (vInsert.positionEquals(v))
						{
							bPresent = true;
							break;
						}
					}

					if (bPresent == false)
					{
						mBodyPoints.push_back(body.getVertex(i, j));
					}
				}

				// else insert directly
				else
				{
					mBodyPoints.push_back(body.getVertex(i, j));
				}
			}
		}

		// update AAB
		// no points altered, so take body AAB
		mAAB = body.getAABB();
	}
    
	void DiFocusedShadowPolicy::PointListBody::buildAndIncludeDirection(
		const DiConvexBody& body, float extrudeDist, const DiVec3& dir)
	{
		// reset point list
		this->reset();

		// intersect the rays formed by the points in the list with the given direction and
		// insert them into the list

		const size_t polyCount = body.getPolygonCount();
		for (size_t iPoly = 0; iPoly < polyCount; ++iPoly)
		{
			const DiPolygon& p = body.getPolygon(iPoly);

			size_t pointCount = p.GetVertexCount();
			for (size_t iPoint = 0; iPoint < pointCount ; ++iPoint)
			{
				// base point
				const DiVec3& pt = p.GetVertex(iPoint);

				// add the base point
				this->addPoint(pt);

				// intersection ray
				DiRay ray(pt, dir);

				const DiVec3 ptIntersect = ray.getPoint(extrudeDist);
				this->addPoint(ptIntersect);

			} // for: polygon point iteration

		} // for: polygon iteration
	}

	const DiAABB& DiFocusedShadowPolicy::PointListBody::getAAB(void) const
	{
		return mAAB;
	}

	void DiFocusedShadowPolicy::PointListBody::addPoint(const DiVec3& point)
	{
		// dont check for doubles, simply add
		mBodyPoints.push_back(point);

		// update AAB
		mAAB.Merge(point);
	}

	void DiFocusedShadowPolicy::PointListBody::addAAB(const DiAABB& aab)
	{
		const DiVec3& min = aab.GetMinimum();
		const DiVec3& max = aab.GetMaximum();

		DiVec3 currentVertex = min;
		// min min min
		addPoint(currentVertex);

		// min min max
		currentVertex.z = max.z;
		addPoint(currentVertex);

		// min max max
		currentVertex.y = max.y;
		addPoint(currentVertex);

		// min max min
		currentVertex.z = min.z;
		addPoint(currentVertex);

		// max max min
		currentVertex.x = max.x;
		addPoint(currentVertex);

		// max max max
		currentVertex.z = max.z;
		addPoint(currentVertex);

		// max min max
		currentVertex.y = min.y;
		addPoint(currentVertex);

		// max min min
		currentVertex.z = min.z;
		addPoint(currentVertex); 

	}

	const DiVec3& DiFocusedShadowPolicy::PointListBody::getPoint(size_t cnt) const
	{
		DI_ASSERT_MESSAGE(cnt < getPointCount(), "Search position out of range");

		return mBodyPoints[ cnt ];
	}

	size_t DiFocusedShadowPolicy::PointListBody::getPointCount(void) const
	{
		return mBodyPoints.size();
	}

	void DiFocusedShadowPolicy::PointListBody::reset(void)
	{
		mBodyPoints.clear();
		mAAB.SetNull();
	}

    void DiNormalShadowPolicy::getShadowCamera (const DiSceneManager *sm, const DiCamera *cam,
                                                    const DiViewport *vp, const DiLight *light, DiCamera *texCam, size_t iteration) const
	{
		DiVec3 pos, dir;
        
		// reset custom view / projection matrix in case already set
		texCam->SetCustomViewMatrix(false);
		texCam->SetCustomProjectionMatrix(false);
		texCam->SetNearClipDistance(light->DeriveShadowNearClipDistance(cam));
		texCam->SetFarClipDistance(light->DeriveShadowFarClipDistance(cam));
        
		// get the shadow frustum's far distance
		float shadowDist = sm->GetShadowFarDistance();
		if (!shadowDist)
		{
			// need a shadow distance, make one up
			shadowDist = cam->GetNearClipDistance() * 300;
		}
		float shadowOffset = shadowDist * (sm->GetShadowDirLightTextureOffset());
        
		// Directional lights
		if (light->GetType() == LIGHT_DIRECTIONAL)
		{
			// set up the shadow texture
			// Set ortho projection
			texCam->SetProjectionType(PT_ORTHOGRAPHIC);
			// set ortho window so that texture covers far dist
			texCam->SetOrthoWindow(shadowDist * 2, shadowDist * 2);
            
			// Calculate look at position
			// We want to look at a spot shadowOffset away from near plane
			// 0.5 is a litle too close for angles
			DiVec3 target = cam->GetDerivedPosition() +
            (cam->GetDerivedDirection() * shadowOffset);
            
			// Calculate direction, which same as directional light direction
			dir = - light->GetDerivedDirection(); // backwards since point down -z
			dir.normalise();
            
			// Calculate position
			// We want to be in the -ve direction of the light direction
			// far enough to project for the dir light extrusion distance
			pos = target + dir * sm->GetShadowDirLightExtrusionDistance();
            
			float worldTexelSize = (shadowDist * 2) / (vp->mWidth * vp->mParent->GetWidth());
            
            //get texCam orientation
            
            DiVec3 up = DiVec3::UNIT_Y;
            // Check it's not coincident with dir
            if (DiMath::Abs(up.dotProduct(dir)) >= 1.0f)
            {
				// Use camera up
				up = DiVec3::UNIT_Z;
            }
            // cross twice to rederive, only direction is unaltered
            DiVec3 left = dir.crossProduct(up);
            left.normalise();
            up = dir.crossProduct(left);
            up.normalise();
            // Derive quaternion from axes
            DiQuat q;
            q.FromAxes(left, up, dir);
            
            //convert world space camera position into light space
            DiVec3 lightSpacePos = q.Inverse() * pos;
            
            //snap to nearest texel
            lightSpacePos.x -= fmod(lightSpacePos.x, worldTexelSize);
            lightSpacePos.y -= fmod(lightSpacePos.y, worldTexelSize);
            
            //convert back to world space
            pos = q * lightSpacePos;
			
		}
		else
        {
            DI_ASSERT_FAIL;
        }
        
		// Finally set position
		texCam->SetPosition(pos);
        
		DiVec3 up = DiVec3::UNIT_Y;
		// Check it's not coincident with dir
		if (DiMath::Abs(up.dotProduct(dir)) >= 1.0f)
		{
			// Use camera up
			up = DiVec3::UNIT_Z;
		}
		// cross twice to rederive, only direction is unaltered
		DiVec3 left = dir.crossProduct(up);
		left.normalise();
		up = dir.crossProduct(left);
		up.normalise();
		// Derive quaternion from axes
		DiQuat q;
		q.FromAxes(left, up, dir);
		texCam->SetOrientation(q);
	}
}
