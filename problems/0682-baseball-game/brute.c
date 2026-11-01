/* Slow and obviously right: write down every number ever printed and
 * never erase one; "C" only crosses the bottom line out. To find the
 * bottom lines, walk up past the crossed-out ones. Parse integers digit
 * by digit and add up in 64 bits. */
static int bottom(const int *gone, int printed, int skip)
{
    int i = printed - 1;

    for (;; i--)
        if (!gone[i] && skip-- == 0)
            return i;
}

int calPoints(char **operations, int operationsSize)
{
    long long value[1000], total = 0, v;
    int gone[1000];
    int printed = 0, i, k, neg;

    for (i = 0; i < operationsSize; i++) {
        const char *op = operations[i];

        if (op[0] == '+' && op[1] == '\0') {
            value[printed] = value[bottom(gone, printed, 0)] +
                             value[bottom(gone, printed, 1)];
        } else if (op[0] == 'D' && op[1] == '\0') {
            value[printed] = 2 * value[bottom(gone, printed, 0)];
        } else if (op[0] == 'C' && op[1] == '\0') {
            gone[bottom(gone, printed, 0)] = 1;
            continue;
        } else {
            neg = op[0] == '-';
            v = 0;
            for (k = neg; op[k]; k++)
                v = 10 * v + (op[k] - '0');
            value[printed] = neg ? -v : v;
        }
        gone[printed++] = 0;
    }
    for (i = 0; i < printed; i++)
        if (!gone[i])
            total += value[i];
    return (int)total;
}
