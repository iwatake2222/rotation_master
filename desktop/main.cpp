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

/* for GLFW */
#include <GL/glew.h>     /* this must be before including glfw*/
#include <GLFW/glfw3.h>

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
    GLFWwindow* window;

    /* Initialize GLFW */
    RUN_CHECK(glfwInit() == GL_TRUE);

    /* Create a window (x4 anti-aliasing, OpenGL3.3 Core Profile)*/
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    RUN_CHECK(window = glfwCreateWindow(720, 480, "main", nullptr, nullptr));
    glfwMakeContextCurrent(window);
    
    /* Ensure not to miss input */
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /* Dark blue background */
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    
    /*** Start loop ***/
    while(1) {
        /* Clear the screen */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap buffers */
        glfwSwapBuffers(window);
        glfwPollEvents();

        /* Check if the ESC key was pressed or the window was closed */
        if (glfwWindowShouldClose(window) != 0 || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }
    }

    /*** Finalize ***/
    /* Close OpenGL window and terminate GLFW */
    glfwTerminate();

    return 0;
}
