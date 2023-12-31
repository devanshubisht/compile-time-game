//
// Created by Devanshu Bisht on 30/12/23.
//

#ifndef COMPILE_TIME_GAME_PARSER_HPP
#define COMPILE_TIME_GAME_PARSER_HPP

#include <tuple>

#include "board.hpp"
#include "constexpr_string.hpp"
#include "constexpr_string_view.hpp"
#include "game_state.hpp"
#include "utils.hpp"

struct BoardParameters {
  int row_count;
  int column_count;
  int score;
  int moves;
  int index_first_candy;
};

constexpr int extract_int(auto it) {
  int result = 0;
  char digit = *it;
  const int digits_max = 5; // assuming score and moves are less than million
  int digits_count = 0;
  while ('0' <= digit && digit <= '9' && digits_count <= digits_max) {
    result = 10 * result + (digit - '0');
    ++it;
    digit = (*it);
    ++digits_count;
  }
  return result;
}

CONSTEXPR int stoi(const constexpr_string_view &s) {
  int result = 0;

  for (const char c : s) {
    result = result * 10 + (c - '0');
  }

  return result;
}

CONSTEXPR auto itos(int x) {
  int digits = digits_amount(x);

  constexpr_string<10> result;

  for (int i = 0; i < digits; ++i) {
    result[digits - 1 - i] = (x % 10) + '0';
    x = x / 10;
  }

  return result.substr(0, digits);
}

constexpr char map_candy_type[] = {' ', 'R', 'G', 'B', 'Y'};

constexpr char encode_candy_type(CandyType t) {
  return map_candy_type[static_cast<int>(t)];
}

constexpr CandyType decode_candy_type(char c) {
  switch (c) {
  case ' ':
    return CandyType::None;
  case 'R':
    return CandyType::Red;
  case 'G':
    return CandyType::Green;
  case 'B':
    return CandyType::Blue;
  case 'Y':
    return CandyType::Yellow;
  default:
    throw std::runtime_error("Invalid candy type");
  }
}

struct state_decoration {
  char left;
  char right;
  char top;
  char bottom;
};

constexpr state_decoration encode_candy_state(CandyState s) {
  state_decoration dec = {' ', ' ', ' ', ' '};

  if (s.matched) {
    dec = {'*', '*', '*', '*'};
  }

  return dec;
}

CONSTEXPR CandyState decode_candy_state(char up_char) {
  CandyState state = {false};

  if (up_char == '*') {
    state.matched = true;
  } else if (up_char != ' ') {
    throw std::runtime_error("Invalid matched state!");
  }

  return state;
}

constexpr int candy_size = 3;
constexpr int row_padding = 5; // space, pipe, pipe, space, newline

constexpr auto parse_board_parameters(auto str) {
  BoardParameters ret{0, 0, 0, 0, 0};

  constexpr int noncandy_rows = 2;                      // border, border
  constexpr int lines_after_topborder_end_to_candy = 1; // empty
  constexpr int offset_candy_after_topborder_end =
      4; // space, space, pipe, space

  auto gs_begin = str.cbegin();
  auto gs_end = str.cend();
  auto topborder_lowerbound = find(gs_begin + 1, gs_end, '\n');
  auto topborder_end = find(topborder_lowerbound + 1, gs_end, '\n');
  auto field_width = topborder_end - topborder_lowerbound;
  ret.column_count = (field_width - row_padding) / candy_size;

  auto score_start = find(gs_begin + 1, gs_end, '>');
  ret.row_count =
      (((score_start - topborder_lowerbound) / field_width) - noncandy_rows) /
      candy_size;

  ret.index_first_candy = (topborder_end - gs_begin) +
                          (lines_after_topborder_end_to_candy * field_width) +
                          offset_candy_after_topborder_end;

  const auto score_value_offset = 9; // length of "> score: "
  ret.score = extract_int(score_start + score_value_offset);

  auto moves_start = find(score_start + 1, gs_end, '>');
  const auto moves_value_offset = 9; // length of "> moves: "
  ret.moves = extract_int(moves_start + moves_value_offset);

  return ret;
}

template <class GameString> CONSTEXPR auto parse_board() {
  constexpr auto board_string = GameString{}();

  constexpr auto board_parameters = parse_board_parameters(board_string);
  constexpr int column_count = board_parameters.column_count;
  constexpr int row_count = board_parameters.row_count;
  constexpr int index_first_candy = board_parameters.index_first_candy;
  bool any_hovered = false;
  int hovered_x = 0;
  int hovered_y = 0;
  bool any_selected = false;
  int selected_x = 0;
  int selected_y = 0;

  std::array<std::array<candy, column_count>, row_count> board{};

  constexpr int width = ((column_count * candy_size) + row_padding);
  constexpr int candy_state_offset =
      (-width); // same horisontal coordinates at previous row

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < column_count; ++j) {
      const int candy_type_index =
          i * candy_size * width + j * candy_size + index_first_candy;
      const int candy_matched = candy_type_index + candy_state_offset;

      board[i][j] = candy{decode_candy_type(board_string[candy_type_index]),
                          decode_candy_state(board_string[candy_matched])};

      char selected_or_hovered = board_string[candy_type_index - 1];
      if (selected_or_hovered == '[') {
        any_selected = true;
        selected_x = i;
        selected_y = j;
      } else if (selected_or_hovered == '(') {
        any_hovered = true;
        hovered_x = i;
        hovered_y = j;
      } else if (selected_or_hovered != ' ' && selected_or_hovered != '*') {
        throw std::runtime_error("Invalid hover state!");
      }
    }
  }

  if (!any_hovered) {
    if (any_selected) {
      hovered_x = selected_x;
      hovered_y = selected_y;
    } else {
      hovered_x = row_count / 2;
      hovered_y = column_count / 2;
    }
  }
  return GameState<row_count, column_count>{board,
                                            hovered_x,
                                            hovered_y,
                                            any_selected,
                                            selected_x,
                                            selected_y,
                                            board_parameters.score,
                                            board_parameters.moves};
}

template <class GameString>
CONSTEXPR auto parse_score(GameString &&get_game_state_string) {
  constexpr auto str = get_game_state_string();

  auto score_begin = find(str.cbegin(), str.cend(), ':') + 2;
  auto score_end = find(score_begin, str.cend(), '\n');

  return stoi({score_begin, static_cast<int>(score_end - score_begin)});
}

template <class GameString>
CONSTEXPR auto parse_moves(GameString &&get_game_state_string) {
  constexpr auto str = get_game_state_string();

  auto score_begin = find(str.cbegin(), str.cend(), ':') + 1;
  auto moves_begin = find(score_begin, str.cend(), ':') + 2;
  auto moves_end = find(moves_begin, str.cend(), '\n');

  return stoi({moves_begin, static_cast<int>(moves_end - moves_begin)});
}

template <class GameString> CONSTEXPR auto parse_game_state() {
  CONSTEXPR auto board = parse_board<GameString>();

  return board;
}

template <std::size_t RowCount, std::size_t ColumnCount>
CONSTEXPR auto
print_board_to_array(const game_engine<RowCount, ColumnCount> &engine) {
  auto board = engine.get_board();

  constexpr int width = ((ColumnCount * candy_size) + row_padding);

  constexpr auto e = [](CandyState s) constexpr {
    return encode_candy_state(s);
  };

  constexpr int board_size_in_char = width * ((RowCount * candy_size) + 2);
  constexpr_string<board_size_in_char> result;

  int cursor = 0;

#define W(C) result[cursor++] = C
#define NEWLINE()                                                              \
  W(' ');                                                                      \
  W('|')
#define ENDLINE()                                                              \
  W('|');                                                                      \
  W(' ');                                                                      \
  W('\n')

#define EPILOG()                                                               \
  W(' ');                                                                      \
  W(' ');                                                                      \
  for (int r = 0; r < width - row_padding; ++r) {                              \
    W('-');                                                                    \
  }                                                                            \
  W(' ');                                                                      \
  W(' ');                                                                      \
  W('\n')

  EPILOG();

  for (const auto &row : board) {
    NEWLINE();

    for (const auto &column : row) {
      W(' ');
      W(e(column.state).top);
      W(' ');
    }

    ENDLINE();
    NEWLINE();

    for (const auto &column : row) {
      W(e(column.state).left);
      W(encode_candy_type(column.type));
      W(e(column.state).right);
    }

    ENDLINE();
    NEWLINE();

    for (const auto &column : row) {
      W(' ');
      W(e(column.state).bottom);
      W(' ');
    }

    ENDLINE();
  }

  EPILOG();

  auto hovered_x = engine.get_hovered_x();
  auto hovered_y = engine.get_hovered_y();
  auto hovered_center =
      (hovered_y * candy_size + 3) + (hovered_x * candy_size + 2) * width;
  result[hovered_center - 1] = '(';
  result[hovered_center + 1] = ')';

  if (engine.is_any_selected()) {
    auto selected_x = engine.get_selected_x();
    auto selected_y = engine.get_selected_y();
    auto selected_center =
        (selected_y * candy_size + 3) + (selected_x * candy_size + 2) * width;
    result[selected_center - 1] = '[';
    result[selected_center + 1] = ']';
  }

  return result;
}

CONSTEXPR auto print_score(auto &engine) {
  return constexpr_string("> score: ")
      .append(itos(engine.get_score()))
      .append(constexpr_string("\n"));
}

CONSTEXPR auto print_moves(auto &engine) {
  return constexpr_string("> moves: ")
      .append(itos(engine.get_moves()))
      .append(constexpr_string("\n"));
}

CONSTEXPR auto print_game_state(auto &engine) {
  auto result = constexpr_string("      Meta crush saga      \n");
  auto board = print_board_to_array(engine);
  auto score = print_score(engine);
  auto moves = print_moves(engine);

  return result.append(board).append(score).append(moves);
}

#endif // COMPILE_TIME_GAME_PARSER_HPP
