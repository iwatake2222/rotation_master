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
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <stdexcept>

/* GoogleTest */
#include <gtest/gtest.h>

#include "matrix.h"
#include "rotation_matrix.h"

namespace {
#if 0
}    // indent guard
#endif

static inline float Deg2Rad(float deg) { return static_cast<float>(deg * M_PI / 180.0); }
static inline float Rad2Deg(float rad) { return static_cast<float>(rad * 180.0 / M_PI); }

class TestRotationMatrix : public testing::Test
{
protected:
    TestRotationMatrix() {
        // You can do set-up work for each test here.
    }

    ~TestRotationMatrix() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(TestRotationMatrix, BasicTest)
{
    EXPECT_TRUE(true);
}



TEST_F(TestRotationMatrix, RotateX)
{
    Matrix mat_object(3, 1, { 10, 20, 0 });
    Matrix mat_rot = RotationMatrix::RotateX(Deg2Rad(30.0f));
    Matrix mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(2, 0));

    mat_object = Matrix(3, 1, { 10, 0, 20 });
    mat_rot = RotationMatrix::RotateX(Deg2Rad(30.0f));
    mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(-20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(2, 0));
}

TEST_F(TestRotationMatrix, RotateY)
{
    Matrix mat_object(3, 1, { 10, 20, 0});
    Matrix mat_rot = RotationMatrix::RotateY(Deg2Rad(30.0f));
    Matrix mat_transformed = mat_rot * mat_object;

    EXPECT_FLOAT_EQ(10.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(-10.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(2, 0));

    mat_object = Matrix(3, 1, { 0, 20, 10});
    mat_rot = RotationMatrix::RotateY(Deg2Rad(30.0f));
    mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(10.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(10.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(2, 0));
}

TEST_F(TestRotationMatrix, RotateZ)
{
    Matrix mat_object(3, 1, { 0, 20, 10});
    Matrix mat_rot = RotationMatrix::RotateZ(Deg2Rad(30.0f));
    Matrix mat_transformed = mat_rot * mat_object;

    EXPECT_FLOAT_EQ(-20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(2, 0));

    mat_object = Matrix(3, 1, { 20, 0, 10});
    mat_rot = RotationMatrix::RotateZ(Deg2Rad(30.0f));
    mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(2, 0));
}

TEST_F(TestRotationMatrix, NormalizeRotationMatrix)
{
    Matrix mat_rot(3, 3, {
        1.0f, -2.0f, 3.0f,
        -4.0f, 5.0f, -6.0f,
        7.0f, -8.0f, 9.0f, }
    );
    Matrix mat_rot_normalized = RotationMatrix::NormalizeRotationMatrix(mat_rot);
    EXPECT_FLOAT_EQ(0.8571429f, mat_rot_normalized(0, 0));
    EXPECT_FLOAT_EQ(0.2857143f, mat_rot_normalized(0, 1));
    EXPECT_FLOAT_EQ(-0.4285714f, mat_rot_normalized(0, 2));
    EXPECT_FLOAT_EQ(-0.1714286f, mat_rot_normalized(1, 0));
    EXPECT_FLOAT_EQ(0.9428571f, mat_rot_normalized(1, 1));
    EXPECT_FLOAT_EQ(0.2857143f, mat_rot_normalized(1, 2));
    EXPECT_FLOAT_EQ(0.4857143f, mat_rot_normalized(2, 0));
    EXPECT_FLOAT_EQ(-0.1714286f, mat_rot_normalized(2, 1));
    EXPECT_FLOAT_EQ(0.8571429f, mat_rot_normalized(2, 2));
}


TEST_F(TestRotationMatrix, RotationVector)
{
    auto test = [](float x_deg, float y_deg, float z_deg) {
        Matrix vec(3, 1, { Deg2Rad(x_deg), Deg2Rad(y_deg), Deg2Rad(z_deg) });
        Matrix mat_rot = RotationMatrix::ConvertRotationVector2RotationMatrix(vec[0], vec[1], vec[2]);
        Matrix vec_reverted = RotationMatrix::ConvertRotationMatrix2RotationVector(mat_rot);
        for (int32_t i = 0; i < 3; i++) {
            EXPECT_NEAR(vec[i], vec_reverted[i], 0.01f);
        }
    };
    
    test(10, 20, 30);
    test(-10, 20, 30);
    test(10, -20, 30);
    test(10, 20, -30);
    test(10, -20, -30);
    test(-10, 20, -30);
    test(-10, -20, 30);
    test(-10, -20, -30);

    test(0, 0, 0);
    test(90, 0, 0);
    test(0, 90, 0);
    test(0, 0, 90);
    test(90, 90, 0);
    test(0, 90, 90);
    test(0, 90, 90);
    test(90, 90, 90);
}

TEST_F(TestRotationMatrix, AxisAngles)
{
    auto test = [](float x, float y, float z, float rad) {
        float d = std::sqrt(x * x + y * y + z * z);
        x /= d;
        y /= d;
        z /= d;
        Matrix vec(4, 1, { x, y, z, Deg2Rad(rad) });
        Matrix mat_rot = RotationMatrix::ConvertAxisAngle2RotationMatrix(vec[0], vec[1], vec[2], vec[3]);
        Matrix vec_reverted = RotationMatrix::ConvertRotationMatrix2AxisAngle(mat_rot);
        for (int32_t i = 0; i < 4; i++) {
            EXPECT_NEAR(vec[i], vec_reverted[i], 0.001f);
        }
    };

    //test(10, 20, 30, 0);
    test(10, 0, 0, 50);
    test(0, 20, 0, 50);
    test(0, 0, 30, 50);
    test(10, 20, 30, 50);
    //test(10, 20, 30, -50);
    test(-10, -20, -30, 50);
}

TEST_F(TestRotationMatrix, Quaternion)
{
    auto test = [](float x, float y, float z, float w) {
        float d = std::sqrt(x * x + y * y + z * z + w * w);
        x /= d;
        y /= d;
        z /= d;
        w /= d;
        Matrix vec(4, 1, { x, y, z, w });
        Matrix mat_rot = RotationMatrix::ConvertQuaternion2RotationMatrix(vec[0], vec[1], vec[2], vec[3]);
        Matrix vec_reverted = RotationMatrix::ConvertRotationMatrix2Quaternion(mat_rot);
        for (int32_t i = 0; i < 4; i++) {
            EXPECT_NEAR(vec[i], vec_reverted[i], 0.001f);
        }
    };

    test(0, 0, 0, 1);
    test(1, 0, 0, 1);
    test(0, 1, 0, 1);
    test(0, 0, 1, 1);
    test(0, 1, 1, 1);
    test(1, 0, 1, 1);
    test(1, 1, 0, 1);
    test(1, 1, 1, 1);
    test(1, 2, 3, 4);
    //test(1, 2, 3, -4);
    test(-1, 2, 3, 4);
    test(-1, -2, -3, 4);
}

TEST_F(TestRotationMatrix, EulerMobile)
{
    auto test = [](float x_deg, float y_deg, float z_deg) {
        Matrix vec(3, 1, { Deg2Rad(x_deg), Deg2Rad(y_deg), Deg2Rad(z_deg) });
        for (int32_t i = 0; i < 6; i++) {
            Matrix mat_rot = RotationMatrix::ConvertEulerMobile2RotationMatrix(static_cast<RotationMatrix::EULER_ORDER>(i), vec[0], vec[1], vec[2]);
            Matrix vec_reverted = RotationMatrix::ConvertRotationMatrix2EulerMobile(static_cast<RotationMatrix::EULER_ORDER>(i), mat_rot);
            for (int32_t j = 0; j < 3; j++) {
                EXPECT_NEAR(Rad2Deg(vec[j]), Rad2Deg(vec_reverted[j]), 0.01f);
            }
        }
    };

    test(0, 0, 0);
    test(10, 20, 30);
    test(-10, 20, 30);
    test(10, -20, 30);
    test(10, 20, -30);
}

TEST_F(TestRotationMatrix, EulerFixed)
{
    auto test = [](float x_deg, float y_deg, float z_deg) {
        Matrix vec(3, 1, { Deg2Rad(x_deg), Deg2Rad(y_deg), Deg2Rad(z_deg) });
        for (int32_t i = 0; i < 6; i++) {
            Matrix mat_rot = RotationMatrix::ConvertEulerFixed2RotationMatrix(static_cast<RotationMatrix::EULER_ORDER>(i), vec[0], vec[1], vec[2]);
            Matrix vec_reverted = RotationMatrix::ConvertRotationMatrix2EulerFixed(static_cast<RotationMatrix::EULER_ORDER>(i), mat_rot);
            for (int32_t j = 0; j < 3; j++) {
                EXPECT_NEAR(Rad2Deg(vec[j]), Rad2Deg(vec_reverted[j]), 0.01f);
            }
        }
    };

    test(0, 0, 0);
    test(10, 20, 30);
    test(-10, 20, 30);
    test(10, -20, 30);
    test(10, 20, -30);
}

TEST_F(TestRotationMatrix, PolarCoordinate)
{
    auto test = [](float x, float y, float z) {
        Matrix vec = RotationMatrix::ConvertXYZ2PolarCoordinate(x, y, z);
        Matrix xyz = RotationMatrix::ConvertPolarCoordinate2XYZ(vec[0], vec[1], vec[2]);
        EXPECT_NEAR(xyz[0], x, 0.01f);
        EXPECT_NEAR(xyz[1], y, 0.01f);
        EXPECT_NEAR(xyz[2], z, 0.01f);
    };

    test(0, 0, 0);
    test(10, 20, 30);
    test(-10, 20, 30);
    test(10, -20, 30);
    test(10, 20, -30);
}

}
