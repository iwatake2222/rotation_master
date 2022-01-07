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
#ifndef SHADER_H
#define SHADER_H

/*** Include ***/
/* for general */
#include <cstdint>
#include <cstdio>

/* for GLFW */
#include <GLFW/glfw3.h>

namespace Shader
{
    GLuint CreateShaderProgram(const char* vertex_shader_text, const char* fragment_shader_text);
    GLuint LoadShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path);
}

#endif
