#pragma once
#include <array>
#include <vector>

template <class T, std::size_t N, class ST = std::size_t>
class small_buffer {
  std::array<T, N> stack_;
  std::vector<T> heap_;
  ST index_{0};

public:
  typedef T value_type;
  typedef ST size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;

  small_buffer() = default;

  explicit small_buffer(size_type count,
                        const T& value = T()) {
    if (count == N) {
      stack_.fill(value);
    } else if (count < N) {
      for (size_t i = 0; i < count; i++) {
        stack_[i] = value;
      }
    } else if (count > N) {
      stack_.fill(value);
      heap_ = std::move(std::vector<T>(count - N, value));
    }
    index_ = count;
  }

  small_buffer(const small_buffer& other) :
    stack_(other.stack_), heap_(other.heap_), index_(other.index_) {}

  small_buffer(small_buffer&& other) :
    stack_(std::move(other.stack_)), heap_(std::move(other.heap_)), index_(std::move(other.index_)) {}

  small_buffer(std::initializer_list<T> init) {
    std::size_t i = 0;
    for (const auto& v : init) {
      if (i < N) {
        stack_[i] = v;
      } else {
        heap_.emplace_back(v);
      }
      i += 1;
    }
    index_ = init.size();
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

  void swap(small_buffer& other) noexcept {
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