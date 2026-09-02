#pragma once

#include <cmath>
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace TestHarness
{
struct TestCase { const char* name; std::function<void()> function; bool expectedFailure; };
std::vector<TestCase>& tests();

struct Registrar
{
    Registrar(const char* name, std::function<void()> function, bool expectedFailure = false);
};

template <typename Actual, typename Expected>
void equal(const Actual& actual, const Expected& expected, const char* actualText,
           const char* expectedText, const char* file, int line)
{
    if (!(actual == expected))
    {
        std::ostringstream message;
        message << file << ':' << line << ": expected " << actualText << " == " << expectedText;
        throw std::runtime_error(message.str());
    }
}

inline void truth(bool value, const char* expression, const char* file, int line)
{
    if (!value)
    {
        std::ostringstream message;
        message << file << ':' << line << ": expected true: " << expression;
        throw std::runtime_error(message.str());
    }
}

inline void near(float actual, float expected, float tolerance, const char* file, int line)
{
    if (std::fabs(actual - expected) > tolerance)
    {
        std::ostringstream message;
        message << file << ':' << line << ": expected " << actual << " near " << expected
                << " (+/- " << tolerance << ')';
        throw std::runtime_error(message.str());
    }
}
}

#define HCS_CONCAT_INNER(a, b) a##b
#define HCS_CONCAT(a, b) HCS_CONCAT_INNER(a, b)
#define TEST_CASE(name) \
    static void HCS_CONCAT(test_, __LINE__)(); \
    static TestHarness::Registrar HCS_CONCAT(registrar_, __LINE__)(name, HCS_CONCAT(test_, __LINE__)); \
    static void HCS_CONCAT(test_, __LINE__)()
#define TEST_CASE_XFAIL(name) \
    static void HCS_CONCAT(test_, __LINE__)(); \
    static TestHarness::Registrar HCS_CONCAT(registrar_, __LINE__)(name, HCS_CONCAT(test_, __LINE__), true); \
    static void HCS_CONCAT(test_, __LINE__)()
#define EXPECT_TRUE(value) TestHarness::truth(static_cast<bool>(value), #value, __FILE__, __LINE__)
#define EXPECT_FALSE(value) TestHarness::truth(!static_cast<bool>(value), "!(" #value ")", __FILE__, __LINE__)
#define EXPECT_EQ(actual, expected) TestHarness::equal((actual), (expected), #actual, #expected, __FILE__, __LINE__)
#define EXPECT_NEAR(actual, expected, tolerance) TestHarness::near((actual), (expected), (tolerance), __FILE__, __LINE__)
