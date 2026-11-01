/* Reference solution: the frequent block has more than q = n / 4
 * (rounded down) copies, so it is the only block that holds both arr[i]
 * and arr[i + q] for some i; any other block has at most q copies. */
int findSpecialInteger(int *arr, int arrSize)
{
    int q = arrSize / 4, i;

    for (i = 0; i + q < arrSize; i++)
        if (arr[i] == arr[i + q])
            return arr[i];
    return arr[0];
}
