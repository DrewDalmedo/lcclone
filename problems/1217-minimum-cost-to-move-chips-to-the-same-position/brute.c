/* Slow and obviously right: try every target t that is a chip's coordinate
 * or one past it (between them they cover both parities). A chip at
 * distance d from t needs at least d % 2 steps of 1, since jumps of 2
 * never change the distance's parity, and d / 2 jumps plus d % 2 steps get
 * it there; add that up over the chips and keep the cheapest target. */
int minCostToMoveChips(int *position, int positionSize)
{
    long long best = -1, cost, t, d;
    int i, j, k;

    for (i = 0; i < positionSize; i++)
        for (k = 0; k <= 1; k++) {
            t = (long long)position[i] + k;
            cost = 0;
            for (j = 0; j < positionSize; j++) {
                d = position[j] > t ? position[j] - t : t - position[j];
                cost += d % 2;
            }
            if (best < 0 || cost < best)
                best = cost;
        }
    return (int)best;
}
