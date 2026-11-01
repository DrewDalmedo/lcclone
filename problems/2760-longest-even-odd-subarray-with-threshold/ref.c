/* Reference solution: one pass, keeping the length of the chain that ends
 * at the current element (0 if none). An element above threshold ends any
 * chain; one whose parity differs from its neighbour's extends the chain
 * before it; otherwise a new chain starts there if it is even. */
int longestAlternatingSubarray(int *nums, int numsSize, int threshold)
{
    int best = 0, len = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] > threshold)
            len = 0;
        else if (len > 0 && nums[i] % 2 != nums[i - 1] % 2)
            len++;
        else if (nums[i] % 2 == 0)
            len = 1;
        else
            len = 0;
        if (len > best)
            best = len;
    }
    return best;
}
