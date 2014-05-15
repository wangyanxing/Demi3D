
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

/****************************************************
  This module was originally from Particle Universe
  Repo: https://github.com/scrawl/particleuniverse
  License: MIT 
****************************************************/

#ifndef RotationSpline_h__
#define RotationSpline_h__

#include "FxPrerequisites.h"

namespace Demi
{
    class DEMI_FX_API RotationalSpline
    {
    public:
        RotationalSpline();

        ~RotationalSpline();

    public:

        void                AddPoint(const DiQuat& p);

        const DiQuat&        GetPoint(unsigned short index) const;

        unsigned short        GetNumPoints(void) const;

        void                Clear(void);

        void                UpdatePoint(unsigned short index, const DiQuat& value);

        DiQuat                 Interpolate(float t, bool useShortestPath=true);

        DiQuat                 Interpolate(unsigned int fromIndex, float t, bool useShortestPath=true);

        void                 SetAutoCalculate(bool autoCalc);

        void                 RecalcTangents(void);

    protected:

        bool                mAutoCalc;

        DiVector<DiQuat>    mPoints;

        DiVector<DiQuat>    mTangents;

    };
}

#endif // RotationSpline_h__
