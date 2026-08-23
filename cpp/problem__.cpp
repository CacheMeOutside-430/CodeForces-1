#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int MAXN = 200005;
int n, k;
ll a[MAXN];
vector<int> adj[MAXN];
ll sub_sum[MAXN];
ll total;

static int dp_arr[MAXN];
static int bfs_order[MAXN];
static int bfs_par[MAXN];
static int bfs_sz;

void dfs_sum(int root) {
    static pair<int,int> dfs_stk[MAXN];
    static int order[MAXN];
    static int par2[MAXN];
    int dsz = 0, sz = 0;
    dfs_stk[dsz++] = {root, -1};
    while (dsz > 0) {
        auto [v, p] = dfs_stk[--dsz];
        order[sz++] = v;
        par2[v] = p;
        for (int u : adj[v]) if (u != p) dfs_stk[dsz++] = {u, v};
    }
    for (int i = sz-1; i >= 0; i--) {
        int v = order[i];
        sub_sum[v] = a[v];
        for (int u : adj[v]) if (u != par2[v]) sub_sum[v] += sub_sum[u];
    }
}

void bfs_build(int root) {
    bfs_sz = 0;
    bfs_order[bfs_sz++] = root;
    bfs_par[root] = -1;
    for (int i = 0; i < bfs_sz; i++) {
        int v = bfs_order[i];
        for (int u : adj[v]) {
            if (u == bfs_par[v]) continue;
            bfs_par[u] = v;
            bfs_order[bfs_sz++] = u;
        }
    }
}

bool check(ll mid) {
    for (int i = bfs_sz - 1; i >= 0; i--) {
        int v = bfs_order[i];

        if (sub_sum[v] < mid) { dp_arr[v] = -1; continue; }
        dp_arr[v] = 0;

        for (int c : adj[v]) {
            if (c == bfs_par[v]) continue;
            if (dp_arr[c] >= 0 && sub_sum[v] - sub_sum[c] >= mid)
                dp_arr[v] = max(dp_arr[v], dp_arr[c] + 1);
        }

        // Single arm check
        for (int c : adj[v]) {
            if (c == bfs_par[v]) continue;
            if (dp_arr[c] >= k - 1 && total - sub_sum[c] >= mid) return true;
        }

        // Two arms check
        static vector<pair<int,ll>> valid2;
        valid2.clear();
        for (int c : adj[v]) {
            if (c == bfs_par[v]) continue;
            if (dp_arr[c] >= 0) valid2.push_back({dp_arr[c], sub_sum[c]});
        }
        if (valid2.size() < 2) continue;

        sort(valid2.begin(), valid2.end(), [](auto& a, auto& b){ return a.first > b.first; });
        int m = valid2.size();

        multiset<ll> pool;
        int j = m - 1;
        int need2_init = max(0, k - 2 - valid2[0].first);
        while (j >= 0 && valid2[j].first < need2_init) j--;
        for (int idx = 0; idx <= j; idx++) pool.insert(valid2[idx].second);

        for (int ii = 0; ii < m; ii++) {
            int arm1 = valid2[ii].first;
            ll sub1 = valid2[ii].second;
            int need2 = max(0, k - 2 - arm1);
            while (j >= 0 && valid2[j].first < need2) {
                pool.erase(pool.find(valid2[j].second));
                j--;
            }
            ll budget = total - mid - sub1;
            if (ii <= j) {
                pool.erase(pool.find(sub1));
                if (!pool.empty() && *pool.begin() <= budget) return true;
                pool.insert(sub1);
            } else {
                if (!pool.empty() && *pool.begin() <= budget) return true;
            }
        }
    }
    return false;
}

void solve() {
    cin >> n >> k;
    total = 0;
    for (int i = 1; i <= n; i++) { cin >> a[i]; total += a[i]; adj[i].clear(); }
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v); adj[v].push_back(u);
    }

    auto bfs_far = [&](int start) -> pair<int,int> {
        static int dist[MAXN];
        fill(dist + 1, dist + n + 1, -1);
        queue<int> q;
        q.push(start); dist[start] = 0;
        int far = start, d = 0;
        while (!q.empty()) {
            int v = q.front(); q.pop();
            if (dist[v] > d) { d = dist[v]; far = v; }
            for (int u : adj[v]) if (dist[u] == -1) { dist[u] = dist[v] + 1; q.push(u); }
        }
        return {far, d};
    };
    auto [f1, _d] = bfs_far(1);
    auto [f2, diam] = bfs_far(f1);
    if (diam < k) { cout << -1 << '\n'; return; }

    dfs_sum(1);
    bfs_build(1);

    ll lo = 1, hi = total, ans = 0;
    while (lo <= hi) {
        ll mid_val = (lo + hi) / 2;
        if (check(mid_val)) { ans = mid_val; lo = mid_val + 1; }
        else hi = mid_val - 1;
    }
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t; cin >> t;
    while (t--) solve();
}