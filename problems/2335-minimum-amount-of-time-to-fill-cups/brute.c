/* Slow and obviously right: work out the fewest seconds for every state
 * (x, y, z) of cups still to fill, smallest states first, trying every way
 * to spend one second: one cup from one nozzle, or one cup each from two. */
int fillCups(int *amount, int amountSize)
{
    static const int moves[6][3] = {
        {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {1, 0, 1}, {0, 1, 1}
    };
    int a = amount[0], b = amount[1], c = amount[2];
    int *best = malloc((size_t)(a + 1) * (b + 1) * (c + 1) * sizeof *best);
    int x, y, z, k, nx, ny, nz, here, result;

    (void)amountSize;
    for (x = 0; x <= a; x++)
        for (y = 0; y <= b; y++)
            for (z = 0; z <= c; z++) {
                here = x == 0 && y == 0 && z == 0 ? 0 : 1000;
                for (k = 0; k < 6; k++) {
                    nx = x - moves[k][0];
                    ny = y - moves[k][1];
                    nz = z - moves[k][2];
                    if (nx < 0 || ny < 0 || nz < 0)
                        continue;
                    if (best[(nx * (b + 1) + ny) * (c + 1) + nz] + 1 < here)
                        here = best[(nx * (b + 1) + ny) * (c + 1) + nz] + 1;
                }
                best[(x * (b + 1) + y) * (c + 1) + z] = here;
            }
    result = best[(a * (b + 1) + b) * (c + 1) + c];
    free(best);
    return result;
}
