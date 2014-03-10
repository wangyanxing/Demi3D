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

#ifndef DiQ3BspPatch_h__
#define DiQ3BspPatch_h__

namespace Demi
{
    /** Bsp vertex
     */
    class DiQ3BspVertex
    {
    public:

        DiQ3BspVertex();

        DiQ3BspVertex(float p[3], float texcoord[2][2], float n[3]);

        DiQ3BspVertex operator + (const DiQ3BspVertex& a);

        DiQ3BspVertex operator * (float a);

    public:

        DiVec3 mPosition;

        DiVec2 mTexcoord[2];

        DiVec3 mNormal;
    };

    /** Bezier patch for BSP rendering
     */
    class DiQ3Bezier
    {
    public:
        DiQ3Bezier();

        ~DiQ3Bezier();

        void Tessellate(int level);

        DiQ3BspVertex mControls[9];
        DiQ3BspVertex *mVertex;

        unsigned int *mIndex;
        unsigned int *mTrianglesPerRow;
        unsigned int *mRowIndex;
        unsigned int mNumIndex;
        unsigned int mNumVertex;

        int mBaseBufferindex;
        int mBaseVertexIndex;
    };

    struct Q3BspPatch
    {
        int size;
        DiQ3Bezier *bezier;
    };

    struct Q3BspFaceRenderer
    {
        int shader;
        int type;
        int vertex;
        int n_vertexes;
        int meshvert;
        int n_meshverts;
        int n_triangles;
        int lm_index;
        float normal[3];
        Q3BspPatch *patch;
    };
}

#endif
