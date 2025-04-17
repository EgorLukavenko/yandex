#ifndef MYTEST_HPP
#define MYTEST_HPP

#include <functional>
#include <iostream>
#include <optional>
#include <string>

namespace mytest {

void myAssert(
    bool condition,
    const std::string &expr,
    const std::string &file,
    int line,
    std::optional<std::string> message = std::nullopt
);

void registration(
    const std::function<void()> &function,
    const std::string &name = ""
);

void pushTestCase(
    const std::function<void()> &function,
    const std::string &name = ""
);

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CHECK(expr) \
    ::mytest::myAssert(static_cast<bool>(expr), #expr, __FILE__, __LINE__)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CHECK_MESSAGE(expr, msg) \
    ::mytest::myAssert(static_cast<bool>(expr), #expr, __FILE__, __LINE__, msg)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MYTEST_INTERNAL_IMPL(a, b) a##b
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define MYTEST_INTERNAL_CONCAT(a, b) MYTEST_INTERNAL_IMPL(a, b)
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TEST_CASE_REGISTER(function, name)                    \
    static int MYTEST_INTERNAL_CONCAT(mytest_int, __LINE__) = \
        (::mytest::registration(function, #name), 0)
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define TEST_CASE(name)                                        \
    static void MYTEST_INTERNAL_CONCAT(mytest_, __LINE__)();   \
    static int MYTEST_INTERNAL_CONCAT(mytest_reg_, __LINE__) = \
        (::mytest::pushTestCase(                               \
             MYTEST_INTERNAL_CONCAT(mytest_, __LINE__), #name  \
         ),                                                    \
         0);                                                   \
    static void MYTEST_INTERNAL_CONCAT(mytest_, __LINE__)()

}  // namespace mytest
#endif  // MYTEST_HPP