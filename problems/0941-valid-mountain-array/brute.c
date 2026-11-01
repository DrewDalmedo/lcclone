/* Slow and obviously right: try every index p strictly inside the array as
 * the top, and check every step before p goes up and every step from p on
 * goes down. */
bool validMountainArray(int *arr, int arrSize)
{
    int p, k, ok;

    for (p = 1; p < arrSize - 1; p++) {
        ok = 1;
        for (k = 0; k < p; k++)
            if (!(arr[k] < arr[k + 1]))
                ok = 0;
        for (k = p; k < arrSize - 1; k++)
            if (!(arr[k] > arr[k + 1]))
                ok = 0;
        if (ok)
            return true;
    }
    return false;
}
