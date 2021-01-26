#include "objects.h"

Camera camera;

void Pawn::draw()
{
    if (is_visible)
    {
        vao->bind();
        vao->program.model(transform.as_mat4());
        // TODO: move to uniform buffer
        vao->program.light_position(light_position);
        glEnable(GL_CULL_FACE); 
        glDrawElements(GL_TRIANGLES, num_visible_triangles * 3, GL_UNSIGNED_INT, 0);
    }
}

void Grid::draw()
{
    if (is_visible)
    {
        vao->bind();
        vao->program.model(transform.as_mat4());
        // TODO: this realistically does not change. 
        // Hardcode? Omit members with annotations?
        vao->program.board_dimension(vao->board_dimension);
        // TODO: move to uniform buffer
        vao->program.light_position(light_position);
        
        glDisable(GL_CULL_FACE); 
        // 6 for 2 triangles, 3 indices per each
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    }
}