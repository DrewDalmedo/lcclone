/* Reference solution: count the 1 bits of each number by clearing the
 * lowest one until none is left, then apply the rule. */
static int ones(int x)
{
    int count = 0;

    while (x != 0) {
        x &= x - 1;
        count++;
    }
    return count;
}

int calculateScore(int a, int b)
{
    int p = ones(a), q = ones(b);

    return p >= q ? p * q : q / p;
}
