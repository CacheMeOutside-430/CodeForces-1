using System;
using System.IO;
using System.Text;
using System.Runtime.CompilerServices;

public class Solution
{
    private const int MAX_VAL = 1000005;
    private static int[,] minPAtC = new int[MAX_VAL, 2]; 

    public static void Main(string[] args)
    {
        FastScanner scanner = new FastScanner();
        if (!scanner.HasNext()) return;

        int n = scanner.NextInt();
        long[] p = new long[n];
        for (int i = 0; i < n; i++) p[i] = scanner.NextLong();

        long[] c = new long[n];
        for (int i = 0; i < n; i++) c[i] = scanner.NextLong();

        int maxP = 0, maxC = 0;
        for (int i = 0; i < n; i++)
        {
            if (p[i] > maxP) maxP = (int)p[i];
            if (c[i] > maxC) maxC = (int)c[i];
        }

        int[] bestPForC = new int[maxC + 2];
        for (int i = 0; i <= maxC + 1; i++) bestPForC[i] = int.MaxValue;

        for (int i = 0; i < n; i++)
        {
            int cc = (int)c[i];
            if (p[i] < bestPForC[cc])
            {
                bestPForC[cc] = (int)p[i];
            }
        }

        int currentMinP = int.MaxValue;
        int[] suffixMinP = new int[maxC + 2];
        for (int i = maxC; i >= 0; i--)
        {
            if (bestPForC[i] < currentMinP) currentMinP = bestPForC[i];
            suffixMinP[i] = currentMinP;
        }

        if (!scanner.HasNext()) return;
        int m = scanner.NextInt();

        long[] tp = new long[m];
        for (int i = 0; i < m; i++) tp[i] = scanner.NextLong();

        long[] tc = new long[m];
        for (int i = 0; i < m; i++) tc[i] = scanner.NextLong();

        long[] d = new long[m];
        for (int i = 0; i < m; i++) d[i] = scanner.NextLong();

        using (StreamWriter sw = new StreamWriter(Console.OpenStandardOutput(), Encoding.UTF8, 65536))
        {
            for (int j = 0; j < m; j++)
            {
                long targetTp = tp[j];
                long targetTc = tc[j];
                long targetD = d[j];

                long minInfluence = long.MaxValue;

                int cBound = (int)Math.Min(maxC, targetTc + targetD - 1);
                int queryIdx = (cBound >= 0) ? cBound : 0;
                
                if (queryIdx <= maxC)
                {
                    long candidateP = suffixMinP[queryIdx];
                    if (candidateP != int.MaxValue)
                    {
                        long ip = (candidateP < targetTp) ? 0 : ((candidateP < targetTp + targetD) ? candidateP : targetTp + targetD);
                        long ic = (maxC < targetTc) ? 0 : ((maxC < targetTc + targetD) ? maxC : targetTc + targetD);
                        if (ip + ic < minInfluence) minInfluence = ip + ic;
                    }
                }

                for (int i = 0; i < n; i++)
                {
                    long ip = (p[i] < targetTp) ? 0 : ((p[i] < targetTp + targetD) ? p[i] : targetTp + targetD);
                    long ic = (c[i] < targetTc) ? 0 : ((c[i] < targetTc + targetD) ? c[i] : targetTc + targetD);
                    long total = ip + ic;
                    if (total < minInfluence) minInfluence = total;
                }

                sw.WriteLine(minInfluence);
            }
            sw.Flush(); 
        }
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