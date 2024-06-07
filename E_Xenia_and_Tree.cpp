/*
Xenia and Tree
https://codeforces.com/problemset/problem/342/E
Prerequisites: graphs, dfs, centroid decomposition
*/

#include <bits/stdc++.h>

using namespace std;

const int LG = 18;
const int N = 100010;
const int INF = 1e8 + 123;

bool bad[N];
vector<int> g[N];
int n, m, sub[N], h[N], cenPar[N], closestBlack[N], dist[LG][N];

/// compute subtree sizes
void trav(int u, int p = -1)
{
    sub[u] = 1;
    for (auto v : g[u])
        if (v != p and !bad[v])
        {
            trav(v, u);
            sub[u] += sub[v];
        }
}

/// find centroid
int centroid(int u, int bound, int p = -1)
{
    for (auto v : g[u])
        if (v != p and !bad[v] and sub[v] > bound)
        {
            return centroid(v, bound, u);
        }
    return u;
}

/// store information from centroid
void go(int root, int layer, int u, int p = -1, int far = 0)
{
    dist[layer][u] = far;
    if (u == 1)
        closestBlack[root] = far;
    for (auto v : g[u])
        if (v != p and !bad[v])
            go(root, layer, v, u, far + 1);
}

/// centroid decomposition routine
int decompose(int u = 1, int depth = 0)
{
    trav(u);
    int root = centroid(u, sub[u] / 2);

    closestBlack[root] = INF, h[root] = depth;
    go(root, depth, root);

    /// remove centroid and proceed recursively
    bad[root] = true;
    for (auto v : g[root])
        if (!bad[v])
        {
            cenPar[decompose(v, depth + 1)] = root;
        }
    return root;
}

void update(int u)
{
    for (int cen = u; cen != -1; cen = cenPar[cen])
    {
        closestBlack[cen] = min(closestBlack[cen], dist[h[cen]][u]);
    }
}

int query(int u)
{
    int ans = INF;
    for (int cen = u; cen != -1; cen = cenPar[cen])
    {
        ans = min(ans, dist[h[cen]][u] + closestBlack[cen]);
    }
    return ans;
}

int main()
{
    cin >> n >> m;
    for (int i = 1, u, v; i < n; ++i)
    {
        scanf("%d %d", &u, &v);
        g[u].emplace_back(v), g[v].emplace_back(u);
    }
    cenPar[decompose()] = -1;
    while (m--)
    {
        int type, u;
        scanf("%d %d", &type, &u);
        if (type == 1)
            update(u);
        else
            printf("%d\n", query(u));
    }
    return 0;
}
