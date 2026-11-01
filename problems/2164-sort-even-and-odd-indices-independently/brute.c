/* Slow and obviously right, from ranks: the j-th even index (counting
 * from 0) gets the smallest even-index value v such that more than j
 * even-index values are <= v; the j-th odd index gets the largest
 * odd-index value v such that more than j odd-index values are >= v. */
int *sortEvenOdd(int *nums, int numsSize, int *returnSize)
{
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, j, k, v, cnt, best, up;

    for (i = 0; i < numsSize; i++) {
        up = i % 2 == 0;
        best = -1;
        for (k = i % 2; k < numsSize; k += 2) {
            v = nums[k];
            cnt = 0;
            for (j = i % 2; j < numsSize; j += 2)
                if (up ? nums[j] <= v : nums[j] >= v)
                    cnt++;
            if (cnt > i / 2 && (best < 0 || (up ? v < best : v > best)))
                best = v;
        }
        out[i] = best;
    }
    *returnSize = numsSize;
    return out;
}
