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
#ifndef CONTAINER_H
#define CONTAINER_H

/*** Include ***/
#include <cstdint>
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "matrix.h"

#ifndef M_PI
static constexpr double M_PI = 3.14159265358979323846;
#endif

enum class REPRESENTATION_TYPE {
    ROTATION_MATRIX = 0,
    ROTATION_VECTOR,
    AXIS_ANGLE,
    QUATERNION,
    EULER_MOBILE,
    EULER_FIXED
};

class AngleUnit {
public:
    AngleUnit() : is_degree(true) {}
    ~AngleUnit() {}

    float GetAngleRange()
    {
        return is_degree ? 360.0f : static_cast<float>(2 * M_PI);
    }
    const char* GetAngleFormat()
    {
        return is_degree ? "%.1f" : "%.3f";
    }
    float GetAngleDragSpeed()
    {
        return is_degree ? 1.0f : 0.01f;
    }

    float Display(float rad)
    {
        return is_degree ? static_cast<float>(360.0 * rad / (2 * M_PI)) : rad;
    }

    float StoreAngle(float displayed_angle)
    {
        float rad = is_degree ? static_cast<float>(2 * M_PI * displayed_angle / 360.0) : displayed_angle;
        return rad;
    }

public:
    bool is_degree;
};

class InputContainer {
public:
    InputContainer() { Reset(); }
    ~InputContainer() {}
    void Reset()
    {
        selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::ROTATION_MATRIX);
        rotation_matrix = Matrix::Identity(3);
        rotation_vector = Matrix(1, 3);
        axis_angle = Matrix(1, 4);
        quaternion = Matrix(1, 4);
        quaternion[3] = 1.0;
        mobile_euler_order = 0;
        mobile_euler_angle = Matrix(1, 3);
        fixed_euler_order = 0;
        fixed_euler_angle = Matrix(1, 3);
    }

public:
    /* contain angle as radian */
    int32_t selected_representation_type;
    Matrix rotation_matrix;
    Matrix rotation_vector;
    Matrix axis_angle;
    Matrix quaternion;
    int32_t mobile_euler_order;
    Matrix mobile_euler_angle;
    int32_t fixed_euler_order;
    Matrix fixed_euler_angle;
};

class OutputContainer {
public:
    OutputContainer() { Reset(); }
    ~OutputContainer() {}
    void Reset()
    {
        rotation_matrix = Matrix::Identity(3);
        rotation_vector = Matrix(1, 3);
        axis_angle = Matrix(1, 4);
        quaternion = Matrix(1, 4);
        quaternion[3] = 1.0;
        for (size_t i = 0; i < sizeof(mobile_euler_angle) / sizeof(Matrix); i++) {
            mobile_euler_angle[i] = Matrix(1, 3);
            fixed_euler_angle[i] = Matrix(1, 3);
        }
    }

public:
    /* contain angle as radian */
    Matrix rotation_matrix;
    Matrix rotation_vector;
    Matrix axis_angle;
    Matrix quaternion;
    Matrix mobile_euler_angle[6];
    Matrix fixed_euler_angle[6];
};


class SettingContainer {
public:
    SettingContainer() {
        is_dark_mode = true;
        is_draw_ground = true;
        is_view_from_axis = true;
        is_camera_revolution = true;
        is_normalize_rotation_matrix = true;
        is_update_input_pressed = false;
        is_reset_view_pressed = false;
        is_reset_value_pressed = false;
    }
    ~SettingContainer() {}

public:
    bool is_dark_mode;
    bool is_draw_ground;
    bool is_view_from_axis;
    bool is_camera_revolution;
    bool is_normalize_rotation_matrix;
    bool is_update_input_pressed;
    bool is_reset_view_pressed;
    bool is_reset_value_pressed;
};


#endif
