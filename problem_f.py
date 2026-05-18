import sys

def solve():
    input = sys.stdin.readline
    q = int(input())
    v = list(map(int, input().split()))
    
    deg = [0] * (q + 5)
    maxdeg = 0
    result = []
    
    for i in range(q):
        u = v[i]
        n = i + 2
        
        deg[n] += 1
        deg[u] += 1
        
        maxdeg = max(maxdeg, deg[n], deg[u])
        
        if maxdeg >= 3:
            result.append("3")
        elif i == 0:  # first query (n=2)
            result.append("1")
        else:
            result.append("2")
    
    print("\n".join(result))

if __name__ == "__main__":
    solve()