#include "thread_pool.h"
#include <cassert>

struct A {};
void foo(const A&) {}

int main(void) 
{
    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    assert(task2.get() == 1);
    
    return 0;
}
