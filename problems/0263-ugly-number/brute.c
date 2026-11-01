/* Slow and obviously right: try every product 2^a * 3^b * 5^c up to n and
 * see whether one of them is n. Nothing is tried when n <= 0. */
bool isUgly(int n)
{
    long long p2, p3, p5;

    for (p2 = 1; p2 <= n; p2 *= 2)
        for (p3 = p2; p3 <= n; p3 *= 3)
            for (p5 = p3; p5 <= n; p5 *= 5)
                if (p5 == n)
                    return true;
    return false;
}
