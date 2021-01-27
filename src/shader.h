#pragma once
#include <glad/glad.h>
#include "defines.h"

int read_entire_file(const char *fname, char *buffer);
bool maybe_log_shader_error(u32 shader, GLenum error_type);
u32 load_shader(const char *fname, GLenum shader_type);
u32 create_shader_program(const char *vertex_filename, const char *fragment_filename);
