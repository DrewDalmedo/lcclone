/* Reference solution: split each quadruplet a < b < c < d into the pairs
 * (a, b) and (c, d); it is special when nums[a] + nums[b] equals
 * nums[d] - nums[c]. Walk b from right to left. Before handling b, record
 * the differences of every pair (b + 1, d), so the table holds exactly the
 * pairs (c, d) with c > b; then look up the sum of every pair (a, b).
 * Only differences from 1 to 99 can match a sum, so the others are not
 * stored. O(n^2). */
int countQuadruplets(int *nums, int numsSize)
{
    int diffs[100] = {0};
    int count = 0, a, b, d, v;

    for (b = numsSize - 3; b >= 1; b--) {
        for (d = b + 2; d < numsSize; d++) {
            v = nums[d] - nums[b + 1];
            if (v > 0)
                diffs[v]++;
        }
        for (a = 0; a < b; a++) {
            v = nums[a] + nums[b];
            if (v < 100)
                count += diffs[v];
        }
    }
    return count;
}
