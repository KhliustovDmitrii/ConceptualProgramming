// Problem:
// Given m (alphabet size), n (word size) generate all
// words in this alphabet in alphabetic order

#include <vector>
#include <cmath>
#include <iostream>

// v 1
// some kids in my wife's group write it this way
std::vector<std::vector<int>> generate_1(int n, int m)
{
    int i, j, k;
    std::vector<std::vector<int>> result;
    // for example, n = 3
    
    for(i=0; i<m; i++)
    {
        for(j=0; j<m; j++)
        {
            for(k=0; k<m; k++)
            {
                result.push_back(std::vector<int>{i, j, k});
            }
        }
    }

    return result;
}

// v 2
// normal solution
// implements "add 1" in terms of abstract words
std::vector<std::vector<int>> generate_2(int n, int m)
{
    std::vector<std::vector<int>> result;
    int i, k;
    int B = std::pow(m, n); // exact number of objects to generate

    std::vector<int> current_word(n, 0);

    result.push_back(current_word);

    i = 1;
    while(i < B) // all words
    {
        k = n-1;
        while(current_word[k] == m-1) // overflow in k'th position
        {
            current_word[k] = 0;
            k--;
        }
        current_word[k]++;
        i++;
        result.push_back(current_word);
    }

    return result;
}

// v 3
// single cycle, no need to compute the boundary
std::vector<std::vector<int>> generate_3(int n, int m)
{
    std::vector<std::vector<int>> result;
    int k;

    std::vector<int> current_word(n, 0);
    result.push_back(current_word);

    k = n-1;

    while(k >= 0) // until we try to update "-1'st" position
    {
        if(current_word[k] == m-1) // processing overflow
        {
            current_word[k] = 0;
            k--;
        }
        else // no overflow
        {
            current_word[k]++;
            result.push_back(current_word);
            k = n-1;
        }
    }

    return result;
}

// v 4
// recursion
std::vector<std::vector<int>> generate_4_run(int n, int m, std::vector<int> word);

std::vector<std::vector<int>> generate_4(int n, int m)
{
    std::vector<int> start_word(n, 0);
    return generate_4_run(n, m, start_word);
}

std::vector<std::vector<int>> generate_4_run(int n, int m, std::vector<int> word)
{
    // get next word
    int k = n-1;
    std::vector<int> next_word(n, 0);

    while(k >= 0 && word[k] == m-1)
    {
        next_word[k] = 0;
        k--;
    }

    // last word
    if(k < 0)
    {
        return std::vector<std::vector<int>>{word};
    }

    // otherwise
    next_word[k] = word[k] + 1;
    k--;
    while(k>=0)
    {
        next_word[k] = word[k];
        k--;
    }

    std::vector<std::vector<int>> lhs = {word};
    std::vector<std::vector<int>> rhs = generate_4_run(n, m, next_word);

    lhs.insert(lhs.end(), rhs.begin(), rhs.end());

    return lhs;
}


// v 5
// corecursive computations (equivalent in this case to tail recursion)
void generate_5_run(std::vector<std::vector<int>> &result, int n, int m);

std::vector<std::vector<int>> generate_5(int n, int m)
{
    std::vector<std::vector<int>> result;

    result.push_back(std::vector<int>(n, 0));

    generate_5_run(result, n, m);

    return result;
}


// helper function for unrolling
void generate_5_run(std::vector<std::vector<int>> &result, int n, int m)
{
    std::vector<int> current_word = result.back();
    int k = n-1;

    while(k >= 0 && current_word[k] == m-1)
    {
        current_word[k] = 0;
        k--;
    }

    if(k>=0)
    {
        current_word[k]++;
        result.push_back(current_word);
        generate_5_run(result, n, m);
    }
}

int main()
{
    std::vector<std::vector<int>> result;

    result = generate_1(3, 4);
    std::cout << "v 1" << std::endl;
    for(auto v : result)
    {
        std::cout << "[";
        for(auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "]" << std::endl;
    }

    result = generate_2(3, 4);
    std::cout << "v 2" << std::endl;
    for(auto v : result)
    {
        std::cout << "[";
        for(auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "]" << std::endl;
    }

    result = generate_3(3, 4);
    std::cout << "v 3" << std::endl;
    for(auto v : result)
    {
        std::cout << "[";
        for(auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "]" << std::endl;
    }

    result = generate_4(3, 4);
    std::cout << "v 4" << std::endl;
    for(auto v : result)
    {
        std::cout << "[";
        for(auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "]" << std::endl;
    }

    result = generate_5(3, 4);
    std::cout << "v 5" << std::endl;
    for(auto v : result)
    {
        std::cout << "[";
        for(auto e : v)
        {
            std::cout << e << " ";
        }
        std::cout << "]" << std::endl;
    }
}