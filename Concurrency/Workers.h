#pragma once
#include "ThreadSafeQueue.h"
#include "RandomIntegerGenerator.h"
#include <iostream>
#include <vector>

class Worker
{
public:
    virtual void process() = 0;
};

class Producer : public Worker
{
public:
    explicit Producer(ThreadSafeQueue<int>* queue_)
        : queue(queue_)
        , generator(std::make_shared<RandomValueGenerator>(1,100))
    {

    }
    ~Producer() = default;

    void process() override
    {
        const int value = generator.get()->generate();
        queue->push(value);
    }

    void operator()()
    {
        if (!queue)
        {
            std::cerr << "Queue not initialized, abort operation." << std::endl;
            return;
        }
        if (!generator)
        {
            std::cerr << "Data Generator not initialized, abort operation." << std::endl;
            return;
        }
        while (true)
        {
            process();
        }
    }

private:
    std::shared_ptr<RandomValueGenerator> generator;
    ThreadSafeQueue<int>* queue;
};

class Consumer : public Worker
{
public:
    explicit Consumer(ThreadSafeQueue<int>* queue_, std::vector<int>::size_type limit_)
        : queue(queue_)
        , limit(limit_)
        , storage(std::vector<int>(limit,0))
    {

    }

    void process() override
    {
        int value = 0;
        if (queue->tryPop(value)) {
            storage.push_back(value);
        }
        if (storage.size() >= limit) {
            std::cout << "Now Print Data...." << std::endl;
            for (const int value : storage)
            {
                std::cout << value << " ";
            }
            std::cout << "\nPrint Finished." << std::endl;
            storage.clear();
            queue->clear();
        }
    }

    void operator()()
    {
        if (!queue)
        {
            std::cerr << "Data pool not initialized, abort operation." << std::endl;
            return;
        }
        if (limit == 0)
        {
            std::cerr << "Limit is invalid, abort operation." << std::endl;
            return;
        }
        while (true)
        {
            process();
        }
    }

private:
    const std::vector<int>::size_type limit;
    std::vector<int> storage;
    ThreadSafeQueue<int>* queue = nullptr;
};
