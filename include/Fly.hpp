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