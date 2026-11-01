/* Slow and obviously right: try every choice of four different positions,
 * w and x for the plus pair and y and z for the minus pair. */
int maxProductDifference(int *nums, int numsSize)
{
    int best = 0, found = 0, w, x, y, z, d;

    for (w = 0; w < numsSize; w++)
        for (x = 0; x < numsSize; x++)
            for (y = 0; y < numsSize; y++)
                for (z = 0; z < numsSize; z++) {
                    if (w == x || w == y || w == z || x == y || x == z ||
                        y == z)
                        continue;
                    d = nums[w] * nums[x] - nums[y] * nums[z];
                    if (!found || d > best) {
                        best = d;
                        found = 1;
                    }
                }
    return best;
}
