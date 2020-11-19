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
    
int main () {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    int cur = 100, ans = cur;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        ans = max(ans, cur += x);
    }
    cout << ans << '\n';
}
