using System;

class Solution
{
    static void Main()
    {
        int t = int.Parse(Console.ReadLine());
        while (t-- > 0)
        {
            string[] parts = Console.ReadLine().Split();
            long n = long.Parse(parts[0]);
            long a = long.Parse(parts[1]);
            long b = long.Parse(parts[2]);

            long groups = n / 3;
            long rem = n % 3;

            long cost = groups * Math.Min(3 * a, b);

            if (rem > 0)
                cost += Math.Min(rem * a, b);

            Console.WriteLine(cost);
        }
    }
}