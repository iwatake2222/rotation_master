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
#include "transformation_matrix.h"

namespace {
#if 0
}    // indent guard
#endif

static inline float Deg2Rad(float deg) { return static_cast<float>(deg * M_PI / 180.0); }
static inline float Rad2Deg(float rad) { return static_cast<float>(rad * 180.0 / M_PI); }

class TestTransformationMatrix : public testing::Test
{
protected:
    TestTransformationMatrix() {
        // You can do set-up work for each test here.
    }

    ~TestTransformationMatrix() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(TestTransformationMatrix, BasicTest)
{
    EXPECT_TRUE(true);
}


TEST_F(TestTransformationMatrix, Translate)
{
    Matrix mat_object(4, 1, {10, 20, 30, 1} );
    Matrix mat_tx = TransformationMatrix::Translate(1.0f, 0.0f, 0.0f);
    Matrix mat_txyz = TransformationMatrix::Translate(1.0f, -2.0f, 3.0f);

    Matrix mat_transformed = mat_tx * mat_object;
    EXPECT_FLOAT_EQ(11.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(30.0f, mat_transformed(2, 0));

    mat_transformed = mat_txyz * mat_object;
    EXPECT_FLOAT_EQ(11.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(18.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(33.0f, mat_transformed(2, 0));
}

TEST_F(TestTransformationMatrix, Scale)
{
    Matrix mat_object(4, 1, {10, 20, 30, 1} );
    Matrix mat_enlarge = TransformationMatrix::Scale(1.0f, 2.0f, 3.0f);
    Matrix mat_transformed = mat_enlarge * mat_object;
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(40.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(90.0f, mat_transformed(2, 0));

    Matrix mat_shorten = TransformationMatrix::Scale(0.5f, 0.1f, 1.0f / 3.0f);
    Matrix mat_transformed2 = mat_shorten * mat_object;
    EXPECT_FLOAT_EQ(5.0f, mat_transformed2(0, 0));
    EXPECT_FLOAT_EQ(2.0f, mat_transformed2(1, 0));
    EXPECT_FLOAT_EQ(10.0f, mat_transformed2(2, 0));
}

TEST_F(TestTransformationMatrix, RotateX)
{
    Matrix mat_object(4, 1, {10, 20, 0, 1} );
    Matrix mat_rot = TransformationMatrix::RotateX(Deg2Rad(30.0f));
    Matrix mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(2, 0));

    mat_object = Matrix(4, 1, {10, 0, 20, 1} );
    mat_rot = TransformationMatrix::RotateX(Deg2Rad(30.0f));
    mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(-20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(2, 0));
}

TEST_F(TestTransformationMatrix, RotateY)
{
    Matrix mat_object(4, 1, {10, 20, 0, 1} );
    Matrix mat_rot = TransformationMatrix::RotateY(Deg2Rad(30.0f));
    Matrix mat_transformed = mat_rot * mat_object;
    
    EXPECT_FLOAT_EQ(10.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(-10.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(2, 0));

    mat_object = Matrix(4, 1, {0, 20, 10, 1} );
    mat_rot = TransformationMatrix::RotateY(Deg2Rad(30.0f));
    mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(10.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f, mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(10.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(2, 0));
}

TEST_F(TestTransformationMatrix, RotateZ)
{
    Matrix mat_object(4, 1, {0, 20, 10, 1} );
    Matrix mat_rot = TransformationMatrix::RotateZ(Deg2Rad(30.0f));
    Matrix mat_transformed = mat_rot * mat_object;
    
    EXPECT_FLOAT_EQ(-20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(2, 0));

    mat_object = Matrix(4, 1, {20, 0, 10, 1} );
    mat_rot = TransformationMatrix::RotateZ(Deg2Rad(30.0f));
    mat_transformed = mat_rot * mat_object;
    EXPECT_FLOAT_EQ(20.0f * std::cos(Deg2Rad(30.0f)), mat_transformed(0, 0));
    EXPECT_FLOAT_EQ(20.0f * std::sin(Deg2Rad(30.0f)), mat_transformed(1, 0));
    EXPECT_FLOAT_EQ(10.0f, mat_transformed(2, 0));
}

TEST_F(TestTransformationMatrix, Rotate)
{
    Matrix mat_object(4, 1, {0, 20, 10, 1} );
    Matrix mat_rot = TransformationMatrix::RotateAxisAngle(Deg2Rad(30.0f), 0.1f, 0.2f, 0.5f);
    Matrix mat_transformed = mat_rot * mat_object;
    
    // todo
}

TEST_F(TestTransformationMatrix, LookAt)
{
    Matrix mat_object(4, 1, {0, 20, 10, 1} );
    Matrix mat_rot = TransformationMatrix::LookAt(0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.0f, 1.0f, 0.0f);
    Matrix mat_transformed = mat_rot * mat_object;

    mat_rot = TransformationMatrix::LookAt( {0.1f, 0.2f, 0.3f}, {0.4f, 0.5f, 0.6f}, {0.0f, 1.0f, 0.0f});
    mat_transformed = mat_rot * mat_object;

    // todo
}

}

