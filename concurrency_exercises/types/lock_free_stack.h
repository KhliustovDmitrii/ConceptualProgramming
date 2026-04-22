#ifndef _LOCKFREE_STACK_H_
#define _LOCKFREE_STACK_H_

#include <atomic>
#include <memory>

template <typename T>
class lock_free_stack
{
public:
    void push(T val);
    std::shared_ptr<T> pop();

private:
    struct node
    {
        std::shared_ptr<T> data;
        node *next;

        node(const T& data_) : data(std::make_shared<T>(data_)) {};
    };

    void try_reclaim(node *old_head);
    static void delete_nodes(node *nodes);
    void chain_pending_nodes(node *nodes);
    void chain_pending_nodes(node *first, node *last);
    void chain_pending_node(node *n);

    std::atomic<node *> head;
    std::atomic<unsigned> threads_in_pop;

    std::atomic<node *> to_be_deleted;
};

template <typename T>
void lock_free_stack<T>::push(T val)
{
    node * const new_node = new node(val);

    new_node->next = head.load();

    while(!head.compare_exchange_weak(new_node->next, new_node));
}

template <typename T>
std::shared_ptr<T> lock_free_stack<T>::pop()
{
    ++threads_in_pop;

    node * old_head = head.load();

    while(old_head && !head.compare_exchange_weak(old_head, old_head->next));

    std::shared_ptr<T> res;
    if(old_head)
    {
        res.swap(old_head.data);
    }

    try_reclaim(old_head);

    return res;
}

template <typename T>
void lock_free_stack<T>::delete_nodes(node *nodes)
{
    while(nodes)
    {
        node *next = nodes->next;
        delete nodes;
        nodes = next;
    }
}

template <typename T>
void lock_free_stack<T>::try_reclaim(node *old_head)
{
    if(threads_in_pop == 1)
    {
        node *nodes_to_delete = to_be_deleted.exchange(nullptr);
        if(!--threads_in_pop)
        {
            delete_nodes(nodes_to_delete);
        }
        else if(nodes_to_delete)
        {
            chain_pending_nodes(nodes_to_delete);
        }

        delete old_head;
    }
    else
    {
        chain_pending_nodes(old_head);
        --threads_in_pop;
    }
}

template <typename T>
void lock_free_stack<T>::chain_pending_nodes(node *nodes)
{
    node *last = nodes;
    while(node * const next = last->next)
    {
        last = next;
    }

    chain_pending_nodes(nodes, last);
}

template <typename T>
void lock_free_stack<T>::chain_pending_nodes(node *first, node *last)
{
    last->next = to_be_deleted;
    while(!to_be_deleted.compare_exchange_weak(last->next, first));
}

template <typename T>
void lock_free_stack<T>::chain_pending_node(node *n)
{
    chain_pending_nodes(n, n);
}
#endif