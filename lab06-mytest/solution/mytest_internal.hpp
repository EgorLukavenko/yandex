#ifndef MYTEST_INTERNAL_HPP
#define MYTEST_INTERNAL_HPP

#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace mytest {

class TestFunction {
public:
    std::string name;
    std::function<void()> function;
};

class TestRunner {
public:
    static TestRunner &getInstance();

    void registerTest(bool success);

    void runTests();

    void addTestCase(
        const std::string &name,
        const std::function<void()> &function
    ) {
        test_cases.emplace_back(TestFunction{name, function});
    }

private:
    TestRunner() = default;

    int tests_count = 0;
    int successful_tests = 0;
    std::vector<mytest::TestFunction> test_cases;
};

}  // namespace mytest

#endif  // MYTEST_INTERNAL_HPP
