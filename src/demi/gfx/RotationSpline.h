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

#ifndef DiRotationSpline_h__
#define DiRotationSpline_h__


namespace Demi
{
    class DI_GFX_API RotationalSpline
    {
    public:
        RotationalSpline();

        ~RotationalSpline();

    public:

        void                AddPoint(const DiQuat& p);

        const DiQuat&       GetPoint(unsigned short index) const;

        unsigned short      GetNumPoints(void) const;

        void                Clear(void);

        void                UpdatePoint(unsigned short index, const DiQuat& value);

        DiQuat              Interpolate(float t, bool useShortestPath=true);

        DiQuat              Interpolate(unsigned int fromIndex, float t, bool useShortestPath=true);

        void                SetAutoCalculate(bool autoCalc);

        void                RecalcTangents(void);

    protected:

        bool                mAutoCalc;

        DiVector<DiQuat>    mPoints;

        DiVector<DiQuat>    mTangents;

    };
}

#endif
