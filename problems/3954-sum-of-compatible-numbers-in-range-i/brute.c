/* Slow and obviously right: try every x from 1 to 300 (past any n + k),
 * keep those at most k away from n, and compare n and x bit by bit,
 * dropping x as soon as some position is 1 in both. */
int sumOfGoodIntegers(int n, int k)
{
    int x, b, dist, clash, sum = 0;

    for (x = 1; x <= 300; x++) {
        dist = x > n ? x - n : n - x;
        if (dist > k)
            continue;
        clash = 0;
        for (b = 0; b < 10; b++)
            if ((n >> b) % 2 == 1 && (x >> b) % 2 == 1)
                clash = 1;
        if (!clash)
            sum += x;
    }
    return sum;
}
