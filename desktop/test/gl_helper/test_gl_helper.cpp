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

#include "object_data.h"
#include "shader.h"
#include "shape.h"
#include "window.h"

namespace {
#if 0
}    // indent guard
#endif

class TestGlHelper : public testing::Test
{
protected:
    TestGlHelper() {
        // You can do set-up work for each test here.
        glfwInit();
        my_window = std::make_unique<Window>();
    }

    ~TestGlHelper() override {
        // You can do clean-up work that doesn't throw exceptions here.
        glfwTerminate();
    }

    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }

    std::unique_ptr<Window> my_window;
};

TEST_F(TestGlHelper, BasicTest)
{
    EXPECT_TRUE(true);
}

TEST_F(TestGlHelper, ObjectData)
{
    EXPECT_NO_THROW(
    std::unique_ptr<Shape> cube0 = std::make_unique<ShapeSolid>(ObjectData::CubeTriangleVertex);
    std::unique_ptr<Shape> cube1 = std::make_unique<ShapeIndex>(ObjectData::CubeWireVertex, ObjectData::CubeWireIndex);
    std::unique_ptr<Shape> cube2 = std::make_unique<ShapeSolidIndex>(ObjectData::CubeTriangleVertex, ObjectData::CubeTriangleIndex);
    std::unique_ptr<Shape> ground = ObjectData::CreateGround(10.0f, 1.0f);
    std::unique_ptr<Shape> axes = ObjectData::CreateAxes(1.5f, 0.2f, { 1.0f, 0.4f, 0.4f }, { 0.4f, 1.0f, 0.4f }, { 0.4f, 0.4f, 1.0f });
    std::unique_ptr<Shape> object_axes = ObjectData::CreateAxes(1.0f, 0.1f, { 0.8f, 0.0f, 0.0f }, { 0.0f, 0.8f, 0.0f }, { 0.0f, 0.0f, 0.8f });
    std::unique_ptr<Shape> object = ObjectData::CreateMonolith(0.5f, 0.8f, 0.01f, { 0.3f, 0.75f, 1.0f }, { 0.5f, 0.5f, 0.5f });
    );
}

TEST_F(TestGlHelper, Shape)
{
    EXPECT_NO_THROW(
    std::unique_ptr<Shape> cube0 = std::make_unique<ShapeSolid>(ObjectData::CubeTriangleVertex);
    cube0->Draw(Matrix::Identity(4), Matrix::Identity(4));
    std::unique_ptr<Shape> cube2 = std::make_unique<ShapeSolidIndex>(ObjectData::CubeWireVertex, ObjectData::CubeWireIndex);
    cube2->Draw(Matrix::Identity(4), Matrix::Identity(4));

    );
}

// todo: Add more test cases

}
