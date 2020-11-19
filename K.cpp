#include <bits/stdc++.h>
using namespace std;
    
#define endl '\n'
#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define ll long long
#define all(v) (v).begin(), (v).end()
using vi = vector<int>;
using ii = pair<int, int>;
using ld = long double;

using VR = valarray<bool>;
    
struct MX : public vector<VR> {
    int m, n;
    MX (int m, int n) : m(m), n(n), vector<VR>(m, VR(n)) {}
};
    
// identity matrix of order n, scaled by k
MX identity (int n, bool k = 1) {
    MX a(n, n);
    for (int i = 0; i < n; i++) a[i][i] = k;
    return a;
}

// a: matrix with M rows and N columns
// transform a to the reduced-row echelon form,
// applying the same elementary operations to b
// return: columns of pivots of lines [0, rank)
// time complexity: O(M*N*N)
template<class T>
vi mirror (MX& a, T& b) {
    vi p;
    int r = 0;
    for (int j = 0; j < a.n; j++) {
        int k = r;
        while (k < a.m && a[k][j] == 0) k++;
        if (k == a.m) continue;
        swap(a[k], a[r]);
        swap(b[k], b[r]);
        for (int i = 0; i < a.m; i++) {
            if (i == r || a[i][j] == 0) continue;
            a[i] -= a[r];
            b[i] -= b[r];
        }
        p.pb(j);
        r++;
    }
    return p;
}

// finds one valid solution, even when there are infinite ones
// if the system is inconsistent, throws an exception
bool hasSolution (MX& a, VR& b) {
    vi p = mirror(a, b);
    int r = p.size();
    for (int i = r; i < a.m; i++) {
        if (b[i]) return false;
    }
    return true;
}
    
const int N = 100;
    
vi adj[N];
    
int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    while (m--) {
        int u, v;
        cin >> u >> v;
        --u, --v;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    MX a(n, n);
    VR b(n);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u])  a[u][v] = 1;
        if (adj[u].size() & 1) {
            a[u][u] = 1;
        } else {
            b[u] = 1;
        }
    }
    cout << (hasSolution(a, b) ? 'Y' : 'N') << '\n';
}
