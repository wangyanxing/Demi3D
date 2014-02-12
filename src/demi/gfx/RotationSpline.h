


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
