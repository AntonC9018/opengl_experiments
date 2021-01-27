#pragma once
#include "defines.h"
#include "descriptors/generic.h"
#include "descriptors/grid.h"

struct Pawn_Vao
{
    u32 id;
    s32 num_triangles;
    Generic_Program program;
    
    inline void bind()
    {
        glBindVertexArray(id);
        program.use();
    };
    void load_shaders(Camera_View_Projection_Block vp_block);
    void load_model();
};

struct Grid_Vao
{
    u32 id;
    Grid_Program program;
    float board_dimension;

    inline void bind()
    {
        glBindVertexArray(id);
        program.use();
    };
    void load_shaders(Camera_View_Projection_Block vp_block);
    void load_model();
};