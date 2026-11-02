/* Slow and obviously right: take stones one at a time, always from the
 * first pile that has any, counting the total afresh before every move,
 * and stop as soon as it is a multiple of k (at the latest when the piles
 * are empty). */
int minOperations(int *nums, int numsSize, int k)
{
    int *piles = malloc((size_t)numsSize * sizeof *piles);
    int moves = 0, total, i;

    memcpy(piles, nums, (size_t)numsSize * sizeof *piles);
    for (;;) {
        total = 0;
        for (i = 0; i < numsSize; i++)
            total += piles[i];
        if (total % k == 0)
            break;
        for (i = 0; piles[i] == 0; i++)
            ;
        piles[i]--;
        moves++;
    }
    free(piles);
    return moves;
}
