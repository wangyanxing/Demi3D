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

#ifndef DiQ3BspShader_h__
#define DiQ3BspShader_h__

#include "Q3BspFormats.h"

namespace Demi
{
    /** Q3 BSP Shader pass
     */
    struct Q3ShaderPass
    {
        uint32      flags;
        DiString    textureName;
        TexGen      texGen;
        
        DiBlendMode blendMode;
        
        bool        customBlend;
        DiCompFunc  depthFunc;
        DiAddMode   addressMode;
        
        // TODO - alphaFunc
        GenFunc     rgbGenFunc;
        WaveType    rgbGenWave;
        float       rgbGenParams[4];    // base, amplitude, phase, frequency
        float       tcModScale[2];
        float       tcModRotate;
        float       tcModScroll[2];
        float       tcModTransform[6];
        bool        tcModTurbOn;
        float       tcModTurb[4];
        WaveType    tcModStretchWave;
        float       tcModStretchParams[4];    // base, amplitude, phase, frequency
        
        DiCompFunc  alphaFunc;
        uint8       alphaVal;
        
        float       animFps;
        uint32      animNumFrames;
        
        DiString    frames[32];
    };
    
    /** Quake3 material
        Yes they called it shader
     */
	class DiQ3Material
    {
    public:
        
        DiQ3Material(const DiString& name);
        
        ~DiQ3Material();
        
    public:
        
        DiMaterialPtr CreateAsMaterial(int lightmapNumber, DiTexturePtr lightmap);
        
    public:
        
        uint32     flags;
        int        numPasses;
		
        typedef DiVector<Q3ShaderPass> PassList;
        PassList   pass;
        
        bool       farbox;            // Skybox
        DiString   farboxName;
        bool       skyDome;
        float      cloudHeight;       // Skydome
        
        DeformFunc deformFunc;
        float      deformParams[5];
        
        //ManualCullingMode cullMode;
        
        bool       fog;
        DiColor    fogColour;
        float      fogDistance;

    protected:

        /** Get alternative JPG to TGA and vice versa
         */
        DiString    GetAlternateTextureName(const DiString& texName);
        
        DiString    mName;
    };
}

#endif
