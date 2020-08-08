#pragma once
#include <array>
#include <vector>
#include <iostream>

namespace sv {

template <class T, std::size_t N, class Allocator = std::allocator<T>>
class small_vector {
  std::array<T, N> stack_;
  std::vector<T, Allocator> heap_;
  std::size_t index_{0};

public:
  typedef T value_type;
  typedef std::size_t size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef Allocator allocator_type;

  small_vector() = default;

  explicit small_vector(size_type count,
                        const T& value = T(),
                        const Allocator& alloc = Allocator()) {
    if (count == N) {
      stack_.fill(value);
    } else if (count < N) {
      for (size_t i = 0; i < count; i++) {
        stack_[i] = value;
      }
    } else if (count > N) {
      stack_.fill(value);
      heap_ = std::move(std::vector<T>(count - N, value, alloc));
    }
    index_ = count;
  }

  small_vector(const small_vector& other, const Allocator& alloc = Allocator()) :
    stack_(other.stack_), heap_(other.heap_, alloc), index_(other.index_) {}

  small_vector(small_vector&& other, const Allocator& alloc = Allocator()) :
    stack_(std::move(other.stack_)), heap_(std::move(other.heap_), alloc), index_(std::move(other.index_)) {}

  small_vector(std::initializer_list<T> init, const Allocator& alloc = Allocator()) {
    // Save to stack for first N elements
    std::size_t i = 0;
    for (const auto& v : init) {
      if (i < N) {
        stack_[i] = v;
      } else {
        break;
      }
      i += 1;
    }

    // Construct vector in heap for rest
    const auto input_size = init.size();
    if (input_size >= N) {
      heap_ = std::move(std::vector<T>(init.begin() + N, init.end(), alloc));
    }
    index_ = input_size;
  }

  small_vector& operator=(const small_vector& rhs) {
    stack_ = rhs.stack_;
    heap_ = rhs.heap_;
    index_ = rhs.index_;
    return *this;
  }

  small_vector& operator=(small_vector&& rhs) {
    std::cout << "In move assignment\n";
    stack_ = std::move(rhs.stack_);
    heap_ = std::move(rhs.heap_);
    index_ = std::move(rhs.index_);
    return *this;
  }

  small_vector& operator=(std::initializer_list<value_type> rhs) {
    index_ = 0;
    heap_.clear();

    std::size_t i = 0;
    for (const auto& v : rhs) {
      if (i < N) {
        stack_[i] = v;
      } else {
        heap_.emplace_back(v);
      }
      i += 1;
    }
    index_ = rhs.size();
    return *this;
  }

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

  reference front() {
    return stack_.front();
  }

  const_reference front() const {
    return stack_.front();
  }

  reference back() {
    if (index_ < N) {
      return stack_.back();
    } else {
      return heap_.back();
    }
  }

  const_reference back() const {
    if (index_ < N) {
      return stack_.back();
    } else {
      return heap_.back();
    }
  }

  T* stack_data() noexcept {
    return stack_.data();
  }

  const T* stack_data() const noexcept {
    return stack_.data();
  }

  T* heap_data() noexcept {
    return heap_.data();
  }

  const T* heap_data() const noexcept {
    return heap_.data();
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

  size_type stack_size() const {
    return N;
  }

  size_type heap_size() const {
    return heap_.size();
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

  void swap(small_vector& other) noexcept {
    stack_.swap(other.stack_);
    heap_.swap(other.heap_);
    index_ = other.index_;
  };

  // Assigns the given value value to all elements in the container
  void fill(const_reference value) {
    stack_.fill(value);
    if (index_ >= N) {
      std::fill(heap_.begin(), heap_.end(), value);
    }
  }

};

}