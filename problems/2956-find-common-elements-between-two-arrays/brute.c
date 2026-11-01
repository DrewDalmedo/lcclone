/* Slow and obviously right: for each card, look through the whole other
 * hand for a card with the same number. */
static int matched(const int *hand, int size, const int *other, int otherSize)
{
    int count = 0, i, j;

    for (i = 0; i < size; i++)
        for (j = 0; j < otherSize; j++)
            if (hand[i] == other[j]) {
                count++;
                break;
            }
    return count;
}

int *findIntersectionValues(int *nums1, int nums1Size, int *nums2,
                            int nums2Size, int *returnSize)
{
    int *out = malloc(2 * sizeof *out);

    out[0] = matched(nums1, nums1Size, nums2, nums2Size);
    out[1] = matched(nums2, nums2Size, nums1, nums1Size);
    *returnSize = 2;
    return out;
}
