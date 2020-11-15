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
//FROM-FILE:problem-solving-paradigms/binary-search/monotonically-decreasing.cpp
template<class F>
ll binsearch (ll l, ll r, F test) { // 111[1]00
    r++;
    while (r - l > 1) {
        ll m = l + r >> 1;
        if (test(m)) l = m;
        else r = m;
    }
    return test(l) ? l : l - 1;  // []000000
}
//END-LIB:

// dp[n][k] := number of subsets with
// exactly k elements from a set with n elements
// time complexity: O(N^2)
// no divisions are necessary

const int N = 1e3;

ll dp[N + 1][N + 1];

void build () {
    for (int n = 0; n <= N; n++) {
        dp[n][0] = dp[n][n] = 1;
        for (int k = 1; k < n; k++) {
            dp[n][k] = dp[n - 1][k] + dp[n - 1][k - 1];
        }
    }
}

int n, m;
vector<ll> p;

ll solve (int i, int k, ll les) {
    if (i == n || k == 0) {
        return 0;
    } else if (dp[n - i - 1][k] <= les) {
        return p[i] + solve(i + 1, k - 1, les - dp[n - i - 1][k]);
    } else {
        return solve(i + 1, k, les);
    }
}

int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    build();
    cin >> n >> m;
    p.resize(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }
    sort(p.rbegin(), p.rend());
    ll inf, sup;
    cin >> inf >> sup;

    ll r = binsearch(0, dp[n][m] - 1, [&] (ll k) {
        return solve(0, m, k) <= sup;
    });

    ll l = binsearch(0, dp[n][m] - 1, [&] (ll k) {
        return solve(0, m, k) < inf;
    });

    cout << r - l << '\n';
}
