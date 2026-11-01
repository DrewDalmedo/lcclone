/* Slow and obviously right: score every pair of positions. */
int maxProduct(int *nums, int numsSize)
{
    int best = 0, score, i, j;

    for (i = 0; i < numsSize; i++)
        for (j = 0; j < numsSize; j++) {
            if (i == j)
                continue;
            score = (nums[i] - 1) * (nums[j] - 1);
            if (score > best)
                best = score;
        }
    return best;
}
