/* Reference solution: a free jump keeps a chip's parity, so a chip reaches
 * every coordinate of its own parity for nothing and every other one for
 * exactly 1. Gather the chips on the parity most of them share; each chip
 * of the other parity pays 1. */
int minCostToMoveChips(int *position, int positionSize)
{
    int odd = 0, i;

    for (i = 0; i < positionSize; i++)
        odd += position[i] % 2;
    return odd < positionSize - odd ? odd : positionSize - odd;
}
