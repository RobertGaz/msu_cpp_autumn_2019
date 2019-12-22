#include "thread_pool.h"

using namespace std;
    

ThreadPool::ThreadPool(size_t poolSize) : stop(false) {
    for (int i = 0; i < poolSize; ++i) {
        threads.emplace_back(
           [this] {
//                cout<<"thr created"<<endl;
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(this->tasks_m);
                        this->need_run.wait(lock, [this] {return this->stop || !(this->tasks.empty()); } );
//                        cout<<"wake_up"<<endl;
                        if (this->stop) {
//                        cout<<"waked_to_stop\n";
                            return;
                        }
                        task = this->tasks.front();
                        this->tasks.pop();
                    }
//                    cout<<"beginning the task\n";
                    task();
                }
            }
        );
    }
}
    
ThreadPool::~ThreadPool() {
//    cout<<"Destructing ThreadPool..."<<endl;
    stop = true;
    need_run.notify_all();
    for (thread& thr : threads) 
        thr.join();
}

