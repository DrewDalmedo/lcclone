/* Reference solution: a leg there and a leg back take 2(n - 1) seconds and
 * bring the ball home to child 0, so only k modulo that matters. Within
 * one round, the ball is at r on the way out, and at 2(n - 1) - r on the
 * way back. */
int numberOfChild(int n, int k)
{
    int round = 2 * (n - 1);
    int r = k % round;

    return r <= n - 1 ? r : round - r;
}
