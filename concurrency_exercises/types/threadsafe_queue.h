#ifndef _THREADSAFE_QUEUE_H_
#define _THREADSAFE_QUEUE_H_

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class threadsafe_queue
{
public:
    threadsafe_queue() {};
    threadsafe_queue(const threadsafe_queue& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data = other.data;
    };

    threadsafe_queue& operator=(const threadsafe_queue&) = delete;

    void push(T val);

    bool try_pop(T& val);
    std::shared_ptr<T> try_pop();

    void wait_and_pop(T& val);
    std::shared_ptr<T> wait_and_pop();

    bool empty() const;

private:
    mutable std::mutex mut;
    std::queue<T> data;
    std::condition_variable data_cond;
};

template <typename T>
void threadsafe_queue<T>::push(T val)
{
    std::lock_guard<std::mutex> lk(mut);
    data.push(val);
    data_cond.notify_one();
}

template <typename T>
bool threadsafe_queue<T>::try_pop(T& val)
{
    std::lock_guard<std::mutex> lk(mut);
    if(data.empty()) return false;

    val = data.front();
    data.pop();

    return true;
}

template <typename T>
std::shared_ptr<T> threadsafe_queue<T>::try_pop()
{
    std::lock_guard<std::mutex> lk(mut);
    if(data.empty()) return std::shared_ptr<T>();

    std::shared_ptr<T> res(std::make_shared<T>(data.front()));
    data.pop();

    return res;
}

template <typename T>
bool threadsafe_queue<T>::empty() const
{
    std::lock_guard<std::mutex> lk(mut);

    return data.empty();
}

template<typename T>
void threadsafe_queue<T>::wait_and_pop(T& val)
{
    std::unique_lock<std::mutex> lk(mut);

    data_cond.wait(lk, [this](){return !data.empty();});
    val = data.front();
    data.pop();
}

template<typename T>
std::shared_ptr<T> threadsafe_queue<T>::wait_and_pop()
{
    std::unique_lock<std::mutex> lk(mut);

    data_cond.wait(lk, [this](){return !data.empty();});
    std::shared_ptr<T> res(std::make_shared<T>(data.front()));
    data.pop();
    return res;
}

#endif