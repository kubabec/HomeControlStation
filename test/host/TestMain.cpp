#include "TestHarness.hpp"

#include <iostream>

namespace TestHarness
{
std::vector<TestCase>& tests()
{
    static std::vector<TestCase> collection;
    return collection;
}

Registrar::Registrar(const char* name, std::function<void()> function, bool expectedFailure)
{
    tests().push_back({name, std::move(function), expectedFailure});
}
}

int main()
{
    unsigned failures = 0;
    unsigned expectedFailures = 0;
    for (const auto& test : TestHarness::tests())
    {
        try
        {
            test.function();
            if (test.expectedFailure)
            {
                ++failures;
                std::cout << "[XPASS] " << test.name << " (known defect no longer reproduces)\n";
            }
            else
            {
                std::cout << "[PASS] " << test.name << '\n';
            }
        }
        catch (const std::exception& error)
        {
            if (test.expectedFailure)
            {
                ++expectedFailures;
                std::cout << "[XFAIL] " << test.name << "\n        " << error.what() << '\n';
            }
            else
            {
                ++failures;
                std::cout << "[FAIL] " << test.name << "\n       " << error.what() << '\n';
            }
        }
    }
    std::cout << "\n" << (TestHarness::tests().size() - failures) << "/"
              << TestHarness::tests().size() << " tests passed";
    if (expectedFailures > 0) std::cout << " or matched known defects";
    std::cout << "; " << expectedFailures << " expected failures\n";
    return failures == 0 ? 0 : 1;
}
