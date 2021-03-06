#define _CRT_SECURE_NO_WARNINGS

#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

// WARNING: could write to bytes after the buffer, no bound checks are being performed!
// returns -1 if something went wrong.
int read_entire_file(const char *fname, char *buffer)
{
    FILE *vertex_shader_file = fopen(fname, "r");
    if (!vertex_shader_file)
    {
        return -1;
    }
    char ch;
    int i = 0;
    while ((ch = getc(vertex_shader_file)) != EOF)
    {
        buffer[i] = ch;
        i++;
    }
    buffer[i] = 0;
    fclose(vertex_shader_file);
    return 0;
}

void crash()
{
    exit(-1);
}

#define MAX_SHADER_FILE_SIZE 2048
#define GL_ERROR_LOG_LENGTH 2048

bool maybe_log_shader_error(u32 shader, GLenum error_type)
{
    char log_buffer[GL_ERROR_LOG_LENGTH];
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success)
    {
        glGetShaderInfoLog(shader, GL_ERROR_LOG_LENGTH, NULL, log_buffer);
        fputs(log_buffer, stderr);
        return true;
    }
    return false;
}

// returns 0 if not successful
// otherwise, returns the id of the created shader
u32 load_shader(const char *fname, GLenum shader_type)
{
    u32 shader = glCreateShader(shader_type);

    {
        char source_buffer[MAX_SHADER_FILE_SIZE];
        if (read_entire_file(fname, source_buffer))
        {
            fprintf(stderr, "Yikes! Couldn't load the shader %s", fname);
            return 0;
        }
        auto src = static_cast<const char *>(source_buffer);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
    }
    
    if (maybe_log_shader_error(shader, GL_COMPILE_STATUS))
    {
        fprintf(stderr, "Yikes! %s didn't compile.", fname);
        return 0;
    }
    
    return shader;
}

u32 create_shader_program(const char *vertex_filename, const char *fragment_filename)
{
    u32 program_id = glCreateProgram();
    u32 vertex_shader = load_shader(vertex_filename, GL_VERTEX_SHADER);
    if (!vertex_shader)
    {
        crash();
    }
    u32 fragment_shader = load_shader(fragment_filename, GL_FRAGMENT_SHADER);
    if (!fragment_shader)
    {
        crash();
    }
    glAttachShader(program_id, fragment_shader);
    glAttachShader(program_id, vertex_shader);
    glLinkProgram(program_id);

    if (maybe_log_shader_error(program_id, GL_LINK_STATUS))
    {
        crash();
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program_id;
}