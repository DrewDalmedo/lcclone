/* Reference solution: going from a to a + 1 turns a's lowest 0 bit into a
 * 1 and the 1 bits below it into 0s, so a | (a + 1) is a with its lowest
 * 0 bit switched on. That is always odd, so the even prime 2 has no
 * answer. For an odd p, a must be p with one bit of its run of trailing
 * 1s cleared; clearing the highest bit of that run gives the smallest a. */
int *minBitwiseArray(int *nums, int numsSize, int *returnSize)
{
    int *ans = malloc((size_t)numsSize * sizeof *ans);
    int i, p, bit;

    for (i = 0; i < numsSize; i++) {
        p = nums[i];
        if (p % 2 == 0) {
            ans[i] = -1;
            continue;
        }
        bit = 1;
        while (p & (bit << 1))
            bit <<= 1;
        ans[i] = p - bit;
    }
    *returnSize = numsSize;
    return ans;
}
