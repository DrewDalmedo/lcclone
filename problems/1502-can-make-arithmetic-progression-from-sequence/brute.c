/* Slow and obviously right: try every order of the elements, building it
 * one place at a time. The first two elements chosen fix the step d, and
 * each later one must be the previous one plus d. */
static bool place(const int *arr, int n, bool *used, int filled, int last,
                  int d)
{
    int i;

    if (filled == n)
        return true;
    for (i = 0; i < n; i++) {
        if (used[i])
            continue;
        if (filled >= 2 && arr[i] - last != d)
            continue;
        used[i] = true;
        if (place(arr, n, used, filled + 1, arr[i],
                  filled == 1 ? arr[i] - last : d)) {
            used[i] = false;
            return true;
        }
        used[i] = false;
    }
    return false;
}

bool canMakeArithmeticProgression(int *arr, int arrSize)
{
    bool *used = calloc((size_t)arrSize, sizeof *used);
    bool ok = place(arr, arrSize, used, 0, 0, 0);

    free(used);
    return ok;
}
