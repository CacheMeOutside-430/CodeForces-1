using System;
using System.Linq;

class Solution
{
    static void Main()
    {
        int t = int.Parse(Console.ReadLine());
        while (t-- > 0)
        {
            int n = int.Parse(Console.ReadLine());
            long[] c = Console.ReadLine().Split().Select(long.Parse).ToArray();

            long ans = 0;
            long totalAll = c.Sum();
            long totalPairs = c.Sum(ci => (ci / 2) * 2);

            foreach (var ci in c)
                if (ci >= 3) ans = Math.Max(ans, ci);

            if (totalPairs >= 4) ans = Math.Max(ans, totalPairs);

            for (int i = 0; i < n; i++)
            {
                long cx = c[i];
                long otherPairs = totalPairs - (c[i] / 2) * 2;

                if (cx >= 2 && otherPairs >= 2)
                    ans = Math.Max(ans, cx + otherPairs);

                long others = totalAll - cx;
                long s = Math.Min(cx / 2, others);
                if (s >= 1 && cx + s >= 3)
                    ans = Math.Max(ans, cx + s);
            }

            Console.WriteLine(ans);
        }
    }
}