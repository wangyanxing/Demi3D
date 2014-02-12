


#ifndef Dicolor_h__
#define Dicolor_h__


#include "MiscPrerequisites.h"

namespace Demi 
{
    typedef uint32 RGBA;
    typedef uint32 ARGB;
    typedef uint32 ABGR;
    typedef uint32 BGRA;

    class DI_MISC_API DiColor
    {
    public:
        static const DiColor ZERO;
        static const DiColor Black;
        static const DiColor White;
        static const DiColor Yellow;
        static const DiColor Orange;
        static const DiColor Red;
        static const DiColor Green;
        static const DiColor Blue;
        static const DiColor Purple;
        static const DiColor LightBlue;

        explicit DiColor( float red = 1.0f,
            float green = 1.0f,
            float blue = 1.0f,
            float alpha = 1.0f ) : r(red), g(green), b(blue), a(alpha)
        { }

        bool operator==(const DiColor& rhs) const;
        bool operator!=(const DiColor& rhs) const;

        float r,g,b,a;

        RGBA GetAsRGBA(void) const;

        ARGB GetAsARGB(void) const;

        BGRA GetAsBGRA(void) const;

        ABGR GetAsABGR(void) const;

        void SetAsRGBA(const RGBA val);

        void SetAsARGB(const ARGB val);

        void SetAsBGRA(const BGRA val);

        void SetAsABGR(const ABGR val);

        void Saturate(void)
        {
            if (r < 0)
                r = 0;
            else if (r > 1)
                r = 1;

            if (g < 0)
                g = 0;
            else if (g > 1)
                g = 1;

            if (b < 0)
                b = 0;
            else if (b > 1)
                b = 1;

            if (a < 0)
                a = 0;
            else if (a > 1)
                a = 1;
        }

        DiColor SaturateCopy(void) const
        {
            DiColor ret = *this;
            ret.Saturate();
            return ret;
        }

        inline float operator [] ( const size_t i ) const
        {
            DI_ASSERT( i < 4 );

            return *(&r+i);
        }

        inline float& operator [] ( const size_t i )
        {
            DI_ASSERT( i < 4 );

            return *(&r+i);
        }

        inline float* Ptr()
        {
            return &r;
        }
        
        inline const float* Ptr() const
        {
            return &r;
        }

        inline DiColor operator + ( const DiColor& rkVector ) const
        {
            DiColor kSum;

            kSum.r = r + rkVector.r;
            kSum.g = g + rkVector.g;
            kSum.b = b + rkVector.b;
            kSum.a = a + rkVector.a;

            return kSum;
        }

        inline DiColor operator - ( const DiColor& rkVector ) const
        {
            DiColor kDiff;

            kDiff.r = r - rkVector.r;
            kDiff.g = g - rkVector.g;
            kDiff.b = b - rkVector.b;
            kDiff.a = a - rkVector.a;

            return kDiff;
        }

        inline DiColor operator * (const float fScalar ) const
        {
            DiColor kProd;

            kProd.r = fScalar*r;
            kProd.g = fScalar*g;
            kProd.b = fScalar*b;
            kProd.a = fScalar*a;

            return kProd;
        }

        inline DiColor operator * ( const DiColor& rhs) const
        {
            DiColor kProd;

            kProd.r = rhs.r * r;
            kProd.g = rhs.g * g;
            kProd.b = rhs.b * b;
            kProd.a = rhs.a * a;

            return kProd;
        }

        inline DiColor operator / ( const DiColor& rhs) const
        {
            DiColor kProd;

            kProd.r = rhs.r / r;
            kProd.g = rhs.g / g;
            kProd.b = rhs.b / b;
            kProd.a = rhs.a / a;

            return kProd;
        }

        inline DiColor operator / (const float fScalar ) const
        {
            DI_ASSERT( fScalar != 0.0 );

            DiColor kDiv;

            float fInv = 1.0f / fScalar;
            kDiv.r = r * fInv;
            kDiv.g = g * fInv;
            kDiv.b = b * fInv;
            kDiv.a = a * fInv;

            return kDiv;
        }

        inline friend DiColor operator * (const float fScalar, const DiColor& rkVector )
        {
            DiColor kProd;

            kProd.r = fScalar * rkVector.r;
            kProd.g = fScalar * rkVector.g;
            kProd.b = fScalar * rkVector.b;
            kProd.a = fScalar * rkVector.a;

            return kProd;
        }

        inline DiColor& operator += ( const DiColor& rkVector )
        {
            r += rkVector.r;
            g += rkVector.g;
            b += rkVector.b;
            a += rkVector.a;

            return *this;
        }

        inline DiColor& operator -= ( const DiColor& rkVector )
        {
            r -= rkVector.r;
            g -= rkVector.g;
            b -= rkVector.b;
            a -= rkVector.a;

            return *this;
        }

        inline DiColor& operator *= (const float fScalar )
        {
            r *= fScalar;
            g *= fScalar;
            b *= fScalar;
            a *= fScalar;
            return *this;
        }

        inline DiColor& operator /= (const float fScalar )
        {
            DI_ASSERT( fScalar != 0.0 );

            float fInv = 1.0f / fScalar;

            r *= fInv;
            g *= fInv;
            b *= fInv;
            a *= fInv;

            return *this;
        }

    };

}

#endif
