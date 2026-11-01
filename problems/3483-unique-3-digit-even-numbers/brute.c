/* Slow and obviously right: take every ordered choice of three different
 * entries, read them as a number, and mark it when it has no leading zero
 * and is even. The answer is the number of marked values. */
int totalNumbers(int *digits, int digitsSize)
{
    char seen[1000] = {0};
    int total = 0, i, j, k, x;

    for (i = 0; i < digitsSize; i++)
        for (j = 0; j < digitsSize; j++)
            for (k = 0; k < digitsSize; k++) {
                if (i == j || i == k || j == k)
                    continue;
                if (digits[i] == 0 || digits[k] % 2 != 0)
                    continue;
                x = digits[i] * 100 + digits[j] * 10 + digits[k];
                if (!seen[x]) {
                    seen[x] = 1;
                    total++;
                }
            }
    return total;
}
