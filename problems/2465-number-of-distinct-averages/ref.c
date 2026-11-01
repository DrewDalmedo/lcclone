/* Reference solution: two midpoints are equal exactly when the two pair sums
 * are, so compare sums, which are whole numbers from 0 to 200. Sort the
 * values with a counting sort, walk two pointers in from both ends, and
 * mark each sum in a table of 201 flags, counting the new ones. */
int distinctAverages(int *nums, int numsSize)
{
    int count[101] = {0}, sorted[100];
    bool seen[201] = {false};
    int i, j, v, distinct = 0;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (v = 0, i = 0; v <= 100; v++)
        while (count[v]-- > 0)
            sorted[i++] = v;
    for (i = 0, j = numsSize - 1; i < j; i++, j--)
        if (!seen[sorted[i] + sorted[j]]) {
            seen[sorted[i] + sorted[j]] = true;
            distinct++;
        }
    return distinct;
}
