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
#include <cstdint>
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "matrix.h"
#include "projection_matrix.h"

/*** Macro ***/

/*** Global variable ***/

/*** Function ***/
Matrix ProjectionMatrix::Orthogonal(float left, float right, float bottom, float top, float z_near, float z_far)
{
    Matrix mat = Matrix::Identity(4);
    const float dx = right - left;
    const float dy = top - bottom;
    const float dz = z_far - z_near;
    if (dx != 0.0f && dy != 0.0f && dz != 0.0f) {
        mat[0] = 2.0f / dx;
        mat[5] = 2.0f / dy;
        mat[10] = -2.0f / dz;
        mat[3] = -(right + left) / dx;
        mat[7] = -(top + bottom) / dy;
        mat[11] = -(z_far + z_near) / dz;
    }
    return mat;
}

Matrix ProjectionMatrix::Frustum(float left, float right, float bottom, float top, float z_near, float z_far)
{
    Matrix mat = Matrix::Identity(4);
    const float dx = right - left;
    const float dy = top - bottom;
    const float dz = z_far - z_near;
    if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
    {
        mat[0] = 2.0f * z_near / dx;
        mat[5] = 2.0f * z_near / dy;
        mat[2] = (right + left) / dx;
        mat[6] = (top + bottom) / dy;
        mat[10] = -(z_far + z_near) / dz;
        mat[11] = -2.0f * z_far * z_near / dz;
        mat[14] = -1.0f;
        mat[15] = 0.0f;
    }
    return mat;
}
Matrix ProjectionMatrix::Perspective(float cx, float cy, float fovy, float aspect, float z_near, float z_far)
{
    Matrix mat = Matrix::Identity(4);
    const float dz = z_far - z_near;
    if (dz != 0.0f) {
        mat[5] = 1.0f / std::tan(fovy * 0.5f);
        mat[0] = mat[5] / aspect;
        mat[2] = cx;
        mat[6] = cy;
        mat[10] = -(z_far + z_near) / dz;
        mat[11] = -2.0f * z_far * z_near / dz;
        mat[14] = -1.0f;
        mat[15] = 0.0f;
    }
    return mat;
}

