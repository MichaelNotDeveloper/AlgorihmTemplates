#include <vector>
#include <random>
#include <stack>
#include <algorithm>
#include <functional>

class Khun
{
public:
    Khun(int n, int m);
    void add_edge(int x, int y);
    void add_matching_edge(int x, int y);
    void clear();
    int find_matching();
    void swap_bipartites();
    std::vector<std::vector<int>> decompose();
    std::vector<std::pair<int, int>> decompose_by_edges();
    void shuffle_edges();

private:
    int _n;
    int _m;
    std::vector<std::vector<int>> _bgraph;
    std::vector<int> _mt;
};
