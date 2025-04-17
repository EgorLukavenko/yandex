#ifndef SHARED_PTR_
#define SHARED_PTR_

#include <atomic>
#include <cstddef>
#include <utility>

namespace ptrs::shared {

template <typename T>
class shared_ptr {
private:
    T *ptr = nullptr;
    std::atomic<std::size_t> *counter = nullptr;

    void release() {
        if (counter != nullptr && --(*counter) == 0) {
            delete ptr;
            delete counter;
        }
    }

public:
    shared_ptr() noexcept = default;

    explicit shared_ptr(T *ptr_)
        : ptr(ptr_), counter(new std::atomic<std::size_t>(1)) {
    }

    shared_ptr(shared_ptr &&moved) noexcept
        : ptr(std::exchange(moved.ptr, nullptr)),
          counter(std::exchange(moved.counter, nullptr)) {
    }

    shared_ptr(const shared_ptr &copied) noexcept
        : ptr(copied.ptr), counter(copied.counter) {
        if (counter != nullptr) {
            ++(*counter);
        }
    }

    shared_ptr &operator=(shared_ptr &&moved) noexcept {
        if (this != &moved) {
            release();
            ptr = std::exchange(moved.ptr, nullptr);
            counter = std::exchange(moved.counter, nullptr);
        }
        return *this;
    }

    shared_ptr &operator=(const shared_ptr &rhs) noexcept {
        if (this != &rhs) {
            release();
            ptr = rhs.ptr;
            counter = rhs.counter;
            if (counter != nullptr) {
                ++(*counter);
            }
        }
        return *this;
    }

    ~shared_ptr() noexcept {
        release();
    }

    T *get() const noexcept {
        return ptr;
    }

    void reset(T *ptr_ = nullptr) {
        release();
        ptr = ptr_;
        counter = ptr_ ? new std::atomic<std::size_t>(1) : nullptr;
    }

    [[nodiscard]] T *operator->() const noexcept {
        return ptr;
    }

    [[nodiscard]] T &operator*() const noexcept {
        return *ptr;
    }

    friend void swap(shared_ptr &lhs, shared_ptr &rhs) noexcept {
        std::swap(lhs.ptr, rhs.ptr);
        std::swap(lhs.counter, rhs.counter);
    }

    [[nodiscard]] explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    bool operator==(const shared_ptr &rhs) const noexcept {
        return ptr == rhs.ptr;
    }

    bool operator!=(const shared_ptr &rhs) const noexcept {
        return ptr != rhs.ptr;
    }
};
}  // namespace ptrs::shared

#endif  // SHARED_PTR_