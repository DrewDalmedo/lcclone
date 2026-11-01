/* Reference solution: count every value in a table indexed by value
 * (values run from 1 to 500), then look for a lucky value from the top
 * down, so the first one found is the largest. */
int findLucky(int *arr, int arrSize)
{
    int count[501] = {0};
    int i, v;

    for (i = 0; i < arrSize; i++)
        count[arr[i]]++;
    for (v = 500; v >= 1; v--)
        if (count[v] == v)
            return v;
    return -1;
}
