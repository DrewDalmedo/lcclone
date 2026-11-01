/* Slow and obviously right: play the moves one at a time, each time
 * putting a block on a shortest tower, until the shortest and the tallest
 * are the same height. */
int minMoves(int *nums, int numsSize)
{
    int *h = malloc((size_t)numsSize * sizeof *h);
    int moves = 0, lo, hi, i;

    memcpy(h, nums, (size_t)numsSize * sizeof *h);
    for (;;) {
        lo = hi = 0;
        for (i = 1; i < numsSize; i++) {
            if (h[i] < h[lo])
                lo = i;
            if (h[i] > h[hi])
                hi = i;
        }
        if (h[lo] == h[hi])
            break;
        h[lo]++;
        moves++;
    }
    free(h);
    return moves;
}
