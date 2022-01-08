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
#include <array>
#include <vector>

#include "matrix.h"
#include "rotation_matrix.h"
#include "transformation_matrix.h"

/*** Macro ***/

/*** Global variable ***/

/*** Function ***/
Matrix TransformationMatrix::Expand3to4(const Matrix& mat3)
{
    Matrix mat4 = Matrix::Identity(4);
    for (int32_t row = 0; row < 3; row++) {
        for (int32_t col = 0; col < 3; col++) {
            mat4(row, col) = mat3(row, col);
        }
    }
    return mat4;
}

Matrix TransformationMatrix::Shrink4to3(const Matrix& mat4)
{
    Matrix mat3 = Matrix::Identity(3);
    for (int32_t row = 0; row < 3; row++) {
        for (int32_t col = 0; col < 3; col++) {
            mat3(row, col) = mat4(row, col);
        }
    }
    return mat3;
}

Matrix TransformationMatrix::Translate(float x, float y, float z)
{
    Matrix mat = Matrix::Identity(4);
    mat[3] = x;
    mat[7] = y;
    mat[11] = z;
    return mat;
}

Matrix TransformationMatrix::Scale(float x, float y, float z)
{
    Matrix mat = Matrix::Identity(4);
    mat[0] = x;
    mat[5] = y;
    mat[10] = z;
    return mat;
}

Matrix TransformationMatrix::RotateX(float rad)
{
    Matrix mat = RotationMatrix::RotateX(rad);
    return Expand3to4(mat);
}

Matrix TransformationMatrix::RotateY(float rad)
{
    Matrix mat = RotationMatrix::RotateY(rad);
    return Expand3to4(mat);
}

Matrix TransformationMatrix::RotateZ(float rad)
{
    Matrix mat = RotationMatrix::RotateZ(rad);
    return Expand3to4(mat);
}

Matrix TransformationMatrix::RotateAxisAngle(float x, float y, float z, float rad)
{
    Matrix mat = RotationMatrix::ConvertAxisAngle2RotationMatrix(x, y, z, rad);
    return Expand3to4(mat);
}

Matrix TransformationMatrix::LookAt(
    float eye_x, float eye_y, float eye_z,
    float gaze_x, float gaze_y, float gaze_z,
    float up_x, float up_y, float up_z)
{
    const Matrix tv(Translate(-eye_x, -eye_y, -eye_z));

    const float tx = eye_x - gaze_x;
    const float ty = eye_y - gaze_y;
    const float tz = eye_z - gaze_z;
    const float rx = up_y * tz - up_z * ty;
    const float ry = up_z * tx - up_x * tz;
    const float rz = up_x * ty - up_y * tx;
    const float sx = ty * rz - tz * ry;
    const float sy = tz * rx - tx * rz;
    const float sz = tx * ry - ty * rx;

    const float s = std::sqrt(sx * sx + sy * sy + sz * sz);
    if (s == 0.0f) return tv;
    Matrix rv = Matrix::Identity(4);
    const float r = std::sqrt(rx * rx + ry * ry + rz * rz);
    const float t = std::sqrt(tx * tx + ty * ty + tz * tz);
    rv[0] = rx / r;
    rv[1] = ry / r;
    rv[2] = rz / r;
    rv[4] = sx / s;
    rv[5] = sy / s;
    rv[6] = sz / s;
    rv[8] = tx / t;
    rv[9] = ty / t;
    rv[10] = tz / t;

    return rv * tv;
}

Matrix TransformationMatrix::LookAt(const std::array<float, 3>& eye, const std::array<float, 3>& gaze, const std::array<float, 3>& up)
{
    return TransformationMatrix::LookAt(eye[0], eye[1], eye[2], gaze[0], gaze[1], gaze[2], up[0], up[1], up[2]);
}

