/* Reference solution: find the largest power of ten that is not above n
 * (growing it only while it stays <= n / 10, so it never overflows), then
 * walk the places down to the units, keeping digit * place for each
 * non-zero digit. */
int *decimalRepresentation(int n, int *returnSize)
{
    int *out = malloc(10 * sizeof *out);
    int place = 1, m = 0, d;

    while (place <= n / 10)
        place *= 10;
    for (; place > 0; place /= 10) {
        d = n / place % 10;
        if (d != 0)
            out[m++] = d * place;
    }
    *returnSize = m;
    return out;
}
