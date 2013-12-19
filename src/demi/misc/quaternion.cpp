
#include "quaternion.h"
#include "vector3.h"
#include "matrix3.h"

namespace Demi 
{

    const float DiQuat::ms_fEpsilon = (float)1e-03;
    const DiQuat DiQuat::ZERO(0.0,0.0,0.0,0.0);
    const DiQuat DiQuat::IDENTITY(1.0,0.0,0.0,0.0);

    //-----------------------------------------------------------------------
    void DiQuat::FromRotationMatrix (const DiMat3& kRot)
    {
        // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
        // article "Quaternion Calculus and Fast Animation".

        float fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
        float fRoot;

        if ( fTrace > 0.0 )
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = DiMath::Sqrt(fTrace + 1.0f);  // 2w
            w = 0.5f*fRoot;
            fRoot = 0.5f/fRoot;  // 1/(4w)
            x = (kRot[2][1]-kRot[1][2])*fRoot;
            y = (kRot[0][2]-kRot[2][0])*fRoot;
            z = (kRot[1][0]-kRot[0][1])*fRoot;
        }
        else
        {
            // |w| <= 1/2
            static size_t s_iNext[3] = { 1, 2, 0 };
            size_t i = 0;
            if ( kRot[1][1] > kRot[0][0] )
                i = 1;
            if ( kRot[2][2] > kRot[i][i] )
                i = 2;
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            fRoot = DiMath::Sqrt(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
            float* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i] = 0.5f*fRoot;
            fRoot = 0.5f/fRoot;
            w = (kRot[k][j]-kRot[j][k])*fRoot;
            *apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
            *apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
        }
    }
    //-----------------------------------------------------------------------
    void DiQuat::ToRotationMatrix (DiMat3& kRot) const
    {
        float fTx  = x+x;
        float fTy  = y+y;
        float fTz  = z+z;
        float fTwx = fTx*w;
        float fTwy = fTy*w;
        float fTwz = fTz*w;
        float fTxx = fTx*x;
        float fTxy = fTy*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTyz = fTz*y;
        float fTzz = fTz*z;

        kRot[0][0] = 1.0f-(fTyy+fTzz);
        kRot[0][1] = fTxy-fTwz;
        kRot[0][2] = fTxz+fTwy;
        kRot[1][0] = fTxy+fTwz;
        kRot[1][1] = 1.0f-(fTxx+fTzz);
        kRot[1][2] = fTyz-fTwx;
        kRot[2][0] = fTxz-fTwy;
        kRot[2][1] = fTyz+fTwx;
        kRot[2][2] = 1.0f-(fTxx+fTyy);
    }
    //-----------------------------------------------------------------------
    void DiQuat::FromAngleAxis (const DiRadian& rfAngle,
        const DiVec3& rkAxis)
    {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        DiRadian fHalfAngle ( rfAngle*0.5 );
        float fSin = DiMath::Sin(fHalfAngle);
        w = DiMath::Cos(fHalfAngle);
        x = fSin*rkAxis.x;
        y = fSin*rkAxis.y;
        z = fSin*rkAxis.z;
    }
    //-----------------------------------------------------------------------
    void DiQuat::ToAngleAxis (DiRadian& rfAngle, DiVec3& rkAxis) const
    {
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        float fSqrLength = x*x+y*y+z*z;
        if ( fSqrLength > 0.0 )
        {
            rfAngle = DiMath::ACos(w) * 2.0f;
            float fInvLength = DiMath::InvSqrt(fSqrLength);
            rkAxis.x = x*fInvLength;
            rkAxis.y = y*fInvLength;
            rkAxis.z = z*fInvLength;
        }
        else
        {
            // angle is 0 (mod 2*pi), so any axis will do
            rfAngle = DiRadian(0.0);
            rkAxis.x = 1.0;
            rkAxis.y = 0.0;
            rkAxis.z = 0.0;
        }
    }
    //-----------------------------------------------------------------------
    void DiQuat::FromAxes (const DiVec3* akAxis)
    {
        DiMat3 kRot;

        for (size_t iCol = 0; iCol < 3; iCol++)
        {
            kRot[0][iCol] = akAxis[iCol].x;
            kRot[1][iCol] = akAxis[iCol].y;
            kRot[2][iCol] = akAxis[iCol].z;
        }

        FromRotationMatrix(kRot);
    }
    //-----------------------------------------------------------------------
    void DiQuat::FromAxes (const DiVec3& xaxis, const DiVec3& yaxis, const DiVec3& zaxis)
    {
        DiMat3 kRot;

        kRot[0][0] = xaxis.x;
        kRot[1][0] = xaxis.y;
        kRot[2][0] = xaxis.z;

        kRot[0][1] = yaxis.x;
        kRot[1][1] = yaxis.y;
        kRot[2][1] = yaxis.z;

        kRot[0][2] = zaxis.x;
        kRot[1][2] = zaxis.y;
        kRot[2][2] = zaxis.z;

        FromRotationMatrix(kRot);

    }
    //-----------------------------------------------------------------------
    void DiQuat::ToAxes (DiVec3* akAxis) const
    {
        DiMat3 kRot;

        ToRotationMatrix(kRot);

        for (size_t iCol = 0; iCol < 3; iCol++)
        {
            akAxis[iCol].x = kRot[0][iCol];
            akAxis[iCol].y = kRot[1][iCol];
            akAxis[iCol].z = kRot[2][iCol];
        }
    }
    //-----------------------------------------------------------------------
    DiVec3 DiQuat::xAxis(void) const
    {
        //float fTx  = 2.0*x;
        float fTy  = 2.0f*y;
        float fTz  = 2.0f*z;
        float fTwy = fTy*w;
        float fTwz = fTz*w;
        float fTxy = fTy*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTzz = fTz*z;

        return DiVec3(1.0f-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);
    }
    //-----------------------------------------------------------------------
    DiVec3 DiQuat::yAxis(void) const
    {
        float fTx  = 2.0f*x;
        float fTy  = 2.0f*y;
        float fTz  = 2.0f*z;
        float fTwx = fTx*w;
        float fTwz = fTz*w;
        float fTxx = fTx*x;
        float fTxy = fTy*x;
        float fTyz = fTz*y;
        float fTzz = fTz*z;

        return DiVec3(fTxy-fTwz, 1.0f-(fTxx+fTzz), fTyz+fTwx);
    }
    //-----------------------------------------------------------------------
    DiVec3 DiQuat::zAxis(void) const
    {
        float fTx  = 2.0f*x;
        float fTy  = 2.0f*y;
        float fTz  = 2.0f*z;
        float fTwx = fTx*w;
        float fTwy = fTy*w;
        float fTxx = fTx*x;
        float fTxz = fTz*x;
        float fTyy = fTy*y;
        float fTyz = fTz*y;

        return DiVec3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
    }
    //-----------------------------------------------------------------------
    void DiQuat::ToAxes (DiVec3& xaxis, DiVec3& yaxis, DiVec3& zaxis) const
    {
        DiMat3 kRot;

        ToRotationMatrix(kRot);

        xaxis.x = kRot[0][0];
        xaxis.y = kRot[1][0];
        xaxis.z = kRot[2][0];

        yaxis.x = kRot[0][1];
        yaxis.y = kRot[1][1];
        yaxis.z = kRot[2][1];

        zaxis.x = kRot[0][2];
        zaxis.y = kRot[1][2];
        zaxis.z = kRot[2][2];
    }

    //-----------------------------------------------------------------------
    DiQuat DiQuat::operator+ (const DiQuat& rkQ) const
    {
        return DiQuat(w+rkQ.w,x+rkQ.x,y+rkQ.y,z+rkQ.z);
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::operator- (const DiQuat& rkQ) const
    {
        return DiQuat(w-rkQ.w,x-rkQ.x,y-rkQ.y,z-rkQ.z);
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::operator* (const DiQuat& rkQ) const
    {
        // NOTE:  Multiplication is not generally commutative, so in most
        // cases p*q != q*p.

        return DiQuat
            (
            w * rkQ.w - x * rkQ.x - y * rkQ.y - z * rkQ.z,
            w * rkQ.x + x * rkQ.w + y * rkQ.z - z * rkQ.y,
            w * rkQ.y + y * rkQ.w + z * rkQ.x - x * rkQ.z,
            w * rkQ.z + z * rkQ.w + x * rkQ.y - y * rkQ.x
            );
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::operator* (float fScalar) const
    {
        return DiQuat(fScalar*w,fScalar*x,fScalar*y,fScalar*z);
    }
    //-----------------------------------------------------------------------
    DiQuat operator* (float fScalar, const DiQuat& rkQ)
    {
        return DiQuat(fScalar*rkQ.w,fScalar*rkQ.x,fScalar*rkQ.y,
            fScalar*rkQ.z);
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::operator- () const
    {
        return DiQuat(-w,-x,-y,-z);
    }
    //-----------------------------------------------------------------------
    float DiQuat::Dot (const DiQuat& rkQ) const
    {
        return w*rkQ.w+x*rkQ.x+y*rkQ.y+z*rkQ.z;
    }
    //-----------------------------------------------------------------------
    float DiQuat::Norm () const
    {
        return w*w+x*x+y*y+z*z;
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::Inverse () const
    {
        float fNorm = w*w+x*x+y*y+z*z;
        if ( fNorm > 0.0 )
        {
            float fInvNorm = 1.0f/fNorm;
            return DiQuat(w*fInvNorm,-x*fInvNorm,-y*fInvNorm,-z*fInvNorm);
        }
        else
        {
            return ZERO;
        }
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::UnitInverse () const
    {
        // assert:  'this' is unit length
        return DiQuat(w,-x,-y,-z);
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::Exp () const
    {
        // If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is near zero,
        // use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

        DiRadian fAngle ( DiMath::Sqrt(x*x+y*y+z*z) );
        float fSin = DiMath::Sin(fAngle);

        DiQuat kResult;
        kResult.w = DiMath::Cos(fAngle);

        if ( fabs(fSin) >= ms_fEpsilon )
        {
            float fCoeff = fSin/(fAngle.valueRadians());
            kResult.x = fCoeff*x;
            kResult.y = fCoeff*y;
            kResult.z = fCoeff*z;
        }
        else
        {
            kResult.x = x;
            kResult.y = y;
            kResult.z = z;
        }

        return kResult;
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::Log () const
    {
        // If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
        // log(q) = A*(x*i+y*j+z*k).  If sin(A) is near zero, use log(q) =
        // sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

        DiQuat kResult;
        kResult.w = 0.0;

        if ( fabs(w) < 1.0 )
        {
            DiRadian fAngle ( DiMath::ACos(w) );
            float fSin = DiMath::Sin(fAngle);
            if ( fabs(fSin) >= ms_fEpsilon )
            {
                float fCoeff = fAngle.valueRadians()/fSin;
                kResult.x = fCoeff*x;
                kResult.y = fCoeff*y;
                kResult.z = fCoeff*z;
                return kResult;
            }
        }

        kResult.x = x;
        kResult.y = y;
        kResult.z = z;

        return kResult;
    }
    //-----------------------------------------------------------------------
    DiVec3 DiQuat::operator* (const DiVec3& v) const
    {
        // nVidia SDK implementation
        DiVec3 uv, uuv;
        DiVec3 qvec(x, y, z);
        uv = qvec.crossProduct(v);
        uuv = qvec.crossProduct(uv);
        uv *= (2.0f * w);
        uuv *= 2.0f;

        return v + uv + uuv;

    }
    //-----------------------------------------------------------------------
    bool DiQuat::equals(const DiQuat& rhs, const DiRadian& tolerance) const
    {
        float fCos = Dot(rhs);
        DiRadian angle = DiRadian(DiMath::ACos(fCos));

        return (fabs(angle.valueRadians()) <= tolerance.valueRadians())
            || DiMath::RealEqual(angle.valueRadians(), DiMath::PI, tolerance.valueRadians());


    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::Slerp (float fT, const DiQuat& rkP,
        const DiQuat& rkQ, bool shortestPath)
    {
        float fCos = rkP.Dot(rkQ);
        DiQuat rkT;

        // Do we need to invert rotation?
        if (fCos < 0.0f && shortestPath)
        {
            fCos = -fCos;
            rkT = -rkQ;
        }
        else
        {
            rkT = rkQ;
        }

        if (fabs(fCos) < 1 - ms_fEpsilon)
        {
            // Standard case (slerp)
            float fSin = DiMath::Sqrt(1 - DiMath::Sqr(fCos));
            DiRadian fAngle = DiRadian(DiMath::ATan2(fSin, fCos));
            float fInvSin = 1.0f / fSin;
            float fCoeff0 = DiMath::Sin(fAngle * (1.0f - fT)) * fInvSin;
            float fCoeff1 = DiMath::Sin(fAngle * fT) * fInvSin;
            return fCoeff0 * rkP + fCoeff1 * rkT;
        }
        else
        {
            // There are two situations:
            // 1. "rkP" and "rkQ" are very close (fCos ~= +1), so we can do a linear
            //    interpolation safely.
            // 2. "rkP" and "rkQ" are almost inverse of each other (fCos ~= -1), there
            //    are an infinite number of possibilities interpolation. but we haven't
            //    have method to fix this case, so just use linear interpolation here.
            DiQuat t = (1.0f - fT) * rkP + fT * rkT;
            // taking the complement requires renormalisation
            t.normalise();
            return t;
        }
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::SlerpExtraSpins (float fT,
        const DiQuat& rkP, const DiQuat& rkQ, int iExtraSpins)
    {
        float fCos = rkP.Dot(rkQ);
        DiRadian fAngle ( DiMath::ACos(fCos) );

        if ( fabs(fAngle.valueRadians()) < ms_fEpsilon )
            return rkP;

        float fSin = DiMath::Sin(fAngle);
        DiRadian fPhase ( DiMath::PI*iExtraSpins*fT );
        float fInvSin = 1.0f/fSin;
        float fCoeff0 = DiMath::Sin(fAngle*(1.0f-fT) - fPhase)*fInvSin;
        float fCoeff1 = DiMath::Sin(fAngle*fT + fPhase)*fInvSin;
        return fCoeff0*rkP + fCoeff1*rkQ;
    }
    //-----------------------------------------------------------------------
    void DiQuat::Intermediate (const DiQuat& rkQ0,
        const DiQuat& rkQ1, const DiQuat& rkQ2,
        DiQuat& rkA, DiQuat& rkB)
    {
        // assert:  q0, q1, q2 are unit quaternions

        DiQuat kQ0inv = rkQ0.UnitInverse();
        DiQuat kQ1inv = rkQ1.UnitInverse();
        DiQuat rkP0 = kQ0inv*rkQ1;
        DiQuat rkP1 = kQ1inv*rkQ2;
        DiQuat kArg = 0.25*(rkP0.Log()-rkP1.Log());
        DiQuat kMinusArg = -kArg;

        rkA = rkQ1*kArg.Exp();
        rkB = rkQ1*kMinusArg.Exp();
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::Squad (float fT,
        const DiQuat& rkP, const DiQuat& rkA,
        const DiQuat& rkB, const DiQuat& rkQ, bool shortestPath)
    {
        float fSlerpT = 2.0f*fT*(1.0f-fT);
        DiQuat kSlerpP = Slerp(fT, rkP, rkQ, shortestPath);
        DiQuat kSlerpQ = Slerp(fT, rkA, rkB);
        return Slerp(fSlerpT, kSlerpP ,kSlerpQ);
    }
    //-----------------------------------------------------------------------
    float DiQuat::normalise(void)
    {
        float len = Norm();
        float factor = 1.0f / DiMath::Sqrt(len);
        *this = *this * factor;
        return len;
    }
    //-----------------------------------------------------------------------
    DiRadian DiQuat::getRoll(bool reprojectAxis) const
    {
        if (reprojectAxis)
        {
            // roll = atan2(localx.y, localx.x)
            // pick parts of xAxis() implementation that we need
            //            float fTx  = 2.0*x;
            float fTy  = 2.0f*y;
            float fTz  = 2.0f*z;
            float fTwz = fTz*w;
            float fTxy = fTy*x;
            float fTyy = fTy*y;
            float fTzz = fTz*z;

            // Vector3(1.0-(fTyy+fTzz), fTxy+fTwz, fTxz-fTwy);

            return DiRadian(DiMath::ATan2(fTxy+fTwz, 1.0f-(fTyy+fTzz)));

        }
        else
        {
            return DiRadian(DiMath::ATan2(2*(x*y + w*z), w*w + x*x - y*y - z*z));
        }
    }
    //-----------------------------------------------------------------------
    DiRadian DiQuat::getPitch(bool reprojectAxis) const
    {
        if (reprojectAxis)
        {
            // pitch = atan2(localy.z, localy.y)
            // pick parts of yAxis() implementation that we need
            float fTx  = 2.0f*x;
            //            float fTy  = 2.0f*y;
            float fTz  = 2.0f*z;
            float fTwx = fTx*w;
            float fTxx = fTx*x;
            float fTyz = fTz*y;
            float fTzz = fTz*z;

            // Vector3(fTxy-fTwz, 1.0-(fTxx+fTzz), fTyz+fTwx);
            return DiRadian(DiMath::ATan2(fTyz+fTwx, 1.0f-(fTxx+fTzz)));
        }
        else
        {
            // internal version
            return DiRadian(DiMath::ATan2(2*(y*z + w*x), w*w - x*x - y*y + z*z));
        }
    }
    //-----------------------------------------------------------------------
    DiRadian DiQuat::getYaw(bool reprojectAxis) const
    {
        if (reprojectAxis)
        {
            // yaw = atan2(localz.x, localz.z)
            // pick parts of zAxis() implementation that we need
            float fTx  = 2.0f*x;
            float fTy  = 2.0f*y;
            float fTz  = 2.0f*z;
            float fTwy = fTy*w;
            float fTxx = fTx*x;
            float fTxz = fTz*x;
            float fTyy = fTy*y;

            // Vector3(fTxz+fTwy, fTyz-fTwx, 1.0-(fTxx+fTyy));

            return DiRadian(DiMath::ATan2(fTxz+fTwy, 1.0f-(fTxx+fTyy)));

        }
        else
        {
            // internal version
            return DiRadian(DiMath::ASin(-2*(x*z - w*y)));
        }
    }
    //-----------------------------------------------------------------------
    DiQuat DiQuat::Nlerp(float fT, const DiQuat& rkP,
        const DiQuat& rkQ, bool shortestPath)
    {
        DiQuat result;
        float fCos = rkP.Dot(rkQ);
        if (fCos < 0.0f && shortestPath)
        {
            result = rkP + fT * ((-rkQ) - rkP);
        }
        else
        {
            result = rkP + fT * (rkQ - rkP);
        }
        result.normalise();
        return result;
    }

}