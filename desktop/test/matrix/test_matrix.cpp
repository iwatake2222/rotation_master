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
#include <cstdlib>
#include <climits>
#include <stdexcept>

/* GoogleTest */
#include <gtest/gtest.h>

#include "matrix.h"

namespace {
#if 0
}    // indent guard
#endif

class TestMatrix : public testing::Test
{
protected:
    TestMatrix() {
        // You can do set-up work for each test here.
    }

    ~TestMatrix() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(TestMatrix, BasicTest)
{
    EXPECT_TRUE(true);
}

TEST_F(TestMatrix, Creation)
{
    EXPECT_NO_THROW(Matrix mat1);
    EXPECT_THROW(Matrix mat1(0, 0), std::invalid_argument);
    EXPECT_THROW(Matrix mat1(1, 0), std::invalid_argument);
    EXPECT_THROW(Matrix mat1(0, 1), std::invalid_argument);
    EXPECT_THROW(Matrix mat1(INT_MAX * 0.8, INT_MAX * 0.7), std::overflow_error);
    
    Matrix mat1(2, 3);
    EXPECT_EQ(0, mat1[0]);
    EXPECT_EQ(0, mat1(0, 0));
    mat1[0] = 55;
    mat1(1, 1) = 66;
    EXPECT_EQ(55, mat1[0]);
    EXPECT_EQ(66, mat1(1, 1));
    EXPECT_THROW(mat1[6], std::out_of_range);
    EXPECT_THROW(mat1(2, 3), std::out_of_range);
    EXPECT_THROW(mat1(1, 3), std::out_of_range);
    EXPECT_THROW(mat1(2, 2), std::out_of_range);

    Matrix mat2(2, 3, { 1, 2, 3, 4, 5, 6 });
    EXPECT_EQ(3, mat2[2]);
    EXPECT_EQ(3, mat2(0, 2));
    EXPECT_EQ(5, mat2[4]);
    EXPECT_EQ(5, mat2(1, 1));

    float*p = mat2.Data();
    p[0] = 99;
    EXPECT_EQ(99, mat2[0]);

    const Matrix* pmat = &mat2;
    const float* p2 = pmat->Data();
    EXPECT_EQ(99, p2[0]);

    Matrix mat3(2, 3, mat2.Data());
    for (int32_t i = 0; i < 6; i++) {
        EXPECT_EQ(mat2[i], mat3[i]);
    }

    Matrix I = Matrix::Identity(3);
    EXPECT_EQ(1, I(0, 0));
    EXPECT_EQ(0, I(0, 1));
    EXPECT_EQ(1, I(1, 1));
    EXPECT_EQ(1, I(2, 2));
    EXPECT_THROW(I(3, 3), std::out_of_range);
}

TEST_F(TestMatrix, Add)
{
    Matrix mat1(2, 3, { 1, 2, 3, 4, 5, 6 });
    Matrix mat2(2, 3, { 7, 8, 9, 10, 11, 12 });
    Matrix mat = mat1 + mat2;
    EXPECT_EQ(8, mat[0]);
    EXPECT_EQ(18, mat[5]);
}

TEST_F(TestMatrix, Sub)
{
    Matrix mat1(2, 3, { 1, 2, 3, 4, 5, 6 });
    Matrix mat2(2, 3, { 7, 8, 9, 10, 11, 12 });
    Matrix mat = mat1 - mat2;
    EXPECT_EQ(-6, mat[0]);
    EXPECT_EQ(-6, mat[5]);
}

TEST_F(TestMatrix, ScalarMul)
{
    Matrix mat1(2, 3, { 1, 2, 3, 4, 5, 6 });
    Matrix mat = mat1 * 3;
    EXPECT_EQ(3, mat[0]);
    EXPECT_EQ(18, mat[5]);
}

TEST_F(TestMatrix, Mul)
{
    Matrix mat1(2, 3, { 1, 2, 3, 4, 5, 6 });
    Matrix mat2(3, 2, { 1, 2, 3, 4, 5, 6 });
    Matrix mat = mat1 * mat2;
    EXPECT_EQ(22, mat[0]);
    EXPECT_EQ(64, mat[3]);
}


TEST_F(TestMatrix, Transpose)
{
    Matrix mat1(2, 3, { 1, 2, 3, 4, 5, 6 });
    Matrix mat = mat1.Transpose();
    EXPECT_EQ(1, mat[0]);
    EXPECT_EQ(4, mat[1]);
}

TEST_F(TestMatrix, Inverse)
{
    Matrix mat1(2, 2, { 1, 2, 3, 4 });
    Matrix mat = mat1.Inverse();
    EXPECT_FLOAT_EQ(-2, mat(0, 0));
    EXPECT_FLOAT_EQ(1, mat(0, 1));
    EXPECT_FLOAT_EQ(1.5, mat(1, 0));
    EXPECT_FLOAT_EQ(-0.5, mat(1, 1));

    Matrix mat2(2, 3);
    EXPECT_THROW(mat2.Inverse(), std::out_of_range);
    Matrix mat3(2, 2);
    EXPECT_THROW(mat3.Inverse(), std::out_of_range);
}

TEST_F(TestMatrix, isc)
{
    Matrix mat1(2, 3, { 1, 2, 3, 4, 5, 6 });
    EXPECT_NO_THROW(mat1.Print());

    Matrix mat2(3, 1);
    EXPECT_NO_THROW(mat2.Print());

    Matrix mat3(1, 3);
    EXPECT_NO_THROW(mat3.Print());
}

}
