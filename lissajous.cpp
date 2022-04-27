#include <SDL.h>
#include "Demo.hpp"

using namespace g80;
auto set_default_vconfig() -> VideoConfig;

auto main(int argc, char *argv[]) -> int {

    VideoConfig vconfig {set_default_vconfig()};

    Demo demo("/Users/everett/Documents/Codes/Projects/Lissajous-Image-Water-Disturbance/assets/sample-image2.bmp", {0, 700, 1920, 500});
    // Demo demo("/Users/everett/Documents/Codes/Projects/Lissajous-Image-Water-Disturbance/assets/sample-image.bmp", {0, 376, 622, 400});
    
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
        "Water effect - Image Disturbance",
        {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720},
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1680, 1050},
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2560, 1600},
        SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN,
        60
    }; 
    return vconfig;
}
