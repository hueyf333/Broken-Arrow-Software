#pragma once

#include <atomic>
#include <array>
#include <cstddef>

namespace Patchwrap {

// Lock-free single-producer single-consumer queue for parameter updates
template<typename T, size_t Size = 1024>
class LockFreeQueue {
public:
    LockFreeQueue() : readIndex(0), writeIndex(0) {}

    // Try to push an item (returns false if queue is full)
    bool push(const T& item) {
        size_t currentWrite = writeIndex.load(std::memory_order_relaxed);
        size_t nextWrite = (currentWrite + 1) % Size;
        
        if (nextWrite == readIndex.load(std::memory_order_acquire)) {
            return false; // Queue is full
        }
        
        buffer[currentWrite] = item;
        writeIndex.store(nextWrite, std::memory_order_release);
        return true;
    }

    // Try to pop an item (returns false if queue is empty)
    bool pop(T& item) {
        size_t currentRead = readIndex.load(std::memory_order_relaxed);
        
        if (currentRead == writeIndex.load(std::memory_order_acquire)) {
            return false; // Queue is empty
        }
        
        item = buffer[currentRead];
        readIndex.store((currentRead + 1) % Size, std::memory_order_release);
        return true;
    }

    // Check if queue is empty
    bool isEmpty() const {
        return readIndex.load(std::memory_order_acquire) == 
               writeIndex.load(std::memory_order_acquire);
    }

    // Get approximate size (best-effort, may be inconsistent due to concurrent access)
    // Note: This is inherently racy for lock-free queues - use for diagnostics only
    size_t size() const {
        size_t write = writeIndex.load(std::memory_order_acquire);
        size_t read = readIndex.load(std::memory_order_acquire);
        if (write >= read) {
            return write - read;
        }
        return Size - (read - write);
    }

private:
    std::array<T, Size> buffer;
    std::atomic<size_t> readIndex;
    std::atomic<size_t> writeIndex;
};

} // namespace Patchwrap
