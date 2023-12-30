//
// Created by Devanshu Bisht on 30/12/23.
//

#ifndef COMPILE_TIME_GAME_BOARD_HPP
#define COMPILE_TIME_GAME_BOARD_HPP

#include <array>
#include <iostream>
#include <utility>

#include "utils.hpp"

enum class CandyType { None = 0, Red, Green, Blue, Yellow };

constexpr int candy_type_count = 4; // Excluding None.

struct CandyState {
  bool matched;
};

struct candy {
  CandyType type;
  CandyState state;
};

template <int RowCount, int ColumnCount>
using board = std::array<std::array<candy, ColumnCount>, RowCount>;

#endif // COMPILE_TIME_GAME_BOARD_HPP
