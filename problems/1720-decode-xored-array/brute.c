/* Slow and obviously right, straight from the definition: for each next
 * value, try every number below 2^17 (every value of arr is below it) and
 * keep the one whose XOR with the value before it is encoded[i]. */
int *decode(int *encoded, int encodedSize, int first, int *returnSize)
{
    int *arr = malloc(((size_t)encodedSize + 1) * sizeof *arr), i, v;

    arr[0] = first;
    for (i = 0; i < encodedSize; i++)
        for (v = 0; v < 131072; v++)
            if ((arr[i] ^ v) == encoded[i]) {
                arr[i + 1] = v;
                break;
            }
    *returnSize = encodedSize + 1;
    return arr;
}
