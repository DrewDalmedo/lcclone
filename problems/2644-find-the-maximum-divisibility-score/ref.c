/* Reference solution: score each divisor by counting the multiples of it
 * in nums, and keep the best one seen so far, replacing it on a higher
 * score or on an equal score with a smaller value. */
int maxDivScore(int *nums, int numsSize, int *divisors, int divisorsSize)
{
    int best = divisors[0], best_score = -1, i, j, score;

    for (i = 0; i < divisorsSize; i++) {
        score = 0;
        for (j = 0; j < numsSize; j++)
            if (nums[j] % divisors[i] == 0)
                score++;
        if (score > best_score ||
            (score == best_score && divisors[i] < best)) {
            best = divisors[i];
            best_score = score;
        }
    }
    return best;
}
