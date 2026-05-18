deg = array of size q+2, init 0
maxdeg = 0
ans = 1
for i=1..q:
  u = v[i], n = i+1
  deg[n] = 1
  deg[u] += 1
  if deg[n] == 3: ans = max(ans, 3)
  if deg[u] == 3: ans = max(ans, 3)
  print ans