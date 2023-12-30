#include <iostream>

#include "game_engine.hpp"
#include "loop_inputs.hpp"
#include "parser.hpp"

// Start: compile-time computation.

int main() {

  // Parse the game state into a convenient object.
  constexpr auto current_state = parse_game_state(get_game_state_string);

  // Feed the engine with the parsed state, and update it with the keyboard
  constexpr auto new_state = game_engine(current_state).update(keyboard_input);

  // Convert the new state into a std::array<char> representation.
  constexpr auto array = print_game_state(new_state);

  // End: compile-time computation.

  // Runtime: just render the state.
  for (const char &c : array) {
    std::cout << c;
  }
}
