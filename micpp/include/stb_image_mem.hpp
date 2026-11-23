// Helps control memory allocation for STB Image by using C++ memory allocation operators
// instead of C's to avoid UB

#ifndef MICPP_STB_IMAGE_MEM_HPP
#define MICPP_STB_IMAGE_MEM_HPP
#include <cstring>

// Returns a new untyped block of memory
inline void* cpp_malloc(const size_t sz) {
    return new unsigned char[sz];
}

// Frees whatever pointer type is wanted to be freed
template <typename T>
void cpp_free(T* p) {
    delete[] p;
}

// Reallocates pointer to specified size
template <typename T>
void* cpp_realloc(T* old_pointer, const size_t sz) {
    void* new_pointer = new unsigned char[sz];

    // Memory allocation failed
    if (!new_pointer) {
        return nullptr;
    }

    if (old_pointer) {
        std::memcpy(new_pointer, old_pointer, sz);
    }

    delete[] old_pointer;

    return new_pointer;
}

#define STBI_MALLOC(sz) cpp_malloc(sz)
#define STBI_FREE(p) cpp_free(p)
#define STBI_REALLOC(p, new_sz) cpp_realloc(p, new_sz)

#endif //MICPP_STB_IMAGE_MEM_HPP
