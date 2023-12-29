#include "loop_inputs.hpp"
#include <iostream>

// Start: compile-time computation.

constexpr auto current_state = parse_game_state(get_game_state_string); // Parse the game state into a convenient object.

constexpr auto new_state = game_engine(current_state) // Feed the engine with the parsed state,
    .update(keyboard_input);                          // Update the engine to obtain a new state.


constexpr auto array = print_game_state(new_state); // Convert the new state into a std::array<char> representation.

// End: compile-time computation.

// Runtime: just render the state.
for (const char& c : array) {  std::cout << c; }

