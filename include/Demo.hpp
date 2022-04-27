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
        Demo (const SDL_Rect &fly_rect, const std::string &bmp_file, const Dim &TAIL = 2) : 
            fly_area_(fly_rect),
            recalc_fly_area_(fly_area_),
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
        Dim N_;
        Dim FLY_RADIUS_{10};
        Dim MAX_FLY_INIT_ANGLE{20};
        SDL_Rect fly_area_, recalc_fly_area_;
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

        auto get_fitted_rect(int w_from, int h_from, int w_to, int h_to) -> SDL_Rect;
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

    auto Demo::get_fitted_rect(int w_from, int h_from, int w_to, int h_to) -> SDL_Rect {

        if (w_from <= w_to && h_from <= h_to)
            return {w_to / 2 - w_from / 2, h_to / 2 - h_from / 2, w_from, h_to};
        
        int w = w_from, h = h_from;
        
        if (w > w_to) {
            float rr = 1.0f * w / w_to; 
            w /= rr; 
            h /= rr;
        }

        if (h > h_to) {
            float rr = 1.0f * h / h_to; 
            w /= rr;
            h /= rr; 
        }

        return {w_to / 2 - w / 2, h_to / 2 - h / 2, w, h};
    }

    auto Demo::init_flies() -> bool {
        
        SDL_Rect resized_bmp_rect = get_fitted_rect(BMP_->w, BMP_->h, surface_->w * 0.90f, surface_->h * 0.90f);
        resized_bmp_rect.x += surface_->w * 0.10f / 2.0f;
        resized_bmp_rect.y += surface_->h * 0.10f / 2.0f;
        SDL_BlitScaled(BMP_, NULL, surface_, &resized_bmp_rect);

        // float ar = 1.0f * BMP_->w / BMP_->h;
        float ar_w = 1.0f * BMP_->w / resized_bmp_rect.w;
        float ar_h = 1.0f * BMP_->h / resized_bmp_rect.h;
        
        SDL_Rect resized_fly_area = fly_area_;
        resized_fly_area.x /= ar_w;
        resized_fly_area.y /= ar_h;
        resized_fly_area.w /= ar_w;
        resized_fly_area.h /= ar_h;
        
        float x = resized_fly_area.x + resized_bmp_rect.x;
        float y = resized_fly_area.y + resized_bmp_rect.y;
        recalc_fly_area_.x = static_cast<Dim32>(x);
        recalc_fly_area_.y = static_cast<Dim32>(y);
        recalc_fly_area_.w = resized_fly_area.w;
        recalc_fly_area_.h = resized_fly_area.h;        
        
        
        float sample_per_row = N_ / recalc_fly_area_.h;         
        float size_of_each_stepf = recalc_fly_area_.w / sample_per_row;
        Dim32 size_of_each_stepi = static_cast<Dim32>(size_of_each_stepf) == 0 ? 1 : static_cast<Dim32>(size_of_each_stepf);
        N_ = resized_fly_area.w * resized_fly_area.h;
        flies_.reserve(N_);

        for (Dim i = 0; i < N_; ++i) {
            
            Dim32 *pixel = static_cast<Dim32 *>(surface_->pixels) + static_cast<Dim32>(y * surface_->w) + static_cast<Dim32>(x) + rnd() % static_cast<Dim32>(size_of_each_stepi);
            Uint8 r, g, b;
            SDL_GetRGB(*pixel, surface_->format, &r, &g, &b);

            flies_.emplace_back(Fly{
                static_cast<Dim32>(x), 
                static_cast<Dim32>(y),
                SDL_MapRGBA(surface_->format, r, g, b, 255),
                SDL_MapRGBA(surface_->format, r / 1.25f, g / 1.25f, b / 1.25f, 255),
                static_cast<Dim16>(1 + rnd() % MAX_FLY_INIT_ANGLE),
                static_cast<Dim16>(1 + rnd() % MAX_FLY_INIT_ANGLE),
                static_cast<Dim16>(1 + rnd() % FLY_RADIUS_),
                static_cast<Dim16>(1 + rnd() % FLY_RADIUS_)});

            x += size_of_each_stepf;
            if (x >= recalc_fly_area_.x + resized_bmp_rect.w) {
                x = recalc_fly_area_.x;
                ++y;
            }
        }
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

            if (x >= recalc_fly_area_.x && x <= recalc_fly_area_.x + recalc_fly_area_.w &&
                y >= recalc_fly_area_.y && y <= recalc_fly_area_.y + recalc_fly_area_.h)
                set_pixel(x, y, fly.e);
        }

        // Update and Draw
        for (auto &fly : flies_) {
            Dim x = fly.cx + fly.xr * cosf_[fly.xa];
            Dim y = fly.cy + fly.yr * sinf_[fly.ya];
            fly.xa = (fly.xa + fly.xan) % 360;
            fly.ya = (fly.ya + fly.yan) % 360;

            if (x >= recalc_fly_area_.x && x <= recalc_fly_area_.x + recalc_fly_area_.w &&
                y >= recalc_fly_area_.y && y <= recalc_fly_area_.y + recalc_fly_area_.h)
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