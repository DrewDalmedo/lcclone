/* Slow and obviously right: try every ordered choice of three different
 * positions of digits, mark the number the three cards read when it
 * doesn't start with 0 and ends in an even digit, then list the marked
 * numbers from smallest to largest. */
int *findEvenNumbers(int *digits, int digitsSize, int *returnSize)
{
    char seen[1000] = {0};
    int i, j, k, v, m = 0;
    int *out = malloc(1000 * sizeof *out);

    for (i = 0; i < digitsSize; i++)
        for (j = 0; j < digitsSize; j++)
            for (k = 0; k < digitsSize; k++) {
                if (i == j || i == k || j == k)
                    continue;
                if (digits[i] == 0 || digits[k] % 2 != 0)
                    continue;
                seen[digits[i] * 100 + digits[j] * 10 + digits[k]] = 1;
            }
    for (v = 0; v < 1000; v++)
        if (seen[v])
            out[m++] = v;
    *returnSize = m;
    return out;
}
