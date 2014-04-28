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
    
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    ABGR DiColor::GetAsABGR(void) const
#else
    RGBA DiColor::GetAsRGBA(void) const
#endif
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
    //---------------------------------------------------------------------
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    BGRA DiColor::GetAsBGRA(void) const
#else
    ARGB DiColor::GetAsARGB(void) const
#endif
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
	//---------------------------------------------------------------------
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
	ARGB DiColor::GetAsARGB(void) const
#else
	BGRA DiColor::GetAsBGRA(void) const
#endif
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
    //---------------------------------------------------------------------
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    RGBA DiColor::GetAsRGBA(void) const
#else
    ABGR DiColor::GetAsABGR(void) const
#endif
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
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    void DiColor::SetAsABGR(const ABGR val)
#else
    void DiColor::SetAsRGBA(const RGBA val)
#endif
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
    //---------------------------------------------------------------------
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    void DiColor::SetAsBGRA(const BGRA val)
#else
    void DiColor::SetAsARGB(const ARGB val)
#endif
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
	//---------------------------------------------------------------------
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
	void DiColor::SetAsARGB(const ARGB val)
#else
	void DiColor::SetAsBGRA(const BGRA val)
#endif
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
    //---------------------------------------------------------------------
#if DEMI_ENDIAN == DEMI_BIG_ENDIAN
    void DiColor::SetAsRGBA(const RGBA val)
#else
    void DiColor::SetAsABGR(const ABGR val)
#endif
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