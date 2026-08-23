using System;
using System.IO;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

public class Solution
{
    private const long INF = long.MaxValue;

    public struct Point
    {
        public long P;
        public long C;
    }

    public struct Node
    {
        public Point Pt;
        public long MinP, MaxP;
        public long MinC, MaxC;
        public int Left, Right;
    }

    private static Point[] points;
    private static List<Node> tree;

    public static void Main(string[] args)
    {
        FastScanner scanner = new FastScanner();
        
        if (!scanner.HasNext()) return;
        int n = scanner.NextInt();

        long[] p = new long[n];
        for (int i = 0; i < n; i++) p[i] = scanner.NextLong();

        long[] c = new long[n];
        for (int i = 0; i < n; i++) c[i] = scanner.NextLong();

        points = new Point[n];
        for (int i = 0; i < n; i++)
        {
            points[i] = new Point { P = p[i], C = c[i] };
        }

        tree = new List<Node>(2 * n);
        int root = BuildKdTree(0, n - 1, 0);

        if (!scanner.HasNext()) return;
        int m = scanner.NextInt();

        long[] tp = new long[m];
        for (int i = 0; i < m; i++) tp[i] = scanner.NextLong();

        long[] tc = new long[m];
        for (int i = 0; i < m; i++) tc[i] = scanner.NextLong();

        long[] d = new long[m];
        for (int i = 0; i < m; i++) d[i] = scanner.NextLong();

        using (StreamWriter sw = new StreamWriter(Console.OpenStandardOutput(), 65536))
        {
            for (int j = 0; j < m; j++)
            {
                long minInf = INF;
                Query(root, tp[j], tc[j], d[j], ref minInf);
                sw.WriteLine(minInf);
            }
        }
    }

    private static int BuildKdTree(int l, int r, int depth)
    {
        if (l > r) return -1;
        int mid = l + (r - l) / 2;

        NthElement(l, r, mid, depth % 2 == 0);

        int idx = tree.Count;
        Node node = new Node
        {
            Pt = points[mid],
            Left = -1,
            Right = -1
        };
        tree.Add(node);

        int leftChild = BuildKdTree(l, mid - 1, depth + 1);
        int rightChild = BuildKdTree(mid + 1, r, depth + 1);

        Node updatedNode = tree[idx];
        updatedNode.Left = leftChild;
        updatedNode.Right = rightChild;
        
        UpdateNode(ref updatedNode);
        tree[idx] = updatedNode;

        return idx;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private static void UpdateNode(ref Node node)
    {
        node.MinP = node.MaxP = node.Pt.P;
        node.MinC = node.MaxC = node.Pt.C;

        if (node.Left != -1)
        {
            Node l = tree[node.Left];
            if (l.MinP < node.MinP) node.MinP = l.MinP;
            if (l.MaxP > node.MaxP) node.MaxP = l.MaxP;
            if (l.MinC < node.MinC) node.MinC = l.MinC;
            if (l.MaxC > node.MaxC) node.MaxC = l.MaxC;
        }
        if (node.Right != -1)
        {
            Node r = tree[node.Right];
            if (r.MinP < node.MinP) node.MinP = r.MinP;
            if (r.MaxP > node.MaxP) node.MaxP = r.MaxP;
            if (r.MinC < node.MinC) node.MinC = r.MinC;
            if (r.MaxC > node.MaxC) node.MaxC = r.MaxC;
        }
    }

    private static void Query(int idx, long tp, long tc, long d, ref long minInf)
    {
        if (idx == -1) return;

        Node curr = tree[idx];
        
        long inf = GetInfluence(curr.Pt, tp, tc, d);
        if (inf < minInf) minInf = inf;

        long lowerBoundP = 0;
        if (curr.MaxP < tp) lowerBoundP = 0;
        else if (curr.MinP >= tp + d) lowerBoundP = tp + d;
        else lowerBoundP = Math.Max(curr.MinP, tp);

        long lowerBoundC = 0;
        if (curr.MaxC < tc) lowerBoundC = 0;
        else if (curr.MinC >= tc + d) lowerBoundC = tc + d;
        else lowerBoundC = Math.Max(curr.MinC, tc);

        if (lowerBoundP + lowerBoundC >= minInf)
        {
            return;
        }

        Query(curr.Left, tp, tc, d, ref minInf);
        Query(curr.Right, tp, tc, d, ref minInf);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private static long GetInfluence(Point pt, long tp, long tc, long d)
    {
        long ip = (pt.P < tp) ? 0 : ((pt.P < tp + d) ? pt.P : tp + d);
        long ic = (pt.C < tc) ? 0 : ((pt.C < tc + d) ? pt.C : tc + d);
        return ip + ic;
    }

    private static void NthElement(int l, int r, int n, bool sortOnP)
    {
        while (l < r)
        {
            int pivotIdx = Partition(l, r, sortOnP);
            if (pivotIdx == n) return;
            else if (pivotIdx < n) l = pivotIdx + 1;
            else r = pivotIdx - 1;
        }
    }

    private static int Partition(int l, int r, bool sortOnP)
    {
        int mid = l + (r - l) / 2;
        Swap(mid, r);
        Point pivot = points[r];
        int i = l;

        for (int j = l; j < r; j++)
        {
            bool condition = sortOnP ? (points[j].P < pivot.P) : (points[j].C < pivot.C);
            if (condition)
            {
                Swap(i, j);
                i++;
            }
        }
        Swap(i, r);
        return i;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    private static void Swap(int i, int j)
    {
        Point temp = points[i];
        points[i] = points[j];
        points[j] = temp;
    }
}

public class FastScanner
{
    private readonly BufferedStream stream;
    private readonly byte[] buffer = new byte[32768];
    private int head = 0;
    private int tail = 0;

    public FastScanner()
    {
        stream = new BufferedStream(Console.OpenStandardInput(), 65536);
    }

    private byte Read()
    {
        if (head >= tail)
        {
            head = 0;
            tail = stream.Read(buffer, 0, buffer.Length);
            if (tail <= 0) return 0;
        }
        return buffer[head++];
    }

    public bool HasNext()
    {
        while (true)
        {
            if (head >= tail)
            {
                head = 0;
                tail = stream.Read(buffer, 0, buffer.Length);
                if (tail <= 0) return false;
            }
            byte b = buffer[head];
            if (b > 32) return true;
            head++;
        }
    }

    public int NextInt()
    {
        return (int)NextLong();
    }

    public long NextLong()
    {
        if (!HasNext()) return 0;
        long res = 0;
        byte b = Read();
        while (b <= 32) b = Read();
        while (b > 32)
        {
            if (b >= '0' && b <= '9')
            {
                res = res * 10 + (b - '0');
            }
            b = Read();
        }
        return res;
    }
}