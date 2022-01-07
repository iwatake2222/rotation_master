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
#ifndef SHAPE_H
#define SHAPE_H

/*** Include ***/
/* for general */
#include <cstdint>
#include <cstdio>
#include <vector>

/* for GLFW */
#include <GLFW/glfw3.h>

#include "matrix.h"

class Object
{
public:
    struct Vertex
    {
        GLfloat position[3];
        GLfloat color[3];
    };
public:
    Object(GLuint position_loc, GLuint color_loc, const std::vector<Object::Vertex>& vertex_list, const std::vector<GLuint>& index_list);
    virtual ~Object();
    void Bind() const;
private:
    Object(const Object& object);   // not allowed
    Object& operator=(const Object& object);    // not allowed
private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
};


class Shape
{
public:
    Shape(const std::vector<Object::Vertex>& vertex_list, const std::vector<GLuint>& index_list = {});
    virtual ~Shape() {}
    void Draw(const Matrix& viewprojection, const Matrix& model) const;

public:
    static void SetLineWidth(float width);

private:
    virtual void Execute() const;

protected:
    GLuint m_program_id;
    GLint m_modelviewprojection_loc;
    GLsizei m_vertex_num;
    GLsizei m_index_num;

private:
    std::shared_ptr<Object> m_object;
};

class ShapeIndex : public Shape
{
public:
    ShapeIndex(const std::vector<Object::Vertex>& vertex_list, const std::vector<GLuint>& index_list);
private:
    virtual void Execute() const override;
};

class ShapeSolid : public Shape
{
public:
    ShapeSolid(const std::vector<Object::Vertex>& vertex_list);
private:
    virtual void Execute() const override;
};

class ShapeSolidIndex : public Shape
{
public:
    ShapeSolidIndex(const std::vector<Object::Vertex>& vertex_list, const std::vector<GLuint>& index_list);
private:
    virtual void Execute() const override;
};


#endif
