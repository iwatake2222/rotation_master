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

#include "container.h"

#include "converter.h"

/*** Macro ***/

/*** Global variable ***/

/*** Function ***/
void Converter::UpdateInput(InputContainer& input_container, OutputContainer& output_container)
{
    input_container.rotation_matrix = output_container.rotation_matrix;
    input_container.quaternion = output_container.quaternion;
    input_container.axis_angle = output_container.axis_angle;
    input_container.axis_angle_mag = output_container.axis_angle_mag;
    input_container.mobile_euler_angle = output_container.mobile_euler_angle[input_container.mobile_euler_order];
    input_container.fixed_euler_angle = output_container.fixed_euler_angle[input_container.fixed_euler_order];
}

void Converter::ResetValue(InputContainer& input_container, OutputContainer& output_container)
{
    input_container.selected_representation_type = static_cast<int32_t>(REPRESENTATION_TYPE::ROTATION_MATRIX);
    input_container.rotation_matrix = Matrix::Identity(3);
    Converter::Convert(input_container, output_container);
}

void Converter::Convert(InputContainer& input_container, OutputContainer& output_container)
{
    /* todo */
    output_container.mobile_euler_angle[0](0, 0) = input_container.mobile_euler_angle(0, 0);
    output_container.mobile_euler_angle[0](0, 1) = input_container.mobile_euler_angle(0, 1);
    output_container.mobile_euler_angle[0](0, 2) = input_container.mobile_euler_angle(0, 2);
}

