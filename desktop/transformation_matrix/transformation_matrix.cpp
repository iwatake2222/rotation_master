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
#include "transformation_matrix.h"

/*** Macro ***/

/*** Global variable ***/

/*** Function ***/
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
    Matrix mat = Matrix::Identity(4);
    mat[5] = std::cos(rad);
    mat[6] = -std::sin(rad);
    mat[9] = std::sin(rad);
    mat[10] = std::cos(rad);
    return mat;
}

Matrix TransformationMatrix::RotateY(float rad)
{
    Matrix mat = Matrix::Identity(4);
    mat[0] = std::cos(rad);
    mat[2] = std::sin(rad);
    mat[8] = -std::sin(rad);
    mat[10] = std::cos(rad);
    return mat;
}

Matrix TransformationMatrix::RotateZ(float rad)
{
    Matrix mat = Matrix::Identity(4);
    mat[0] = std::cos(rad);
    mat[1] = -std::sin(rad);
    mat[4] = std::sin(rad);
    mat[5] = std::cos(rad);
    return mat;
}

Matrix TransformationMatrix::Rotate(float rad, float x, float y, float z)
{
    Matrix mat = Matrix::Identity(4);
    const float d = std::sqrt(x * x + y * y + z * z);
    if (d > 0.0f) {
        const float l = x / d;
        const float m = y / d;
        const float n = z / d;
        const float l2(l * l);
        const float m2 = m * m;
        const float n2 = n * n;
        const float lm = l * m;
        const float mn = m * n;
        const float nl = n * l;
        const float c = std::cos(rad);
        const float s = std::sin(rad);
        const float c1 = 1.0f - c;
        mat[0] = (1.0f - l2) * c + l2;
        mat[1] = lm * c1 - n * s;
        mat[2] = nl * c1 + m * s;
        mat[4] = lm * c1 + n * s;
        mat[5] = (1.0f - m2) * c + m2;
        mat[6] = mn * c1 - l * s;
        mat[8] = nl * c1 - m * s;
        mat[9] = mn * c1 + l * s;
        mat[10] = (1.0f - n2) * c + n2;
    }
    return mat;
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

