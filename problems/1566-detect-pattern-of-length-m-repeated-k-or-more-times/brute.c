/* Slow and obviously right: try every start whose k groups fit, and
 * compare each group with the first, element by element. */
bool containsPattern(int *arr, int arrSize, int m, int k)
{
    int i, c, t, ok;

    for (i = 0; i + m * k <= arrSize; i++) {
        ok = 1;
        for (c = 1; c < k && ok; c++)
            for (t = 0; t < m && ok; t++)
                if (arr[i + c * m + t] != arr[i + t])
                    ok = 0;
        if (ok)
            return true;
    }
    return false;
}
