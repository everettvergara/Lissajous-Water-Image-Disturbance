/*
 *  Fly.hpp - A fly is simply a point
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

#ifndef _FLY_HPP_
#define _FLY_HPP_

#include <vector>
#include "Commons.hpp"
#include "EmptyAllocator.hpp"

namespace g80 {
    struct Fly {
        Dim32 cx;
        Dim32 cy;
        Color c;
        Color e;

        Dim16 xan;
        Dim16 yan;
        Dim16 xr;
        Dim16 yr;

        AngleIx xa;
        AngleIx ya;
        AngleIx xta;
        AngleIx yta;

    };

    using Flies = std::vector<Fly, EmptyAllocator<Fly>>;
}

#endif 