#include "mytest.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include "mytest_internal.hpp"

namespace mytest {

// cppcheck-suppress unusedFunction
void myAssert(
    bool condition,
    const std::string &expr,
    const std::string &file,
    int line,
    std::optional<std::string> message
) {
    if (condition) {
        return;
    }
    std::cerr << "CHECK(" << expr << ") at " << file << ":" << line
              << " failed!\n";
    if (message.has_value()) {
        std::cerr << "    message: " << message.value() << "\n";
    }
}

TestRunner &TestRunner::getInstance() {
    static TestRunner instance;
    return instance;
}

void TestRunner::registerTest(bool success) {
    tests_count++;
    if (success) {
        successful_tests++;
    }
}

// cppcheck-suppress unusedFunction
void pushTestCase(
    const std::function<void()> &function,
    const std::string &name
) {
    TestRunner::getInstance().addTestCase(name, function);
}

void registration(
    const std::function<void()> &function,
    const std::string &name
) {
    std::cerr << "Running " << name << "...\n";
    const std::ostringstream buffer;
    std::streambuf *old = std::cerr.rdbuf(buffer.rdbuf());
    function();
    std::cerr.rdbuf(old);
    const bool success = buffer.str().empty();
    std::cerr << buffer.str();
    TestRunner::getInstance().registerTest(success);
}

void TestRunner::runTests() {
    std::sort(
        test_cases.begin(), test_cases.end(),
        [](const TestFunction &a, const TestFunction &b) {
            return a.name < b.name;
        }
    );
    for (auto &test_case : test_cases) {
        registration(test_case.function, test_case.name);
    }
    std::cerr << "===== Tests passed: " << successful_tests << "/"
              << tests_count << " =====\n";
    // NOLINTNEXTLINE (concurrency-mt-unsafe)
    std::exit((tests_count == successful_tests) ? 0 : 1);
}

}  // namespace mytest
