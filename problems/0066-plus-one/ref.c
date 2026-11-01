/* Reference solution: add 1 at the last digit and carry leftwards. Each 9
 * the carry reaches becomes 0; the first other digit takes the carry and
 * stops it. If the carry runs past the first digit, every digit was 9 and
 * the answer is 1 followed by digitsSize zeros. */
int *plusOne(int *digits, int digitsSize, int *returnSize)
{
    int *out;
    int i;

    for (i = digitsSize - 1; i >= 0 && digits[i] == 9; i--)
        ;
    if (i < 0) {
        out = calloc((size_t)digitsSize + 1, sizeof *out);
        out[0] = 1;
        *returnSize = digitsSize + 1;
        return out;
    }
    out = malloc((size_t)digitsSize * sizeof *out);
    memcpy(out, digits, (size_t)i * sizeof *out);
    out[i] = digits[i] + 1;
    for (i++; i < digitsSize; i++)
        out[i] = 0;
    *returnSize = digitsSize;
    return out;
}
