/* Reference solution, in O(n) time: if the numbers can be lined up with a
 * fixed step, the line runs from the smallest to the largest with the step
 * d = (max - min) / (n - 1), which must divide exactly. When d = 0 every
 * number equals the smallest. Otherwise every number must sit on one of
 * the n places min, min + d, ..., max, and no place may be taken twice. */
bool canMakeArithmeticProgression(int *arr, int arrSize)
{
    int lo = arr[0], hi = arr[0], d, i, slot;
    bool *taken, ok = true;

    for (i = 1; i < arrSize; i++) {
        if (arr[i] < lo)
            lo = arr[i];
        if (arr[i] > hi)
            hi = arr[i];
    }
    if ((hi - lo) % (arrSize - 1) != 0)
        return false;
    d = (hi - lo) / (arrSize - 1);
    if (d == 0)
        return true;
    taken = calloc((size_t)arrSize, sizeof *taken);
    for (i = 0; i < arrSize && ok; i++) {
        if ((arr[i] - lo) % d != 0) {
            ok = false;
        } else {
            slot = (arr[i] - lo) / d;
            if (taken[slot])
                ok = false;
            taken[slot] = true;
        }
    }
    free(taken);
    return ok;
}
