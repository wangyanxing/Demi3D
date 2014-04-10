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

/// This file is adapted from Ogre 2.0 (working version)

namespace Demi
{
    inline Arrayfloat MathlibC::Modf4( Arrayfloat x, Arrayfloat &outIntegral )
    {
#if DEMI_PLATFORM == DEMI_PLATFORM_ANDROID
        double _outIntegral;
        Arrayfloat fractpart = modf( x, &_outIntegral );
        outIntegral = static_cast<Arrayfloat>( _outIntegral );
        return fractpart;
#else
        return modff( x, &outIntegral );
#endif
    }
    
    inline Arrayfloat MathlibC::ACos4( Arrayfloat x)
    {
        return acos( x );
    }
    
    inline Arrayfloat MathlibC::Sin4( Arrayfloat x )
    {
        return sin( x );
    }
    
    inline Arrayfloat MathlibC::Cos4( Arrayfloat x )
    {
        return cos( x );
    }
    
    inline void MathlibC::SinCos4( Arrayfloat x, Arrayfloat &outSin, Arrayfloat &outCos )
    {
        outSin = sin( x );
        outCos = cos( x );
    }
}
