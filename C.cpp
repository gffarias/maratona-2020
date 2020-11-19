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
    
const int MOD = 998244353;
    
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
    bool operator== (const MB& o) const { return x == o.x; }
    bool operator< (const MB& o) const { return x < o.x; }
};
    
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
uniform_int_distribution<int> base('z' + 1, 1e8);
const unsigned ll base1 = (base(rng) << 1) + 1;
const MB base2 = (base(rng) << 1) + 1;
    
struct RabinKarp {
    vector<unsigned ll> p1, pref1;
    vector<MB> p2, pref2;
    RabinKarp (const string& s, int n) :
        p1(n + 1, 1), pref1(n + 1), p2(n + 1, 1), pref2(n + 1)
    {
        for (int k = 0; k < n; k++) {
            p1[k + 1] = p1[k]*base1;
            p2[k + 1] = p2[k]*base2;
        }
        for (int k = 0; k < n; k++) {
            pref1[k + 1] = pref1[k]*base1 + s[k];
            pref2[k + 1] = pref2[k]*base2 + MB(s[k]);
        }
    }
    unsigned ll hash1 (int i, int len) {
        return pref1[i + len] - pref1[i]*p1[len];
    }
    MB hash2 (int i, int len) {
        return pref2[i + len] - pref2[i]*p2[len];
    }
    bool equal (int i, int j, int len) {
        return hash1(i, len) == hash1(j, len)
        && hash2(i, len) == hash2(j, len);
    }
};
    
const int MS = 2e6 + 5, SIGMA = 'z' - 'a' + 1;
    
int getID (char c) { return c - 'a'; }
    
namespace pt {
    int to[MS + 1][SIGMA], term[MS + 1], z;
    void init () {
        memset(to, -1, sizeof to);
        memset(term, -1, sizeof term);
        z = 1;
    }
    void insert (const string& s, int k) {
        int cur = 0;
        for (char c : s) {
            int lb = getID(c);
            if (to[cur][lb] == -1) {
                to[cur][lb] = z++;
            }
            cur = to[cur][lb];
        }
        term[cur] = k;
    }
    vector<ii> ans;
    vi dfs (int u) {
        vi sub;
        if (term[u] != -1) sub.pb(u);
        for (int lb = 0; lb < SIGMA; lb++) {
            if (to[u][lb] != -1) {
                auto g = dfs(to[u][lb]);
                for (int v : g) {
                    if (term[u] != -1) {
                        ans.eb(term[u], term[v]);
                    }
                    sub.pb(v);
                }
            }
        }
        return sub;
    }
};
    
namespace st {
    int to[MS + 1][SIGMA], term[MS + 1], z;
    void init () {
        memset(to, -1, sizeof to);
        memset(term, -1, sizeof term);
        z = 1;
    }
    void insert (const string& s, int k) {
        int cur = 0;
        for (char c : s) {
            int lb = getID(c);
            if (to[cur][lb] == -1) {
                to[cur][lb] = z++;
            }
            cur = to[cur][lb];
        }
        term[cur] = k;
    }
    vector<ii> ans;
    vi dfs (int u) {
        vi sub;
        if (term[u] != -1) sub.pb(u);
        for (int lb = 0; lb < SIGMA; lb++) {
            if (to[u][lb] != -1) {
                auto g = dfs(to[u][lb]);
                for (int v : g) {
                    if (term[u] != -1) {
                        ans.eb(term[u], term[v]);
                    }
                    sub.pb(v);
                }
            }
        }
        return sub;
    }
};
    
int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int pn, sn;
    cin >> pn >> sn;
    pt::init();
    vector<RabinKarp> ph;
    vi pl(pn);
    for (int i = 0; i < pn; i++) {
        string s;
        cin >> s;
        pl[i] = s.length();
        ph.eb(s, pl[i]);
        pt::insert(s, i);
    }
    st::init();
    vector<RabinKarp> sh;
    vi sl(sn);
    for (int i = 0; i < sn; i++) {
        string s;
        cin >> s;
        sl[i] = s.length();
        sh.eb(s, sl[i]);
        reverse(all(s));
        st::insert(s, i);
    }
    pt::dfs(0);
    st::dfs(0);
    vector<ii> pa = pt::ans, sa = st::ans;
    vector<bool> pb(pn, true), sb(sn, true);
    vector<pair<pair<unsigned ll, MB>, ii>> px, sx;
    vector<pair<unsigned ll, MB>> py, sy;
    for (auto it : pa) {
        auto [i, j] = it;
        int l = pl[j] - pl[i], k = pl[j] - l;
        pair<unsigned ll, MB> hsh = {ph[j].hash1(k, l), ph[j].hash2(k, l)};
        px.eb(hsh, it);
        py.pb(hsh);
    }
    for (auto it : sa) {
        auto [i, j] = it;
        int l = sl[j] - sl[i], k = 0;
        pair<unsigned ll, MB> hsh = {sh[j].hash1(k, l), sh[j].hash2(k, l)};
        sx.eb(hsh, it);
        sy.pb(hsh);
    }
    sort(all(py));
    py.erase(unique(all(py)), py.end());
    sort(all(sy));
    sy.erase(unique(all(sy)), sy.end());
    vector<pair<unsigned ll, MB>> both(min(py.size(), sy.size()));
    both.erase(set_intersection(all(py), all(sy), both.begin()), both.end());
    for (auto [hsh, it] : px) {
        if (binary_search(all(both), hsh)) {
            auto [i, j] = it;
            pb[i] = pb[j] = false;
        }
    }
    for (auto [hsh, it] : sx) {
        if (binary_search(all(both), hsh)) {
            auto [i, j] = it;
            sb[i] = sb[j] = false;
        }
    }
    int pc = accumulate(all(pb), 0), sc = accumulate(all(sb), 0);
    cout << pc << ' ' << sc << '\n';
}