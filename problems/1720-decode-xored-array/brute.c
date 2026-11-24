/* Obviously right, straight from the definition: XOR acts on each bit on
 * its own, so find each next value one bit at a time. For each of the 17
 * bits (every value of arr is below 2^17), try 0 and 1, and keep the one
 * whose XOR with that bit of the value before it is that bit of
 * encoded[i]. (Trying every whole number below 2^17 instead took up to
 * 0.2 s at full size, too close to a third of the time limit.) */
int *decode(int *encoded, int encodedSize, int first, int *returnSize)
{
    int *arr = malloc(((size_t)encodedSize + 1) * sizeof *arr), i, b, t;

    arr[0] = first;
    for (i = 0; i < encodedSize; i++) {
        arr[i + 1] = 0;
        for (b = 0; b < 17; b++)
            for (t = 0; t <= 1; t++)
                if ((((arr[i] >> b) & 1) ^ t) == ((encoded[i] >> b) & 1))
                    arr[i + 1] |= t << b;
    }
    *returnSize = encodedSize + 1;
    return arr;
}
