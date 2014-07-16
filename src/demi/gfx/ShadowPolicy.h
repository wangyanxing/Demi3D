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

#ifndef __ShadowCameraSetup_H__
#define __ShadowCameraSetup_H__

#include "GfxPrerequisites.h"
#include "Math/Polygen.h"
#include "ConvexBody.h"

namespace Demi
{
    class DI_GFX_API DiShadowCameraPolicy
	{
	public:

		virtual void getShadowCamera (const DiSceneManager *sm, const DiCamera *cam,
									  const DiViewport *vp, const DiLight *light, DiCamera *texCam, size_t iteration) const = 0;

		virtual ~DiShadowCameraPolicy() {}
        
	};
    
    class DI_GFX_API DiNormalShadowPolicy : public DiShadowCameraPolicy
	{
	public:

		DiNormalShadowPolicy(){}

		virtual ~DiNormalShadowPolicy(){}
        
        virtual void getShadowCamera (const DiSceneManager *sm, const DiCamera *cam,
									  const DiViewport *vp, const DiLight *light, DiCamera *texCam, size_t iteration) const;
	};

	class DI_GFX_API DiFocusedShadowPolicy : public DiShadowCameraPolicy
	{
	protected:

		static const DiMat4 msNormalToLightSpace;
		static const DiMat4 msLightSpaceToNormal;

		/** Temporary preallocated frustum to set up a projection matrix in 
		::calculateShadowMappingMatrix()
		*/
		DiFrustum* mTempFrustum;

		/** Temporary preallocated camera to set up a light frustum for clipping in ::calculateB.
		*/
		DiCamera* mLightFrustumCamera;
		mutable bool mLightFrustumCameraCalculated;

		/// Use tighter focus region?
		bool mUseAggressiveRegion;

		/** Internal class holding a point list representation of a convex body.
		*/
		class DI_GFX_API PointListBody
		{
			DiPolygon::VertexList mBodyPoints;
			DiAABB		mAAB;

		public:
			PointListBody();
			PointListBody(const DiConvexBody& body);
			~PointListBody();

			void merge(const PointListBody& plb);

			void build(const DiConvexBody& body, bool filterDuplicates = true);

			void buildAndIncludeDirection(const DiConvexBody& body, 
				float extrudeDist, const DiVec3& dir);

			/** Returns the bounding box representation.
			*/
			const DiAABB& getAAB(void) const;	

			/** Adds a specific point to the body list.
			*/
			void addPoint(const DiVec3& point);

			/** Adds all points of an AAB.
			*/
			void addAAB(const DiAABB& aab);

			/** Returns a point.
			*/
			const DiVec3& getPoint(size_t cnt) const;

			/** Returns the point count.
			*/
			size_t getPointCount(void) const;

			/** Resets the body.
			*/
			void reset(void);

		};

		// Persistent calculations to prevent reallocation
		mutable DiConvexBody mBodyB;
		mutable PointListBody mPointListBodyB;
		mutable PointListBody mPointListBodyLVS;

	protected:

		void calculateShadowMappingMatrix(const DiSceneManager& sm, const DiCamera& cam, 
			const DiLight& light, DiMat4 *out_view, 
			DiMat4 *out_proj, DiCamera *out_cam) const;

		void calculateB(const DiSceneManager& sm, const DiCamera& cam, const DiLight& light, 
			const DiAABB& sceneBB, const DiAABB& receiverBB, PointListBody *out_bodyB) const;

		void calculateLVS(const DiSceneManager& sm, const DiCamera& cam, const DiLight& light,
			const DiAABB& sceneBB, PointListBody *out_LVS) const;

		DiVec3 getLSProjViewDir(const DiMat4& lightSpace, const DiCamera& cam,
			const PointListBody& bodyLVS) const;

		DiVec3 getNearCameraPoint_ws(const DiMat4& viewMatrix, 
			const PointListBody& bodyLVS) const;

		DiMat4 transformToUnitCube(const DiMat4& m, const PointListBody& body) const;

		DiMat4 buildViewMatrix(const DiVec3& pos, const DiVec3& dir, const DiVec3& up) const;

	public:

		DiFocusedShadowPolicy(void);

		virtual ~DiFocusedShadowPolicy(void);

		virtual void getShadowCamera(const DiSceneManager *sm, const DiCamera *cam, 
			const DiViewport *vp, const DiLight *light, DiCamera *texCam, size_t iteration) const;

		void setUseAggressiveFocusRegion(bool aggressive) { mUseAggressiveRegion = aggressive; }

		bool getUseAggressiveFocusRegion() const { return mUseAggressiveRegion; }

	};
}

#endif
