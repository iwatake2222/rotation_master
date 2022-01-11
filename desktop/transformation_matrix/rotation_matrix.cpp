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
static constexpr float ALMOST_ONE = 0.9999999f;

/*** Global variable ***/

/*** Function ***/
static float clamp_one(float value)
{
    if (value <= -1.0) return -ALMOST_ONE;
    else if (value >= 1.0) return ALMOST_ONE;
    else return value;
}

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

Matrix RotationMatrix::NormalizeRotationMatrix(const Matrix mat3_rot)
{
    Matrix vec = RotationMatrix::ConvertRotationMatrix2Quaternion(mat3_rot);
    Matrix mat3_rot_normalized = RotationMatrix::ConvertQuaternion2RotationMatrix(vec[0], vec[1], vec[2], vec[3]);
    return mat3_rot_normalized;
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
    /* http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm */
    Matrix mat3_rot = Matrix::Identity(3);
    
    const float c = std::cos(rad);
    const float s = std::sin(rad);
    const float t = 1.0f - c;
    mat3_rot[0] = t * x * x + c;
    mat3_rot[1] = t * x * y - z * s;
    mat3_rot[2] = t * x * z + y * s;
    mat3_rot[3] = t * x * y + z * s;
    mat3_rot[4] = t * y * y + c;
    mat3_rot[5] = t * y * z - x * s;
    mat3_rot[6] = t * x * z - y * s;
    mat3_rot[7] = t * y * z + x * s;
    mat3_rot[8] = t * z * z + c;

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

    /* http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm */
    Matrix mat3_rot = Matrix::Identity(3);
    mat3_rot(0, 0) = 1 - 2 * y * y - 2 * z * z;
    mat3_rot(0, 1) = 2 * x * y - 2 * z * w;
    mat3_rot(0, 2) = 2 * x * z + 2 * y * w;
    mat3_rot(1, 0) = 2 * x * y + 2 * z * w;
    mat3_rot(1, 1) = 1 - 2 * x * x - 2 * z * z;
    mat3_rot(1, 2) = 2 * y * z - 2 * x * w;
    mat3_rot(2, 0) = 2 * x * z - 2 * y * w;
    mat3_rot(2, 1) = 2 * y * z + 2 * x * w;
    mat3_rot(2, 2) = 1 - 2 * x * x - 2 * y * y;
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


Matrix RotationMatrix::ConvertRotationMatrix2RotationVector(const Matrix& mat3_rot)
{
    Matrix vec3 = Matrix(3, 1);

    Matrix vec4_axisangle = ConvertRotationMatrix2AxisAngle(mat3_rot);
    vec3[0] = vec4_axisangle[0] * vec4_axisangle[3];
    vec3[1] = vec4_axisangle[1] * vec4_axisangle[3];
    vec3[2] = vec4_axisangle[2] * vec4_axisangle[3];
    return vec3;
}

Matrix RotationMatrix::ConvertRotationMatrix2AxisAngle(const Matrix& mat3_rot)
{
    Matrix vec4 = Matrix(4, 1);
    /* http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/ */
    float d = static_cast<float>(std::sqrt(
        std::pow(mat3_rot(2, 1) - mat3_rot(1, 2), 2)
        + std::pow(mat3_rot(0, 2) - mat3_rot(2, 0), 2)
        + std::pow(mat3_rot(1, 0) - mat3_rot(0, 1), 2)
    ));
    if (d > 0) {
        vec4[0] = (mat3_rot(2, 1) - mat3_rot(1, 2)) / d;
        vec4[1] = (mat3_rot(0, 2) - mat3_rot(2, 0)) / d;
        vec4[2] = (mat3_rot(1, 0) - mat3_rot(0, 1)) / d;
        vec4[3] = std::acos((mat3_rot(0, 0) + mat3_rot(1, 1) + mat3_rot(2, 2) - 1) / 2);
    }
    
    return vec4;
}

Matrix RotationMatrix::ConvertRotationMatrix2Quaternion(const Matrix& mat3_rot)
{
    /* http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm */
    Matrix vec4 = Matrix(4, 1);
    
    float tr = mat3_rot(0, 0) + mat3_rot(1, 1) + mat3_rot(2, 2);
    if (tr > 0) {
        float S = std::sqrt(tr + 1.0f) * 2;
        vec4[3] = 0.25f * S;
        vec4[0] = (mat3_rot(2, 1) - mat3_rot(1, 2)) / S;
        vec4[1] = (mat3_rot(0, 2) - mat3_rot(2, 0)) / S;
        vec4[2] = (mat3_rot(1, 0) - mat3_rot(0, 1)) / S;
    } else if ((mat3_rot(0, 0) > mat3_rot(1, 1)) & (mat3_rot(0, 0) > mat3_rot(2, 2))) {
        float S = std::sqrt(1.0f + mat3_rot(0, 0) - mat3_rot(1, 1) - mat3_rot(2, 2)) * 2;
        vec4[3] = (mat3_rot(2, 1) - mat3_rot(1, 2)) / S; 
        vec4[0] = 0.25f * S;
        vec4[1] = (mat3_rot(0, 1) + mat3_rot(1, 0)) / S;
        vec4[2] = (mat3_rot(0, 2) + mat3_rot(2, 0)) / S;
    } else if (mat3_rot(1, 1) > mat3_rot(2, 2)) {
        float S = std::sqrt(1.0f + mat3_rot(1, 1) - mat3_rot(0, 0) - mat3_rot(2, 2)) * 2;
        vec4[3] = (mat3_rot(0, 2) - mat3_rot(2, 0)) / S;
        vec4[0] = (mat3_rot(0, 1) + mat3_rot(1, 0)) / S;
        vec4[1] = 0.25f * S; 
        vec4[2] = (mat3_rot(1, 2) + mat3_rot(2, 1)) / S;
    } else {
        float S = std::sqrt(1.0f + mat3_rot(2, 2) - mat3_rot(0, 0) - mat3_rot(1, 1)) * 2;
        vec4[3] = (mat3_rot(1, 0) - mat3_rot(0, 1)) / S;
        vec4[0] = (mat3_rot(0, 2) + mat3_rot(2, 0)) / S;
        vec4[1] = (mat3_rot(1, 2) + mat3_rot(2, 1)) / S;
        vec4[2] = 0.25f * S; 
    }
  
    return vec4;
}

Matrix RotationMatrix::ConvertRotationMatrix2EulerMobile(RotationMatrix::EULER_ORDER order, const Matrix& mat3_rot)
{
    /* https://github.com/mrdoob/three.js/blob/cab469bc0dad1f79b83a7b8c3c51dcf9d7291622/src/math/Euler.js#L104 */
    Matrix vec3 = Matrix(3, 1);
    switch (order) {
    case EULER_ORDER::XYZ:
        vec3[1] = std::asin(clamp_one(mat3_rot(0, 2)));
        if (std::abs(mat3_rot(0, 2)) < ALMOST_ONE) {
            vec3[0] = std::atan2(-mat3_rot(1, 2), mat3_rot(2, 2));
            vec3[2] = std::atan2(-mat3_rot(0, 1), mat3_rot(0, 0));
        } else {
            vec3[0] = std::atan2(mat3_rot(2, 1), mat3_rot(1, 1));
            vec3[2] = 0;
        }
        break;
    case EULER_ORDER::XZY:
        vec3[2] = std::asin(clamp_one(-mat3_rot(0, 1)));
        if (std::abs(mat3_rot(0, 1)) < ALMOST_ONE) {
            vec3[0] = std::atan2(mat3_rot(2, 1), mat3_rot(1, 1));
            vec3[1] = std::atan2(mat3_rot(0, 2), mat3_rot(0, 0));
        } else {
            vec3[0] = 0;
            vec3[1] = std::atan2(-mat3_rot(1, 2), mat3_rot(2, 2));
        }
        break;
        break;
    case EULER_ORDER::YXZ:
        vec3[0] = std::asin(clamp_one(-mat3_rot(1, 2)));
        if (std::abs(mat3_rot(1, 2)) < ALMOST_ONE) {
            vec3[1] = std::atan2(mat3_rot(0, 2), mat3_rot(2, 2));
            vec3[2] = std::atan2(mat3_rot(1, 0), mat3_rot(1, 1));
        } else {
            vec3[1] = std::atan2(-mat3_rot(2, 0), mat3_rot(0, 0));
            vec3[2] = 0;
        }
        break;
    case EULER_ORDER::YZX:
        vec3[2] = std::asin(clamp_one(mat3_rot(1, 0)));
        if (std::abs(mat3_rot(1, 0)) < ALMOST_ONE) {
            vec3[0] = std::atan2(-mat3_rot(1, 2), mat3_rot(1, 1));
            vec3[1] = std::atan2(-mat3_rot(2, 0), mat3_rot(0, 0));
        } else {
            vec3[0] = 0;
            vec3[1] = std::atan2(mat3_rot(0, 2), mat3_rot(2, 2));
        }
        break;
    case EULER_ORDER::ZXY:
        vec3[0] = std::asin(clamp_one(mat3_rot(2, 1)));
        if (std::abs(mat3_rot(2, 1)) < ALMOST_ONE) {
            vec3[1] = std::atan2(-mat3_rot(2, 0), mat3_rot(2, 2));
            vec3[2] = std::atan2(-mat3_rot(0, 1), mat3_rot(1, 1));
        } else {
            vec3[1] = 0;
            vec3[2] = std::atan2(mat3_rot(1, 0), mat3_rot(0, 0));
        }
        break;
    case EULER_ORDER::ZYX:
        vec3[1] = std::asin(clamp_one(-mat3_rot(2, 0)));
        if (std::abs(mat3_rot(2, 0)) < ALMOST_ONE) {
            vec3[0] = std::atan2(mat3_rot(2, 1), mat3_rot(2, 2));
            vec3[2] = std::atan2(mat3_rot(1, 0), mat3_rot(0, 0));
        } else {
            vec3[0] = 0;
            vec3[2] = std::atan2(-mat3_rot(0, 1), mat3_rot(1, 1));
        }
        break;
    }
    return vec3;
}
Matrix RotationMatrix::ConvertRotationMatrix2EulerFixed(RotationMatrix::EULER_ORDER order, const Matrix& mat3_rot)
{
    /* https://github.com/mrdoob/three.js/blob/cab469bc0dad1f79b83a7b8c3c51dcf9d7291622/src/math/Euler.js#L104 */
    Matrix vec3 = Matrix(3, 1);
    switch (order) {
    case EULER_ORDER::XYZ:
        vec3[1] = std::asin(clamp_one(-mat3_rot(2, 0)));
        if (std::abs(mat3_rot(2, 0)) < ALMOST_ONE) {
            vec3[0] = std::atan2(mat3_rot(2, 1), mat3_rot(2, 2));
            vec3[2] = std::atan2(mat3_rot(1, 0), mat3_rot(0, 0));
        } else {
            vec3[0] = 0;
            vec3[2] = std::atan2(-mat3_rot(0, 1), mat3_rot(1, 1));
        }
        break;
    case EULER_ORDER::XZY:
        vec3[2] = std::asin(clamp_one(mat3_rot(1, 0)));
        if (std::abs(mat3_rot(1, 0)) < ALMOST_ONE) {
            vec3[0] = std::atan2(-mat3_rot(1, 2), mat3_rot(1, 1));
            vec3[1] = std::atan2(-mat3_rot(2, 0), mat3_rot(0, 0));
        } else {
            vec3[0] = 0;
            vec3[1] = std::atan2(mat3_rot(0, 2), mat3_rot(2, 2));
        }
        break;
    case EULER_ORDER::YXZ:
        vec3[0] = std::asin(clamp_one(mat3_rot(2, 1)));
        if (std::abs(mat3_rot(2, 1)) < ALMOST_ONE) {
            vec3[1] = std::atan2(-mat3_rot(2, 0), mat3_rot(2, 2));
            vec3[2] = std::atan2(-mat3_rot(0, 1), mat3_rot(1, 1));
        } else {
            vec3[1] = 0;
            vec3[2] = std::atan2(mat3_rot(1, 0), mat3_rot(0, 0));
        }
        break;
    case EULER_ORDER::YZX:
        vec3[2] = std::asin(clamp_one(-mat3_rot(0, 1)));
        if (std::abs(mat3_rot(0, 1)) < ALMOST_ONE) {
            vec3[0] = std::atan2(mat3_rot(2, 1), mat3_rot(1, 1));
            vec3[1] = std::atan2(mat3_rot(0, 2), mat3_rot(0, 0));
        } else {
            vec3[0] = 0;
            vec3[1] = std::atan2(-mat3_rot(1, 2), mat3_rot(2, 2));
        }
        break;
    case EULER_ORDER::ZXY:
        vec3[0] = std::asin(clamp_one(-mat3_rot(1, 2)));
        if (std::abs(mat3_rot(1, 2)) < ALMOST_ONE) {
            vec3[1] = std::atan2(mat3_rot(0, 2), mat3_rot(2, 2));
            vec3[2] = std::atan2(mat3_rot(1, 0), mat3_rot(1, 1));
        } else {
            vec3[1] = std::atan2(-mat3_rot(2, 0), mat3_rot(0, 0));
            vec3[2] = 0;
        }
        break;
    case EULER_ORDER::ZYX:
        vec3[1] = std::asin(clamp_one(mat3_rot(0, 2)));
        if (std::abs(mat3_rot(0, 2)) < ALMOST_ONE) {
            vec3[0] = std::atan2(-mat3_rot(1, 2), mat3_rot(2, 2));
            vec3[2] = std::atan2(-mat3_rot(0, 1), mat3_rot(0, 0));
        } else {
            vec3[0] = std::atan2(mat3_rot(2, 1), mat3_rot(1, 1));
            vec3[2] = 0;
        }
        break;
    }
    return vec3;
}


/* Note: xyz is not on OpenGL coordinate */
Matrix RotationMatrix::ConvertXYZ2PolarCoordinate(float x, float y, float z)
{
    Matrix vec3 = Matrix(3, 1);
    float r = std::sqrt(x * x + y * y + z * z);
    if (r == 0) return vec3;
    float theta_rad = 0;
    theta_rad = std::acos(z / r);
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
