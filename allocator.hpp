#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>

class TLSFAllocator {
public:
    explicit TLSFAllocator(std::size_t memoryPoolSize);
    ~TLSFAllocator();

    void* allocate(std::size_t size);
    void deallocate(void* ptr);

    void* getMemoryPoolStart() const;
    std::size_t getMemoryPoolSize() const;
    std::size_t getMaxAvailableBlockSize() const;

    // Disable copy/move
    TLSFAllocator(const TLSFAllocator&) = delete;
    TLSFAllocator& operator=(const TLSFAllocator&) = delete;
    TLSFAllocator(TLSFAllocator&&) = delete;
    TLSFAllocator& operator=(TLSFAllocator&&) = delete;

private:
    struct BlockHeader {
        std::size_t size; // size of user data area
        bool isFree;
        BlockHeader* prev;
        BlockHeader* next;
    };

    void* memoryPool;
    std::size_t poolSize;
    BlockHeader* head; // start of block list

    void splitBlock(BlockHeader* block, std::size_t size);
    void coalesce(BlockHeader* block);
    BlockHeader* findFreeBlock(std::size_t size);
};
