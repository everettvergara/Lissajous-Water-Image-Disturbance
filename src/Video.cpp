#include <iostream>
#include "Video.h"

using namespace g80;

Video::Video() : 
    is_init_(SDL_VideoInit(NULL) == 0 ? true : false) {
}

Video::~Video() {
    if (window_)
        SDL_DestroyWindow(window_);
    SDL_VideoQuit();
}

auto Video::create_window(const VideoConfig &vconfig) -> bool {

    if (!is_init_)
        return false;

    if (vconfig.FPS == 0)
        return false;

    if (window_)
        return false;

    window_ = SDL_CreateWindow(
        vconfig.title.c_str(), 
        vconfig.rect.x, 
        vconfig.rect.y, 
        vconfig.rect.w, 
        vconfig.rect.h, 
        vconfig.flags);

    if (window_ == NULL)
        return false;
    
    surface_ = SDL_GetWindowSurface(window_);

    if (surface_ == NULL)
        return false;

    MSPF_ = 1000 / vconfig.FPS;

    pixel_end_ = static_cast<Uint32 *>(surface_->pixels) + surface_->w * surface_->h;

    return true;
}

auto Video::destroy_window() -> void {
    if (window_)
        SDL_DestroyWindow(window_);
    
    window_ = {NULL};
    is_init_ = {false};
}

auto Video::is_init() -> bool { 
    return is_init_;
}

auto Video::is_running() -> bool { 
    return is_running_;
}

auto Video::preprocess_states() -> bool {
    return true;
}


auto Video::update_states() -> bool {
    return true;
}

auto Video::capture_events() -> bool {
    return true;
}

auto Video::update_window_surface() -> bool {
    return SDL_UpdateWindowSurface(window_) == 0;
}

auto Video::run() -> bool {
    
    is_running_ = true;

    while (is_running_) {

        Uint32 start = SDL_GetTicks(); 

        if (!update_states() ||
            !update_window_surface() ||
            !capture_events()) {
            is_running_ = false;
            return false;
        }
        
        Uint32 end = SDL_GetTicks();
        Uint32 elapsed = end - start;
        if (elapsed < MSPF_)
            SDL_Delay(MSPF_ - elapsed);

    }
    
    return true;
}
