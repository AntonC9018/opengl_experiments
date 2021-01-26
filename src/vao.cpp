#include "vao.h"
#include "gl_init.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#define STB_DS_IMPLEMENTATION
#include <stb/stb_ds.h>
#include <tiny_obj_loader.h>

uint32_t create_board_vao(float board_dimension)
{
    uint32_t board_vao;
    glGenVertexArrays(1, &board_vao);

    float vertex_data[] = {
        0.0f, 0.0f, 0.0f,
        board_dimension, 0.0f, 0.0f,
        board_dimension, board_dimension, 0.0f,
        0.0f, board_dimension, 0.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    uint32_t vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(board_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, 0);
    glEnableVertexAttribArray(0);

    return board_vao;
}

struct Pawn_Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct Pawn_Index
{
    uint32_t vertex;
    uint32_t normal;
};

uint32_t max(uint32_t* array, size_t size)
{
    uint32_t m = 0;
    for (int i = 0; i < size; i++)
    {
        if (m < array[i])
            m = array[i];
    }
    return m;
}

uint32_t create_pawn_vao(int32_t* triangle_count)
{
    uint32_t pawn_vao;
    glGenVertexArrays(1, &pawn_vao);
    glBindVertexArray(pawn_vao);

    {
        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile("models/pawn.obj"))
        {
            if (!reader.Error().empty())
            {
                printf("TinyObjReader: %s", reader.Error().c_str());
            }
            return -1;
        }

        if (!reader.Warning().empty())
        {
            printf("TinyObjReader: %s", reader.Warning().c_str());
        }

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();

        std::vector<Pawn_Vertex> vertices;
        struct { Pawn_Index key; uint32_t value; } *index_map = NULL;
        std::vector<uint32_t> indices;
        size_t num_indices = shapes[0].mesh.indices.size();
        indices.reserve(num_indices);
        uint32_t next_index = 0;

        for (int i = 0; i < num_indices; i++)
        {
            auto index = shapes[0].mesh.indices[i];
            auto my_index = Pawn_Index{ (uint32_t)index.vertex_index, (uint32_t)index.normal_index };
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
        *triangle_count = (uint32_t)num_indices / 3;

        uint32_t vbo, ebo;
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

    return pawn_vao;
}