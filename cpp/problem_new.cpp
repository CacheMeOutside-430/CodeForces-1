#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    vector<long long> a(n+1);
    for(int i = 1; i <= n; i++) cin >> a[i];
    
    // dp[i][l] = max score where day i is attended and ends streak of length l
    // best[i] = max score for days 1..i (day i may or may not be attended)
    
    const long long NEG_INF = -1e18;
    vector<vector<long long>> dp(n+1, vector<long long>(n+1, NEG_INF));
    vector<long long> best(n+2, 0);
    // best[-1] = best[0] = 0
    
    long long ans = 0;
    
    for(int i = 1; i <= n; i++){
        // streak of length 1 starting at i
        long long prev = (i >= 2) ? best[i-2] : 0;
        dp[i][1] = prev + a[i] - 1;
        
        // extend streak from i-1
        for(int l = 2; l <= i; l++){
            if(dp[i-1][l-1] != NEG_INF){
                dp[i][l] = dp[i-1][l-1] + a[i] - (2*l - 1);
            }
        }
        
        best[i] = best[i-1];
        for(int l = 1; l <= i; l++){
            if(dp[i][l] != NEG_INF){
                best[i] = max(best[i], dp[i][l]);
            }
        }
        ans = max(ans, best[i]);
    }
    
    cout << ans << "\n";
    return 0;
}