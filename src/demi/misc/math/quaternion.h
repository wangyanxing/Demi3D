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

#ifndef Diquaternion_h__
#define Diquaternion_h__


#include "MiscPrerequisites.h"
#include "math/mathMisc.h"
#include <utility>

namespace Demi 
{
    class DI_MISC_API DiQuat
    {
    public:
        inline DiQuat (
            float fW = 1.0,
            float fX = 0.0, float fY = 0.0, float fZ = 0.0)
        {
            w = fW;
            x = fX;
            y = fY;
            z = fZ;
        }
        
        inline DiQuat(const DiMat3& rot)
        {
            this->FromRotationMatrix(rot);
        }
        
        inline DiQuat(const DiRadian& rfAngle, const DiVec3& rkAxis)
        {
            this->FromAngleAxis(rfAngle, rkAxis);
        }

        inline DiQuat(float angle, const DiVec3& rkAxis)
        {
            this->FromAngleAxis(DiRadian(angle), rkAxis);
        }
        
        inline DiQuat(const DiVec3& xaxis, const DiVec3& yaxis, const DiVec3& zaxis)
        {
            this->FromAxes(xaxis, yaxis, zaxis);
        }
        
        inline DiQuat(const DiVec3* akAxis)
        {
            this->FromAxes(akAxis);
        }

        inline DiQuat(float* valptr)
        {
            memcpy(&w, valptr, sizeof(float)*4);
        }

        inline void swap(DiQuat& other)
        {
            std::swap(w, other.w);
            std::swap(x, other.x);
            std::swap(y, other.y);
            std::swap(z, other.z);
        }

        inline float operator [] ( const size_t i ) const
        {
            DI_ASSERT( i < 4 );

            return *(&w+i);
        }

        inline float& operator [] ( const size_t i )
        {
            DI_ASSERT( i < 4 );

            return *(&w+i);
        }

        inline float* ptr()
        {
            return &w;
        }

        inline const float* ptr() const
        {
            return &w;
        }

        void FromRotationMatrix (const DiMat3& kRot);
        void ToRotationMatrix (DiMat3& kRot) const;
        void FromAngleAxis (const DiRadian& rfAngle, const DiVec3& rkAxis);
        void ToAngleAxis (DiRadian& rfAngle, DiVec3& rkAxis) const;
        inline void ToAngleAxis (DiDegree& dAngle, DiVec3& rkAxis) const {
            DiRadian rAngle;
            ToAngleAxis ( rAngle, rkAxis );
            dAngle = rAngle;
        }
        void FromAxes (const DiVec3* akAxis);
        void FromAxes (const DiVec3& xAxis, const DiVec3& yAxis, const DiVec3& zAxis);
        void ToAxes (DiVec3* akAxis) const;
        void ToAxes (DiVec3& xAxis, DiVec3& yAxis, DiVec3& zAxis) const;
        /// Get the local x-axis
        DiVec3 xAxis(void) const;
        /// Get the local y-axis
        DiVec3 yAxis(void) const;
        /// Get the local z-axis
        DiVec3 zAxis(void) const;

        inline DiQuat& operator= (const DiQuat& rkQ)
        {
            w = rkQ.w;
            x = rkQ.x;
            y = rkQ.y;
            z = rkQ.z;
            return *this;
        }
        DiQuat operator+ (const DiQuat& rkQ) const;
        DiQuat operator- (const DiQuat& rkQ) const;
        DiQuat operator* (const DiQuat& rkQ) const;
        DiQuat operator* (float fScalar) const;

        DI_MISC_API friend DiQuat operator* (float fScalar, const DiQuat& rkQ);

        DiQuat operator- () const;
        inline bool operator== (const DiQuat& rhs) const
        {
            return (rhs.x == x) && (rhs.y == y) &&
                (rhs.z == z) && (rhs.w == w);
        }
        inline bool operator!= (const DiQuat& rhs) const
        {
            return !operator==(rhs);
        }

        bool IsIdentity() const { return w == 1 && x == 0 && y == 0 && z == 0; }

        // functions of a quaternion
        float Dot (const DiQuat& rkQ) const;  // dot product
        float Norm () const;  // squared-length
        /// Normalises this quaternion, and returns the previous length
        float normalise(void); 
        DiQuat Inverse () const;  // apply to non-zero quaternion
        DiQuat UnitInverse () const;  // apply to unit-length quaternion
        DiQuat Exp () const;
        DiQuat Log () const;

        DiVec3 operator* (const DiVec3& rkVector) const;

        DiRadian getRoll(bool reprojectAxis = true) const;
        
        DiRadian getPitch(bool reprojectAxis = true) const;
        
        DiRadian getYaw(bool reprojectAxis = true) const;        
        
        bool equals(const DiQuat& rhs, const DiRadian& tolerance) const;
        
        static DiQuat Slerp (float fT, const DiQuat& rkP,
            const DiQuat& rkQ, bool shortestPath = false);

        static DiQuat SlerpExtraSpins (float fT,
            const DiQuat& rkP, const DiQuat& rkQ,
            int iExtraSpins);

        static void Intermediate (const DiQuat& rkQ0,
            const DiQuat& rkQ1, const DiQuat& rkQ2,
            DiQuat& rka, DiQuat& rkB);

        static DiQuat Squad (float fT, const DiQuat& rkP,
            const DiQuat& rkA, const DiQuat& rkB,
            const DiQuat& rkQ, bool shortestPath = false);

        static DiQuat Nlerp(float fT, const DiQuat& rkP, 
            const DiQuat& rkQ, bool shortestPath = false);

        static const float ms_fEpsilon;

        static const DiQuat ZERO;
        static const DiQuat IDENTITY;

        float w, x, y, z;

        inline bool isNaN() const
        {
            return DiMath::isNaN(x) || DiMath::isNaN(y) || DiMath::isNaN(z) || DiMath::isNaN(w);
        }
    };

}

#endif
