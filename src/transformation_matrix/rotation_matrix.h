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
#ifndef ROTATION_MATRIX_H
#define ROTATION_MATRIX_H

/*** Include ***/
#include <cstdint>
#include <cstdio>
#include <vector>

#include "matrix.h"

namespace RotationMatrix
{
    enum class EULER_ORDER {
        XYZ = 0,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX,
    };

    /* The following functions return 3x3 rotation matrix */
    Matrix RotateX(float rad);
    Matrix RotateY(float rad);
    Matrix RotateZ(float rad);
    Matrix NormalizeRotationMatrix(const Matrix mat3_rot);
    Matrix ConvertRotationVector2RotationMatrix(float x_rad, float y_rad, float z_rad);
    Matrix ConvertAxisAngle2RotationMatrix(float x, float y, float z, float rad);
    Matrix ConvertQuaternion2RotationMatrix(float x, float y, float z, float w);
    Matrix ConvertEulerMobile2RotationMatrix(RotationMatrix::EULER_ORDER order, float x, float y, float z);
    Matrix ConvertEulerFixed2RotationMatrix(RotationMatrix::EULER_ORDER order, float x, float y, float z);

    /* The following functions input 3x3 rotation matrix, and return vector (3x1 or 4x1) */
    Matrix ConvertRotationMatrix2RotationVector(const Matrix& mat3_rot);   /* return 3 x 1 vector */
    Matrix ConvertRotationMatrix2AxisAngle(const Matrix& mat3_rot);   /* return 4 x 1 vector */
    Matrix ConvertRotationMatrix2Quaternion(const Matrix& mat3_rot);   /* return 4 x 1 vector */
    Matrix ConvertRotationMatrix2EulerMobile(RotationMatrix::EULER_ORDER order, const Matrix& mat3_rot);   /* return 3 x 1 vector */
    Matrix ConvertRotationMatrix2EulerFixed(RotationMatrix::EULER_ORDER order, const Matrix& mat3_rot);   /* return 3 x 1 vector */

    /* Otehr functions */
    Matrix ConvertXYZ2PolarCoordinate(float x, float y, float z);   /* return 3 x 1 vector */
    Matrix ConvertPolarCoordinate2XYZ(float r, float theta_rad, float phi_rad);  /* return 3 x 1 vector */
};

#endif
