#include <algorithm>
#include <utility>
#include <vector>
#include <queue>
#include <cstdint>
#include <functional>

struct edge
{
    size_t x, y;
    int flow = 0, cap = 1, cost = 0;
    int info;
    edge(size_t x, size_t y);
    edge(size_t x, size_t y, int cap);
    edge(size_t x, size_t y, int cap, int cost);
    edge(size_t x, size_t y, int cap, int cost, int info);
    void setinfo(int custom_info);
};

class Flow
{
public:
    Flow();
    Flow(size_t source, size_t draint, size_t n);
    void SetSource(size_t source);
    void SetDrain(size_t drain);
    void SetN(size_t n);
    void SetZoom(int zoom);
    int GetZoom();
    void addEdge(edge e, bool undir);
    void addDirectedEdge(size_t x, size_t y, int cap, int cost, int info);
    void addUndirectedEdge(size_t x, size_t y, int cap, int cost, int info);
    bool constructLevelGraph();
    int getDistance();
    int64_t constructBlockingFlow(bool use_dist);
    std::vector<edge> GetEdges();
private:
    void pushflow(size_t index_of_edge, int flow);
    size_t _source = 0;
    size_t _drain = 0;
    size_t _n = 0;
    const int _maxflow = __INT_MAX__;
    int _zoom = 0;
    std::vector<int> _dist;
    std::vector<edge> _edges;
    std::vector<std::vector<size_t>> _graph;
};

class Dinica
{
public:
    Flow flow;
    Dinica(Flow flow);
    int64_t constructMaxFlow();
    int64_t constructMaxFlowWithZomming();
};
