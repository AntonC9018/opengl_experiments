#include "vao.h"
#include "gl_init.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
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
        2, 3, 4
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

uint32_t create_pawn_vao(int32_t &triangle_count)
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
        triangle_count = shapes[0].mesh.indices.size() / 3;
        uint32_t num_indices = shapes[0].mesh.indices.size();
        uint32_t *indices = (uint32_t*)malloc(num_indices * 4); 
        float* normals = (float*)malloc(attrib.vertices.size() * 4);

        for (int i = 0; i < num_indices; i++)
        {
            auto index = shapes[0].mesh.indices[i];
            indices[i] = index.vertex_index;
            memcpy(&normals[indices[i] * 3], &attrib.normals[3 * index.normal_index], 12);
        }

        static const uint32_t vbo = 0, nbo = 1, tbo = 2, ebo = 3;
        uint32_t bufs[4] = {0};
        glGenBuffers(4, bufs);

        glBindBuffer(GL_ARRAY_BUFFER, bufs[vbo]);
        glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * 4, attrib.vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(vbo);
        glVertexAttribPointer(vbo, 3, GL_FLOAT, GL_FALSE, 12, 0);

        glBindBuffer(GL_ARRAY_BUFFER, bufs[nbo]);
        glBufferData(GL_ARRAY_BUFFER, attrib.vertices.size() * 4, normals, GL_STATIC_DRAW);
        glEnableVertexAttribArray(nbo);
        glVertexAttribPointer(nbo, 3, GL_FLOAT, GL_FALSE, 12, 0);

        free(normals);

        // glBindBuffer(GL_ARRAY_BUFFER, bufs[tbo]);
        // glBufferData(GL_ARRAY_BUFFER, attrib.texcoords.size() * 4, attrib.texcoords.data(), GL_STATIC_DRAW);
        // glEnableVertexAttribArray(tbo);
        // glVertexAttribPointer(tbo, 2, GL_FLOAT, GL_FALSE, 8, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufs[ebo]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * 4, indices, GL_STATIC_DRAW);
    }

    return pawn_vao;
}