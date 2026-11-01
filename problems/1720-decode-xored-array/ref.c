/* Reference solution: since x XOR x = 0, XORing arr[i] with encoded[i]
 * cancels arr[i] and leaves arr[i + 1]. Start from first and apply each
 * entry of encoded in turn. */
int *decode(int *encoded, int encodedSize, int first, int *returnSize)
{
    int *arr = malloc(((size_t)encodedSize + 1) * sizeof *arr), i;

    arr[0] = first;
    for (i = 0; i < encodedSize; i++)
        arr[i + 1] = arr[i] ^ encoded[i];
    *returnSize = encodedSize + 1;
    return arr;
}
