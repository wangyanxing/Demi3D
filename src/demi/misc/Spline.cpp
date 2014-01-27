#include "MiscPch.h"
#include "Spline.h"

namespace Demi
{
    DiSpline::DiSpline()
    {
        mCoeffs[0][0] = 2;
        mCoeffs[0][1] = -2;
        mCoeffs[0][2] = 1;
        mCoeffs[0][3] = 1;
        mCoeffs[1][0] = -3;
        mCoeffs[1][1] = 3;
        mCoeffs[1][2] = -2;
        mCoeffs[1][3] = -1;
        mCoeffs[2][0] = 0;
        mCoeffs[2][1] = 0;
        mCoeffs[2][2] = 1;
        mCoeffs[2][3] = 0;
        mCoeffs[3][0] = 1;
        mCoeffs[3][1] = 0;
        mCoeffs[3][2] = 0;
        mCoeffs[3][3] = 0;

        mAutoCalc = true;
    }
    
    DiSpline::~DiSpline()
    {
    }
    
    void DiSpline::AddPoint(const DiVec3& p)
    {
        mPoints.push_back(p);
        if (mAutoCalc)
        {
            RecalcTangents();
        }
    }
    
    DiVec3 DiSpline::Interpolate(float t) const
    {
        float fSeg = t * (mPoints.size() - 1);
        unsigned int segIdx = (unsigned int)fSeg;
        t = fSeg - segIdx;

        return Interpolate(segIdx, t);
    }
    
    DiVec3 DiSpline::Interpolate(unsigned int fromIndex, float t) const
    {
        DI_ASSERT (fromIndex < mPoints.size());

        if ((fromIndex + 1) == mPoints.size())
        {
            return mPoints[fromIndex];

        }

        if (t == 0.0f)
        {
            return mPoints[fromIndex];
        }
        else if(t == 1.0f)
        {
            return mPoints[fromIndex + 1];
        }

        float t2, t3;
        t2 = t * t;
        t3 = t2 * t;
        DiVec4 powers(t3, t2, t, 1);

        const DiVec3& point1 = mPoints[fromIndex];
        const DiVec3& point2 = mPoints[fromIndex+1];
        const DiVec3& tan1 = mTangents[fromIndex];
        const DiVec3& tan2 = mTangents[fromIndex+1];
        DiMat4 pt;

        pt[0][0] = point1.x;
        pt[0][1] = point1.y;
        pt[0][2] = point1.z;
        pt[0][3] = 1.0f;
        pt[1][0] = point2.x;
        pt[1][1] = point2.y;
        pt[1][2] = point2.z;
        pt[1][3] = 1.0f;
        pt[2][0] = tan1.x;
        pt[2][1] = tan1.y;
        pt[2][2] = tan1.z;
        pt[2][3] = 1.0f;
        pt[3][0] = tan2.x;
        pt[3][1] = tan2.y;
        pt[3][2] = tan2.z;
        pt[3][3] = 1.0f;

        DiVec4 ret = powers * mCoeffs * pt;

        return DiVec3(ret.x, ret.y, ret.z);
    }
    
    void DiSpline::RecalcTangents(void)
    {
        // tangent[i] = 0.5 * (point[i+1] - point[i-1])
        size_t i, numPoints;
        bool isClosed;

        numPoints = mPoints.size();
        if (numPoints < 2)
        {
            return;
        }

        if (mPoints[0] == mPoints[numPoints-1])
        {
            isClosed = true;
        }
        else
        {
            isClosed = false;
        }

        mTangents.resize(numPoints);

        for(i = 0; i < numPoints; ++i)
        {
            if (i ==0)
            {
                if (isClosed)
                {
                    mTangents[i] = 0.5 * (mPoints[1] - mPoints[numPoints-2]);
                }
                else
                {
                    mTangents[i] = 0.5 * (mPoints[1] - mPoints[0]);
                }
            }
            else if (i == numPoints-1)
            {
                if (isClosed)
                {
                    mTangents[i] = mTangents[0];
                }
                else
                {
                    mTangents[i] = 0.5 * (mPoints[i] - mPoints[i-1]);
                }
            }
            else
            {
                mTangents[i] = 0.5 * (mPoints[i+1] - mPoints[i-1]);
            }
        }
    }
    
    const DiVec3& DiSpline::GetPoint(unsigned short index) const
    {
        assert (index < mPoints.size() && "Point index is out of bounds!!");

        return mPoints[index];
    }
    
    unsigned short DiSpline::GetNumPoints(void) const
    {
        return (unsigned short)mPoints.size();
    }
    
    void DiSpline::Clear(void)
    {
        mPoints.clear();
        mTangents.clear();
    }
    
    void DiSpline::UpdatePoint(unsigned short index, const DiVec3& value)
    {
        assert (index < mPoints.size() && "Point index is out of bounds!!");

        mPoints[index] = value;
        if (mAutoCalc)
        {
            RecalcTangents();
        }
    }
    
    void DiSpline::SetAutoCalculate(bool autoCalc)
    {
        mAutoCalc = autoCalc;
    }
}

