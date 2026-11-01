/* Reference solution: one pass with a table of the values seen so far,
 * seen[v + 1000] for v in [-1000, 1000]. For each x, before marking it,
 * look among the earlier values for 2x (when that is in range) and, for
 * an even x, for x / 2. Looking before marking keeps a lone 0 from
 * pairing with itself. */
bool checkIfExist(int *arr, int arrSize)
{
    bool seen[2001] = {false};
    int i, x;

    for (i = 0; i < arrSize; i++) {
        x = arr[i];
        if (2 * x >= -1000 && 2 * x <= 1000 && seen[2 * x + 1000])
            return true;
        if (x % 2 == 0 && seen[x / 2 + 1000])
            return true;
        seen[x + 1000] = true;
    }
    return false;
}
