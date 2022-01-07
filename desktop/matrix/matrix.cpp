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
#include <climits>
#include <vector>
#include <stdexcept>

#include "matrix.h"

/*** Macro ***/

/*** Global variable ***/

/*** Function ***/

Matrix::Matrix()
    : m_rows(0), m_cols(0)
{
    // do nothing
}

Matrix::Matrix(int32_t rows, int32_t cols)
{
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("size must be greater than 0");
    }
    if (cols > INT_MAX / rows) {
        throw std::overflow_error("Too huge size");
    }

    m_rows = rows;
    m_cols = cols;
    m_data_array = std::vector<float>(static_cast<size_t>(m_rows) * m_cols);
}

Matrix::Matrix(int32_t rows, int32_t cols, float* data)
    : Matrix(rows, cols)
{
    std::copy(data, data + static_cast<size_t>(m_rows) * m_cols, m_data_array.data());
}

Matrix::Matrix(int32_t rows, int32_t cols, const std::vector<float>& data)
{
    m_rows = rows;
    m_cols = cols;
    m_data_array = data;
}

Matrix::~Matrix()
{
    // do nothing
}

const float* Matrix::Data() const
{
    return m_data_array.data();
}

float* Matrix::Data()
{
    return &m_data_array[0];
}

const float& Matrix::operator[](int32_t i) const
{
    return m_data_array.at(i);
}

float& Matrix::operator[](int32_t i)
{
    return m_data_array.at(i);
}

float& Matrix::operator() (int32_t row, int32_t col)
{
    if (row >= m_rows || col >= m_cols) throw std::out_of_range("Invalid index");
    return m_data_array.at(row * m_cols + col);
}

const float& Matrix::operator() (int32_t row, int32_t col) const
{
    if (row >= m_rows || col >= m_cols) throw std::out_of_range("Invalid index");
    return m_data_array.at(row * m_cols + col);
}

Matrix Matrix::operator+(const Matrix& right) const
{
    const Matrix& left = *this;
    if ((left.m_rows != right.m_rows) || (left.m_cols != right.m_cols)) throw std::out_of_range("Invalid index");
    Matrix ret(left.m_rows, right.m_cols);
    for (int32_t i = 0; i < m_rows * m_cols; i++) {
        ret[i] = left[i] + right[i];
    }
    return ret;
}

Matrix Matrix::operator-(const Matrix& right) const
{
    const Matrix& left = *this;
    if ((left.m_rows != right.m_rows) || (left.m_cols != right.m_cols)) throw std::out_of_range("Invalid index");
    Matrix ret(left.m_rows, right.m_cols);
    for (int32_t i = 0; i < m_rows * m_cols; i++) {
        ret[i] = left[i] - right[i];
    }
    return ret;
}

Matrix Matrix::operator*(const float& k) const
{
    const Matrix& left = *this;
    Matrix ret(left.m_rows, left.m_cols);
    for (int32_t i = 0; i < m_rows * m_cols; i++) {
        ret[i] = left[i] * k;
    }
    return ret;
}

Matrix Matrix::operator*(const Matrix& right) const
{
    const Matrix& left = *this;
    if (left.m_cols != right.m_rows) throw std::out_of_range("Invalid index");
    Matrix ret(left.m_rows, right.m_cols);
    for (int32_t row = 0; row < left.m_rows; row++) {
        for (int32_t col = 0; col < right.m_cols; col++) {
            ret(row, col) = 0.0f;
            for (int32_t i = 0; i < left.m_cols; i++) {
                ret(row, col) += left(row, i) * right(i, col);
            }
        }
    }
    return ret;
}
Matrix Matrix::Transpose() const
{
    const Matrix& mat = *this;
    Matrix ret(mat.m_cols, mat.m_rows);
    for (int32_t row = 0; row < m_rows; row++) {
        for (int32_t col = 0; col < m_cols; col++) {
            ret(col, row) = mat(row, col);
        }
    }
    return ret;
}

Matrix Matrix::Inverse() const
{
    if (m_rows != m_cols) {
        throw std::out_of_range("Invalid shape");
    }
    Matrix mat = *this;
    int32_t n = mat.m_rows;
    Matrix ret = Identity(n);
    for (int32_t y = 0; y < n; y++) {
        if (mat(y, y) == 0) {
            throw std::out_of_range("Tried to calculate an inverse of non - singular matrix");
        }
        float scale_to_1 = 1.0f / mat(y, y);
        for (int32_t x = 0; x < n; x++) {
            mat(y, x) *= scale_to_1;
            ret(y, x) *= scale_to_1;
        }
        for (int32_t yy = 0; yy < n; yy++) {
            if (yy != y) {
                float scale_to_0 = mat(yy, y);
                for (int32_t x = 0; x < n; x++) {
                    mat(yy, x) -= mat(y, x) * scale_to_0;
                    ret(yy, x) -= ret(y, x) * scale_to_0;
                }
            }
        }
    }
    return ret;
}

void Matrix::Print() const
{
    if (m_rows == 1) {
        for (int32_t x = 0; x < m_cols; x++) {
            printf("%4.2f ", (*this)(0, x));
        }
    } else if (m_cols == 1) {
        for (int32_t y = 0; y < m_rows; y++) {
            printf("%4.2f ", (*this)(y, 0));
        }
    } else {
        for (int32_t y = 0; y < m_rows; y++) {
            for (int32_t x = 0; x < m_cols; x++) {
                printf("%4.2f ", (*this)(y, x));
            }
            printf("\n");
        }
    }
    printf("\n");
}

Matrix Matrix::Identity(int32_t size)
{
    Matrix ret(size, size);
    std::fill(ret.Data(), ret.Data() + size * size, 0.0f);
    for (int32_t i = 0; i < size; i++) {
        ret(i, i) = 1.0f;
    }
    return ret;
}
