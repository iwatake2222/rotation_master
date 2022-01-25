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
#include <array>
#include <vector>

/* for GLFW */
#include <GL/glew.h>     /* this must be before including glfw*/
#include <GLFW/glfw3.h>

/* for my modules */
#include "matrix.h"
#include "transformation_matrix.h"
#include "projection_matrix.h"
#include "rotation_matrix.h"

#include "window.h"

/*** Macro ***/
/* macro function */
#define RUN_CHECK(x)                                         \
  if (!(x)) {                                                \
    fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
    exit(1);                                                 \
  }

/* Setting */
static constexpr float KEY_SPEED = 3.0f; // 3 units / second
static constexpr float MOUSE_ROT_SPEED = 0.001f;
static constexpr float MOUSE_MOV_SPEED = 0.01f;
static constexpr float MOUSE_WHEEL_SPEED = 1.0f;

/*** Global variable ***/


/*** Function ***/
void Window::CbResize(GLFWwindow* window, int32_t width, int32_t height)
{
    int32_t fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    Window* const instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        instance->m_width = width;
        instance->m_height = height;
    }
}

void Window::CbWheel(GLFWwindow* window, double x, double y)
{
    Window* const instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        //instance->m_camera_pos[2] -= static_cast<float>(y) * MOUSE_WHEEL_SPEED;
        instance->MoveCameraPosFromCameraCoordinate(0, 0, -static_cast<float>(y) * MOUSE_WHEEL_SPEED);
    }
}

void Window::CbKeyboard(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    Window* const instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (instance) {
        //if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        //    instance->m_is_darkmode = !instance->m_is_darkmode;
        //}
    }
}

int32_t Window::GetWidth()
{
    return m_width;
}

int32_t Window::GetHeight()
{
    return m_height;
}

void Window::SetIsDarkMode(bool is_darkmode)
{
    m_is_darkmode = is_darkmode;
}

void Window::SetIsCameraRevolution(bool is_camera_revolution)
{
    m_is_camera_revolution = is_camera_revolution;
}

Matrix Window::GetViewProjectionFromAxisX(float cx, float cy, float fovy, float z_near, float z_far)
{
    Matrix view = Matrix::Identity(4);
    view = TransformationMatrix::Translate(-1.0f, 0.0f, 0.0f) * view;  /* move to origin */
    view = TransformationMatrix::RotateY(static_cast<float>(-M_PI / 2.0)) * view; /* rotate*/
    const float aspect = static_cast<float>(m_width) / m_height;
    const Matrix projection = ProjectionMatrix::Perspective(cx, cy, fovy, aspect, z_near, z_far);
    return projection * view;
}

Matrix Window::GetViewProjectionFromAxisY(float cx, float cy, float fovy, float z_near, float z_far)
{
    Matrix view = Matrix::Identity(4);
    view = TransformationMatrix::Translate(0.0f, -1.0f, 0.0f) * view;  /* move to origin */
    view = TransformationMatrix::RotateX(static_cast<float>(M_PI / 2.0)) * view; /* rotate*/
    const float aspect = static_cast<float>(m_width) / m_height;
    const Matrix projection = ProjectionMatrix::Perspective(cx, cy, fovy, aspect, z_near, z_far);
    return projection * view;
}

Matrix Window::GetViewProjectionFromAxisZ(float cx, float cy, float fovy, float z_near, float z_far)
{
    Matrix view = Matrix::Identity(4);
    view = TransformationMatrix::Translate(0.0f, 0.0f, -1.0f) * view;  /* move to origin */
    const float aspect = static_cast<float>(m_width) / m_height;
    const Matrix projection = ProjectionMatrix::Perspective(cx, cy, fovy, aspect, z_near, z_far);
    return projection * view;
}
Matrix Window::GetViewProjection(float cx, float cy, float fovy, float z_near, float z_far)
{
    Matrix view = Matrix::Identity(4);
    view = TransformationMatrix::Translate(-m_camera_pos[0], -m_camera_pos[1], -m_camera_pos[2]) * view;  /* move to origin */
    view = TransformationMatrix::RotateX(m_camera_angle[0]) * TransformationMatrix::RotateY(m_camera_angle[1]) * TransformationMatrix::RotateZ(m_camera_angle[2]) * view; /* rotate*/
    const float aspect = static_cast<float>(m_width) / m_height;
    const Matrix projection = ProjectionMatrix::Perspective(cx, cy, fovy, aspect, z_near, z_far);
    return projection * view;
}

Window::Window(int32_t width, int32_t height, const char* title)
{
    /* Initialize variables */
    m_width = width;
    m_height = height;
    m_is_darkmode = true;
    std::fill(m_camera_pos.begin(), m_camera_pos.end(), 0.0f);
    std::fill(m_camera_angle.begin(), m_camera_angle.end(), 0.0f);

    /* Create a window (x4 anti-aliasing, OpenGL3.3 Core Profile)*/
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    RUN_CHECK(m_window = glfwCreateWindow(width, height, title, nullptr, nullptr));
    glfwMakeContextCurrent(m_window);

    /* Initialize GLEW. This must be after initializing GLFW, creating window and setting current context window */
    glewExperimental = true;
    RUN_CHECK(glewInit() == GLEW_OK);

    /* Enable Backface Culling (Don't draw backface) */
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    /* enable Depth buffer */
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);

    /* Sync buffer swap timing with vsync */
    glfwSwapInterval(1);

    /* Store this pointer to window */
    glfwSetWindowUserPointer(m_window, this);

    /* Ensure not to miss input */
    glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    /* Set callback */
    glfwSetWindowSizeCallback(m_window, CbResize);
    CbResize(m_window, width, height);

    glfwSetScrollCallback(m_window, CbWheel);
    glfwSetKeyCallback(m_window, CbKeyboard);

    m_last_time = glfwGetTime();
    glfwGetCursorPos(m_window, &m_last_mouse_x, &m_last_mouse_y);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
}

GLFWwindow* Window::GetWindow()
{
    return m_window;
}

void Window::LookAt(const std::array<float, 3>& eye, const std::array<float, 3>& gaze, const std::array<float, 3>& up)
{
    /* Store XYZ angles instead or rotation matrix */
    Matrix mat = TransformationMatrix::LookAt(eye, gaze, up);
    std::copy(eye.begin(), eye.end(), m_camera_pos.begin());
    m_camera_angle[1] = std::asin(mat(0, 2));
    if (std::cos(m_camera_angle[1]) != 0) {
        m_camera_angle[0] = std::atan(-mat(1, 2) / mat(2, 2));
        m_camera_angle[2] = std::atan(-mat(0, 1) / mat(0, 0));
    } else {
        m_camera_angle[0] = std::atan(mat(2, 1) / mat(1, 1));
        m_camera_angle[2] = 0;
    }
}

bool Window::FrameStart()
{
    if (!m_window) return false;
    if (glfwWindowShouldClose(m_window) == GL_TRUE) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }
    //if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //    return false;
    //}

    glfwMakeContextCurrent(m_window);
    glfwPollEvents();

    double current_time = glfwGetTime();
    float delta_time = float(current_time - m_last_time);
    m_last_time = current_time;

    double mouse_x, mouse_y;
    glfwGetCursorPos(m_window, &mouse_x, &mouse_y);
    float mouse_move_x = (float)(mouse_x - m_last_mouse_x);
    float mouse_move_y = (float)(mouse_y - m_last_mouse_y);
    m_last_mouse_x = mouse_x;
    m_last_mouse_y = mouse_y;

    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) != GLFW_RELEASE) {
        if (!m_is_camera_revolution) {
            m_camera_angle[1] += mouse_move_x * MOUSE_ROT_SPEED;
            m_camera_angle[0] += mouse_move_y * MOUSE_ROT_SPEED;
        } else{
            /* move in polar coordinate, then look at the origin */
            /* note: the xyz order in polar coordinate is different from that of OpenGL */
            Matrix vec3_polar = RotationMatrix::ConvertXYZ2PolarCoordinate(m_camera_pos[2], m_camera_pos[0], m_camera_pos[1]);
            vec3_polar[2] -= mouse_move_x * MOUSE_MOV_SPEED;
            vec3_polar[1] -= mouse_move_y * MOUSE_MOV_SPEED;
            Matrix vec3_zxy = RotationMatrix::ConvertPolarCoordinate2XYZ(vec3_polar[0], vec3_polar[1], vec3_polar[2]);
            m_camera_pos[0] = vec3_zxy[1];
            m_camera_pos[1] = vec3_zxy[2];
            m_camera_pos[2] = vec3_zxy[0];
            if (m_camera_pos[2] < 0) {
                /* todo: it doesn't work well when Z < 0*/
                m_camera_pos[2] = 0.000001f;
            }
            LookAt({ m_camera_pos[0], m_camera_pos[1], m_camera_pos[2] }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
        }
    }
    if (!m_is_camera_revolution && glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_3) != GLFW_RELEASE) {
        //m_camera_pos[0] -= mouse_move_x * MOUSE_MOV_SPEED;
        //m_camera_pos[1] -= -mouse_move_y * MOUSE_MOV_SPEED;
        float dx_in_camera_cord = -mouse_move_x * MOUSE_MOV_SPEED;
        float dy_in_camera_cord = mouse_move_y * MOUSE_MOV_SPEED;
        MoveCameraPosFromCameraCoordinate(dx_in_camera_cord, dy_in_camera_cord, 0);
    }
    
    //if (glfwGetKey(m_window, GLFW_KEY_W) != GLFW_RELEASE) {
    //    m_camera_pos[2] -= delta_time * KEY_SPEED;
    //} else if (glfwGetKey(m_window, GLFW_KEY_S) != GLFW_RELEASE) {
    //    m_camera_pos[2] += delta_time * KEY_SPEED;
    //}
    //if (glfwGetKey(m_window, GLFW_KEY_A) != GLFW_RELEASE) {
    //    m_camera_pos[0] -= delta_time * KEY_SPEED;
    //} else if (glfwGetKey(m_window, GLFW_KEY_D) != GLFW_RELEASE) {
    //    m_camera_pos[0] += delta_time * KEY_SPEED;
    //}
    //if (glfwGetKey(m_window, GLFW_KEY_Z) != GLFW_RELEASE) {
    //    m_camera_pos[1] -= delta_time * KEY_SPEED;
    //} else if (glfwGetKey(m_window, GLFW_KEY_X) != GLFW_RELEASE) {
    //    m_camera_pos[1] += delta_time * KEY_SPEED;
    //}
    
    if (m_is_darkmode) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
    } else {
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return true;
}


void Window::SwapBuffers()
{
    glfwSwapBuffers(m_window);
}

void Window::MoveCameraPosFromCameraCoordinate(float dx, float dy, float dz)
{
    // dx, dy, dz are in camera coordinate
    Matrix rot = TransformationMatrix::RotateX(m_camera_angle[0]) * TransformationMatrix::RotateY(m_camera_angle[1]);
    Matrix trans = TransformationMatrix::Translate(dx, dy, dz);
    Matrix pos_in_world = rot.Transpose() * trans;  // use transpose instead of inverse, since rot is orthogonal matrix
    m_camera_pos[0] += pos_in_world(0, 3);  // tx in world coordinate
    m_camera_pos[1] += pos_in_world(1, 3);  // ty in world coordinate
    m_camera_pos[2] += pos_in_world(2, 3);  // tz in world coordinate
}

