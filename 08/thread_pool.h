#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <queue>
#include <future>

using namespace std;
    
class ThreadPool
{
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex tasks_m;
    condition_variable need_run;
    bool stop;
    
public:
    explicit ThreadPool(size_t poolSize);
    
    ~ThreadPool();
        
    // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {   
        using return_type = decltype(func(args...));
        auto binded_func = bind(func, args...);
        
        shared_ptr<packaged_task<return_type()>> pack_task = make_shared<packaged_task<return_type()>>(binded_func);
    
        future<return_type> fut = pack_task->get_future();

        {
            lock_guard<mutex> lock(tasks_m);
            tasks.push([pack_task] {(*pack_task)();});
        }
        
        need_run.notify_one();
        
        return fut;
    }
};

