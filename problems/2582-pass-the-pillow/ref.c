/* Reference solution: the pillow's moves repeat every 2(n - 1) seconds,
 * n - 1 out to chair n and n - 1 back to chair 1. Within that cycle it is
 * in chair t + 1 on the way out and in chair 2n - 1 - t on the way back.
 * O(1) time. */
int passThePillow(int n, int time)
{
    int t = time % (2 * (n - 1));

    return t <= n - 1 ? t + 1 : 2 * n - 1 - t;
}
