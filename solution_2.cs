using System;

class Solution
{
    static void Main()
    {
        int t = int.Parse(Console.ReadLine());
        while (t-- > 0)
        {
            string s = Console.ReadLine().Trim();
            int n = s.Length;

            //test against all pairs of digits, keep track of best length of string ending with each digit
            int[] dp = new int[5];

            foreach (char ch in s)
            {
                int d = ch - '0';

                if (d == 4) continue;

                int best = dp[0];
                for (int prev = 1; prev <= 3; prev++) 
                {
                    if ((prev * 10 + d) % 4 == 0) continue;
                    best = Math.Max(best, dp[prev]);
                }

                dp[d] = Math.Max(dp[d], best + 1);
            }

            int maxKeep = 0;
            for (int i = 0; i <= 4; i++)
                maxKeep = Math.Max(maxKeep, dp[i]);

            Console.WriteLine(n - maxKeep);
        }
    }
}