#include <iostream>
#include <vector>
#include <SDL.h>

#include "Demo.hpp"

using namespace g80;
auto set_default_vconfig() -> VideoConfig;

auto main(int argc, char *argv[]) -> int {

    VideoConfig vconfig {set_default_vconfig()};
    // The bounds of the lissajous flies must not be beyond the dim of the image
    // Demo demo(450000, {0, 700, 1920, 500}, "/Users/everett/Documents/Codes/Projects/Lissajous-Image-Disturbance/assets/sample-image2.bmp");
    
    // TODO: Option to move water sideways i.e. choosing a lissajous pattern 

    // bug in sample image1. not getting the h of the image.
    // bug segmentation fault if 300,000 on sample-image1.
    Demo demo(100000, {0, 376, 622, 400}, "/Users/everett/Documents/Codes/Projects/Lissajous-Image-Water-Disturbance/assets/sample-image.bmp");

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
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600},
        {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720},
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080},
        // {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2560, 1600},
        SDL_WINDOW_SHOWN,
        60
    }; 
    return vconfig;
}
