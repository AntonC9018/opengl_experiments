#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h>
#include <tiny_obj_loader.h>

#include "vao.h"
#include "defines.h"
#include "gl_init.h"
#include "shader.h"

struct Pawn_Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct Pawn_Index
{
    u32 vertex;
    u32 normal;
};

u32 max(u32* array, s64 size)
{
    u32 m = 0;
    for (int i = 0; i < size; i++)
    {
        if (m < array[i])
            m = array[i];
    }
    return m;
}

void Pawn_Vao::load_shaders(Camera_View_Projection_Block vp_block)
{
    program.id = create_shader_program("shader_src/generic.vs", "shader_src/generic.fs");
    program.query_locations();
    program.Camera_View_Projection_block(vp_block);
}

void Pawn_Vao::load_model()
{
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile("models/pawn.obj"))
    {
        if (!reader.Error().empty())
        {
            fprintf(stderr, "TinyObjReader: %s", reader.Error().c_str());
        }
    }

    if (!reader.Warning().empty())
    {
        fprintf(stderr, "TinyObjReader: %s", reader.Warning().c_str());
    }

    auto &attrib = reader.GetAttrib();
    auto &shapes = reader.GetShapes();

    std::vector<Pawn_Vertex> vertices;
    struct { Pawn_Index key; u32 value; } *index_map = NULL;
    std::vector<u32> indices;
    size_t num_indices = shapes[0].mesh.indices.size();
    indices.reserve(num_indices);
    u32 next_index = 0;

    for (int i = 0; i < num_indices; i++)
    {
        auto index = shapes[0].mesh.indices[i];
        auto my_index = Pawn_Index{ (u32)index.vertex_index, (u32)index.normal_index };
        auto key = hmgeti(index_map, my_index);
        if (key == -1)
        {
            hmput(index_map, my_index, next_index);
            indices.push_back(next_index);
            vertices.push_back({
                {
                    attrib.vertices[3 * index.vertex_index], 
                    attrib.vertices[3 * index.vertex_index + 1], 
                    attrib.vertices[3 * index.vertex_index + 2]
                },
                {
                    attrib.normals[3 * index.normal_index], 
                    attrib.normals[3 * index.normal_index + 1], 
                    attrib.normals[3 * index.normal_index + 2]
                }
            });
            next_index++;
        }
        else
        {
            indices.push_back(index_map[key].value);
        }
    }

    hmfree(index_map);

    // number of faces
    num_triangles = (u32)num_indices / 3;

    u32 vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Pawn_Vertex), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Pawn_Vertex), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Pawn_Vertex), (void*)12);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * 4, indices.data(), GL_STATIC_DRAW);
}

void Grid_Vao::load_model()
{
    glGenVertexArrays(1, &id);

    float vertex_data[] = {
        0.0f, 0.0f, 0.0f,
        board_dimension, 0.0f, 0.0f,
        board_dimension, board_dimension, 0.0f,
        0.0f, board_dimension, 0.0f
    };

    u32 indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    u32 vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
    glEnableVertexAttribArray(0);
}

void Grid_Vao::load_shaders(Camera_View_Projection_Block vp_block)
{
    program.id = create_shader_program("shader_src/grid.vs", "shader_src/grid.fs");
    program.query_locations();
    program.Camera_View_Projection_block(vp_block);
}
