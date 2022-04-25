#ifndef _DEMO_HPP_
#define _DEMO_HPP_

#include <iostream>
#include <ctime>
#include <cmath>
#include <array>
#include <vector>
#include <cassert>

#include "Commons.hpp"
#include "Video.h"
#include "Fly.hpp"
#include "EmptyAllocator.hpp"

namespace g80 {

    class Demo : public Video {

    public: 
        Demo (const Dim N, const std::string &bmp_file, const Dim TAIL = 2) : 
            N_(N),
            TAIL_(TAIL),
            BMP_(SDL_LoadBMP(bmp_file.c_str())) {

            if (!BMP_) {
                SDL_Log("Error loading the bitmap!");
                exit(0);
            }
        }   
        ~Demo() {
            SDL_FreeSurface(BMP_);
        }
        auto create_window(const VideoConfig &video_config) -> bool;
        auto preprocess_states() -> bool;
        auto update_states() -> bool;
        auto capture_events() -> bool;

    private:
        const Dim N_;
        Dim FLY_RADIUS_{20};
        Dim MAX_FLY_INIT_ANGLE{20};
        Dim TAIL_;

        Flies flies_;
        std::array<float, 360> cosf_;
        std::array<float, 360> sinf_;
        SDL_Surface *BMP_{NULL};

        inline auto rnd() -> Dim32 {
            static std::time_t now = time(&now);
            static Dim32 seed = now;
            static Dim32 a = 1103515245;
            static Dim32 c = 12345;
            static Dim32 m = 2147483647;
            static RandNum rand = (seed * a + c) % m;
            return rand = (rand * a + c) % m; 
        }
        auto init_sincos_table() -> bool;
        auto init_reserved_flies() -> bool;
        auto init_flies() -> bool;
        auto init_fly_tail() -> bool;
    };

    auto Demo::create_window(const VideoConfig &video_config) -> bool {
        if (!Video::create_window(video_config)) return false;
        return true;
    }

    auto Demo::init_sincos_table() -> bool {
        float angle = 0.0f;
        float angle_inc = M_PI * 2.0f / 360.0f;
        for (AngleIx i = 0; i < 360; ++i) {            
            cosf_[i] = cosf(angle);     
            sinf_[i] = sinf(angle);     
            angle += angle_inc;
        }
        return true;
    }

    auto Demo::init_reserved_flies() -> bool {
        flies_.reserve(N_);
        return true;
    }

    auto Demo::init_flies() -> bool {
        
        SDL_Surface *bmp = SDL_CreateRGBSurface(0, BMP_->w, BMP_->h, 32, 0, 0, 0, 0);
        SDL_BlitSurface(BMP_, NULL, bmp, NULL );
        // return true;

        float sample_per_row = 1.0f * N_ / bmp->h;         
        float size_of_each_step = bmp->w / sample_per_row;
        float x = 0.0f, y = 0.0f;

        for (Dim i = 0; i < N_; ++i) {
            Uint32 *pixel = static_cast<Uint32 *>(bmp->pixels) + (int)(y * bmp->w) + (int)(x * size_of_each_step);
            Uint8 r, g, b;
            SDL_GetRGB(*pixel, bmp->format, &r, &g, &b);

            flies_.emplace_back(Fly{
                surface_->w / 2 - bmp->w / 2 + static_cast<Dim>(x * size_of_each_step), 
                surface_->h / 2 - bmp->h / 2 + static_cast<Dim>(y),
                SDL_MapRGBA(surface_->format, r, g, b, 255),
                static_cast<Dim16>(1 + rnd() % MAX_FLY_INIT_ANGLE),
                static_cast<Dim16>(1 + rnd() % MAX_FLY_INIT_ANGLE),
                static_cast<Dim16>(1 + rnd() % FLY_RADIUS_),
                static_cast<Dim16>(1 + rnd() % FLY_RADIUS_)});

            if (++x >= sample_per_row) {
                x = 0.0f;
                ++y;
            }
        }

        SDL_FreeSurface(bmp);

        return true;
    }

    auto Demo::init_fly_tail() -> bool {
        for (auto &fly : flies_) {
            Dim max_offset_x = (360 / fly.xan) - TAIL_;
            Dim max_offset_y = (360 / fly.yan) - TAIL_;
            fly.xa = (rnd() % max_offset_x * fly.xan) + TAIL_ * fly.xan;
            fly.ya = (rnd() % max_offset_y * fly.yan) + TAIL_ * fly.yan;
            fly.xta = fly.xa - TAIL_ * fly.xan;
            fly.yta = fly.ya - TAIL_ * fly.yan;        
        }
        return true;
    }

    auto Demo::preprocess_states() -> bool {
        if (!init_sincos_table() ||
            !init_reserved_flies() ||
            !init_flies() ||
            !init_fly_tail())
            return false;
        
        return true;
    }

    auto Demo::update_states() -> bool {

        SDL_LockSurface(surface_);

        // Erase
        for (auto &fly : flies_) {
            Dim x = fly.cx + fly.xr * cosf_[fly.xta];
            Dim y = fly.cy + fly.yr * sinf_[fly.yta];
            fly.xta = (fly.xta + fly.xan) % 360;
            fly.yta = (fly.yta + fly.yan) % 360;
            set_pixel(x, y, 0);
        }

        // Update and Draw
        for (auto &fly : flies_) {
            Dim x = fly.cx + fly.xr * cosf_[fly.xa];
            Dim y = fly.cy + fly.yr * sinf_[fly.ya];
            fly.xa = (fly.xa + fly.xan) % 360;
            fly.ya = (fly.ya + fly.yan) % 360;
            set_pixel(x, y, fly.c);
        }
        
        SDL_UnlockSurface(surface_);
        return true;
    }

    auto Demo::capture_events() -> bool {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
                is_running_ = false;

            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    is_running_ = false;
            }
        }
        return true;
    }
}

#endif 