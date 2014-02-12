


#ifndef Dimatrix3_h__
#define Dimatrix3_h__


#include "MiscPrerequisites.h"
#include <float.h>
#include <utility>
#include "vector3.h"

// NB All code adapted from Wild Magic 0.2 Matrix math (free source code)
// http://www.geometrictools.com/

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.
namespace Demi 
{
    class DI_MISC_API DiMat3
    {
    public:
       
        inline DiMat3 ()
        {
            setIdentity();
        }

        inline explicit DiMat3 (const float arr[3][3])
        {
            memcpy(m,arr,9*sizeof(float));
        }
        inline DiMat3 (const DiMat3& rkMatrix)
        {
            memcpy(m,rkMatrix.m,9*sizeof(float));
        }
        DiMat3 (float fEntry00, float fEntry01, float fEntry02,
                    float fEntry10, float fEntry11, float fEntry12,
                    float fEntry20, float fEntry21, float fEntry22)
        {
            m[0][0] = fEntry00;
            m[0][1] = fEntry01;
            m[0][2] = fEntry02;
            m[1][0] = fEntry10;
            m[1][1] = fEntry11;
            m[1][2] = fEntry12;
            m[2][0] = fEntry20;
            m[2][1] = fEntry21;
            m[2][2] = fEntry22;
        }

        /** Exchange the contents of this matrix with another. 
        */
        inline void swap(DiMat3& other)
        {
            std::swap(m[0][0], other.m[0][0]);
            std::swap(m[0][1], other.m[0][1]);
            std::swap(m[0][2], other.m[0][2]);
            std::swap(m[1][0], other.m[1][0]);
            std::swap(m[1][1], other.m[1][1]);
            std::swap(m[1][2], other.m[1][2]);
            std::swap(m[2][0], other.m[2][0]);
            std::swap(m[2][1], other.m[2][1]);
            std::swap(m[2][2], other.m[2][2]);
        }

        // member access, allows use of construct mat[r][c]
        inline float* operator[] (size_t iRow) const
        {
            return (float*)m[iRow];
        }
        /*inline operator float* ()
        {
            return (float*)m[0];
        }*/
        DiVec3 GetColumn (size_t iCol) const;
        void SetColumn(size_t iCol, const DiVec3& vec);
        void FromAxes(const DiVec3& xAxis, const DiVec3& yAxis, const DiVec3& zAxis);

        // assignment and comparison
        inline DiMat3& operator= (const DiMat3& rkMatrix)
        {
            memcpy(m,rkMatrix.m,9*sizeof(float));
            return *this;
        }
        bool operator== (const DiMat3& rkMatrix) const;
        inline bool operator!= (const DiMat3& rkMatrix) const
        {
            return !operator==(rkMatrix);
        }

        // arithmetic operations
        DiMat3 operator+ (const DiMat3& rkMatrix) const;
        DiMat3 operator- (const DiMat3& rkMatrix) const;
        DiMat3 operator* (const DiMat3& rkMatrix) const;
        DiMat3 operator- () const;

        // matrix * vector [3x3 * 3x1 = 3x1]
        DiVec3 operator* (const DiVec3& rkVector) const;

        // vector * matrix [1x3 * 3x3 = 1x3]
        DI_MISC_API friend DiVec3 operator* (const DiVec3& rkVector,
            const DiMat3& rkMatrix);

        // matrix * float
        DiMat3 operator* (float fScalar) const;

        // float * matrix
        DI_MISC_API friend DiMat3 operator* (float fScalar, const DiMat3& rkMatrix);

        // utilities
        DiMat3 Transpose () const;
        bool Inverse (DiMat3& rkInverse, float fTolerance = 1e-06) const;
        DiMat3 Inverse (float fTolerance = 1e-06) const;
        float Determinant () const;

        // singular value decomposition
        void SingularValueDecomposition (DiMat3& rkL, DiVec3& rkS,
            DiMat3& rkR) const;
        void SingularValueComposition (const DiMat3& rkL,
            const DiVec3& rkS, const DiMat3& rkR);

        inline void setIdentity()
        {
            m[0][0] = 1;
            m[0][1] = 0;
            m[0][2] = 0;

            m[1][0] = 0;
            m[1][1] = 1;
            m[1][2] = 0;

            m[2][0] = 0;
            m[2][1] = 0;
            m[2][2] = 1;
        }

        // Gram-Schmidt orthonormalization (applied to columns of rotation matrix)
        void Orthonormalize ();

        // orthogonal Q, diagonal D, upper triangular U stored as (u01,u02,u12)
        void QDUDecomposition (DiMat3& rkQ, DiVec3& rkD,
            DiVec3& rkU) const;

        float SpectralNorm () const;

        // matrix must be orthonormal
        void ToAxisAngle (DiVec3& rkAxis, DiRadian& rfAngle) const;
        inline void ToAxisAngle (DiVec3& rkAxis, DiDegree& rfAngle) const
        {
            DiRadian r;
            ToAxisAngle ( rkAxis, r );
            rfAngle = r;
        }
        void FromAngleAxis (const DiVec3& rkAxis, const DiRadian& fRadians);

        // The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
        // where yaw is rotation about the Up vector, pitch is rotation about the
        // Right axis, and roll is rotation about the Direction axis.
        bool ToEulerAnglesXYZ (DiRadian& rfYAngle, DiRadian& rfPAngle,
            DiRadian& rfRAngle) const;
        bool ToEulerAnglesXZY (DiRadian& rfYAngle, DiRadian& rfPAngle,
            DiRadian& rfRAngle) const;
        bool ToEulerAnglesYXZ (DiRadian& rfYAngle, DiRadian& rfPAngle,
            DiRadian& rfRAngle) const;
        bool ToEulerAnglesYZX (DiRadian& rfYAngle, DiRadian& rfPAngle,
            DiRadian& rfRAngle) const;
        bool ToEulerAnglesZXY (DiRadian& rfYAngle, DiRadian& rfPAngle,
            DiRadian& rfRAngle) const;
        bool ToEulerAnglesZYX (DiRadian& rfYAngle, DiRadian& rfPAngle,
            DiRadian& rfRAngle) const;

        void FromEulerAnglesXYZ (const DiRadian& fYAngle, const DiRadian& fPAngle, const DiRadian& fRAngle);
        void FromEulerAnglesXZY (const DiRadian& fYAngle, const DiRadian& fPAngle, const DiRadian& fRAngle);
        void FromEulerAnglesYXZ (const DiRadian& fYAngle, const DiRadian& fPAngle, const DiRadian& fRAngle);
        void FromEulerAnglesYZX (const DiRadian& fYAngle, const DiRadian& fPAngle, const DiRadian& fRAngle);
        void FromEulerAnglesZXY (const DiRadian& fYAngle, const DiRadian& fPAngle, const DiRadian& fRAngle);
        void FromEulerAnglesZYX (const DiRadian& fYAngle, const DiRadian& fPAngle, const DiRadian& fRAngle);
        // eigensolver, matrix must be symmetric
        void EigenSolveSymmetric (float afEigenvalue[3],
            DiVec3 akEigenvector[3]) const;

        void SetRotationVDir( const DiVec3& vdir ) 
        {
            DI_ASSERT((DiMath::Abs(1-(vdir.dotProduct(vdir))))<0.01);        //check if unit-vector
            //set default initialisation for up-vector    
            m[0][0]=1;    m[0][1]=0;        m[0][2]=0;
            m[1][0]=0;    m[1][1]=0;        m[1][2]=-vdir.z;
            m[2][0]=0;    m[2][1]=vdir.z;    m[2][2]=0;
            //calculate look-at matrix
            double l = sqrt(vdir.x*vdir.x+vdir.y*vdir.y);
            if (l>0.0001)    
            {
                double xl=-vdir.x/l; 
                double yl=vdir.y/l;
                m[0][0]=float(yl);    m[0][1]=float(vdir.x); m[0][2]=float(xl*vdir.z);
                m[1][0]=float(xl);    m[1][1]=float(vdir.y); m[1][2]=float(-vdir.z*yl);
                m[2][0]=0;            m[2][1]=float(vdir.z); m[2][2]=float(l);
            }
        }

        static void TensorProduct (const DiVec3& rkU, const DiVec3& rkV,
            DiMat3& rkProduct);

        /** Determines if this matrix involves a scaling. */
        inline bool hasScale() const
        {
            // check magnitude of column vectors (==local axes)
            float t = m[0][0] * m[0][0] + m[1][0] * m[1][0] + m[2][0] * m[2][0];
            if (!DiMath::RealEqual(t, 1.0, (float)1e-04))
                return true;
            t = m[0][1] * m[0][1] + m[1][1] * m[1][1] + m[2][1] * m[2][1];
            if (!DiMath::RealEqual(t, 1.0, (float)1e-04))
                return true;
            t = m[0][2] * m[0][2] + m[1][2] * m[1][2] + m[2][2] * m[2][2];
            if (!DiMath::RealEqual(t, 1.0, (float)1e-04))
                return true;

            return false;
        }

        static const float EPSILON;
        static const DiMat3 ZERO;
        static const DiMat3 IDENTITY;

    protected:
        // support for eigensolver
        void Tridiagonal (float afDiag[3], float afSubDiag[3]);
        bool QLAlgorithm (float afDiag[3], float afSubDiag[3]);

        // support for singular value decomposition
        static const float ms_fSvdEpsilon;
        static const unsigned int ms_iSvdMaxIterations;
        static void Bidiagonalize (DiMat3& kA, DiMat3& kL,
            DiMat3& kR);
        static void GolubKahanStep (DiMat3& kA, DiMat3& kL,
            DiMat3& kR);

        // support for spectral norm
        static float MaxCubicRoot (float afCoeff[3]);

        float m[3][3];

        // for faster access
        friend class DiMat4;
    };

}

#endif
