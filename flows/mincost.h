#include "dinica.cpp"
#include <utility>
#include <unordered_set>

class MinCost : public Dinica
{
public:
    MinCost(Flow flow) : Dinica(flow) {}
    int find_mincost_basic();
};
