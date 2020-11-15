#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define fi first
#define se second
#define ll long long
#define all(v) (v).begin(), (v).end()
using vi = vector<int>;
using ii = pair<int, int>;
using ld = double;

//BEGIN-LIB:
//FROM-FILE:data-structures/binary-indexed-tree/binary-indexed-tree.cpp
template<class T>
struct BIT {
    int n;
    vector<T> bit;
    BIT (int n) : n(n), bit(n + 1) {}
    int lso (int i) { return i & -i; }
    void update (int i, T x) {
        for (int j = i; j <= n; j += lso(j)) bit[j] += x;
    }
    T prefix (int i) {
        T sum = 0;
        for (int j = i; j > 0; j -= lso(j)) sum += bit[j];
        return sum;
    }
    T query (int l, int r) {
        return prefix(r) - prefix(l - 1);
    }
};
//END-LIB:

const int N = 1e6;

int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n, l, r;
    cin >> n >> l >> r;
    int k = r - l + 1;
    ld p = 1;
    p /= k;
    ld ans;
    if (l > 0) {
        BIT<ld> bit(2*N + 10);
        for (int i = n - 1; i >= 0; i--) {
            ld aux = p*(bit.query(i + l + 1, i + r + 1) + k);
            bit.update(i + 1, aux);
        }
        ans = bit.prefix(1);
    } else {
        BIT<ld> bit(2*N + 10);
        for (int i = n - 1; i >= 0; i--) {
            ld x = 1 + p*bit.query(i + l + 2, i + r + 1);
            ld y = 1;
            y /= (1 - p);
            bit.update(i + 1, y*x);
        }
        ans = bit.prefix(1);
    }
    cout << setprecision(12) << fixed;
    cout << ans << '\n';
}
