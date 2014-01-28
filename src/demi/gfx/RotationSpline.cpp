
#include "GfxPch.h"
#include "RotationSpline.h"

namespace Demi
{
    RotationalSpline::RotationalSpline()
        : mAutoCalc(true)
    {
    }
    
    RotationalSpline::~RotationalSpline()
    {
    }
    
    void RotationalSpline::AddPoint(const DiQuat& p)
    {
        mPoints.push_back(p);
        if (mAutoCalc)
        {
            RecalcTangents();
        }
    }
    
    DiQuat RotationalSpline::Interpolate(float t, bool useShortestPath)
    {
        float fSeg = t * (mPoints.size() - 1);
        unsigned int segIdx = (unsigned int)fSeg;
        t = fSeg - segIdx;

        return Interpolate(segIdx, t, useShortestPath);
    }
    
    DiQuat RotationalSpline::Interpolate(unsigned int fromIndex, float t,
        bool useShortestPath)
    {
        DI_ASSERT (fromIndex >= 0 && fromIndex < mPoints.size());

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

        DiQuat &p = mPoints[fromIndex];
        DiQuat &q = mPoints[fromIndex+1];
        DiQuat &a = mTangents[fromIndex];
        DiQuat &b = mTangents[fromIndex+1];

        return DiQuat::Squad(t, p, a, b, q, useShortestPath);

    }
    
    void RotationalSpline::RecalcTangents(void)
    {
        unsigned int i, numPoints;
        bool isClosed;

        numPoints = (unsigned int)mPoints.size();

        if (numPoints < 2)
        {
            return;
        }

        mTangents.resize(numPoints);

        if (mPoints[0] == mPoints[numPoints-1])
        {
            isClosed = true;
        }
        else
        {
            isClosed = false;
        }

        DiQuat invp, part1, part2, preExp;
        for(i = 0; i < numPoints; ++i)
        {
            DiQuat &p = mPoints[i];
            invp = p.Inverse();

            if (i ==0)
            {
                part1 = (invp * mPoints[i+1]).Log();
                if (isClosed)
                {
                    part2 = (invp * mPoints[numPoints-2]).Log();
                }
                else
                {
                    part2 = (invp * p).Log();
                }
            }
            else if (i == numPoints-1)
            {
                if (isClosed)
                {
                    part1 = (invp * mPoints[1]).Log();
                }
                else
                {
                    part1 = (invp * p).Log();
                }
                part2 = (invp * mPoints[i-1]).Log();
            }
            else
            {
                part1 = (invp * mPoints[i+1]).Log();
                part2 = (invp * mPoints[i-1]).Log();
            }

            preExp = -0.25 * (part1 + part2);
            mTangents[i] = p * preExp.Exp();
        }
    }
    
    const DiQuat& RotationalSpline::GetPoint(unsigned short index) const
    {
        DI_ASSERT (index < mPoints.size());

        return mPoints[index];
    }
    
    unsigned short RotationalSpline::GetNumPoints(void) const
    {
        return (unsigned short)mPoints.size();
    }
    
    void RotationalSpline::Clear(void)
    {
        mPoints.clear();
        mTangents.clear();
    }
    
    void RotationalSpline::UpdatePoint(unsigned short index, const DiQuat& value)
    {
        DI_ASSERT (index < mPoints.size());

        mPoints[index] = value;
        if (mAutoCalc)
        {
            RecalcTangents();
        }
    }
    
    void RotationalSpline::SetAutoCalculate(bool autoCalc)
    {
        mAutoCalc = autoCalc;
    }
}