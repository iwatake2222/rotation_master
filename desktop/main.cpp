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
#include <memory>

/* for GLFW */
#include <GLFW/glfw3.h>

/* for ImGui */
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/* for my modules */
#include "matrix.h"
#include "transformation_matrix.h"
#include "window.h"
#include "shape.h"
#include "object_data.h"

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
    //std::atexit(glfwTerminate);
    glfwSetTime(0.0);

    /* Initialize window class */
    Window my_window;
    my_window.LookAt({ 0.0f, 1.5f, 2.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });

    /* Initialize ImGui */
    /* imgui:  Setup Dear ImGui context */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    /* imgui:  Setup Dear ImGui style */
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    /* imgui:  Setup Platform/Renderer backends */
    ImGui_ImplGlfw_InitForOpenGL(my_window.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    /* imgui: state */
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /* Create shape */
    std::unique_ptr<Shape> cube0 = std::make_unique<ShapeSolid>(ObjectData::CubeTriangleVertex);
    std::unique_ptr<Shape> cube1 = std::make_unique<ShapeIndex>(ObjectData::CubeWireVertex, ObjectData::CubeWireIndex);
    std::unique_ptr<Shape> ground = ObjectData::CreateGround(10.0f, 1.0f);
    std::unique_ptr<Shape> axes = ObjectData::CreateAxes(1.5f, 0.2f, { 1.0f, 0.4f, 0.4f }, { 0.4f, 1.0f, 0.4f }, { 0.4f, 0.4f, 1.0f });
    std::unique_ptr<Shape> object_axes = ObjectData::CreateAxes(1.0f, 0.1f, { 0.8f, 0.0f, 0.0f }, { 0.0f, 0.8f, 0.0f }, { 0.0f, 0.0f, 0.8f });
    std::unique_ptr<Shape> object = ObjectData::CreateMonolith(0.5f, 0.8f, 0.01f, { 0.3f, 0.75f, 1.0f }, { 0.5f, 0.5f, 0.5f });

    /*** Start loop ***/
    while(1) {
        if (my_window.FrameStart() == false) break;

        glLineWidth(0.5f);
        ground->Draw(my_window.GetViewProjection(), TransformationMatrix::Translate(0.0f, -1.0f, 0.0f));
        glLineWidth(2.0f);
        axes->Draw(my_window.GetViewProjection(), Matrix::Identity(4));
        
        Matrix model = Matrix::Identity(4);
        model = TransformationMatrix::Rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f);
        object->Draw(my_window.GetViewProjection(), model);
        glLineWidth(10.0f);
        //glDisable(GL_DEPTH_TEST);
        object_axes->Draw(my_window.GetViewProjection(), model);
        //glEnable(GL_DEPTH_TEST);

        glLineWidth(1.0f);
        const Matrix r = TransformationMatrix::Rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f);
        const Matrix translation0 = TransformationMatrix::Translate(3.0f, 3.0f, 0.0f);
        const Matrix model0 = translation0 * r;
        cube0->Draw(my_window.GetViewProjection(), model0);
        const Matrix translation1 = TransformationMatrix::Translate(3.0f, 0.0f, 0.0f);
        const Matrix model1 = translation1 * r;
        cube1->Draw(my_window.GetViewProjection(), model1);


        /* imgui:  Start the Dear ImGui frame */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
        /* imgui:  Rendering */
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        my_window.SwapBuffers();
    }

    /*** Finalize ***/
    /* imgui: cleanup */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* Close OpenGL window and terminate GLFW */
    //glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
