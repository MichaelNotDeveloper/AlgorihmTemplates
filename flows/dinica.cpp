#include "dinica.h"

edge::edge(size_t x, size_t y) : x(x), y(y) {}
edge::edge(size_t x, size_t y, int cap) : x(x), y(y), cap(cap) {}
edge::edge(size_t x, size_t y, int cap, int cost) : x(x), y(y), cap(cap), cost(cost) {}

Flow::Flow()
{
}

Flow::Flow(size_t source, size_t drain, size_t n) : _source(source), _drain(drain), _n(n)
{
    _graph.assign(_n + 1, std::vector<size_t>());
}

void Flow::SetDrain(size_t drain)
{
    _drain = drain;
}
void Flow::SetSource(size_t source)
{
    _source = source;
}

void Flow::SetN(size_t n)
{
    _n = n;
    _graph.assign(_n + 1, std::vector<size_t>());
}

void Flow::SetZoom(int zoom)
{
    _zoom = zoom;
}

int Flow::GetZoom()
{
    return _zoom;
}

void Flow::addDirectedEdge(size_t x, size_t y, int cap = 1, int cost = 0)
{
    _graph[x].push_back(_edges.size());
    _edges.push_back(edge(x, y, cap, cost));
    _graph[y].push_back(_edges.size());
    _edges.push_back(edge(y, x, 0, cost));
}

void Flow::addUndirectedEdge(size_t x, size_t y, int cap = 1, int cost = 0)
{
    _graph[x].push_back(_edges.size());
    _edges.push_back(edge(x, y, cap, cost));
    _graph[y].push_back(_edges.size());
    _edges.push_back(edge(y, x, cap, cost));
}

void Flow::pushflow(size_t index_of_edge, int flow)
{
    _edges[index_of_edge].flow += flow;
    _edges[index_of_edge ^ 1].flow -= flow;
}

bool Flow::constructLevelGraph()
{
    _dist.assign(_n + 1, -1);
    std::queue<size_t> q;
    q.push(_source);
    _dist[_source] = 0;
    while (!q.empty())
    {

        size_t index = q.front();
        q.pop();
        for (size_t u : _graph[index])
        {

            if (_dist[_edges[u].y] == -1 && _edges[u].cap - _edges[u].flow >= _zoom)
            {
                _dist[_edges[u].y] = _dist[index] + 1;
                q.push(_edges[u].y);
            }
        }
    }
    return _dist[_drain] != -1;
}

int64_t Flow::constructBlockingFlow(bool use_dist = true)
{
    std::vector<size_t> ptr(_n + 1, 0);
    std::function<int(size_t, int)> dfs = ([&](size_t index, int maxflow = __INT_MAX__) -> int
                                           {
        if (index == this->_drain)
        {
            return maxflow;
        }
        for (; ptr[index] < _graph[index].size(); ptr[index]++)
        {
            edge line = _edges[_graph[index][ptr[index]]];
            if ((!use_dist || _dist[line.x] + 1 == _dist[line.y]) && line.cap - line.flow >= _zoom)
            {
                int flow = dfs(line.y, std::min(maxflow, line.cap - line.flow));
                if (flow)
                {
                    _edges.pop_back();
                    this->pushflow(_graph[index][ptr[index]], flow);
                    return flow;
                }
            }
        }
        return 0; });

    int64_t blocking_flow = 0;
    int flow = 0;
    do
    {
        flow = dfs(_source, __INT_MAX__);
        blocking_flow += flow;
    } while (flow);
    return blocking_flow;
}

Dinica::Dinica(Flow flow) : flow(flow) {}

int64_t Dinica::constructMaxFlow()
{
    int64_t maxflow = 0;
    while (flow.constructLevelGraph())
    {
        maxflow += flow.constructBlockingFlow();
    }
    return maxflow;
}

int64_t Dinica::constructMaxFlowWithZomming()
{
    int64_t maxflow = 0;
    flow.SetZoom(1 << 30);
    while (flow.GetZoom())
    {
        while (flow.constructLevelGraph())
        {
            maxflow += flow.constructBlockingFlow();
        }
        flow.SetZoom(flow.GetZoom() >> 1);
    }
    return maxflow;
}