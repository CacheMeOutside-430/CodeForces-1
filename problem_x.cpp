#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXFACT = 1500005;

long long fact[MAXFACT], inv_fact[MAXFACT];

long long power(long long a, long long b, long long mod) {
    long long res = 1; a %= mod;
    while (b > 0) {
        if (b & 1) res = res * a % mod;
        a = a * a % mod;
        b >>= 1;
    }
    return res;
}

void precompute() {
    fact[0] = 1;
    for (int i = 1; i < MAXFACT; i++) fact[i] = fact[i-1] * i % MOD;
    inv_fact[MAXFACT-1] = power(fact[MAXFACT-1], MOD-2, MOD);
    for (int i = MAXFACT-2; i >= 0; i--) inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;
}

// C(n, k) for n >= 0, k >= 0
long long C(long long n, long long k) {
    if (k < 0 || n < 0 || k > n) return 0;
    if (k == 0) return 1;
    if (n >= MAXFACT) {
        // shouldn't happen given constraints
        return 0;
    }
    return fact[n] % MOD * inv_fact[k] % MOD * inv_fact[n-k] % MOD;
}

struct Node {
    long long sf_exact; // exact sum of fixed values (for M computation)
    long long sf;       // sf mod MOD
    long long spf;      // sum of pos * val for fixed, mod MOD
    int nf;             // count of free positions
    long long sx;       // sum of positions of free, mod MOD
    long long V_ff;     // sum_{p,q in F} max(p,q)*bp*bq, mod MOD
    long long V_fx;     // sum_{p in F, q in X} max(p,q)*bp, mod MOD
    long long V_xx;     // sum_{p,q in X} max(p,q), mod MOD
};

inline long long md(long long x) { return ((x % MOD) + MOD) % MOD; }

Node combine(const Node& L, const Node& R) {
    Node res;
    res.sf_exact = L.sf_exact + R.sf_exact;
    res.sf = (L.sf + R.sf) % MOD;
    res.spf = (L.spf + R.spf) % MOD;
    res.nf = L.nf + R.nf;
    res.sx = (L.sx + R.sx) % MOD;
    // V_ff: LL + RR + cross(p in L, q in R): max(p,q)=q, so spf_R * sf_L
    res.V_ff = (L.V_ff + R.V_ff + R.spf % MOD * L.sf) % MOD;
    // V_fx: LL + RR + (p in F_L, q in X_R): max=q, so sx_R*sf_L
    //                 + (p in F_R, q in X_L): max=p, so spf_R*nf_L
    res.V_fx = (L.V_fx + R.V_fx + R.sx % MOD * L.sf + R.spf % MOD * L.nf) % MOD;
    // V_xx: LL + RR + 2*(p in X_L, q in X_R): max=q, so 2*sx_R*nf_L
    res.V_xx = (L.V_xx + R.V_xx + 2 * R.sx % MOD * L.nf) % MOD;
    return res;
}

// Segment tree
const int MAXN = 300005;
Node tree[4 * MAXN];
int a_arr[MAXN]; // original array values
int n_seg;

Node make_leaf(int pos, int val) {
    Node nd;
    if (val == -1) {
        // free
        nd.sf_exact = 0;
        nd.sf = 0;
        nd.spf = 0;
        nd.nf = 1;
        nd.sx = pos;  // pos is 1-indexed in original array
        nd.V_ff = 0;
        nd.V_fx = 0;
        nd.V_xx = (long long)pos; // max(p,p)*1 = p
    } else {
        // fixed
        nd.sf_exact = val;
        nd.sf = val % MOD;
        nd.spf = (long long)pos % MOD * (val % MOD) % MOD;
        nd.nf = 0;
        nd.sx = 0;
        nd.V_ff = (long long)pos % MOD * (val % MOD) % MOD * (val % MOD) % MOD; // max(p,p)*b^2 = p*b^2
        nd.V_fx = 0;
        nd.V_xx = 0;
    }
    return nd;
}

void build(int node, int l, int r) {
    if (l == r) {
        tree[node] = make_leaf(l, a_arr[l]);
        return;
    }
    int mid = (l + r) / 2;
    build(2*node, l, mid);
    build(2*node+1, mid+1, r);
    tree[node] = combine(tree[2*node], tree[2*node+1]);
}

void update(int node, int l, int r, int pos, int val) {
    if (l == r) {
        a_arr[l] = val;
        tree[node] = make_leaf(l, val);
        return;
    }
    int mid = (l + r) / 2;
    if (pos <= mid) update(2*node, l, mid, pos, val);
    else update(2*node+1, mid+1, r, pos, val);
    tree[node] = combine(tree[2*node], tree[2*node+1]);
}

Node query(int node, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return tree[node];
    int mid = (l + r) / 2;
    if (qr <= mid) return query(2*node, l, mid, ql, qr);
    if (ql > mid) return query(2*node+1, mid+1, r, ql, qr);
    return combine(query(2*node, l, mid, ql, qr), query(2*node+1, mid+1, r, ql, qr));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    precompute();
    
    int t;
    cin >> t;
    while (t--) {
        int n, q;
        cin >> n >> q;
        for (int i = 1; i <= n; i++) cin >> a_arr[i];
        build(1, 1, n);
        
        while (q--) {
            int op;
            cin >> op;
            if (op == 1) {
                int p, v;
                cin >> p >> v;
                update(1, 1, n, p, v);
            } else {
                int l, r, m;
                cin >> l >> r >> m;
                Node nd = query(1, 1, n, l, r);
                
                long long S = nd.sf_exact;
                long long M = (long long)m - S;
                if (M < 0) { cout << 0 << '\n'; continue; }
                
                int nf = nd.nf;
                // r+1 mod MOD
                long long rp1 = (long long)(r + 1) % MOD;
                
                long long sf = nd.sf % MOD;
                long long nf_mod = nf % MOD;
                
                // W_ff = (r+1)*sf^2 - V_ff
                long long W_ff = md(rp1 * sf % MOD * sf % MOD - nd.V_ff);
                // W_fx = (r+1)*sf*nf - V_fx
                long long W_fx = md(rp1 * sf % MOD * nf_mod % MOD - nd.V_fx);
                // W_xx_diag = (r+1)*nf - sx
                long long W_xx_diag = md(rp1 * nf_mod % MOD - nd.sx);
                // W_xx = (r+1)*nf^2 - V_xx
                long long W_xx = md(rp1 * nf_mod % MOD * nf_mod % MOD - nd.V_xx);
                
                // A = C(M+nf-1, nf-1), B = C(M+nf-1, nf), D = C(M+nf-1, nf+1)
                long long Mnf1 = M + nf - 1; // n for binomial
                long long A = C(Mnf1, nf-1);
                long long B_coef = C(Mnf1, nf);
                long long D = C(Mnf1, nf+1);
                
                // g = A*W_ff + 2B*W_fx + (D+B)*W_xx_diag + D*W_xx
                long long ans = 0;
                ans = (ans + A % MOD * W_ff) % MOD;
                ans = (ans + 2 * B_coef % MOD * W_fx) % MOD;
                ans = (ans + (D + B_coef) % MOD * W_xx_diag) % MOD;
                ans = (ans + D * W_xx) % MOD;
                ans %= MOD;
                
                cout << ans << '\n';
            }
        }
    }
    return 0;
}
