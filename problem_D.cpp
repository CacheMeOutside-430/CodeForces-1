#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int multTestQ;
    cin >> multTestQ;

    while (multTestQ--) {
        int n;
        cin >> n;

        vector<int> a(n + 1), b(n + 1);
        vector<vector<int>> posA(n + 2), posB(n + 2);

        for (int i = 1; i <= n; i++) {
            cin >> a[i];
            posA[a[i]].push_back(i);
        }

        for (int i = 1; i <= n; i++) {
            cin >> b[i];
            posB[b[i]].push_back(i);
        }

        auto nextPos = [&](const vector<int>& positions, int after) {
            auto it = upper_bound(positions.begin(), positions.end(), after);
            return it == positions.end() ? n + 1 : *it;
        };

        vector<int> memo(n + 1, -1);

        auto firstBadAfter = [&](int start) {
            vector<int> path;
            int at = start;
            int res;

            while (true) {
                if (memo[at] != -1) {
                    res = memo[at];
                    break;
                }

                path.push_back(at);
                int need = a[at] + 1;

                if (need > n) {
                    res = n + 1;
                    break;
                }

                int pa = nextPos(posA[need], at);
                int pb = nextPos(posB[need], at);

                if (pa != pb) {
                    res = min(pa, pb);
                    break;
                }

                if (pa == n + 1) {
                    res = n + 1;
                    break;
                }

                at = pa;
            }

            for (int pos : path) memo[pos] = res;
            return res;
        };

        long long ans = 0;

        for (int l = 1; l <= n; l++) {
            int pa = nextPos(posA[1], l - 1);
            int pb = nextPos(posB[1], l - 1);

            int bad;
            if (pa != pb) {
                bad = min(pa, pb);
            } else if (pa == n + 1) {
                bad = n + 1;
            } else {
                bad = firstBadAfter(pa);
            }

            ans += bad - l;
        }

        cout << ans << '\n';
    }

    return 0;
}
