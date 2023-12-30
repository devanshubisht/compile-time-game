//
// Created by Devanshu Bisht on 30/12/23.
//

#ifndef COMPILE_TIME_GAME_CONSTEXPR_STRING_HPP
#define COMPILE_TIME_GAME_CONSTEXPR_STRING_HPP

#include <array>

template <std::size_t N> class constexpr_string {
public:
  constexpr constexpr_string() : data_{}, size_{N} {};

  constexpr constexpr_string(const char (&a)[N]) : data_{}, size_{N - 1} {
    for (size_t i = 0; i < size_; i++) {
      data_[i] = a[i];
    }
  }

  constexpr constexpr_string(const char *c, std::size_t size)
      : data_{}, size_{size} {
    for (std::size_t i = 0; i < N && i < size; ++i) {
      data_[i] = c[i];
    }
  }

  template <std::size_t M>
  constexpr constexpr_string(constexpr_string<M> &other) : data_{}, size_{M} {
    for (std::size_t i = 0; i < M; ++i) {
      data_[i] = other[i];
    }
  }

  template <std::size_t M>
  constexpr constexpr_string(const constexpr_string<M> &other, std::size_t size)
      : data_{}, size_{size} {
    for (std::size_t i = 0; i < other.size(); ++i) {
      data_[i] = other[i];
    }
  }

  template <std::size_t M>
  constexpr auto constexpr_string append(constexpr_string<M> &other) {
    constexpr_string<N + M> newString{*this, N + M};
    for (std::size_t i = N; i < M; ++i) {
      newString[i] = other[i];
    }
    return newString;
  }

  // auto cannot be used here because then it will be runtime as the compiler wont know whats the final size
  // Alternative we can input a template non type argument to dictate whats the size
  constexpr constexpr_string<N> substr(std::size_t pos, std::size_t len = -1) const
  {
    if (pos > size_) {
      throw std::runtime_error("out of range");
    }

    const std::size_t new_size = (len == static_cast<std::size_t>(-1)) ? (size_ - pos) : std::min(len, (size_ - pos));
    return constexpr_string<N>(data_ + pos, new_size);
  }

  constexpr const char &operator[](std::size_t i) const {
    if (i < size_) {
      return data_[i];
    } else {
      throw std::out_of_range("");
    }
  }

  constexpr char *begin() { return data_; }

  constexpr char *end() { return data_ + size_; }

private:
  char data_[N];
  std::size_t size_;
};

#endif // COMPILE_TIME_GAME_CONSTEXPR_STRING_HPP
