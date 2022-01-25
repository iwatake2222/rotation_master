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
#include <string>
#include <memory>
#include <functional>

/* for GLFW */
#include <GLFW/glfw3.h>

/* for my modules */
#include "matrix.h"
#include "transformation_matrix.h"
#include "shape.h"
#include "object_data.h"
#include "container.h"
#include "window.h"
#include "ui.h"
#include "rotation_matrix.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

/*** Macro ***/
/* macro functions */
#define RUN_CHECK(x)                                         \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

/* Setting */
static constexpr float PROJECTION_OFFSET_CX = -0.2f; /* add offset for cx because we have UI bar on the left side */
static constexpr float PROJECTION_OFFSET_CY = 0.2f; /* add offset for cx because we have UI bar on the left side */

/*** Global variable ***/

/*** Function ***/
static void ConvertAll(InputContainer& input_container, OutputContainer& output_container, bool is_normalize_rotation_matrix)
{
    /* First, Convert the selected input representation to rotatin matrix (mat3_rot) */
    Matrix mat3_rot = Matrix::Identity(3);
    switch (static_cast<REPRESENTATION_TYPE>(input_container.selected_representation_type)) {
    case REPRESENTATION_TYPE::ROTATION_MATRIX:
        for (int32_t i = 0; i < 9; i++) {
            mat3_rot[i] = input_container.rotation_matrix[i];
        }
        if (is_normalize_rotation_matrix) {
            mat3_rot = RotationMatrix::NormalizeRotationMatrix(mat3_rot);
        }
        break;
    case REPRESENTATION_TYPE::ROTATION_VECTOR:
        mat3_rot = RotationMatrix::ConvertRotationVector2RotationMatrix(input_container.rotation_vector[0], input_container.rotation_vector[1], input_container.rotation_vector[2]);
        break;
    case REPRESENTATION_TYPE::AXIS_ANGLE:
        mat3_rot = RotationMatrix::ConvertAxisAngle2RotationMatrix(input_container.axis_angle[0], input_container.axis_angle[1], input_container.axis_angle[2], input_container.axis_angle[3]);
        break;
    case REPRESENTATION_TYPE::QUATERNION:
        mat3_rot = RotationMatrix::ConvertQuaternion2RotationMatrix(input_container.quaternion[0], input_container.quaternion[1], input_container.quaternion[2], input_container.quaternion[3]);
        break;
    case REPRESENTATION_TYPE::EULER_MOBILE:
        mat3_rot = RotationMatrix::ConvertEulerMobile2RotationMatrix(static_cast<RotationMatrix::EULER_ORDER>(input_container.mobile_euler_order), input_container.mobile_euler_angle[0], input_container.mobile_euler_angle[1], input_container.mobile_euler_angle[2]);
        break;
    case REPRESENTATION_TYPE::EULER_FIXED:
        mat3_rot = RotationMatrix::ConvertEulerFixed2RotationMatrix(static_cast<RotationMatrix::EULER_ORDER>(input_container.fixed_euler_order), input_container.fixed_euler_angle[0], input_container.fixed_euler_angle[1], input_container.fixed_euler_angle[2]);
        break;
    }

    /* Then, Convert the calculated rotation matrix (mat3_rot) to all the other representations (output) */
    for (int32_t i = 0; i < 9; i++) {
        output_container.rotation_matrix[i] = mat3_rot[i];
    }
    output_container.rotation_vector = RotationMatrix::ConvertRotationMatrix2RotationVector(mat3_rot);
    output_container.axis_angle = RotationMatrix::ConvertRotationMatrix2AxisAngle(mat3_rot);
    output_container.quaternion = RotationMatrix::ConvertRotationMatrix2Quaternion(mat3_rot);
    for (int32_t i = 0; i < static_cast<int32_t>(sizeof(output_container.mobile_euler_angle) / sizeof(Matrix)); i++) {
        output_container.mobile_euler_angle[i] = RotationMatrix::ConvertRotationMatrix2EulerMobile(static_cast<RotationMatrix::EULER_ORDER>(i), mat3_rot);
        output_container.fixed_euler_angle[i] = RotationMatrix::ConvertRotationMatrix2EulerFixed(static_cast<RotationMatrix::EULER_ORDER>(i), mat3_rot);
    }
}

static void OverwriteInput(InputContainer& input_container, OutputContainer& output_container)
{
    input_container.rotation_matrix = output_container.rotation_matrix;
    input_container.quaternion = output_container.quaternion;
    input_container.axis_angle = output_container.axis_angle;
    input_container.rotation_vector = output_container.rotation_vector;
    input_container.mobile_euler_angle = output_container.mobile_euler_angle[input_container.mobile_euler_order];
    input_container.fixed_euler_angle = output_container.fixed_euler_angle[input_container.fixed_euler_order];
}

static void ResetValues(InputContainer& input_container, OutputContainer& output_container)
{
    input_container.Reset();
    output_container.Reset();
}

int main(int argc, char *argv[])
{
    /*** Initialize ***/
    /* Initialize OpenGL */
    RUN_CHECK(glfwInit() == GL_TRUE);
    glfwSetTime(0.0);

    /* Initialize window class */
    Window my_window;
    my_window.LookAt({ 2.0f, 2.0f, 3.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

    /* Initialize ImGui */
    Ui my_ui(my_window);

    /* Containers */
    AngleUnit angle_unit;
    InputContainer input_container;
    OutputContainer output_container;
    SettingContainer setting_container;

    /* Create shape */
    std::unique_ptr<Shape> ground = ObjectData::CreateGround(10.0f, 1.0f);
    std::unique_ptr<Shape> axes = ObjectData::CreateAxes(1.5f, 0.2f, { 1.0f, 0.4f, 0.4f }, { 0.4f, 1.0f, 0.4f }, { 0.4f, 0.4f, 1.0f });
    std::unique_ptr<Shape> object_axes = ObjectData::CreateAxes(1.0f, 0.1f, { 0.8f, 0.0f, 0.0f }, { 0.0f, 0.8f, 0.0f }, { 0.0f, 0.0f, 0.8f });
    std::unique_ptr<Shape> object = ObjectData::CreateMonolith(0.5f, 0.8f, 0.01f, { 0.3f, 0.75f, 1.0f }, { 0.5f, 0.5f, 0.5f });

    /*** Start loop ***/
    static std::function<void()> loop;
    loop = [&]() {
        if (my_window.FrameStart() == false) return;

        /* Draw bases */
        if (setting_container.is_draw_ground) {
            Shape::SetLineWidth(0.5f);
            ground->Draw(my_window.GetViewProjection(PROJECTION_OFFSET_CX, PROJECTION_OFFSET_CY), TransformationMatrix::Translate(0.0f, -1.0f, 0.0f));
        }
        Shape::SetLineWidth(2.0f);
        axes->Draw(my_window.GetViewProjection(PROJECTION_OFFSET_CX, PROJECTION_OFFSET_CY), Matrix::Identity(4));
        
        /* Draw monolith */
        Matrix model_pose = TransformationMatrix::Expand3to4(output_container.rotation_matrix);
        object->Draw(my_window.GetViewProjection(PROJECTION_OFFSET_CX, PROJECTION_OFFSET_CY), model_pose);
        Shape::SetLineWidth(10.0f);
        object_axes->Draw(my_window.GetViewProjection(PROJECTION_OFFSET_CX, PROJECTION_OFFSET_CY), model_pose);

        /* Draw monolith from each axis*/
        if (setting_container.is_view_from_axis) {
            Shape::SetLineWidth(2.0f);
            static constexpr float SIZE_VIEW_FROM_AXIS = 0.1f;
            static constexpr float START_POS_OF_VIEW_FROM_AXIS = 0.4f;
            static constexpr float INTERVAL_OF_VIEW_FROM_AXIS = 0.6f;
            model_pose = model_pose * TransformationMatrix::Scale(SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS);
            Matrix view_projection_from_x = my_window.GetViewProjectionFromAxisX(-(0.91f - SIZE_VIEW_FROM_AXIS), -START_POS_OF_VIEW_FROM_AXIS + INTERVAL_OF_VIEW_FROM_AXIS * 0);
            Matrix view_projection_from_y = my_window.GetViewProjectionFromAxisY(-(0.91f - SIZE_VIEW_FROM_AXIS), -START_POS_OF_VIEW_FROM_AXIS + INTERVAL_OF_VIEW_FROM_AXIS * 1);
            Matrix view_projection_from_z = my_window.GetViewProjectionFromAxisZ(-(0.91f - SIZE_VIEW_FROM_AXIS), -START_POS_OF_VIEW_FROM_AXIS + INTERVAL_OF_VIEW_FROM_AXIS * 2);
            axes->Draw(view_projection_from_x, TransformationMatrix::Scale(SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS));
            object->Draw(view_projection_from_x, model_pose);
            object_axes->Draw(view_projection_from_x, model_pose);
            axes->Draw(view_projection_from_y, TransformationMatrix::Scale(SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS));
            object->Draw(view_projection_from_y, model_pose);
            object_axes->Draw(view_projection_from_y, model_pose);
            axes->Draw(view_projection_from_z, TransformationMatrix::Scale(SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS, SIZE_VIEW_FROM_AXIS));
            object->Draw(view_projection_from_z, model_pose);
            object_axes->Draw(view_projection_from_z, model_pose);
        }

        /* Draw UI */
        my_ui.Update(my_window, angle_unit, input_container, output_container, setting_container);

        if (setting_container.is_reset_view_pressed) {
            my_window.LookAt({ 2.0f, 2.0f, 3.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
        }

        /* Convert representations of rotation */
        ConvertAll(input_container, output_container, setting_container.is_normalize_rotation_matrix);
        if (setting_container.is_update_input_pressed) {
            OverwriteInput(input_container, output_container);
        }
        if (setting_container.is_reset_value_pressed) {
            ResetValues(input_container, output_container);
        }

        my_window.SetIsDarkMode(setting_container.is_dark_mode);
        my_window.SetIsCameraRevolution(setting_container.is_camera_revolution);

        /* Update display */
        my_window.SwapBuffers();
    };

#ifdef __EMSCRIPTEN__
    struct dummy {
        static void dummy_loop() { loop(); }
    };
    emscripten_set_main_loop(dummy::dummy_loop, 60, true);
#else
    while(true) loop();
#endif

    /*** Finalize ***/
    glfwTerminate();

    return 0;
}
