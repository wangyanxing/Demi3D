
/********************************************************************
    File:       DynamicAttribute.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

#include "Spline.h"

namespace Demi
{
    enum InterpolationType
    {
        IT_LINEAR,
        IT_SPLINE
    };

    struct ControlPointSorter
    {
        inline bool operator() (const DiVec2& a, const DiVec2& b)
        {
            return a.x < b.x;
        }
    };

    class DI_MISC_API DiDynamicAttribute
    {
    public:

        enum DynamicAttributeType
        {
            DAT_FIXED,
            DAT_RANDOM,
            DAT_CURVED
        };

        DiDynamicAttribute(){}

        virtual                    ~DiDynamicAttribute(){}

        virtual    float            GetValue(float x = 0) = 0;

        DynamicAttributeType    GetType() const{return mType;}

        void                    SetType(DynamicAttributeType type){mType = type;}

        virtual void            CopyTo(DiDynamicAttribute* att) = 0;

        bool                    IsValueChangedExternally() const;

        DynamicAttributeType    mType;

        bool                    mValueChangedExternally;
    };
    
    //////////////////////////////////////////////////////////////////////////

    class DI_MISC_API DiAttributeFixed : public DiDynamicAttribute
    {
    public:

        DiAttributeFixed();

        DiAttributeFixed(const DiAttributeFixed& rhs);

        virtual                    ~DiAttributeFixed();

    public:

        virtual    float            GetValue(float x = 0);

        virtual void            SetValue(float value);

        virtual void            CopyTo(DiDynamicAttribute* att);

    protected:

        float                    mValue;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_MISC_API DiAttributeRandom : public DiDynamicAttribute
    {
    public:

        DiAttributeRandom();

        DiAttributeRandom(const DiAttributeRandom& rhs);

        virtual                    ~DiAttributeRandom();

    public:

        virtual    float            GetValue(float x = 0);

        void                    SetMin (float min);

        float                    GetMin (void) const;

        void                    SetMax (float max);

        float                    GetMax (void) const;

        void                    SetMinMax (float min, float max);

        virtual void            CopyTo(DiDynamicAttribute* att);

    protected:

        float                    mMin;

        float                    mMax;
    };

    //////////////////////////////////////////////////////////////////////////

    class DI_MISC_API DiAttributeCurved : public DiDynamicAttribute
    {
    public:

        typedef DiVector<DiVec2> ControlPointList;

    public:

        DiAttributeCurved (void);

        DiAttributeCurved (InterpolationType interpolationType);

        DiAttributeCurved (const DiAttributeCurved& rhs);

    public:

        void                    SetInterpolationType (InterpolationType interpolationType);

        InterpolationType        GetInterpolationType (void) const;

        virtual float            GetValue (float x = 0);

        virtual void            AddControlPoint (float x, float y);

        const ControlPointList& GetControlPoints (void) const;

        void                    ProcessControlPoints (void);

        size_t                    GetNumControlPoints(void) const;

        void                    RemoveAllControlPoints(void);

        virtual void            CopyTo(DiDynamicAttribute* dynamicAttribute);

    protected:

        float                    mRange;

        DiSpline                mSpline;

        InterpolationType        mInterpolationType;

        ControlPointList        mControlPoints;

    protected:

        inline ControlPointList::iterator FindNearestControlPointIterator(float x)
        {
            ControlPointList::iterator it;
            ControlPointList::iterator itEnd = mControlPoints.end();
            for (it = mControlPoints.begin(); it != itEnd; ++it)
            {
                if (x < (*it).x)
                {
                    if (it == mControlPoints.begin())
                    {
                        return it;
                    }
                    else
                    {
                        return --it;
                    }
                }
            }

            return --it;
        }

        inline ControlPointList::iterator GetFirstValidIterator(void)
        {
            return mControlPoints.begin();
        }

        inline ControlPointList::iterator GetLastValidIterator(void)
        {
            return mControlPoints.end() - 1;
        }
    };

    class DI_MISC_API DiDynamicAttributeHelper
    {
    public:
        float Calculate(DiDynamicAttribute* dyn, float x,float defaultValue = 0.0f);
    };

}