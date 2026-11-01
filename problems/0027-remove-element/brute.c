/* Slow and obviously right: for each value from 0 to 50 other than val,
 * count how often it appears in nums and write that many copies into a
 * new array; then copy the new array back to the front of nums. */
int removeElement(int *nums, int numsSize, int val)
{
    int kept[100];
    int v, i, k = 0;

    for (v = 0; v <= 50; v++) {
        if (v == val)
            continue;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == v)
                kept[k++] = v;
    }
    for (i = 0; i < k; i++)
        nums[i] = kept[i];
    return k;
}
