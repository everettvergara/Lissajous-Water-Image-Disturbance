/*
 *  Lissajous.cpp - Main program to run a demo of Lissajous curves to animate water in a 2D image.
 *  Copyright (C) 2022 Everett Gaius S. Vergara
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *	
 */

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
        SDL_WINDOW_SHOWN,
        60
    }; 
    return vconfig;
}
