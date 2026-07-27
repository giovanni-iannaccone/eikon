#include <print>

#include <eikon/files.hpp>
#include "../include/tframework.hpp"

template <class F, typename ...Args>
void timed_execution(F &&f, Args&&... args)
{
    auto begin = std::chrono::steady_clock::now();

    std::invoke(
        std::forward<F>(f),
        std::forward<Args>(args)...
    );

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);

    std::println("\n[ Elapsed time: {}s ]\n", elapsed.count() * 1e-3);
}

void ext_runner()
{
    for (auto &&ext: eikon::files::names)
        run_tests(ext);
}

int main()
{
    std::print("[ Running test suite for eikon ]");
    timed_execution(ext_runner);
    return 0;
}
