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