while; do :
    # Start a compilation step using G++
    g++ -stc++20 -o renderer main.cpp -DKEYBOARD_INPUT="$keypressed"

    keypressed=get_key_pressed()

    # Clean the screen.
    clear

    # Obtain the rendering
    current_state=$(./renderer)
    echo $current_state # Show the rendering to the player

    # Place the rendering into a current_state.txt file and wrap it into a raw string literal.
    echo "R\"(" > current_state.txt
    echo $current_state >> current_state.txt
    echo ")\"" >> current_state.txt
done
