#pragma once
#include <glad/glad.h>
#include <stdint.h>

int read_entire_file(const char *fname, char *buffer);
bool maybe_log_shader_error(uint32_t shader, GLenum error_type);
uint32_t load_shader(const char *fname, GLenum shader_type);
uint32_t create_shader_program(const char *vertex_filename, const char *fragment_filename);
