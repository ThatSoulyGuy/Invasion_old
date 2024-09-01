#pragma once

#include "Util/Typedefs.hpp"

using namespace Invasion::Util;

namespace Invasion::Thread
{
    class ThreadPool
    {

    public:

        ThreadPool(size_t numThreads)
        {
            Start(numThreads);
        }

        ~ThreadPool()
        {
            Stop();
        }

        template<typename T>
        auto operator+=(T task) -> Future<decltype(task())>
        {
            auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));

            {
                std::unique_lock<Mutex> lock{ eventMutex };
                tasks.emplace([=] { (*wrapper)(); });
            }

            eventVar.notify_one();
            return wrapper->get_future();
        }

    private:

        std::vector<Util::Thread> threads;
        ConditionVariable eventVar;
        Mutex eventMutex;
        bool stopping = false;
        std::queue<Function<void()>> tasks;

        void Start(size_t numThreads)
        {
            for (auto i = 0u; i < numThreads; ++i)
            {
                threads.emplace_back([=] 
                {
                    while (true)
                    {
                        Function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock{ eventMutex };

                            eventVar.wait(lock, [=]
                            { 
                                return stopping || !tasks.empty(); 
                            });

                            if (stopping && tasks.empty())
                                break;

                            task = std::move(tasks.front());
                            tasks.pop();
                        }

                        task();
                    }
                });
            }
        }

        void Stop() noexcept
        {
            {
                std::unique_lock<Mutex> lock{ eventMutex };
                stopping = true;
            }

            eventVar.notify_all();

            for (auto& thread : threads)
                thread.join();
        }
    };
}