/* Slow and obviously right: try every a, b and c from 1 to n. */
int countTriples(int n)
{
    int count = 0, a, b, c;

    for (a = 1; a <= n; a++)
        for (b = 1; b <= n; b++)
            for (c = 1; c <= n; c++)
                if (a * a + b * b == c * c)
                    count++;
    return count;
}
