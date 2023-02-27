#pragma once
#include <future>
#include <thread>
#include <random>
#include <deque>
#include <memory>
#include <iostream>

namespace FutureTest
{
    class Processor
    {
    public:
        virtual void process() = 0;
    };


    template<typename T>
    class MutexQueue
    {
    public:
        MutexQueue() = default;
        ~MutexQueue() = default;

        void push_back(T&& value)
        {
            std::lock_guard<std::mutex> lock(mutex);
            data.push_back(std::move(value));
        }

        void pop_front()
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (data.empty()) {
                return;
            }
            data.pop_front();
        }

        std::shared_ptr<T> front()
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (data.empty()) {
                return std::make_shared<T>();
            }
            return std::make_shared<T>(std::move(data.front()));
        }

    private:
        mutable std::mutex mutex;
        std::deque<T> data;
    };

    using Tasks = MutexQueue<std::packaged_task<int()>>;
    using Results = MutexQueue<std::future<int>>;
    using TasksPtr = std::shared_ptr<Tasks>;
    using ResultsPtr = std::shared_ptr<Results>;

    class TaskGenerator : public Processor
    {
    public:
        explicit TaskGenerator(TasksPtr tasks_, ResultsPtr results_)
            :Processor()
            , tasks(tasks_)
            , results(results_)
        {
        }

        void operator()()
        {
            while (true)
            {
                process();
            }
        }
        
        int getValue()
        {
            std::random_device device;
            return static_cast<int>(device());
        }

    private:
        void process() override
        {
            std::packaged_task<int()> task(std::bind(&TaskGenerator::getValue, this));
            results->push_back(std::move(task.get_future()));
            tasks->push_back(std::move(task));
        }
    private:
        TasksPtr tasks;
        ResultsPtr results;
    };

    class TaskProcessor : public Processor
    {
    public:
        explicit TaskProcessor(TasksPtr tasks_)
            :Processor()
            ,tasks(tasks_)
        {

        }

        void operator()()
        {
            while (true)
            {
                process();
            }
        }
    private:
        void process() override
        {
            std::shared_ptr<std::packaged_task<int()>> task = tasks->front();
            if (task.get()) {
                (*task.get())();
                tasks->pop_front();
            }
        }
    private:
        TasksPtr tasks;
    };

    class ResultProcesser : public Processor
    {
    public:
        ResultProcesser(ResultsPtr results_)
            :Processor()
            , results(results_)
        {

        }

        void operator()()
        {
            while (true)
            {
                process();
            }
        }

    private:
        void process() override
        {
            std::shared_ptr<std::future<int>> future = results->front();
            if (future.get()) {

                try
                {
                    std::cout << "Get result> " << (*(future.get())).get() << std::endl;
                    results.get()->pop_front();
                } catch (const std::future_error& error)
                {
                    std::cerr << error.code() << " " << error.what();
                }
            }
        }
    private:  
        ResultsPtr results;
    };
}