// throw.cpp: a C++ program that dies of an uncaught exception, which
// std::terminate turns into SIGABRT. Run by tests/unit/test_runner.c.
#include <stdexcept>

int main()
{
    throw std::runtime_error("boom");
}
