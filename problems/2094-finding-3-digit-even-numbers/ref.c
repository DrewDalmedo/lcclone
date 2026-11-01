/* Reference solution: count how many cards show each digit, then go
 * through the even numbers 100, 102, ..., 998 in order and keep each one
 * whose three digits the counts can cover. The list comes out sorted and
 * without repeats, and has at most 450 numbers. */
int *findEvenNumbers(int *digits, int digitsSize, int *returnSize)
{
    int count[10] = {0}, need[10], i, v, m = 0;
    int *out = malloc(450 * sizeof *out);

    for (i = 0; i < digitsSize; i++)
        count[digits[i]]++;
    for (v = 100; v < 1000; v += 2) {
        int a = v / 100, b = v / 10 % 10, c = v % 10;

        memset(need, 0, sizeof need);
        need[a]++;
        need[b]++;
        need[c]++;
        if (need[a] <= count[a] && need[b] <= count[b] && need[c] <= count[c])
            out[m++] = v;
    }
    *returnSize = m;
    return out;
}
