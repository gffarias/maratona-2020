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
    
int m, n;
    
bool inside (int i, int j) { return i >= 0 && i < m && j >= 0 && j < n; }
    
void add (string& s, char ch) {
    int len = s.length();
    string a(1, ch), b(len + 1, ch);
    merge(all(s), all(a), b.begin());
    s.swap(b);
}
    
int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    cin >> m >> n;
    vector<string> a;
    for (int i = 0; i < m; i++) {
        string s;
        cin >> s;
        a.pb(s);
    }
    int z;
    cin >> z;
    unordered_map<string, int> h;
    for (int i = 0; i < z; i++) {
        string s;
        cin >> s;
        sort(all(s));
        h[s] = 1 << i;
    }
    vector<vi> ans(m, vi(n));
    for (int i = 0; i < m; i++) {
        for (int l = 0; l < n; l++) {
            string s;
            for (int r = l; r < n; r++) {
                add(s, a[i][r]);
                if (h.count(s)) {
                    int mask = h[s];
                    for (int j = l; j <= r; j++) {
                        ans[i][j] |= mask;
                    }
                }
            }
        }
    }
    for (int j = 0; j < n; j++) {
        for (int l = 0; l < m; l++) {
            string s;
            for (int r = l; r < m; r++) {
                add(s, a[r][j]);
                if (h.count(s)) {
                    int mask = h[s];
                    for (int i = l; i <= r; i++) {
                        ans[i][j] |= mask;
                    }
                }
            }
        }
    }
    for (int i = 0; i < m; i++) {
        int si = i, sj = 0;
        do {
            string s;
            int ti = si, tj = sj;
            do {
                add(s, a[ti][tj]);
                if (h.count(s)) {
                    int mask = h[s];
                    int ci = si, cj = sj;
                    do {
                        ans[ci][cj] |= mask;
                        if (ci == ti && cj == tj) break;
                        ci++;
                        cj++;
                    } while (true);
                }
                ti++;
                tj++;
                if (!inside(ti, tj)) break;
            } while (true);
            si++;
            sj++;
            if (!inside(si, sj)) break;
        } while (true);
    }
    for (int j = 0; j < n; j++) {
        int si = 0, sj = j;
        do {
            string s;
            int ti = si, tj = sj;
            do {
                add(s, a[ti][tj]);
                if (h.count(s)) {
                    int mask = h[s];
                    int ci = si, cj = sj;
                    do {
                        ans[ci][cj] |= mask;
                        if (ci == ti && cj == tj) break;
                        ci++;
                        cj++;
                    } while (true);
                }
                ti++;
                tj++;
                if (!inside(ti, tj)) break;
            } while (true);
            si++;
            sj++;
            if (!inside(si, sj)) break;
        } while (true);
    }
    for (int i = 0; i < m; i++) {
        int si = i, sj = 0;
        do {
            string s;
            int ti = si, tj = sj;
            do {
                add(s, a[ti][tj]);
                if (h.count(s)) {
                    int mask = h[s];
                    int ci = si, cj = sj;
                    do {
                        ans[ci][cj] |= mask;
                        if (ci == ti && cj == tj) break;
                        ci--;
                        cj++;
                    } while (true);
                }
                ti--;
                tj++;
                if (!inside(ti, tj)) break;
            } while (true);
            si--;
            sj++;
            if (!inside(si, sj)) break;
        } while (true);
    }
    for (int j = 0; j < n; j++) {
        int si = m - 1, sj = j;
        do {
            string s;
            int ti = si, tj = sj;
            do {
                add(s, a[ti][tj]);
                if (h.count(s)) {
                    int mask = h[s];
                    int ci = si, cj = sj;
                    do {
                        ans[ci][cj] |= mask;
                        if (ci == ti && cj == tj) break;
                        ci--;
                        cj++;
                    } while (true);
                }
                ti--;
                tj++;
                if (!inside(ti, tj)) break;
            } while (true);
            si--;
            sj++;
            if (!inside(si, sj)) break;
        } while (true);
    }
    int cnt = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cnt += __builtin_popcount(ans[i][j]) > 1;
        }
    }
    cout << cnt << '\n';
}
