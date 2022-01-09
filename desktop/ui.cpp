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
#include <vector>
#include <string>
#include <stdexcept>

/* for ImGui */
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "window.h"
#include "container.h"

#include "ui.h"

/*** Macro ***/

/*** Global variable ***/
/* this must be the same order as that of defiend in RotationMatrix */
static const char* EULER_ORDER_STR[]= {
    "XYZ",
    "XZY",
    "YXZ",
    "YZX",
    "ZXY",
    "ZYX",
};

/*** Function ***/
Ui::Ui(Window& window)
{
    /* imgui:  Setup Dear ImGui context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;  // do not use ini
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    /* imgui:  Setup Dear ImGui style */
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    /* imgui:  Setup Platform/Renderer backends */
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

Ui::~Ui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Ui::Update(Window& window, AngleUnit& angle_unit, InputContainer& input_container, OutputContainer& output_container, SettingContainer& setting_container)
{
    /* imgui:  Start the Dear ImGui frame */
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    float width_window_conversion = 0.0f;
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(450.0f, static_cast<float>(window.GetHeight())), ImGuiCond_Once);
        ImGui::Begin("Conversion", nullptr);

        /*** Input angles ***/
        ImGui::RadioButton("Rotation Matrix", &input_container.selected_representation_type, static_cast<int32_t>(REPRESENTATION_TYPE::ROTATION_MATRIX));
        if (ImGui::BeginTable("Rotation Matrix", 3)) {
            bool is_value_changed = false;
            for (int32_t row = 0; row < 3; row++) {
                ImGui::TableNextRow();
                for (int32_t col = 0; col < 3; col++) {
                    ImGui::TableSetColumnIndex(col);
                    ImGui::PushItemWidth(-FLT_MIN);
                    std::string label = std::string("R") + std::to_string(row) + std::to_string(col);
                    is_value_changed |= ImGui::DragFloat(("##" + label).c_str(), &input_container.rotation_matrix(row, col), 0.005f, -1.0f, 1.0f, (label + ": %.3f").c_str());
                        
                }
            }
            ImGui::EndTable();
            if (is_value_changed) input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::ROTATION_MATRIX);
        }
        ImGui::Separator();

        ImGui::RadioButton("Rotation Vector", &input_container.selected_representation_type, static_cast<int32_t>(REPRESENTATION_TYPE::ROTATION_VECTOR));
        if (ImGui::BeginTable("Rotation Vector", 3)) {
            bool is_value_changed = false;
            float val[3] = { angle_unit.Display(input_container.rotation_vector(0, 0)), angle_unit.Display(input_container.rotation_vector(0, 1)), angle_unit.Display(input_container.rotation_vector(0, 2)) };
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##x", &val[0], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("x: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::TableSetColumnIndex(1); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##y", &val[1], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("y: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::TableSetColumnIndex(2); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##z", &val[2], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("z: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::EndTable();
            for (int32_t i = 0; i < 3; i++) {
                input_container.rotation_vector(0, i) = angle_unit.StoreAngle(val[i]);  /* (rad or deg) to rad */
            }
            if (is_value_changed) input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::ROTATION_VECTOR);

        }
        ImGui::Separator();

        ImGui::RadioButton("Axis-angle", &input_container.selected_representation_type, static_cast<int32_t>(REPRESENTATION_TYPE::AXIS_ANGLE));
        if (ImGui::BeginTable("Axis-angle", 4)) {
            bool is_value_changed = false;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##x", &input_container.axis_angle(0, 0), 0.005f, -1.0, 1.0f, "x: %.3f");
            ImGui::TableSetColumnIndex(1); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##y", &input_container.axis_angle(0, 1), 0.005f, -1.0, 1.0f, "y: %.3f");
            ImGui::TableSetColumnIndex(2); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##z", &input_container.axis_angle(0, 2), 0.005f, -1.0, 1.0f, "z: %.3f");
            float val = angle_unit.Display(input_container.axis_angle(0, 3));
            ImGui::TableSetColumnIndex(3); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##angle", &val, angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("angle: ") + angle_unit.GetAngleFormat()).c_str());
            input_container.axis_angle(0, 3) = angle_unit.StoreAngle(val);  /* (rad or deg) to rad */
            ImGui::EndTable();
            if (is_value_changed) input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::AXIS_ANGLE);
        }
        ImGui::Separator();

        ImGui::RadioButton("Quaternion", &input_container.selected_representation_type, static_cast<int32_t>(REPRESENTATION_TYPE::QUATERNION));
        if (ImGui::BeginTable("Quaternion", 4)) {
            bool is_value_changed = false;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##x", &input_container.quaternion(0, 0), 0.005f, -1.0, 1.0f, "x: %.3f");
            ImGui::TableSetColumnIndex(1); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##y", &input_container.quaternion(0, 1), 0.005f, -1.0, 1.0f, "y: %.3f");
            ImGui::TableSetColumnIndex(2); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##z", &input_container.quaternion(0, 2), 0.005f, -1.0, 1.0f, "z: %.3f");
            ImGui::TableSetColumnIndex(3); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##w", &input_container.quaternion(0, 3), 0.005f, -1.0, 1.0f, "w: %.3f");
            ImGui::EndTable();
            if (is_value_changed) input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::QUATERNION);
        }
        ImGui::Separator();

        ImGui::RadioButton("Intrinsic (Mobile) Euler Angle", &input_container.selected_representation_type, static_cast<int32_t>(REPRESENTATION_TYPE::EULER_MOBILE));
        if (ImGui::BeginTable("Intrinsic (Mobile) Euler Angle", 4)) {
            bool is_value_changed = false;
            float val[3] = { angle_unit.Display(input_container.mobile_euler_angle(0, 0)), angle_unit.Display(input_container.mobile_euler_angle(0, 1)), angle_unit.Display(input_container.mobile_euler_angle(0, 2)) };
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::Combo("#combo", &input_container.mobile_euler_order, EULER_ORDER_STR, IM_ARRAYSIZE(EULER_ORDER_STR));
            ImGui::TableSetColumnIndex(1); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##x", &val[0], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("x: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::TableSetColumnIndex(2); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##y", &val[1], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("y: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::TableSetColumnIndex(3); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##z", &val[2], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("z: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::EndTable();
            for (int32_t i = 0; i < 3; i++) {
                input_container.mobile_euler_angle(0, i) = angle_unit.StoreAngle(val[i]);  /* (rad or deg) to rad */
            }
            if (is_value_changed) input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::EULER_MOBILE);
        }
        ImGui::Separator();

        ImGui::RadioButton("Extrinsic (Fixed) Euler Angle", &input_container.selected_representation_type, static_cast<int32_t>(REPRESENTATION_TYPE::EULER_FIXED));
        if (ImGui::BeginTable("Extrinsic (Fixed) Euler Angle", 4)) {
            bool is_value_changed = false;
            float val[3] = { angle_unit.Display(input_container.fixed_euler_angle(0, 0)), angle_unit.Display(input_container.fixed_euler_angle(0, 1)), angle_unit.Display(input_container.fixed_euler_angle(0, 2)) };
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::Combo("#combo", &input_container.fixed_euler_order, EULER_ORDER_STR, IM_ARRAYSIZE(EULER_ORDER_STR));
            ImGui::TableSetColumnIndex(1); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##x", &val[0], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("x: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::TableSetColumnIndex(2); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##y", &val[1], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("y: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::TableSetColumnIndex(3); ImGui::PushItemWidth(-FLT_MIN); is_value_changed |= ImGui::DragFloat("##z", &val[2], angle_unit.GetAngleDragSpeed(), -angle_unit.GetAngleRange(), angle_unit.GetAngleRange(), (std::string("z: ") + angle_unit.GetAngleFormat()).c_str());
            ImGui::EndTable();
            for (int32_t i = 0; i < 3; i++) {
                input_container.fixed_euler_angle(0, i) = angle_unit.StoreAngle(val[i]);  /* (rad or deg) to rad */
            }
            if (is_value_changed) input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::EULER_FIXED);
        }

        /*** Converted angles ***/
        ImGui::Separator();
        ImGui::Separator();

        ImGui::Text("Rotation Matrix");
        if (ImGui::BeginTable("Rotation Matrix", 3)) {
            for (int32_t row = 0; row < 3; row++) {
                ImGui::TableNextRow();
                for (int32_t col = 0; col < 3; col++) {
                    ImGui::TableSetColumnIndex(col);
                    std::string label = std::string("R") + std::to_string(row) + std::to_string(col);
                    ImGui::Text((label + ": %.3f").c_str(), output_container.rotation_matrix(row, col));
                }
            }
            ImGui::EndTable();
        }
        ImGui::Separator();

        ImGui::Text("Rotation Vector");
        if (ImGui::BeginTable("Rotation Vector", 3)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text((std::string("  x: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.rotation_vector(0, 0)));
            ImGui::TableSetColumnIndex(1); ImGui::Text((std::string("y: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.rotation_vector(0, 1)));
            ImGui::TableSetColumnIndex(2); ImGui::Text((std::string("z: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.rotation_vector(0, 2)));
            ImGui::EndTable();
        }
        ImGui::Separator();

        ImGui::Text("Axis-angle");
        if (ImGui::BeginTable("Axis-angle", 4)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("  x: %.3f", output_container.axis_angle(0, 0));
            ImGui::TableSetColumnIndex(1); ImGui::Text("y: %.3f", output_container.axis_angle(0, 1));
            ImGui::TableSetColumnIndex(2); ImGui::Text("z: %.3f", output_container.axis_angle(0, 2));
            ImGui::TableSetColumnIndex(3); ImGui::Text((std::string("angle: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.axis_angle(0, 3)));
            ImGui::EndTable();
        }
        ImGui::Separator();

        ImGui::Text("Quaternion");
        if (ImGui::BeginTable("Quaternion", 4)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::Text("  x: %.3f", output_container.quaternion(0, 0));
            ImGui::TableSetColumnIndex(1); ImGui::Text("y: %.3f", output_container.quaternion(0, 1));
            ImGui::TableSetColumnIndex(2); ImGui::Text("z: %.3f", output_container.quaternion(0, 2));
            ImGui::TableSetColumnIndex(3); ImGui::Text("w: %.3f", output_container.quaternion(0, 3));
            ImGui::EndTable();
        }
        ImGui::Separator();

        ImGui::Text("Intrinsic (Mobile) Euler Angle");
        if (ImGui::BeginTable("Intrinsic (Mobile) Euler Angle", 4)) {
            for (int32_t i = 0; i < static_cast<int32_t>(sizeof(EULER_ORDER_STR) / sizeof(char*)); i++) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", EULER_ORDER_STR[i]);
                ImGui::TableSetColumnIndex(1); ImGui::Text((std::string("x: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.mobile_euler_angle[i](0, 0)));
                ImGui::TableSetColumnIndex(2); ImGui::Text((std::string("y: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.mobile_euler_angle[i](0, 1)));
                ImGui::TableSetColumnIndex(3); ImGui::Text((std::string("z: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.mobile_euler_angle[i](0, 2)));

            }
            ImGui::EndTable();
        }
        ImGui::Separator();

        ImGui::Text("Extrinsic (Fixed) Euler Angle");
        if (ImGui::BeginTable("Extrinsic (Fixed) Euler Angle", 4)) {
            for (int32_t i = 0; i < static_cast<int32_t>(sizeof(EULER_ORDER_STR) / sizeof(char*)); i++) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::Text("%s", EULER_ORDER_STR[i]);
                ImGui::TableSetColumnIndex(1); ImGui::Text((std::string("x: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.fixed_euler_angle[i](0, 0)));
                ImGui::TableSetColumnIndex(2); ImGui::Text((std::string("y: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.fixed_euler_angle[i](0, 1)));
                ImGui::TableSetColumnIndex(3); ImGui::Text((std::string("z: ") + angle_unit.GetAngleFormat()).c_str(), angle_unit.Display(output_container.fixed_euler_angle[i](0, 2)));

            }
            ImGui::EndTable();
        }
        width_window_conversion = ImGui::GetWindowWidth();
        ImGui::End();
    }
    
    
    float width_window_setting = 0.0f;
    {
        ImGui::SetNextWindowPos(ImVec2(width_window_conversion, 0));
        //ImGui::SetNextWindowSize(ImVec2(400.0f, 150.0f), ImGuiCond_Once);
        ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        /*** Settings ***/
        int32_t dark_mode = setting_container.is_dark_mode ? 1 : 0;
        ImGui::Text("Color:"); ImGui::SameLine();
        ImGui::RadioButton("Dark", &dark_mode, 1); ImGui::SameLine();
        ImGui::RadioButton("Bright", &dark_mode, 0); ImGui::SameLine();
        setting_container.is_dark_mode = (dark_mode == 1);

        int32_t draw_ground = setting_container.is_draw_ground ? 1 : 0;
        ImGui::Text("  /  Ground:"); ImGui::SameLine();
        ImGui::RadioButton("Draw", &draw_ground, 1); ImGui::SameLine();
        ImGui::RadioButton("Hide", &draw_ground, 0);
        setting_container.is_draw_ground = (draw_ground == 1);

        ImGui::Separator();

        int32_t radio_degree = angle_unit.is_degree ? 1 : 0;
        ImGui::Text("Unit:"); ImGui::SameLine();
        ImGui::RadioButton("Radians", &radio_degree, 0); ImGui::SameLine();
        ImGui::RadioButton("Degrees", &radio_degree, 1);
        angle_unit.is_degree = (radio_degree == 1);

        int32_t go_around = setting_container.is_go_around ? 1 : 0;
        ImGui::Text("Move:"); ImGui::SameLine();
        ImGui::RadioButton("GoAround", &go_around, 1); ImGui::SameLine();
        ImGui::RadioButton("Free", &go_around, 0);
        setting_container.is_go_around = (go_around == 1);

        int32_t is_normalize_rotation_matrix = setting_container.is_normalize_rotation_matrix ? 1 : 0;
        ImGui::Text("Normalize Rotation Matrix:"); ImGui::SameLine();
        ImGui::RadioButton("On", &is_normalize_rotation_matrix, 1); ImGui::SameLine();
        ImGui::RadioButton("Off", &is_normalize_rotation_matrix, 0);
        setting_container.is_normalize_rotation_matrix = (is_normalize_rotation_matrix == 1);

        ImGui::Separator();
        setting_container.is_reset_view_pressed = ImGui::Button("  Reset View  "); ImGui::SameLine();
        setting_container.is_reset_value_pressed = ImGui::Button("  Reset Value  ");

        ImGui::Separator();
        setting_container.is_update_input_pressed = ImGui::Button("Overwrite input values by the converted values");
        ImGui::Separator();

        width_window_setting = ImGui::GetWindowWidth();
        ImGui::End();
    }
    

    {
        ImGui::SetNextWindowPos(ImVec2(width_window_conversion + width_window_setting, 0));
        ImGui::Begin("How to use", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text(
            "Rotation values input:\n"
            "  Mouse button drag on the input area: change value\n"
            "  Ctrl + mouse button click on the input area: enter value\n"
            "  Mouse button click on a radio button: select representation\n"
            "View:\n"
            "  Right mouse button drag: rotate camera\n"
            "  Middle mouse button drag: move camera (Free mode only)\n"
            "  Scroll mouse wheel: move forward/backward camera\n"
        );

        ImGui::End();
    }

    /* imgui:  Rendering */
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
