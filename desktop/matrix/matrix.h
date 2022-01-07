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
#ifndef MATRIX_H
#define MATRIX_H

/*** Include ***/
#include <cstdint>
#include <cstdio>
#include <vector>

class Matrix
{
public:
    Matrix();
    Matrix(int32_t rows, int32_t cols);
    Matrix(int32_t rows, int32_t cols, float* data);
    Matrix(int32_t rows, int32_t cols, const std::vector<float>& data);
    ~Matrix();
    const float* Data() const;
    float* Data();
    const float& operator[](int32_t i) const;
    float& operator[](int32_t i);
    float& operator() (int32_t row, int32_t col);
    const float& operator() (int32_t row, int32_t col) const;
    Matrix operator+(const Matrix& right) const;
    Matrix operator-(const Matrix& right) const;
    Matrix operator*(const float& k) const;
    Matrix operator*(const Matrix& right) const;
    Matrix Transpose() const;
    Matrix Inverse() const;
    void Print() const;

    static Matrix Identity(int32_t size);

private:
    std::vector<float> m_data_array;
    int32_t m_rows;
    int32_t m_cols;
};

#endif
