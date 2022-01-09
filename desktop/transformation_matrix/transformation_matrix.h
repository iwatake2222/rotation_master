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
#ifndef TRANSFORMATION_MATRIX_H
#define TRANSFORMATION_MATRIX_H

/*** Include ***/
#include <cstdint>
#include <cstdio>
#include <array>
#include <vector>

#include "matrix.h"

namespace TransformationMatrix
{
    Matrix Shrink4to3(const Matrix& mat4);

    /* All functions return 4x4 transform matrix*/
    Matrix Expand3to4(const Matrix& mat3);
    Matrix Translate(float x, float y, float z);
    Matrix Scale(float x, float y, float z);
    Matrix RotateX(float rad);
    Matrix RotateY(float rad);
    Matrix RotateZ(float rad);
    Matrix RotateAxisAngle(float x, float y, float z, float rad);
    Matrix LookAt(
        float eye_x, float eye_y, float eye_z,
        float gaze_x, float gaze_y, float gaze_z,
        float up_x, float up_y, float up_z);
    Matrix LookAt(const std::array<float, 3>& eye, const std::array<float, 3>& gaze, const std::array<float, 3>& up);
}


#endif
