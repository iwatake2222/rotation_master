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
#include "converter.h"

/*** Macro ***/
/* macro functions */
#define RUN_CHECK(x)                                         \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

/* Setting */

/*** Global variable ***/


/*** Function ***/
int main(int argc, char *argv[])
{
    /*** Initialize ***/
    /* Initialize OpenGL */
    RUN_CHECK(glfwInit() == GL_TRUE);
    glfwSetTime(0.0);

    /* Initialize window class */
    Window my_window;
    my_window.LookAt({ 2.0f, 2.0f, 3.0f }, { -0.5f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

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
    while(1) {
        if (my_window.FrameStart() == false) break;

        my_window.SetIsDarkMode(setting_container.is_dark_mode);

        /* Draw bases */
        if (setting_container.is_draw_ground) {
            Shape::SetLineWidth(0.5f);
            ground->Draw(my_window.GetViewProjection(), TransformationMatrix::Translate(0.0f, -1.0f, 0.0f));
        }
        Shape::SetLineWidth(2.0f);
        axes->Draw(my_window.GetViewProjection(), Matrix::Identity(4));
        
        /* Draw monolith */
        Matrix model = Matrix::Identity(4);
        model = TransformationMatrix::RotateX(output_container.mobile_euler_angle[0](0, 0)) * TransformationMatrix::RotateY(output_container.mobile_euler_angle[0](0, 1)) * TransformationMatrix::RotateZ(output_container.mobile_euler_angle[0](0, 2));
        object->Draw(my_window.GetViewProjection(), model);
        Shape::SetLineWidth(10.0f);
        object_axes->Draw(my_window.GetViewProjection(), model);

        /* Draw UI */
        my_ui.Update(my_window, angle_unit, input_container, output_container, setting_container);

        if (setting_container.is_reset_view_pressed) {
            my_window.LookAt({ 2.0f, 2.0f, 3.0f }, { -0.5f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
        }

        /* Convert representations of rotation */
        Converter::Convert(input_container, output_container);
        if (setting_container.is_update_input_pressed) {
            Converter::UpdateInput(input_container, output_container);
        }
        if (setting_container.is_reset_value_pressed) {
            Converter::ResetValue(input_container, output_container);
        }

        /* Update display */
        my_window.SwapBuffers();
    }
    
    /*** Finalize ***/
    glfwTerminate();

    return 0;
}
