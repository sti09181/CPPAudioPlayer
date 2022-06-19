/////////////////////////////
///// MemoryMonitor.hpp /////
/////////////////////////////

#pragma once
#include "../Libraries.hpp"

#ifndef NDEBUG
class MemoryMonitor
{
public:
    MemoryMonitor() = delete;
    ~MemoryMonitor() = delete;

    MemoryMonitor(const MemoryMonitor&) = delete;
    MemoryMonitor& operator=(const MemoryMonitor&) = delete;
    MemoryMonitor(MemoryMonitor&&) = delete;
    MemoryMonitor& operator=(MemoryMonitor&&) = delete;

    friend void* operator new(size_t size);
    friend void* operator new(size_t size, std::align_val_t alignment);
    friend void operator delete(void* memory) noexcept;
    friend void operator delete(void* memory, std::align_val_t alignment) noexcept;

private:
    static inline int countOfBlocksAllocated = 0;

    static inline void allocated(const size_t& size)
    {
        std::clog << "Memory block " << ++countOfBlocksAllocated << " allocated for " << size << " bytes\n";
    }

    static inline void freed()
    {
        std::clog << "Memory block " << countOfBlocksAllocated-- << " deallocated\n";
    }
};

void* operator new(size_t size)
{
    MemoryMonitor::allocated(size);

    if (size == 0)
        size = 1;

    void* memory;

    while ((memory = malloc(size)) == nullptr)
    {
        std::new_handler handler = std::get_new_handler();

        if (handler)
            handler();
        else
            throw std::bad_alloc();
    }

    return memory;
}

void* operator new[](size_t size)
{
    return operator new(size);
}

void* operator new(size_t size, std::align_val_t alignment)
{
    MemoryMonitor::allocated(size);

    if (size == 0)
        size = 1;

    if (static_cast<size_t>(alignment) < sizeof(void*))
        alignment = std::align_val_t(sizeof(void*));

    void* memory;

    while ((memory = _aligned_malloc(size, static_cast<size_t>(alignment))) == nullptr)
    {
        std::new_handler handler = std::get_new_handler();

        if (handler)
            handler();
        else
            throw std::bad_alloc();
    }

    return memory;
}

void* operator new[](size_t size, std::align_val_t alignment)
{
    return operator new(size, alignment);
}

void* operator new(size_t size, const std::nothrow_t&) noexcept
{
    void* memory;

    try
    {
        memory = operator new(size);
    }
    catch (...) {}

    return memory;
}

void* operator new[](size_t size, const std::nothrow_t&) noexcept
{
    void* memory;

    try
    {
        memory = operator new[](size);
    }
    catch (...) {}

    return memory;
}

void* operator new(size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    void* memory;

    try
    {
        memory = operator new(size, alignment);
    }
    catch (...) {}

    return memory;
}

void* operator new[](size_t size, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    void* memory;

    try
    {
        memory = operator new[](size, alignment);
    }
    catch (...) {}

    return memory;
}

void operator delete(void* memory) noexcept
{
    MemoryMonitor::freed();

    free(memory);

    memory = nullptr;
}

void operator delete[](void* memory) noexcept
{
    operator delete(memory);
}

void operator delete(void* memory, std::align_val_t) noexcept
{
    MemoryMonitor::freed();

    _aligned_free(memory);

    memory = nullptr;
}

void operator delete[](void* memory, std::align_val_t alignment) noexcept
{
    operator delete(memory, alignment);
}

void operator delete(void* memory, size_t) noexcept
{
    operator delete(memory);
}

void operator delete[](void* memory, size_t) noexcept
{
    operator delete[](memory);
}

void operator delete(void* memory, size_t, std::align_val_t alignment) noexcept
{
    operator delete(memory, alignment);
}

void operator delete[](void* memory, size_t, std::align_val_t alignment) noexcept
{
    operator delete[](memory, alignment);
}

void operator delete(void* memory, const std::nothrow_t&) noexcept
{
    operator delete(memory);
}

void operator delete[](void* memory, const std::nothrow_t&) noexcept
{
    operator delete[](memory);
}

void operator delete(void* memory, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    operator delete(memory, alignment);
}

void operator delete[](void* memory, std::align_val_t alignment, const std::nothrow_t&) noexcept
{
    operator delete[](memory, alignment);
}
#endif // !NDEBUG
