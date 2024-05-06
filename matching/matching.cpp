#include "matching.h"

Khun::Khun(int n, int m) : _n(n), _m(m)
{
    _bgraph.assign(n + 1, std::vector<int>());
    _mt.assign(m + 1, -1);
}

void Khun::add_edge(int x, int y)
{
    _bgraph[x].push_back(y);
}

void Khun::add_matching_edge(int x, int y)
{
    _mt[y] = x;
}

void Khun::clear()
{
    for (int &u : _mt)
    {
        u = -1;
    }
    for (std::vector<int> &u : _bgraph)
    {
        u.clear();
    }
}

int Khun::find_matching()
{

    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> order(_n + 1);
    std::iota(std::begin(order), std::end(order), 0);
    std::shuffle(order.begin(), order.end(), g);

    int size_of_matching = 0;

    for (int vertex : order)
    {
        for (int edge : _bgraph[vertex])
        {
            if (_mt[edge] != -1)
            {
                _mt[edge] = -1;
                break;
            }
        }
    }

    std::shuffle(order.begin(), order.end(), g);

    std::vector<int> used(_n + 1, 0);
    int timer = 1;

    std::function<bool(int)> dfs = [&](int index) -> bool
    {
        if (used[index] == timer)
        {
            return false;
        }
        used[index] = timer;
        for (auto u : _bgraph[index])
        {
            if (_mt[u] == -1 || dfs(_mt[u]))
            {
                _mt[u] = static_cast<int>(index);
                return true;
            }
        }
        return false;
    };

    for (int vertex : order)
    {
        bool result = dfs(vertex);
        if (result)
        {
            size_of_matching++;
            timer++;
        }
    }
    return size_of_matching;
}

void Khun::swap_bipartites()
{
    std::swap(_n, _m);
    std::vector<std::vector<int>> newbgraph(_n + 1, std::vector<int>());
    std::vector<int> newmt(_m + 1, -1);
    for (size_t i = 1; i <= _m; i++)
    {
        for (int edge : _bgraph[i])
        {
            newbgraph[edge].push_back(static_cast<int>(i));
        }
    }
    for (size_t i = 0; i <= _n; i++)
    {
        if (_mt[i] != -1)
        {
            newmt[_mt[i]] = static_cast<int>(i);
        }
    }
    _bgraph = newbgraph;
    _mt = newmt;
}

std::vector<std::vector<int>> Khun::decompose()
{
    std::vector<std::vector<int>> decomposed_bgpraph(_n, std::vector<int>());
    for (size_t i = 1; i < _n; i++)
    {
        if (_mt[i] != -1)
        {
            decomposed_bgpraph[_mt[i]].push_back(static_cast<int>(i));
        }
    }
    return decomposed_bgpraph;
}
