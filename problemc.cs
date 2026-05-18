using System;
using System.Text;

class Program
{
    class FastScanner
    {
        private readonly System.IO.Stream stream = Console.OpenStandardInput();
        private readonly byte[] buffer = new byte[1 << 16];
        private int ptr, len;

        private byte Read()
        {
            if (ptr >= len)
            {
                len = stream.Read(buffer, 0, buffer.Length);
                ptr = 0;
                if (len == 0) return 0;
            }
            return buffer[ptr++];
        }

        public int NextInt()
        {
            byte c;
            do c = Read(); while (c <= ' ');

            int sign = 1;
            if (c == '-')
            {
                sign = -1;
                c = Read();
            }

            int val = 0;
            while (c > ' ')
            {
                val = val * 10 + (c - '0');
                c = Read();
            }

            return val * sign;
        }

        public long NextLong()
        {
            byte c;
            do c = Read(); while (c <= ' ');

            long sign = 1;
            if (c == '-')
            {
                sign = -1;
                c = Read();
            }

            long val = 0;
            while (c > ' ')
            {
                val = val * 10 + (c - '0');
                c = Read();
            }

            return val * sign;
        }
    }

    static void Main()
    {
        var fs = new FastScanner();
        int t = fs.NextInt();
        var sb = new StringBuilder();

        while (t-- > 0)
        {
            int n = fs.NextInt();
            long[] c = new long[n];

            long total = 0;
            long max = 0;

            for (int i = 0; i < n; i++)
            {
                c[i] = fs.NextLong();
                total += c[i];
                if (c[i] > max) max = c[i];
            }

            long ans = Math.Min(total, 2 * (total - max));

            if (ans < 3) ans = 0;

            sb.AppendLine(ans.ToString());
        }

        Console.Write(sb.ToString());
    }
}