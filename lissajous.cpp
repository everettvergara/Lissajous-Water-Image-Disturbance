#include <iostream>
#include <vector>
#include <SDL.h>

#include "Demo.hpp"

using namespace g80;
auto set_default_vconfig() -> VideoConfig;

auto main(int argc, char *argv[]) -> int {

    VideoConfig vconfig {set_default_vconfig()};
    Demo demo(25000, "/Users/everett/Documents/Codes/Projects/Lissajous-Image/assets/sample-image.bmp");

    if (!demo.is_init() ||
        !demo.create_window(set_default_vconfig()) ||
        !demo.preprocess_states() ||
        !demo.run()) { 

        SDL_Log("Error... whatever!");
        return -1;
    }

    return 0;
}

auto set_default_vconfig() -> VideoConfig {
    VideoConfig vconfig {
        "Smiley Lissajous Flies",
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600},
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720},
        {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080},
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2560, 1600},
        SDL_WINDOW_SHOWN,
        60
    }; 
    return vconfig;
}
