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
//END-LIB:

const int N = 1e7;

char s[N];

int main () {
    scanf("%s", s);
    string str(s);
    int games[2] = {};
    int score[2] = {};
    int winner = -1;
    bool saq = 0;
    auto win = [&] (bool b) {
        games[b]++;
        score[0] = score[1] = 0;
        if (games[b] == 2) {
            winner = b;
        }
        saq = b;
    };
    for (char c : str) {
        if (c == 'Q') {
            if (winner == 0) {
                printf("%d (winner) - %d\n", games[0], games[1]);
            } else if (winner == 1) {
                printf("%d - %d (winner)\n", games[0], games[1]);
            } else if (saq) {
                printf("%d (%d) - %d (%d*)\n", games[0], score[0], games[1], score[1]);
            } else {
                printf("%d (%d*) - %d (%d)\n", games[0], score[0], games[1], score[1]);
            }
        } else if (winner == -1) {
            if (c == 'S') {
                score[saq]++;
            } else {
                score[!saq]++;
                saq = !saq;
            }
            if (score[0] >= 10) {
                win(0);
            } else if (score[1] >= 10) {
                win(1);
            } else if (score[0] >= 5 && score[0] - score[1]>= 2) {
                win(0);
            } else if (score[1] >= 5 && score[1] - score[0] >= 2) {
                win(1);
            }
        }
    }
}