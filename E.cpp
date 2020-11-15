#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define fi first
#define se second
#define ll long long
#define all(v) (v).begin(), (v).end()
using vi = vector<int>;
using ii = pair<int, int>;
using ld = long double;

//BEGIN-LIB:
//FROM-FILE:graph/sparse/neighbour-list/representation/unweighted-graph.cpp
struct Graph {  // unweighted graph
    int first, last, n;
    vector<vi> adj;
    // b == 0: vertices indices in range [0, n)
    // b == 1: vertices indices in range [1, n]
    Graph (int n, bool b) :
        first(b), last(n + b), n(n), adj(last)
    {}
    void addEdge (int u, int v) {
        adj[u].pb(v);
        adj[v].pb(u);  // for undirected graph
    }
    void addArc (int u, int v) {
        adj[u].pb(v);  // for directed graph
    }
};
//FROM-FILE:graph/sparse/neighbour-list/trees/lowest-common-ancestor/binary-lifting.cpp
const int MAX_LG = 20;  // TODO adjust maximum lg(|V|) according to input size

struct BinaryLifting {  // offline tree path query
    struct TreePath {
        int anc;
        ll best;
        TreePath (int u = 0) : anc(u), best(0) {}
        TreePath operator+ (TreePath o) {
            TreePath ans;
            ans.anc = o.anc;
            return ans;
        }
        void operator+= (TreePath o) { *this = *this + o; }
    };
    vi lvl;
    vector<vector<TreePath>> up;
    BinaryLifting (const Graph& g) :
       lvl(g.last), up(MAX_LG + 1, vector<TreePath>(g.last))
    {  // g: weighted tree in neighbour list representation
        function<void(int, int)> dfs = [&] (int u, int par) {
            for (int v : g.adj[u]) {  // C++ 17
                if (v != par) {
                    lvl[v] = 1 + lvl[u];
                    up[0][v].anc = u;
                    for (int i = 0, a = u; i + 1 <= MAX_LG; a = up[i++][a].anc) {
                        up[i + 1][v] = up[i][v] + up[i][a];
                    }
                    dfs(v, u);
                }
            }
        };
        dfs(g.first, g.first);
    }
    TreePath lift (int u, int k) {
        TreePath ans(u);
        for (int i = 0; k; i++, k >>= 1) {
            if (k & 1) {
                ans += up[i][u];
                u = ans.anc;
            }
        }
        return ans;
    }
};
//FROM-FILE:graph/sparse/neighbour-list/trees/heavy-light-decomposition/unweighted-graph.cpp
struct HLD {
    int n;
    vi lvl, par, pre, post, top;
    // g: tree in unweighted-graph representation
    // vertices in g can either be 0-indexed, or 1-indexed
    // ith vertice in g will be mapped to position pre[i] of segment tree array
    // segment tree array will be 1-based, with size n
    HLD (Graph& g) :
        n(0), lvl(g.last), par(g.last), pre(g.last), post(g.last), top(g.last)
    {
        vi sz(g.last);
        function<void(int)> dfs[2];
        dfs[0] = [&] (int u) {
            n++;
            sz[u] = 1;
            for (int &v : g.adj[u]) {
                if (v != par[u]) {
                    lvl[v] = 1 + lvl[u];
                    par[v] = u;
                    dfs[0](v);
                    sz[u] += sz[v];
                    int& w = g.adj[u].front();
                    if (w == par[u] || sz[v] > sz[w]) swap(v, w);
                }
            }
        };
        dfs[0](par[g.first] = g.first);
        int t = 0;
        dfs[1] = [&] (int u) {
            pre[u] = ++t;
            for (int v : g.adj[u]) {
                if (v != par[u]) {
                    top[v] = v == g.adj[u].front() ? top[u] : v;
                    dfs[1](v);
                }
            }
            post[u] = t;
        };
        dfs[1](top[g.first] = g.first);
    }
    int mirror (int i) { return n - i + 1; }
    // lowest common ancestor of u and v
    int lca (int u, int v) {
        while (top[u] != top[v]) {
            if (lvl[top[u]] < lvl[top[v]]) swap(u, v);
            u = par[top[u]];
        }
        if (lvl[u] > lvl[v]) swap(u, v);
        return u;
    }
    // number of edges in the path from u to v
    int dist (int u, int v) {
        return lvl[u] + lvl[v] - (lvl[lca(u, v)] << 1);
    }
    // the range of the segment tree including all the nodes
    // in the subtree rooted at u
    ii subTreeRange (int u) {
        return {pre[u], post[u]};
    }
    // returns the ranges of the segment tree array covering
    // the vertices along the path from u to v
    // the pair {first, last} represents the range [l, r] (both inclusive)
    // always first <= last, so the ranges start from the upper vertex and
    // end at the lower vertex
    // assumes the binary operator for combining ranges of segment tree
    // is commutative (e.g. max/min weight, sum of weights), so the order
    // in which the information on ranges are combined won't respect
    // the actual sequence of vertices from u to v
    vector<ii> pathRanges (int u, int v) {
        vector<ii> ans;
        while (top[u] != top[v]) {
            if (lvl[top[u]] < lvl[top[v]]) swap(u, v);
            ans.emplace_back(pre[top[u]], pre[u]);
            u = par[top[u]];
        }
        if (lvl[u] > lvl[v]) swap(u, v);
        ans.emplace_back(pre[u], pre[v]);
        return ans;
    }
};
//FROM-FILE:data-structures/segment-tree/persistent/offline-order-statistics.cpp
struct Node {
    int sum;
    Node *lc, *rc;
    Node (int x) : sum(x) {}
    Node (Node* lc, Node *rc) : lc(lc), rc(rc), sum(lc->sum + rc->sum) {}
    Node* update (int l, int r, int i) {
        if (l == r) {
            return new Node(sum + 1);
        } else {
            int m = l + r >> 1;
            if (i <= m) {
                return new Node(lc->update(l, m, i), rc);
            } else {
                return new Node(lc, rc->update(m + 1, r, i));
            }
        }
    }
    int query (int l, int r, int ql, int qr) {
        if (l <= ql && r <= qr) {
            return sum;
        } else if (r < ql || l > qr) {
            return 0;
        } else {
            int m = l + r >> 1;
            return lc->query(l, m, ql, qr) + query(m + 1, r, ql, qr);
        }
    }
};

Node* build (int l, int r) {
    if (l == r) {
        return new Node(0);
    } else {
        int m = l + r >> 1;
        return new Node(build(l, m), build(m + 1, r));
    }
}

// successfully tested with problems:
// https://www.spoj.com/problems/MKTHNUM/
// https://www.spoj.com/problems/COT/
//FROM-FILE:problem-solving-paradigms/binary-search/monotonically-decreasing.cpp
template<class F>
int search (int l, int r, F test) { // 111[1]00
    r++;
    while (r - l > 1) {
        int m = l + r >> 1;
        if (test(m)) l = m;
        else r = m;
    }
    return test(l) ? l : l - 1;  // []000000
}
//END-LIB:

const int N = 1e5;

int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    vi age(n + 1);
    Graph g(n, 1);
    for (int u = 1; u <= n; u++) {
        int v;
        cin >> age[u] >> v;
        if (u == v) continue;
        g.addEdge(u, v);
    }
    BinaryLifting bl(g);
    HLD hld(g);
    vector<ii> qrs;
    for (int i = 0; i < m; i++) {
        int u, l, r;
        cin >> u >> l >> r;
        int k = search(0, bl.lvl[u], [&] (k) {
            return age[bl.lift(u, k).anc] <= r;
        });
        int a = bl.lift(u, k).anc;
        qrs.emplace_back(l, a);
    }
    sort(all(qrs));
    vector<Node*> tree(1, build(0, N));
    for (int i = 1; i <= m; i++) {
        tree[i] = tree[i]->update(0, N, hld.pre[qrs[i].se]);
    }
    for (int u = 1; u <= n; u++) {
        int ans = 0;
        for (auto [l, r] : hld.pathRanges(u, 1)) {
            int k = search(0, m - 1, [&] (int k) {
                return qrs[k].se <= ;
            });
            ans = tree[k].query(0, N, 0, qrs[k].fi);
        }
        cout << ans << ' ';
    }
}
