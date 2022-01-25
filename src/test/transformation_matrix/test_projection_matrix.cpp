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
#include <stdexcept>

/* GoogleTest */
#include <gtest/gtest.h>

#include "matrix.h"
#include "projection_matrix.h"

namespace {
#if 0
}    // indent guard
#endif

class TestProjectionMatrix : public testing::Test
{
protected:
    TestProjectionMatrix() {
        // You can do set-up work for each test here.
    }

    ~TestProjectionMatrix() override {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }
};

TEST_F(TestProjectionMatrix, BasicTest)
{
    EXPECT_TRUE(true);
}

TEST_F(TestProjectionMatrix, Create)
{
    EXPECT_NO_THROW(
        Matrix mat = ProjectionMatrix::Orthogonal(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
    );

    EXPECT_NO_THROW(
        Matrix mat = ProjectionMatrix::Frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
    );

    EXPECT_NO_THROW(
        Matrix mat = ProjectionMatrix::Perspective(0.0f, 0.0f, 1.0f, 1920.0f / 1080.0f, 0.1f, 1000.0f);
    );
}

// todo: Add more test cases

}
