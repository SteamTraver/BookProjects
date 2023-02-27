#pragma once
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <memory>

template<typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    void push(const T& t);
    
    bool tryPop(T& t);
    std::shared_ptr<T> tryPop();

    void waitPop(T& t);
    std::shared_ptr<T> waitPop();

    bool empty() const;
    void clear();
private:
    mutable std::mutex mutex;
    std::condition_variable cond;
    std::queue<T> data;
};

template<typename T>
inline void ThreadSafeQueue<T>::push(const T& t)
{
    std::lock_guard<std::mutex> lock(mutex);
    data.push(t);
    cond.notify_one();
}

template<typename T>
inline bool ThreadSafeQueue<T>::tryPop(T& t)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (data.empty()) {
        return false;
    }
    t = data.front();
    data.pop();
    return true;
}

template<typename T>
inline std::shared_ptr<T> ThreadSafeQueue<T>::tryPop()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (data.empty()) {
        return std::shared_ptr<T>();
    }
    std::shared_ptr<T> result = std::make_shared<T>(data.front());
    data.pop();
    return result;
}

template<typename T>
inline void ThreadSafeQueue<T>::waitPop(T& t)
{
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [this] { return !data.empty(); });
    t = data.front();
    data.pop();
}

template<typename T>
inline std::shared_ptr<T> ThreadSafeQueue<T>::waitPop()
{
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [this] { return !data.empty(); });
    std::shared_ptr<T> result = std::make_shared<T>(data.front());
    data.pop();
    return result;
}

template<typename T>
inline bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lock(mutex);
    return data.empty();
}

template<typename T>
inline void ThreadSafeQueue<T>::clear()
{
    std::lock_guard<std::mutex> lock(mutex);
    std::queue<T> empty;
    data.swap(empty);
}
