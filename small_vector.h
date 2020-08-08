#pragma once
#include <array>
#include <vector>

template <class T, std::size_t N, class ST = std::size_t>
class small_vector {
  std::array<T, N> stack_;
  std::vector<T> heap_;
  ST index_{0};

public:
  typedef T value_type;
  typedef ST size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;

  void push_back(const T& value) {
    if (index_ < N) {
      stack_[index_] = value;
    } else {
      heap_.push_back(value);
    }
    index_ += 1;
  }

  void push_back(T&& value) {
    if (index_ < N) {
      stack_[index_] = std::move(value);
    } else {
      heap_.push_back(std::move(value));
    }
    index_ += 1;
  }

  void pop_back() {
    if (index_ >= N) {
      heap_.pop_back();
      index_ -= 1;
    }
  }

  reference operator[](size_type pos) {
    if (pos < N) {
      return stack_[pos];
    } else {
      return heap_[pos - N];
    }
  }

  const_reference operator[](size_type pos) const {
    if (pos < N) {
      return stack_[pos];
    } else {
      return heap_[pos - N];
    }
  }

  reference at(size_type pos) {
    if (pos < N) {
      return stack_[pos];
    } else {
      return heap_.at(pos - N);
    }
  }

  const_reference at(size_type pos) const {
    if (pos < N) {
      return stack_[pos];
    } else {
      return heap_.at(pos - N);
    }
  }

  bool empty() const {
    return index_ == 0;
  }

  size_type max_size() const {
    return N + heap_.max_size();
  }

  size_type capacity() const {
    return N + heap_.capacity();
  }

  size_type size() const {
    return index_;
  }

  void shrink_to_fit() {
    if (index_ >= N) {
      heap_.shrink_to_fit();
    }
  }

  // Resizes the container to contain count elements.
  void resize(size_type count, T value = T()) {
    if (count <= N) {
      // new `count` of elements completely fit on stack
      for (size_t i = count; i < N; i++) {
        stack_[i] = value;
      }
      if (index_ >= N) {
        heap_.clear();
      }
    }
    else {
      heap_.resize(count - N, value);
    }
    index_ = count;
  }

};