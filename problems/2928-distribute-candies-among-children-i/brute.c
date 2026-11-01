/* Slow and obviously right: try every share for each of the three
 * children and count the triples that add up to n. */
int distributeCandies(int n, int limit)
{
    int a, b, c, count = 0;

    for (a = 0; a <= limit; a++)
        for (b = 0; b <= limit; b++)
            for (c = 0; c <= limit; c++)
                if (a + b + c == n)
                    count++;
    return count;
}
