/* Slow and obviously right: sort a copy of nums, then try every k from 1
 * to 200, writing out the list 1, 2, ..., k-1, k, k and comparing it with
 * the sorted copy element by element. */
bool isGood(int *nums, int numsSize)
{
    int sorted[100], list[201];
    int n = numsSize, k, i, j, t, same;

    for (i = 0; i < n; i++)
        sorted[i] = nums[i];
    for (i = 0; i < n; i++)
        for (j = 0; j + 1 < n - i; j++)
            if (sorted[j] > sorted[j + 1]) {
                t = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = t;
            }
    for (k = 1; k <= 200; k++) {
        for (i = 0; i < k; i++)
            list[i] = i + 1;
        list[k] = k;
        if (k + 1 != n)
            continue;
        same = 1;
        for (i = 0; i < n; i++)
            if (sorted[i] != list[i])
                same = 0;
        if (same)
            return true;
    }
    return false;
}
