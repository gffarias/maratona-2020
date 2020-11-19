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
    
template<class S>
vi prefixFunction (const S &s) {
    int n = s.size();
    vi pi(n);
    for (int i = 0; i + 1 < n; i++) {
        int j = pi[i];
        while (j > 0 && s[i + 1] != s[j]) {
            j = pi[j - 1];
        }
        pi[i + 1] = (s[i + 1] == s[j] ? j + 1 : 0);
    }
    return pi;
}
    
template<class S>
vi longestPrefixMatching (const S& s, const S& t) {
    int n = s.size(), m = t.size();
    vi pi(prefixFunction(s)), len(m);
    int prev = 0;
    for (int i = 0; i < m; i++) {
        int j = (prev == n ? pi[n - 1] : prev);
        while (j > 0 && t[i] != s[j]) {
            j = pi[j - 1];
        }
        int cur = (t[i] == s[j] ? j + 1 : 0);
        prev = len[i] = cur;
    }
    return len;   // len[i] = length of the longest prefix of s ENDING at t[i]
}
    
template<class S>
vector<bool> patternMatching (const S& s, const S& t) {
    int n = s.size(), m = t.size();
    vi len(longestPrefixMatching(s, t));
    vector<bool> match(m);
    for (int i = 0; i < m; i++) match[i] = (len[i] == n);
    return match; // match at ending positions: match[i] = (t.substr(i - n + 1, n) == s)
}
    
template<class S>
int cyclicShifts (const S& s, const S& t) {
    int match = 0;
    int n = s.size();
    if (n == t.size()) {
        S cat(t);
        cat.insert(cat.end(), t.begin(), t.end());
        vector<bool> end(patternMatching(s, cat));
        for (int i = n; i < cat.size(); i++) {
            match += end[i];
        }
    }
    return match;
}
    
template<class T>
struct Matrix {
    int n;     // order (number of rows and columns) of square matrix
    vector<vector<T>> a;
    // initializes this matrix as kI := identity matrix scaled by k
    Matrix (int n, int k) : n(n), a(n, vector<T>(n)) {
        for (int i = 0; i < n; i++) a[i][i] = k;
    }
    // matrix-matrix multiplication
    Matrix operator* (const Matrix& o) {
        Matrix p(n, 0);
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) p.a[i][j] += a[i][k]*o.a[k][j];
        return p;
    }
    // matrix-vector multiplication
    vector<T> operator* (const vector<T>& v) {
        vector<T> u(n);
        for (int i = 0; i < n; i++) for (int k = 0; k < n; k++)
            u[i] += a[i][k]*v[k];
        return u;
    }
    // exponentiates this matrix to the power of e
    // complexity: O((n^3) lg e)
    // n: order of this matrix
    // e: exponent
    Matrix to (ll e) {
        Matrix p(n, 1), b = *this;
        while (e > 0) {
            if (e & 1) p = p * b;
            b = b * b;
            e >>= 1;
        }
        return p;
    }
    T& operator() (int i, int j) { return a[i][j]; }  // manually inputing
};
    
int MOD;
    
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
    
bool operator== (const MB& a, const MB& b) { return a.x == b.x; }
bool operator!= (const MB& a, const MB& b) { return a.x != b.x; }
    
int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n, k;
    cin >> n >> MOD >> k;
    auto prev = [&] (int i) { return i == 0 ? n - 1 : i - 1; };
    vector<MB> a, b;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        a.eb(x % MOD);
    }
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        b.eb(x % MOD);
    }
    vector<MB> s(n), t(n);
    for (int i = 0; i < n; i++) s[i] = a[prev(i)] - a[i];
    for (int i = 0; i < n; i++) t[i] = b[i] - b[prev(i)];
    int m = cyclicShifts(s, t);
    bool ok = true;
    for (int i = 0; i < n; i++) {
        ok &= a[i] + b[i] == a[prev(i)] + b[prev(i)];
    }
    MOD = 1e9 + 7;
    vector<MB> v;
    if (ok) {
        v = {1, 0};
    } else {
        v = {0, 1};
    }
    Matrix<MB> p(2, 2);
    p(0, 0) = m - 1;
    p(0, 1) = m;
    p(1, 0) = n - m;
    p(1, 1) = n - m - 1;
    cout << (p.to(k)*v)[0] << '\n';
}