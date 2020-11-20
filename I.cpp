#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define eb emplace_back
#define fi first
#define se second
#define ll long long
#define all(v) (v).begin(), (v).end()
using vi = vector<int>;
using ii = pair<int, int>;
using ld = long double;

const int MOD = 1e9 + 7;

struct MB {
    int x;
    // implicit conversion in assignment statements
    MB (int x = 0) : x(x < 0 ? x + MOD : x) {}
    MB operator+ (const MB& o) const {
        int y = x + o.x;
        return y >= MOD ? y - MOD : y;
    }
    MB operator- (const MB& o) const {
        int y = x + MOD - o.x;
        return y >= MOD ? y - MOD : y;
    }
    MB operator* (const MB& o) const { return (ll) x*o.x % MOD; }
    void operator+= (const MB& o) { *this = *this + o; }
    void operator-= (const MB& o) { *this = *this - o; }
    void operator*= (const MB& o) { *this = *this*o; }
    operator int() { return x; }  // direct comparisons to int values
    friend ostream& operator<< (ostream& os, MB& o) { return os << o.x; }
    friend istream& operator>> (istream& is, MB& o) { return is >> o.x; }
};

template<class T>
T fastExp (T a, ll n) {
    T ans(1);
    while (n) {
        if (n & 1) ans = ans * a;
        a = a * a;
        n >>= 1;
    }
    return ans;
}

// assumes MOD is prime

MB operator/ (const MB& lhs, const MB& rhs) {
    return lhs*fastExp(rhs, MOD - 2);
}

void operator/= (MB& cur, const MB& o) { cur = cur/o; }

const int N = 1e5;

vi adj[N];

MB dp[N][2], aux[N][2];

void solve (int u) {
    if (adj[u].empty()) {
        dp[u][0] = dp[u][1] = 1;
    } else {
        for (int v : adj[u]) solve(v);
        for (int b = 0; b < 2; b++) {
            aux[0][b] = 1;
            int i = 0;
            for (int v : adj[u]) {
                aux[i + 1][b] = aux[i][b]*dp[v][0];
                i++;
            }
            reverse(all(adj[u]));
        }
        int m = adj[u].size();
        int l = 0, r = m - 1;
        for (int i = 0; r >= 0; i++, l++, r--) {
            int v = adj[u][i];
            MB temp = aux[l][0]*dp[v][1]*aux[r][1];
            dp[u][0] += temp;
            dp[u][1] += temp;
        }
        dp[u][0] += aux[m][0];
    }
}

int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for (int v = 1; v < n; v++) {
        int u;
        cin >> u;
        adj[--u].pb(v);
    }
    solve(0);
    cout << dp[0][0] << '\n';
}
