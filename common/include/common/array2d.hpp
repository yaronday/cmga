/* MIT License
 *
 * Copyright (c) 2026 Yaron Dayan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#define PICO_ARRAY2D_VERSION_MAJOR 1
#define PICO_ARRAY2D_VERSION_MINOR 0
#define PICO_ARRAY2D_VERSION_PATCH 0

#include <cassert>
#include <span>
#include <vector>

template <typename T> class Array2D {
private:
  std::vector<T> data_;
  std::size_t rows_;
  std::size_t cols_;

public:
  Array2D() noexcept : rows_(0), cols_(0) {}

  Array2D(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols), data_(rows * cols) {}

  Array2D(std::size_t rows, std::size_t cols, T initial_value)
      : rows_(rows), cols_(cols), data_(rows * cols, initial_value) {}

  [[nodiscard]] constexpr const T &operator()(std::size_t row, std::size_t col) const noexcept {
    assert(row < rows_ && col < cols_ && "Array2D index out of range");
    return data_[row * cols_ + col];
  }

  // Element access with bounds checking in debug mode
  [[nodiscard]] constexpr T &operator()(std::size_t row, std::size_t col) noexcept {
    return const_cast<T &>(std::as_const(*this)(row, col));
  }

  [[nodiscard]] constexpr std::size_t rows() const noexcept { return rows_; }
  [[nodiscard]] constexpr std::size_t cols() const noexcept { return cols_; }
  [[nodiscard]] constexpr std::size_t size() const noexcept { return data_.size(); }
  [[nodiscard]] constexpr bool empty() const noexcept { return data_.empty(); }

  [[nodiscard]] T *data() noexcept { return data_.data(); }
  [[nodiscard]] const T *data() const noexcept { return data_.data(); }

  auto begin() noexcept { return data_.begin(); }
  auto end() noexcept { return data_.end(); }
  auto begin() const noexcept { return data_.cbegin(); }
  auto end() const noexcept { return data_.cend(); }

  // if preallocation is needed, this should be called before emplace_back
  void reserve(std::size_t total_elements) { data_.reserve(total_elements); }

  // Core insertion logic handled via std::span to prevent code duplication
  bool emplace_back(std::span<const T> row) {
    if (cols_ == 0) {
      cols_ = row.size();
    } else {
      assert(row.size() == cols_ && "Array2D row length mismatch");
    }
    data_.insert(data_.end(), row.begin(), row.end());
    ++rows_;
    return true;
  }

  // Overload for initializer_list delegating directly to the span implementation
  bool emplace_back(std::initializer_list<T> row) {
    return emplace_back(std::span<const T>(row.begin(), row.size()));
  }

  [[nodiscard]] constexpr std::span<const T> row_view(std::size_t row) const noexcept {
    assert(row < rows_ && "Array2D row index out of range");
    return std::span<const T>(data_.data() + row * cols_, cols_);
  }

  [[nodiscard]] constexpr std::span<T> row_view(std::size_t row) noexcept {
    return std::span<T>(const_cast<T *>(std::as_const(*this).row_view(row).data()), cols_);
  }

  [[nodiscard]] constexpr std::span<const T> operator[](std::size_t row) const noexcept {
    return row_view(row);
  }

  [[nodiscard]] constexpr std::span<T> operator[](std::size_t row) noexcept {
    return row_view(row);
  }
};