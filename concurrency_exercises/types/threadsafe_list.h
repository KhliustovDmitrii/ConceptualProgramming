#ifndef _THREADSAFE_LIST_H_
#define _THREADSAFE_LIST_H_

#include <memory>
#include <mutex>

template <typename T>
class threadsafe_list
{
public:

    threadsafe_list() {};
    ~threadsafe_list()
    {
        remove_all_where([](node const &){return true;});
    }

    threadsafe_list(threadsafe_list const &other) = delete;
    threadsafe_list& operator=(threadsafe_list const &other) = delete;

    void push_front(T const &val);

    template <typename Function>
    void apply(Function f);

    template <typename Predicate>
    std::shared_ptr<T> find_first_where(Predicate p);

    template <typename Predicate>
    void remove_all_where(Predicate p);

private:

    struct node
    {
        node() : next() {};

        node(T const &val) : data(std::make_shared<T> val), next() {};

        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };

    node head;
};

template <typename T>
void threadsafe_list<T>::push_front(T const& val)
{
    std::unique_ptr<node> new_node(std::make_unique<node>(val));

    std::lock_guard<std::mutex> lk(head.m);
    new_node->next = std::move(head.next);
    head.next = std::move(new_node);
}

template <typename T>
template <typename Function>
void threadsafe_list<T>::apply(Function f)
{
    node* current = &head;
    std::unique_lock<std::mutex> lk(head.m);

    while(node * const next = current->next.get())
    {
        std::unique_lock<std::mutex> next_lk(next->m);
        lk.unlock();
        f(*next->data);
        current = next;
        lk = std::move(next_lk);
    }
}

template <typename T>
template <typename Predicate>
std::shared_ptr<T> threadsafe_list<T>::find_first_where(Predicate p)
{
    node* current = &head;
    std::unique_lock<std::mutex> lk(head.m);

    while(node * const next = current->next.get())
    {
        std::unique_lock<std::mutex> next_lk(next->m);
        lk.unlock();

        if(p(*next->data))
            return next->data;
        
        current = next;
        lk = std::move(next_lk);
    }

    return std::shared_ptr<T>();
}

template <typename T>
template <typename Predicate>
void threadsafe_list<T>::remove_all_where(Predicate p)
{
    node *current = &head;
    std::unique_lock<std::mutex> lk(head.m);

    while(node * const next = current->next.get())
    {
        std::unique_lock<std::mutex> next_lk(next->m);

        if(p(*next->data))
        {
            std::unique_ptr<node> old_next = std::move(current->next);
            current->next = std::move(next->next);
            next_lk.unlock();
        }
        else
        {
            lk.unlock();
            current = next;
            lk = std::move(next_lk);
        }
    }
}

#endif