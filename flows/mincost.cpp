#include "mincost.h"

int MinCost::find_mincost_basic()
{
    std::unordered_set<int> deletion;
    do
    {
        constructMaxFlowWithZomming();
        std::vector<int> dist(flow.size() + 1, 1e9);
        std::vector<int> from(flow.size() + 1, -1);
        dist[flow.source()] = 0;
        std::vector<edge> edges = flow.GetEdges();

        for (size_t i = 0; i < flow.size(); i++)
        {
            for (size_t i = 0; i < edges.size(); i++)
            {
                if (edges[i].cap - edges[i].flow and dist[edges[i].x] + (edges[i].cap - edges[i].flow) * edges[i].cost < dist[edges[i].y])
                {
                    dist[edges[i].y] = dist[edges[i].x] + (edges[i].cap - edges[i].flow) * edges[i].cost;
                    from[edges[i].y] = static_cast<int>(i);
                }
            }
        }
        int deletion_maxflow = 1e9;
        deletion.clear();
        int back_prop = -1;

        for (edge u : edges)
        {
            if (u.cap > u.flow and dist[u.x] + (u.cap - u.flow) * u.cost < dist[u.y])
            {
                back_prop = static_cast<int>(u.y);
                break;
            }
        }

        if (back_prop == -1)
        {
            break;
        }

        std::unordered_set<int> been;

        do
        {
            been.insert(back_prop);
            back_prop = static_cast<int>(edges[from[back_prop]].x);
        } while (!been.count(back_prop));

        int start = back_prop;

        do
        {
            deletion.insert(from[back_prop]);
            back_prop = static_cast<int>(edges[from[back_prop]].x);
        } while (start != back_prop);

        for (size_t i = 0; i < edges.size(); i++)
        {
            if (deletion.count(static_cast<int>(i)))
            {
                deletion_maxflow = min(deletion_maxflow, edges[i].cap - edges[i].flow);
            }
        }
        for (size_t i = 0; i < edges.size(); i++)
        {
            if (deletion.count(static_cast<int>(i)))
            {
                flow.pushflow(i, deletion_maxflow);
            }
        }

    } while (true);

    int flowcost = 0;
    for (auto u : flow.GetEdges())
    {
        flowcost += u.cost * u.flow;
    }

    return -flowcost / 2;
}