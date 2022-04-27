/*
 *  EmptyAllocator.hpp - Do not init anything for the vector
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

#ifndef _EMPTY_ALLOCATOR_HPP_
#define _EMPTY_ALLOCATOR_HPP_

namespace g80 {
    template<typename T>
    class EmptyAllocator : public std::allocator<T> {
    public:
        // todo: allow empty alloc to acccept Variadic Args
        auto allocate(size_t n) -> T * {
            auto p = new T[n];
            return p; 
        }

        auto deallocate(T *p, size_t n) -> void {
            delete []p;
        }
    };
}

#endif 