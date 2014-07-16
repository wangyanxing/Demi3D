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

#ifndef DiShadows_h__
#define DiShadows_h__

#include "Texture.h"

namespace Demi
{
#define MAX_SHADOW_TEXTURES 1
    
    struct ShadowTextureConfig
	{
        ShadowTextureConfig() = default;
        
        ShadowTextureConfig(uint32 w, uint32 h, DiPixelFormat fmt)
        :width(w), height(h), format(fmt)
        {
        }
        
        ShadowTextureConfig(uint32 w, uint32 h)
        :width(w), height(h)
        {
        }
        
		uint32 width{ 512 };
		
        uint32 height{ 512 };
		
        DiPixelFormat format{ PF_X8R8G8B8 };
        
		uint16 depthBufferPoolId{ 1 };
	};
}

#endif