#pragma once

#include <iostream>
#include <vector>
#include <memory>

template <typename T, typename E, typename H> 
void publish(const T &x, E, H);

class object_t;

template <typename E, typename T>
void publish(const object_t &x, E e, T t);

class object_t {
public:
  template <typename T>
  explicit object_t(T x)
      : self_(std::make_unique<model_t<T>>(std::move(x))) {}

  object_t(object_t const &x) = delete;
  object_t &operator=(object_t const &x) = delete;

  object_t(object_t &&x) noexcept = default;
  object_t &operator=(object_t &&x) noexcept = default;

  template <typename E, typename T>
  friend void publish(const object_t &x, E e, T t) { x.self_->publish(std::move(e), std::move(t)); }

private:
  struct concept_t {
    virtual ~concept_t() = default;
  };
  template <typename T> 
  struct model_t final : concept_t {
    model_t(T x) : data_(std::move(x)) {}

    template <typename E>
    void publish(E e, T t) const { 
        return data_(std::mvoe(e), std::move(t));
     }
    T data_;
  };
  std::unique_ptr<concept_t> self_;
};

using collection_t = std::vector<object_t>;
