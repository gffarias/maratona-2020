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
//FROM-FILE:computational-geometry/2D/integer/rectangle-intersection.cpp
bool solve (ii a, ii b) {
    if (b.fi < a.fi) swap(a, b);
    return b.fi <= a.se;
}
//END-LIB:

struct Line {
    bool ver;
    int len;
    int r, c;
    Line (int d, int l, int r, int c) : ver(d), len(l), r(r), c(c) {}
    bool intersect (Line o) {
        if (ver && o.ver) {
            return c == o.c && solve({r, r + len - 1}, {o.r, o.r + o.len - 1});
        } else if (!ver && !o.ver) {
            return r == o.r && solve({c, c + len - 1}, {o.c, o.c + o.len - 1});
        } else if (ver && !o.ver) {
            return o.c <= c && c <= o.c + o.len - 1 && r <= o.r && o.r <= r + len - 1;
        } else {
            return c <= o.c && o.c <= c + len - 1 && o.r <= r && r <= o.r + o.len - 1;
        }
    }
    pair<ii, ii> ext () {
        if (ver) {
            return {ii(r, c), ii(r + len - 1, c)};
        } else {
            return {ii(r, c), ii(r, c + len - 1)};
        }
    }
};

bool inside (ii p) {
    return p.fi >= 1 && p.fi <= 10 && p.se >= 1 && p.se <= 10;
}

int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<Line> in;
    for (int i = 0; i < n; i++) {
        int d, l, r, c;
        cin >> d >> l >> r >> c;
        in.emplace_back(d, l, r, c);
    }
    bool ok = true;
    for (int i = 0; i < n; i++) {
        auto [a, b] = in[i].ext();
        ok &= inside(a) && inside(b);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            ok &= !in[i].intersect(in[j]);
        }
    }
    cout << (ok ? 'Y' : 'N') << '\n';
}
