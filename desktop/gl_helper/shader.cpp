/* Copyright 2022 iwatake2222

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
/*** Include ***/
/* for general */
#include <cstdint>
#include <cstdio>
#include <fstream> 
#include <vector>
#include <string>

/* for GLFW */
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

GLuint Shader::CreateShaderProgram(const char* vertex_shader_text, const char* fragment_shader_text)
{
    GLuint program_id = glCreateProgram();

    if (vertex_shader_text != nullptr) {
        /* Compile */
        GLuint shader_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(shader_id, 1, &vertex_shader_text, nullptr);
        glCompileShader(shader_id);
        /* Check */
        GLint result = GL_FALSE;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            printf("Vertex shader compile error\n");
            int info_log_length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
            if (info_log_length > 0) {
                std::vector<char> error_message(info_log_length + 1);
                glGetShaderInfoLog(shader_id, info_log_length, NULL, &error_message[0]);
                printf("%s\n", &error_message[0]);
            }
        } else {
            glAttachShader(program_id, shader_id);
        }
        glDeleteShader(shader_id);
    }
    if (fragment_shader_text != nullptr) {
        /* Compile */
        GLuint shader_id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(shader_id, 1, &fragment_shader_text, nullptr);
        glCompileShader(shader_id);
        /* Check */
        GLint result = GL_FALSE;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            printf("Fragment shader compile error\n");
            int info_log_length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
            if (info_log_length > 0) {
                std::vector<char> error_message(info_log_length + 1);
                glGetShaderInfoLog(shader_id, info_log_length, NULL, &error_message[0]);
                printf("%s\n", &error_message[0]);
            }
        } else {
            glAttachShader(program_id, shader_id);
        }
        glDeleteShader(shader_id);
    }

    /* Link */
    glLinkProgram(program_id);

    /* Check */
    GLint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        printf("Program error\n");
        int info_log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        if (info_log_length > 0) {
            std::vector<char> error_message(info_log_length + 1);
            glGetProgramInfoLog(program_id, info_log_length, NULL, &error_message[0]);
            printf("%s\n", &error_message[0]);
        }
        glDeleteProgram(program_id);
        return 0;   // error
    }
    return program_id;
}


GLuint Shader::LoadShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path)
{
    /* Read shader code from file */
    std::string vertex_shader_code;
    std::ifstream vertex_shader_stream(vertex_shader_path, std::ios::in);
    if (vertex_shader_stream.is_open()) {
        std::string text_line = "";
        while (getline(vertex_shader_stream, text_line))
            vertex_shader_code += "\n" + text_line;
        vertex_shader_stream.close();
    } else {
        printf("Unable to open vertex shader: %s\n", vertex_shader_path);
        return 0;
    }

    std::string fragment_shader_code;
    std::ifstream fragment_shader_stream(fragment_shader_path, std::ios::in);
    if (fragment_shader_stream.is_open()) {
        std::string text_line = "";
        while (getline(fragment_shader_stream, text_line))
            fragment_shader_code += "\n" + text_line;
        fragment_shader_stream.close();
    } else {
        printf("Unable to open fragment shader: %s\n", fragment_shader_path);
        return 0;
    }

    /* Craete program */
    return Shader::CreateShaderProgram(vertex_shader_code.c_str(), fragment_shader_code.c_str());
}
