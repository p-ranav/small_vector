#pragma once
#include <array>
#include <vector>
#include <iostream>

namespace sv {

template <class T, std::size_t N, class Allocator = std::allocator<T>>
class small_vector {
  std::array<T, N> stack_;
  std::vector<T, Allocator> heap_;
  std::size_t size_{0};

public:
  typedef T value_type;
  typedef std::size_t size_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef Allocator allocator_type;
  typedef T* iterator;
  typedef const T* const_iterator;

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
    } else {
      // use heap
      heap_ = std::move(std::vector<T>(count, value, alloc));
    }
    size_ = count;
  }

  small_vector(const small_vector& other, const Allocator& alloc = Allocator()) :
    stack_(other.stack_), heap_(other.heap_, alloc), size_(other.size_) {}

  small_vector(small_vector&& other, const Allocator& alloc = Allocator()) :
    stack_(std::move(other.stack_)), heap_(std::move(other.heap_), alloc), size_(std::move(other.size_)) {}

  small_vector(std::initializer_list<T> initlist, const Allocator& alloc = Allocator()) {
    const auto input_size = initlist.size();
    if (input_size <= N) {
      std::copy(initlist.begin(), initlist.end(), stack_.begin());
    } else {
      heap_ = std::move(std::vector<T>(initlist, alloc));
    }
    size_ = input_size;
  }

  small_vector& operator=(const small_vector& rhs) {
    stack_ = rhs.stack_;
    heap_ = rhs.heap_;
    size_ = rhs.size_;
    return *this;
  }

  small_vector& operator=(small_vector&& rhs) {
    stack_ = std::move(rhs.stack_);
    heap_ = std::move(rhs.heap_);
    size_ = std::move(rhs.size_);
    return *this;
  }

  small_vector& operator=(std::initializer_list<value_type> rhs) {
    if (rhs.size() <= N) {
      stack_ = rhs;
    } else {
      heap_ = rhs;
    }
    size_ = rhs.size();
  }

  void push_back(const T& value) {
    if (size_ < N) {
      stack_[size_] = value;
    } else {
      // move everything to heap
      std::move(stack_.begin(), stack_.end(), std::back_inserter(heap_));
      heap_.push_back(value);
    }
    size_ += 1;
  }

  void push_back(T&& value) {
    if (size_ < N) {
      stack_[size_] = std::move(value);
    } else {
      // move everything to heap
      std::move(stack_.begin(), stack_.end(), std::back_inserter(heap_));
      heap_.push_back(std::move(value));
    }
    size_ += 1;
  }

  void pop_back() {
    if (size_ < N) {
      size_ -= 1;
    } else {
      // currently using heap
      heap_.pop_back();
      size_ -= 1;

      // now check if all data can fit on stack
      // if so, move back to stack
      if (size_ < N) {
        std::move(heap_.begin(), heap_.end(), stack_.begin());
      }
    }
  }

  reference at(size_type pos) {
    if (size_ < N) {
      return stack_.at(pos);
    } else {
      return heap_.at(pos);
    }
  }

  const_reference at(size_type pos) const {
    if (size_ < N) {
      return stack_.at(pos);
    } else {
      return heap_.at(pos);
    }
  }

  reference operator[](size_type pos) {
    if (size_ < N) {
      return stack_[pos];
    } else {
      return heap_[pos];
    }
  }

  const_reference operator[](size_type pos) const {
    if (size_ < N) {
      return stack_[pos];
    } else {
      return heap_[pos];
    }
  }

  reference front() {
    if (size_ < N) {
      return stack_.front();
    } else {
      return heap_.front();
    }
  }

  const_reference front() const {
    if (size_ < N) {
      return stack_.front();
    } else {
      return heap_.front();
    }
  }

  reference back() {
    if (size_ < N) {
      return stack_[size_ - 1];
    } else {
      return heap_[size_ - 1];
    }
  }

  const_reference back() const {
    if (size_ < N) {
      return stack_[size_ - 1];
    } else {
      return heap_[size_ - 1];
    }
  }

  T* data() noexcept {
    if (size_ < N) {
      return stack_.data();
    } else {
      return heap_.data();
    }
  }

  const T* data() const noexcept {
    if (size_ < N) {
      return stack_.data();
    } else {
      return heap_.data();
    }
  }

  bool empty() const {
    return size_ == 0;
  }

  size_type size() const {
    return size_;
  }

  void shrink_to_fit() {
    if (size_ >= N) {
      heap_.shrink_to_fit();
    }
  }

  // Resizes the container to contain count elements.
  void resize(size_type count, T value = T()) {
    if (count <= N) {
      // new `count` of elements completely fit on stack
      if (size_ >= N) {
        // currently, all data on heap
        // move back to stack
        std::move(heap_.begin(), heap_.end(), stack_.begin());
      } else {
        // all data already on stack
        // just update size
      }
    } else {
      // new `count` of data is going to be on the heap
      // check if data is currently on the stack
      if (size_ < N) {
        // move to heap
        std::move(stack_.begin(), stack_.end(), std::back_inserter(heap_));
      }
      heap_.resize(count, value);
    }
    size_ = count;
  }

  void swap(small_vector& other) noexcept {
    stack_.swap(other.stack_);
    heap_.swap(other.heap_);
    size_ = other.size_;
  };

  // Assigns the given value value to all elements in the container
  void fill(const_reference value) {
    if (size_ < N) {
      stack_.fill(value);
    } else {
      std::fill(heap_.begin(), heap_.end(), value);
    }
  }
};

}