#ifndef _THREADSAFE_LOOKUP_TABLE_H_
#define _THREADSAFE_LOOKUP_TABLE_H_

#include <memory>
#include <list>
#include <optional>
#include <vector>
#include <map>
#include <shared_mutex>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table
{
public:
    threadsafe_lookup_table(size_t num_buckets = 19, Hash const &hasher_ = Hash()) :
    buckets(num_buckets), hasher(hasher_)
    {
        for(size_t i=0; i<buckets.size(); i++)
            buckets[i].reset(new bucket_type);
    }

    threadsafe_lookup_table(threadsafe_lookup_table const &other) = delete;
    threadsafe_lookup_table& operator=(threadsafe_lookup_table const &other) = delete;

    std::optional<Value> value_for(Key const &k);
    void add_update_mapping(Key const &k, Value v);
    void remove_mapping(Key const &k);

    std::map<Key, Value> get_snapshot() const;

private:
    class bucket_type
    {
    public:
        std::optional<Value> value_for(Key const &k);
        void add_update_mapping(Key const &k, Value v);
        void remove_mapping(Key const &k);

    private:
        using bucket_value = std::pair<Key, Value>;
        using bucket_data = std::list<bucket_value>;
        using bucket_iterator = typename bucket_data::iterator;

        bucket_data data;
        mutable std::shared_mutex mut;

        bucket_iterator find_entry_for(Key const &k)
        {
            return std::find_if(data.begin(), data.end(), 
            [&](bucket_value const &item){
                return item.first == k;
            });
        }
    };

    Hash hasher;
    std::vector<std::unique_ptr<bucket_type>> buckets;

    bucket_type& get_bucket(Key const &k);
};

template <typename Key, typename Value, typename Hash>
std::optional<Value>
threadsafe_lookup_table<Key, Value, Hash>::bucket_type::value_for(Key const& k)
{
    std::shared_lock<std::shared_mutex> lk(mut);

    auto search_result = find_entry_for(k);

    if(search_result == data.end())
        return std::nullopt;

    return search_result->second;
}

template <typename Key, typename Value, typename Hash>
void
threadsafe_lookup_table<Key, Value, Hash>::bucket_type::add_update_mapping(Key const& k, Value v)
{
    std::unique_lock<std::shared_mutex> lk(mut);

    auto search_result = find_entry_for(k);

    if(search_result == data.end())
        data.push_back(bucket_value(k, v));
    else
        search_result->second = v;
}

template <typename Key, typename Value, typename Hash>
void
threadsafe_lookup_table<Key, Value, Hash>::bucket_type::remove_mapping(Key const& k)
{
    std::unique_lock<std::shared_mutex> lk(mut);

    auto search_result = find_entry_for(k);

    if(search_result != data.end())
        data.erase(search_result);
}

template <typename Key, typename Value, typename Hash>
typename threadsafe_lookup_table<Key, Value, Hash>::bucket_type&
threadsafe_lookup_table<Key, Value, Hash>::get_bucket(Key const& k)
{
    size_t bucket_num = hasher(k)%buckets.size();
    return *buckets[bucket_num];
}

template <typename Key, typename Value, typename Hash>
std::optional<Value>
threadsafe_lookup_table<Key, Value, Hash>::value_for(Key const& k)
{
    return get_bucket(k).value_for(k);
}

template <typename Key, typename Value, typename Hash>
void
threadsafe_lookup_table<Key, Value, Hash>::add_update_mapping(Key const& k, Value v)
{
    return get_bucket(k).add_update_mapping(k, v);
}

template <typename Key, typename Value, typename Hash>
void
threadsafe_lookup_table<Key, Value, Hash>::remove_mapping(Key const& k)
{
    return get_bucket(k).remove_mapping(k);
}

template <typename Key, typename Value, typename Hash>
std::map<Key, Value>
threadsafe_lookup_table<Key, Value, Hash>::get_snapshot() const
{
    std::vector<std::shared_lock<std::shared_mutex>> lock_vec;

    for(size_t i=0; i<buckets.size(); i++)
        lock_vec.push_back(std::shared_lock<std::shared_mutex>(buckets[i].mut));

    std::map<Key, Value> result;

    for(size_t i=0; i<buckets.size(); i++)
    {
        for(auto it = buckets[i].data.begin();
            it != buckets[i].data.end();
            it++)
        {
            result.insert(*it);
        }
    }

    return result;
}

#endif