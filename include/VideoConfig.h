#ifndef _VIDEO_CONFIG_H_
#define _VIDEO_CONFIG_H_

#include <string>
#include "SDL.h"

namespace g80 {
    struct VideoConfig {
        std::string title;
        SDL_Rect rect;
        Uint32 flags;
        Uint16 FPS = 60;    
    };
}

#endif 