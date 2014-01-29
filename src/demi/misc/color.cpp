
#include "MiscPch.h"
#include "color.h"

namespace Demi 
{
    const DiColor DiColor::ZERO      = DiColor(0.0,0.0,0.0,0.0);
    const DiColor DiColor::Black     = DiColor(0.0,0.0,0.0);
    const DiColor DiColor::White     = DiColor(1.0,1.0,1.0);
    const DiColor DiColor::Yellow    = DiColor(1.0,1.0,0.0);
    const DiColor DiColor::Orange    = DiColor(1.0f,0.55f,0.1f);
    const DiColor DiColor::Red       = DiColor(1.0,0.0,0.0);
    const DiColor DiColor::Green     = DiColor(0.0,1.0,0.0);
    const DiColor DiColor::Blue      = DiColor(0.0,0.0,1.0);
    const DiColor DiColor::Purple    = DiColor(1.0,0.0,1.0);
    const DiColor DiColor::LightBlue = DiColor(0.3f,1.0f,1.0f);

    RGBA DiColor::GetAsRGBA(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (RGBA = 8888)

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 = val8 << 24;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 16;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8 << 8;

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 += val8;

        return val32;
    }

    ARGB DiColor::GetAsARGB(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ARGB = 8888)

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 = val8 << 24;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8 << 16;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 8;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8;

        return val32;
    }

    BGRA DiColor::GetAsBGRA(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ARGB = 8888)

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 = val8 << 24;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 16;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8 << 8;

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 += val8;


        return val32;
    }

    ABGR DiColor::GetAsABGR(void) const
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ABRG = 8888)

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 = val8 << 24;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8 << 16;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 8;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8;


        return val32;
    }
    //---------------------------------------------------------------------
    void DiColor::SetAsRGBA(const RGBA val)
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (RGBA = 8888)

        // Red
        r = ((val32 >> 24) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 16) & 0xFF) / 255.0f;

        // Blue
        b = ((val32 >> 8) & 0xFF) / 255.0f;

        // Alpha
        a = (val32 & 0xFF) / 255.0f;
    }

    void DiColor::SetAsARGB(const ARGB val)

    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ARGB = 8888)

        // Alpha
        a = ((val32 >> 24) & 0xFF) / 255.0f;

        // Red
        r = ((val32 >> 16) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 8) & 0xFF) / 255.0f;

        // Blue
        b = (val32 & 0xFF) / 255.0f;
    }

    void DiColor::SetAsBGRA(const BGRA val)
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ARGB = 8888)

        // Blue
        b = ((val32 >> 24) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 16) & 0xFF) / 255.0f;

        // Red
        r = ((val32 >> 8) & 0xFF) / 255.0f;

        // Alpha
        a = (val32 & 0xFF) / 255.0f;
    }

    void DiColor::SetAsABGR(const ABGR val)
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ABGR = 8888)

        // Alpha
        a = ((val32 >> 24) & 0xFF) / 255.0f;

        // Blue
        b = ((val32 >> 16) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 8) & 0xFF) / 255.0f;

        // Red
        r = (val32 & 0xFF) / 255.0f;
    }

    bool DiColor::operator==(const DiColor& rhs) const
    {
        return (r == rhs.r &&
            g == rhs.g &&
            b == rhs.b &&
            a == rhs.a);
    }

    bool DiColor::operator!=(const DiColor& rhs) const
    {
        return !(*this == rhs);
    }
}