/* Reference solution: k equal groups of m in a row means a run of
 * m * (k - 1) consecutive positions j with arr[j] == arr[j + m]. Count the
 * current run of such positions, starting over at every mismatch. */
bool containsPattern(int *arr, int arrSize, int m, int k)
{
    int need = m * (k - 1), run = 0, j;

    for (j = 0; j + m < arrSize; j++) {
        if (arr[j] == arr[j + m]) {
            if (++run >= need)
                return true;
        } else {
            run = 0;
        }
    }
    return false;
}
