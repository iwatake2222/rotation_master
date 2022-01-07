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
#include <array>
#include <vector>
#include <memory>

#include "shape.h"

#include "object_data.h"

/*** Macro ***/
/* macro function */

/* Setting */

/*** Global variable ***/


/*** Function ***/
std::unique_ptr<ShapeIndex> ObjectData::CreateGround(float size, float interval, std::array<float, 3> color_vec3)
{
    GLuint index = 0;
    std::vector<Object::Vertex> vertex_list;
    std::vector<GLuint> index_list;
    for (float z = -size; z <= size; z += interval) {
        vertex_list.push_back({ -size, 0.0f, z, color_vec3[0], color_vec3[1], color_vec3[2] });
        vertex_list.push_back({ size, 0.0f, z, color_vec3[0], color_vec3[1], color_vec3[2] });
        index_list.push_back(index++);
        index_list.push_back(index++);
    }
    for (float x = -size; x <= size; x += interval) {
        vertex_list.push_back({ x, 0.0f, -size, color_vec3[0], color_vec3[1], color_vec3[2] });
        vertex_list.push_back({ x, 0.0f, size, color_vec3[0], color_vec3[1], color_vec3[2] });
        index_list.push_back(index++);
        index_list.push_back(index++);
    }
    return std::make_unique<ShapeIndex>(vertex_list, index_list);
}

static std::vector<std::array<float, 3>> CreateArrowZPointList(float size, float arrow_size)
{
    std::vector<std::array<float, 3>> vertex_list;
    vertex_list.push_back({ 0.0f, 0.0f, -size });
    vertex_list.push_back({ 0.0f, 0.0f, size });

    vertex_list.push_back({ 0.0f, 0.0f, size });
    vertex_list.push_back({ 0.0f, arrow_size, size - arrow_size });

    vertex_list.push_back({ 0.0f, 0.0f, size });
    vertex_list.push_back({ 0.0f, -arrow_size, size - arrow_size });

    vertex_list.push_back({ 0.0f, 0.0f, size });
    vertex_list.push_back({ arrow_size, 0.0f, size - arrow_size });

    vertex_list.push_back({ 0.0f, 0.0f, size });
    vertex_list.push_back({ -arrow_size, 0.0f, size - arrow_size });

    vertex_list.push_back({ arrow_size, 0.0f, size - arrow_size });
    vertex_list.push_back({ 0.0f, arrow_size, size - arrow_size });

    vertex_list.push_back({ 0.0f, arrow_size, size - arrow_size });
    vertex_list.push_back({ -arrow_size, 0.0f, size - arrow_size });

    vertex_list.push_back({ -arrow_size, 0.0f, size - arrow_size });
    vertex_list.push_back({ 0.0f, -arrow_size, size - arrow_size });

    vertex_list.push_back({ 0.0f, -arrow_size, size - arrow_size });
    vertex_list.push_back({ arrow_size, 0.0f, size - arrow_size });

    return vertex_list;
}

std::unique_ptr<Shape> ObjectData::CreateAxes(float size, float arrow_size, std::array<float, 3> color_x, std::array<float, 3> color_y, std::array<float, 3> color_z)
{
    const auto point_list = CreateArrowZPointList(size, arrow_size);
    std::vector<Object::Vertex> vertex_list;
    /* X axis */
    for (const auto& point : point_list) {
        vertex_list.push_back({ point[2], point[0], point[1], color_x[0], color_x[1], color_x[2] });
    }
    /* Y axis */
    for (const auto& point : point_list) {
        vertex_list.push_back({ point[1], point[2], point[0], color_y[0], color_y[1], color_y[2] });
    }
    /* Z axis */
    for (const auto& point : point_list) {
        vertex_list.push_back({ point[0], point[1], point[2], color_z[0], color_z[1], color_z[2] });
    }

    return std::make_unique<Shape>(vertex_list);
}

std::unique_ptr<ShapeSolid> ObjectData::CreateMonolith(float width, float height, float thickness, std::array<float, 3> color_front, std::array<float, 3> color_back)
{
    std::vector<Object::Vertex> vertex_list;
    const std::vector<std::array<float, 3>> base_vertex_list {
        /* front */
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },

        /* back */
        { 1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },

        { -1.0f, -1.0f, -1.0f },
        { -1.0f, -1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },
        { -1.0f, -1.0f, -1.0f },
        { -1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, -1.0f },

        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, -1.0f, -1.0f },
        { 1.0f, -1.0f, 1.0f },
        { -1.0f, -1.0f, 1.0f },

        { 1.0f, -1.0f, 1.0f },
        { 1.0f, -1.0f, -1.0f },
        { 1.0f, 1.0f, -1.0f },
        { 1.0f, -1.0f, 1.0f },
        { 1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },

        { -1.0f, 1.0f, -1.0f },
        { -1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { -1.0f, 1.0f, -1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, -1.0f },
    };

    /* front */
    for (size_t i = 0; i < 6; i++) {
        const auto& point = base_vertex_list[i];
        vertex_list.push_back({ point[0] * width, point[1] * height, point[2] * thickness, color_front[0], color_front[1], color_front[2] });
    }
    /* back */
    for (size_t i = 6; i < 12; i++) {
        const auto& point = base_vertex_list[i];
        vertex_list.push_back({ point[0] * width, point[1] * height, point[2] * thickness, color_back[0], color_back[1], color_back[2] });
    }
    /* other faces */
    for (size_t i = 12; i < base_vertex_list.size(); i++) {
        const auto& point = base_vertex_list[i];
        vertex_list.push_back({ point[0] * width, point[1] * height, point[2] * thickness,
            (color_front[0] + color_back[0]) / 2.0f, (color_front[1] + color_back[1]) / 2.0f, (color_front[2] + color_back[2]) / 2.0f });
    }

    return std::make_unique<ShapeSolid>(vertex_list);
}

const std::vector<Object::Vertex> ObjectData::CubeWireVertex
{
    { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f },
    { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.8f },
    { -1.0f, 1.0f, 1.0f, 0.0f, 0.8f, 0.0f },
    { -1.0f, 1.0f, -1.0f, 0.0f, 0.8f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.8f, 0.0f, 0.0f },
    { 1.0f, -1.0f, -1.0f, 0.8f, 0.0f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.0f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f }
};

const std::vector<GLuint> ObjectData::CubeWireIndex
{
    1, 0,
    2, 7,
    3, 0,
    4, 7,
    5, 0,
    6, 7,
    1, 2,
    2, 3,
    3, 4,
    4, 5,
    5, 6,
    6, 1
};

const std::vector<Object::Vertex> ObjectData::CubeTriangleVertex
{
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, 1.0f, -1.0f, 0.1f, 0.8f, 0.1f },

    { 1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { -1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { 1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f },

    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f },
    { 1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f },
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f },
    { -1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f },

    { 1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, -1.0f, -1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.8f },

    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f },
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f },
    { 1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f },

    { -1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { 1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { -1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f }
};

const std::vector<GLuint> ObjectData::CubeTriangleIndex
{
    0, 1, 2, 3, 4, 5,
    6, 7, 8, 9, 10, 11,
    12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
};