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

#ifndef DiMeshFormat_h__
#define DiMeshFormat_h__


namespace Demi 
{
    enum DiMeshChunkID 
    {
        DI_SUBMESH                                = 0x1000, 

            DI_GEOMETRY                           = 0x1100,
                
                DI_GEOMETRY_VERTEX_DECLARATION    = 0x1200,

                DI_GEOMETRY_VERTEX_BUFFER         = 0x1300,

        DI_MESH_BOUNDS                            = 0x2000,

        DI_MESH_WEIGHTS                           = 0x3000,
    };
}

#endif
