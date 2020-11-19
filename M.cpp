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
    
#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;
    
/* An Interval Tree is a dynamic set that allows online
* insertion and removal of line intervals.
* Given a queried interval [ql, qr],
* if there is any interval [l, r] in the set such that l <= qr and ql <= r,
* then this intersecting interval can be found in logarithmic time
* (or it can be determined that there is no such interval)
* T: integral type for intervals endpoints
* (careless instantiation with floating-point types
* might result in precision errors)
*/
template<class T>
struct IntervalTree {
    // user defined node updator class for policy based tree
    template<class CNI, class NI, class _, class __>
    struct Update {
        typedef T metadata_type;
        virtual CNI node_begin() const = 0;
        virtual CNI node_end() const = 0;
        void operator() (NI v, CNI nil) {
            T x = (*v)->se;
            auto lc = v.get_l_child();
            if (lc != nil) x = max(x, lc.get_metadata());
            auto rc = v.get_r_child();
            if (rc != nil) x = max(x, rc.get_metadata());
            const_cast<T&>(v.get_metadata()) = x;
        }
        /* Finds any interval intersecting with [ql, qr],
        * returning a Const_Node_Iterator pointing to it,
        * or node_end(), if there are no intersections
        * Complexity: logarithmic on the tree size
        */
        CNI search (T ql, T qr) {
            CNI v = node_begin(), nil = node_end();
            while (v != nil) {
                T l = (*v)->fi.fi, r = (*v)->se;
                if (qr < l || r < ql) {
                    auto lc = v.get_l_child();
                    if (lc != nil && lc.get_metadata() >= ql) {
                        v = lc;
                    } else {
                        v = v.get_r_child();
                    }
                } else {
                    break;
                }
            }
            return v;
        }
    };
    
    tree<pair<T, int>, T, less<pair<T, int>>, rb_tree_tag, Update> t;
    
    /* Intervals are distinguished by their integer index i,
    * so that there can be multiple intervals sharing the same endpoints */
    void insert (T l, T r, int i) { t.insert({{l, i}, r}); }
    void erase (T l, T r, int i) { t.erase({l, i}); }
    
    bool hasOverlaps (T ql, T qr) {
        return t.search(ql, qr) != t.node_end();
    }
    tuple<T, T, int> findAnyOverlap (T ql, T qr) {
        auto v = t.search(ql, qr);
        if (v == t.node_end()) {
            throw -1;
        } else {
            T l, r;
            int i;
            tie(l, i) = (*v)->fi;
            r = (*v)->se;
            return {l, r, i};
        }
    }
};
    
int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    IntervalTree<ll> tr;
    for (int i = 1; i <= n; i++) {
        int x, y;
        cin >> x >> y;
        ll l = y << 1, r = l;
        l -= x, r += x;
        tr.insert(l, r, i);
    }
    MB ans;
    while (m--) {
        MB a, b;
        cin >> a >> b;
    
        int x = ans + a;
        x++;
        int y = ans + b;
        y <<= 1;
        ll ql = y, qr = ql;
        ql -= x, qr += x;
    
        vi v;
        vector<tuple<ll, ll, int>> bye;
        while (true) {
            try {
                auto [l, r, i] = tr.findAnyOverlap(ql, qr);
                v.pb(i);
                tr.erase(l, r, i);
                bye.eb(l, r, i);
            } catch (...) {
                break;
            }
        }
        for (auto [l, r, i] : bye) {
            tr.insert(l, r, i);
        }
        sort(all(v));
        v.erase(unique(all(v)), v.end());
    
        ans = 0;
        MB p = 1;
        for (MB i : v) {
            ans += p*i;
            p *= 5782344;
        }
    
        cout << ans << '\n';
    }
}