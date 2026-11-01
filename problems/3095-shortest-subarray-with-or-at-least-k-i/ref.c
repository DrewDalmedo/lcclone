/* Reference solution: a sliding window. For each of the six bits a value
 * can have, keep how many elements of the window have it set, so that
 * dropping the left element clears a bit only when no other element in
 * the window still has it. While the window's OR is at least k, record
 * its length and drop its left element. */
int minimumSubarrayLength(int *nums, int numsSize, int k)
{
    int bits[6] = {0};
    int left = 0, best = -1, right, b, value;

    for (right = 0; right < numsSize; right++) {
        for (b = 0; b < 6; b++)
            if (nums[right] >> b & 1)
                bits[b]++;
        while (left <= right) {
            value = 0;
            for (b = 0; b < 6; b++)
                if (bits[b] > 0)
                    value |= 1 << b;
            if (value < k)
                break;
            if (best == -1 || right - left + 1 < best)
                best = right - left + 1;
            for (b = 0; b < 6; b++)
                if (nums[left] >> b & 1)
                    bits[b]--;
            left++;
        }
    }
    return best;
}
