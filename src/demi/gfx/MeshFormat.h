
/********************************************************************
    File:       MeshFormat.h
    Creator:    demiwangya
*********************************************************************/

#pragma once

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