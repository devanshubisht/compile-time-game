//
// Created by Devanshu Bisht on 30/12/23.
//

#ifndef COMPILE_TIME_GAME_GAME_STATE_HPP
#define COMPILE_TIME_GAME_GAME_STATE_HPP
#include <cstddef>

template <std::size_t RowCount, std::size_t ColumnCount> struct GameState {
  std::array<std::array<candy, ColumnCount>, RowCount> board;
  int hovered_x;
  int hovered_y;
  bool any_selected;
  int selected_x;
  int selected_y;
  int score;
  int moves;
};

#endif // COMPILE_TIME_GAME_GAME_STATE_HPP
