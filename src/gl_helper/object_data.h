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
#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H

/*** Include ***/
/* for general */
#include <cstdint>
#include <cstdio>
#include <array>
#include <vector>
#include <memory>

#include "shape.h"

namespace ObjectData
{
    std::unique_ptr<ShapeIndex> CreateGround(float size, float interval, std::array<float, 3> color_vec3 = { 0.0f, 0.5f, 0.5f });
    std::unique_ptr<Shape> CreateAxes(float size, float arrow_size, std::array<float, 3> color_x, std::array<float, 3> color_y, std::array<float, 3> color_z);
    std::unique_ptr<ShapeSolid> CreateMonolith(float width, float height, float thickness, std::array<float, 3> color_front, std::array<float, 3> color_back);

    extern const std::vector<Object::Vertex> CubeWireVertex;
    extern const std::vector<GLuint> CubeWireIndex;
    extern const std::vector<Object::Vertex> CubeTriangleVertex;
    extern const std::vector<GLuint> CubeTriangleIndex;
}

#endif
