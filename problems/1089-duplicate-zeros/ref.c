/* Reference solution: two passes, O(1) extra memory. The first pass counts
 * zeros to find the last original element that still fits, and whether
 * that element, a zero, fits only once. The second pass writes the kept
 * elements from the back, each zero twice, so that nothing is overwritten
 * before it has been read. */
void duplicateZeros(int *arr, int arrSize)
{
    int len = 0, i = 0, w;

    /* After the loop, arr[0..i) are the kept elements, and len is how many
     * places they would take: arrSize, or arrSize + 1 when the last kept
     * element is a zero whose second copy falls off the end. */
    while (len < arrSize) {
        len += arr[i] == 0 ? 2 : 1;
        i++;
    }
    w = arrSize - 1;
    if (len > arrSize) {
        /* arr[i - 1] is a zero; only its first copy fits. */
        arr[w--] = 0;
        i--;
    }
    while (i > 0) {
        i--;
        if (arr[i] == 0)
            arr[w--] = 0;
        arr[w--] = arr[i];
    }
}
