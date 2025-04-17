#ifndef UNIQUE_PTR_
#define UNIQUE_PTR_

#include <cstddef>
#include <optional>
#include <stdexcept>
#include <utility>

namespace ptrs::unique {

template <typename T>
class default_deleter {
public:
    void operator()(T *ptr) {
        delete ptr;
    }
};

template <typename T, class Deleter = default_deleter<T>>
class unique_ptr {
private:
    T *ptr = nullptr;
    Deleter deleter;

public:
    unique_ptr() noexcept = default;
    explicit unique_ptr(T *ptr_) : ptr(ptr_), deleter(){};
    unique_ptr(T *ptr_, Deleter deleter_)
        : ptr(ptr_), deleter(std::move(deleter_)){};

    unique_ptr &operator=(const unique_ptr &) = delete;
    unique_ptr(const unique_ptr &) = delete;

    unique_ptr &operator=(unique_ptr &&moved) noexcept {
        if (&moved != this) {
            reset(moved.ptr);
            deleter = std::move(moved.deleter);
            moved.ptr = nullptr;
        }
        return *this;
    }

    ~unique_ptr() noexcept {
        if (ptr) {
            deleter(ptr);
        }
    }

    bool operator==(const unique_ptr &rhs) const {
        return ptr == rhs.ptr;
    }

    bool operator!=(const unique_ptr &rhs) const {
        return ptr != rhs.ptr;
    }

    unique_ptr(unique_ptr &&moved) noexcept
        : ptr(std::exchange(moved.ptr, nullptr)),
          deleter(std::move(moved.deleter)) {
    }

    [[nodiscard]] T *get() const noexcept {
        return ptr;
    }

    [[nodiscard]] T *release() {
        return std::exchange(ptr, nullptr);
    }

    void reset(T *ptr_ = nullptr) {
        T *old_ptr = ptr;
        ptr = ptr_;
        if (old_ptr) {
            deleter(old_ptr);
        }
    }

    T *operator->() const noexcept {
        return ptr;
    }

    T &operator*() const {
        if (!ptr) {
            throw std::runtime_error("");
        }
        return *ptr;
    }

    [[nodiscard]] explicit operator bool() const {
        return ptr != nullptr;
    }

    friend void swap(unique_ptr &lhs, unique_ptr &rhs) {
        std::swap(lhs.ptr, rhs.ptr);
        std::swap(lhs.deleter, rhs.deleter);
    }
};  // class unique_ptr

}  // namespace ptrs::unique

#endif  // UNIQUE_PTR_
