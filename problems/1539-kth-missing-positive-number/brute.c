/* Slow and obviously right: try 1, 2, 3, ... in turn, look each one up in
 * the whole of arr, and stop at the k-th one not found. */
int findKthPositive(int *arr, int arrSize, int k)
{
    int x = 0, i, found;

    while (k > 0) {
        x++;
        found = 0;
        for (i = 0; i < arrSize; i++)
            if (arr[i] == x)
                found = 1;
        if (!found)
            k--;
    }
    return x;
}
