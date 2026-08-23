#include <bits/stdc++.h>
using namespace std;

static const long long INF = (long long)4e18;

struct BIT {
    int n;
    vector<long long> b;

    BIT(int n=0){init(n);}

    void init(int n_) {
        n = n_;
        b.assign(n+1, INF);
    }

    void update(int i, long long v){
        for (++i;i<=n;i+=i&-i)
            b[i]=min(b[i],v);
    }

    long long query(int i){
        long long r=INF;
        for (++i;i>0;i-=i&-i)
            r=min(r,b[i]);
        return r;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin>>n;
    vector<int> p(n), c(n);
    for (auto &x:p) cin>>x;
    for (auto &x:c) cin>>x;

    int m; cin>>m;
    vector<int> tp(m), tc(m), d(m);
    for (auto &x:tp) cin>>x;
    for (auto &x:tc) cin>>x;
    for (auto &x:d) cin>>x;

    vector<int> cc=c;
    sort(cc.begin(),cc.end());
    cc.erase(unique(cc.begin(),cc.end()),cc.end());

    auto getC=[&](int x){
        return lower_bound(cc.begin(),cc.end(),x)-cc.begin();
    };

    vector<long long> ans(m, INF);

    vector<int> ord(n);
    iota(ord.begin(),ord.end(),0);

    // Sweep 1: p >= tp, c >= tc
    {
        vector<int> ordq(m);
        iota(ordq.begin(),ordq.end(),0);
        sort(ord.begin(),ord.end(),[&](int a,int b){return p[a]<p[b];});
        sort(ordq.begin(),ordq.end(),[&](int a,int b){return tp[a]<tp[b];});

        BIT bit(cc.size());
        int j=0;

        for(int id:ordq){
            while(j<n && p[ord[j]]<tp[id]){
                bit.update(getC(c[ord[j]]), p[ord[j]]+c[ord[j]]);
                j++;
            }
            long long best=bit.query(cc.size()-1);
            ans[id]=min(ans[id], best - tp[id] - tc[id]);
        }
    }

    // Sweep 2: symmetric (right side)
    {
        vector<int> ordq(m);
        iota(ordq.begin(),ordq.end(),0);
        sort(ord.begin(),ord.end(),[&](int a,int b){return p[a]>p[b];});
        sort(ordq.begin(),ordq.end(),[&](int a,int b){return tp[a]+d[a]>tp[b]+d[b];});

        BIT bit(cc.size());
        int j=0;

        for(int id:ordq){
            int lim = tp[id]+d[id];
            while(j<n && p[ord[j]]>=lim){
                bit.update(getC(c[ord[j]]), p[ord[j]]+c[ord[j]]);
                j++;
            }
            long long best=bit.query(cc.size()-1);
            ans[id]=min(ans[id], best - lim - (tc[id]+d[id]));
        }
    }

    for(auto x:ans) cout<<x<<"\n";
}