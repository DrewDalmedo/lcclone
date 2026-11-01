/* Slow and obviously right: count up to n with two countdowns, to the next
 * multiple of 3 and to the next multiple of 5. A number whose countdown
 * reaches zero gets "Fizz" or "Buzz" appended (both, in that order, when
 * both do), and a number that got neither is written out with its digits
 * peeled off by repeated division. */
char **fizzBuzz(int n, int *returnSize)
{
    char **out = malloc((size_t)n * sizeof *out);
    char digits[16];
    int three = 3, five = 5, i, k, len, v;

    for (i = 1; i <= n; i++) {
        char *s = calloc(16, 1);

        three--;
        five--;
        if (three == 0) {
            strcat(s, "Fizz");
            three = 3;
        }
        if (five == 0) {
            strcat(s, "Buzz");
            five = 5;
        }
        if (s[0] == '\0') {
            len = 0;
            for (v = i; v > 0; v /= 10)
                digits[len++] = (char)('0' + v % 10);
            for (k = 0; k < len; k++)
                s[k] = digits[len - 1 - k];
        }
        out[i - 1] = s;
    }
    *returnSize = n;
    return out;
}
