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

#ifndef DiK2MapLoader_h__
#define DiK2MapLoader_h__

#include "K2Prerequisites.h"

namespace Demi
{
    class DiK2BaseMapLoader
    {
    public:

        DiK2BaseMapLoader() : mWidth(0), mHeight(0) {}

        virtual void Load(DiDataStreamPtr data) = 0;

        virtual void Unload() = 0;

        uint32  GetWidth() const { return mWidth; }

        uint32  GetHeight() const { return mHeight; }

    protected:

        uint32  mWidth;

        uint32  mHeight;
    };

    /** height map class used for terrain rendering
     */
    class DEMI_K2_API DiK2HeightMap : public DiK2BaseMapLoader
    {
    public:

        DiK2HeightMap();

        ~DiK2HeightMap();

    public:

        void    Load(DiDataStreamPtr data);
        
        void    Unload();

        float*  GetBuffer() {return mBuffer;}
        
    private:
        
        float*  mBuffer;
    };

    /** height map class used for terrain rendering
    */
    struct K2TileLayer
    {
        K2TileLayer() : value(0), diffuseID(0), normalID(0)
        {}
        uint16 value;
        uint16 diffuseID;
        uint16 normalID;
    };
    
    class DEMI_K2_API DiK2TileMap : public DiK2BaseMapLoader
    {
    public:

        DiK2TileMap();

        ~DiK2TileMap();

    public:

        void            Load(DiDataStreamPtr data);

        /** Load with original values
         */
        void            Load(uint32 width, uint32 height);

        void            Unload();

        K2TileLayer*    GetBuffer(int layer) { return mBuffer[layer]; }

    private:

        K2TileLayer*    mBuffer[2];
    };

    /** terrain vertex color map
    */
    class DEMI_K2_API DiK2VertexColorMap : public DiK2BaseMapLoader
    {
    public:

        DiK2VertexColorMap();

        ~DiK2VertexColorMap();

    public:

        void    Load(DiDataStreamPtr data);

        void    Unload();

        uint32* GetBuffer() { return mBuffer; }

    private:

        uint32* mBuffer;
    };
}

#endif