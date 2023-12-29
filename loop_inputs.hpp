#ifndef META_CRUSH_SAGA_LOOP_INPUTS_HPP
#define META_CRUSH_SAGA_LOOP_INPUTS_HPP

#include <iostream>

// The file gets the current state the game, before it is combined to the new state.

constexpr KeyboardInput keyboard_input = KeyboardInput::KEYBOARD_INPUT; // Get the current keyboard input as a macro

constexpr auto get_game_state_string = []() constexpr
    {
        auto game_state_string = constexpr_string(
            #include "current_state.txt"
        );
        return game_state_string;
    };

#endif //META_CRUSH_SAGA_LOOP_INPUTS_HPP