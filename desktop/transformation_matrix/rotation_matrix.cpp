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
#include <stdexcept>

#include "matrix.h"
#include "rotation_matrix.h"

/*** Macro ***/

/*** Global variable ***/

/*** Function ***/
Matrix RotationMatrix::RotateX(float rad)
{
    Matrix mat3_rot = Matrix::Identity(3);
    mat3_rot[4] = std::cos(rad);
    mat3_rot[5] = -std::sin(rad);
    mat3_rot[7] = std::sin(rad);
    mat3_rot[8] = std::cos(rad);
    return mat3_rot;
}

Matrix RotationMatrix::RotateY(float rad)
{
    Matrix mat3_rot = Matrix::Identity(3);
    mat3_rot[0] = std::cos(rad);
    mat3_rot[2] = std::sin(rad);
    mat3_rot[6] = -std::sin(rad);
    mat3_rot[8] = std::cos(rad);
    return mat3_rot;
}

Matrix RotationMatrix::RotateZ(float rad)
{
    Matrix mat3_rot = Matrix::Identity(3);
    mat3_rot[0] = std::cos(rad);
    mat3_rot[1] = -std::sin(rad);
    mat3_rot[3] = std::sin(rad);
    mat3_rot[4] = std::cos(rad);
    return mat3_rot;
}

Matrix RotationMatrix::ConvertAxisAngle2RotationMatrix(float x, float y, float z, float rad)
{
    /* Normalize */
    float d = x * x + y * y + z * z;
    d = std::sqrt(d);
    if (d <= 0.0f) return Matrix::Identity(3);

    x /= d;
    y /= d;
    z /= d;

    /* Rodrigues' rotation formula */
    /* https://ja.wikipedia.org/wiki/ロドリゲスの回転公式 */
    Matrix mat3_rot = Matrix::Identity(3);
    const float c = std::cos(rad);
    const float s = std::sin(rad);
    mat3_rot[0] = c + x * x * (1 - c);;
    mat3_rot[1] = x * y * (1 - c) - z * s;
    mat3_rot[2] = z * x * (1 - c) + y * s;
    mat3_rot[3] = x * y * (1 - c) + z * s;
    mat3_rot[4] = c + y * y * (1 - c);
    mat3_rot[5] = y * z * (1 - c) - x * s;
    mat3_rot[6] = z * x * (1 - c) - y * s;
    mat3_rot[7] = y * z * (1 - c) + x * s;
    mat3_rot[8] = c + z * z * (1 - c);

    return mat3_rot;

}

Matrix RotationMatrix::ConvertRotationVector2RotationMatrix(float x_rad, float y_rad, float z_rad)
{
    float rad = x_rad * x_rad + y_rad * y_rad + z_rad * z_rad;
    rad = std::sqrt(rad);
    if (rad <= 0.0f) return Matrix::Identity(3);

    float x = x_rad / rad;
    float y = y_rad / rad;
    float z = z_rad / rad;

    Matrix mat3_rot = ConvertAxisAngle2RotationMatrix(x, y, z, rad);
    return mat3_rot;
}

Matrix RotationMatrix::ConvertQuaternion2RotationMatrix(float x, float y, float z, float w)
{
    /* Normalize */
    float d = x * x + y * y + z * z + w * w;
    d = std::sqrt(d);
    x /= d;
    y /= d;
    z /= d;
    w /= d;

    Matrix mat3_rot = Matrix::Identity(3);
    mat3_rot(0, 0) = 2 * w * w + 2 * x * x - 1;
    mat3_rot(0, 1) = 2 * x * y - 2 * z * w;
    mat3_rot(0, 2) = 2 * x * z + 2 * y * w;

    mat3_rot(1, 0) = 2 * x * y + 2 * z * w;
    mat3_rot(1, 1) = 2 * w * w + 2 * y * y - 1;
    mat3_rot(1, 2) = 2 * y * z - 2 * x * w;

    mat3_rot(2, 0) = 2 * x * z - 2 * y * w;
    mat3_rot(2, 1) = 2 * y * z + 2 * x * w;
    mat3_rot(2, 2) = 2 * w * w + 2 * z * z - 1;
    return mat3_rot;
}

Matrix RotationMatrix::ConvertEulerMobile2RotationMatrix(RotationMatrix::EULER_ORDER order, float x, float y, float z)
{
    Matrix mat3_rot;
    switch (order) {
    case EULER_ORDER::XYZ:
        mat3_rot = RotateX(x) * RotateY(y) * RotateZ(z);
        break;
    case EULER_ORDER::XZY:
        mat3_rot = RotateX(x) * RotateZ(z) * RotateY(y);
        break;
    case EULER_ORDER::YXZ:
        mat3_rot = RotateY(y) * RotateX(x) * RotateZ(z);
        break;
    case EULER_ORDER::YZX:
        mat3_rot = RotateY(y) * RotateZ(z) * RotateX(x);
        break;
    case EULER_ORDER::ZXY:
        mat3_rot = RotateZ(z) * RotateX(x) * RotateY(y);
        break;
    case EULER_ORDER::ZYX:
        mat3_rot = RotateZ(z) * RotateY(y) * RotateX(x);
        break;
    }
    return mat3_rot;
}

Matrix RotationMatrix::ConvertEulerFixed2RotationMatrix(RotationMatrix::EULER_ORDER order, float x, float y, float z)
{
    Matrix mat3_rot;
    switch (order) {
    case EULER_ORDER::XYZ:
        mat3_rot = RotateZ(z) * RotateY(y) * RotateX(x);
        break;
    case EULER_ORDER::XZY:
        mat3_rot = RotateY(y) * RotateZ(z) * RotateX(x);
        break;
    case EULER_ORDER::YXZ:
        mat3_rot = RotateZ(z) * RotateX(x) * RotateY(y);
        break;
    case EULER_ORDER::YZX:
        mat3_rot = RotateX(x) * RotateZ(z) * RotateY(y);
        break;
    case EULER_ORDER::ZXY:
        mat3_rot = RotateY(y) * RotateX(x) * RotateZ(z);
        break;
    case EULER_ORDER::ZYX:
        mat3_rot = RotateX(x) * RotateY(y) * RotateZ(z);
        break;
    }

    return mat3_rot;
}

/* Note: xyz is not on OpenGL coordinate */
Matrix RotationMatrix::ConvertXYZ2PolarCoordinate(float x, float y, float z)
{
    Matrix vec3 = Matrix(3, 1);
    float r = std::sqrt(x * x + y * y + z * z);
    float theta_rad = 0;
    if (r != 0) theta_rad = std::acos(z / r);
    float phi_rad = 0;
    phi_rad = std::acos(x / std::sqrt(x * x + y * y));
    if (y < 0) phi_rad *= -1;
    vec3[0] = r;
    vec3[1] = theta_rad;
    vec3[2] = phi_rad;
    return vec3;
}

/* Note: xyz is not on OpenGL coordinate */
Matrix RotationMatrix::ConvertPolarCoordinate2XYZ(float r, float theta_rad, float phi_rad)
{
    Matrix vec3 = Matrix(3, 1);
    float x = r * std::sin(theta_rad) * std::cos(phi_rad);
    float y = r * std::sin(theta_rad) * std::sin(phi_rad);
    float z = r * std::cos(theta_rad);
    vec3[0] = x;
    vec3[1] = y;
    vec3[2] = z;
    return vec3;
}
