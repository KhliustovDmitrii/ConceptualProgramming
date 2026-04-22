#ifndef _THREADSAFE_STACK_H_
#define _THREADSAFE_STACK_H_

#include <exception>
#include <memory>
#include <stack>
#include <mutex>

// stack-specific exception
class empty_stack : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

// thread safe class implementation
template <typename T>
class threadsafe_stack
{
public:

    threadsafe_stack() {};

    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data;
    };

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T val);
    std::shared_ptr<T> pop();
    void pop(T& val);
    bool empty() const;

private:
    std::stack<T> data;
    mutable std::mutex m;
};

template <typename T>
void threadsafe_stack<T>::push(T val)
{
    std::lock_guard<std::mutex> lock(m);
    data.push(val);
};

template <typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop()
{
    std::lock_guard<std::mutex> lock(m);

    if(data.empty()) throw empty_stack("");
    
    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));

    data.pop();
    return res;
};

template <typename T>
void threadsafe_stack<T>::pop(T& val)
{
    std::lock_guard<std::mutex> lock(m);

    if(data.empty()) throw empty_stack("");
    
    val = data.top();

    data.pop();
};

template <typename T>
bool threadsafe_stack<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m);

    return data.empty();
};

#endif