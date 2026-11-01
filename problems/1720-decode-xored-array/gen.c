/* first and encoded. Values are a mix of small numbers (so bits overlap
 * and cancel), numbers around 2^16 and numbers just below 10^5 (whose XORs
 * reach 131071), and anything from 0 to 10^5. Small cases have 2 to 8
 * values of arr; full-size ones have 10^4. Sometimes encoded is all zeros,
 * so every value equals first. */
static int value(void)
{
    int kind = (int)rnd(0, 3);

    if (kind == 0)
        return (int)rnd(0, 15);
    if (kind == 1)
        return (int)rnd(65530, 65540);
    if (kind == 2)
        return (int)rnd(98300, 100000);
    return (int)rnd(0, 100000);
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 10000, zeros = rnd_chance(15), i;
    int *encoded = malloc((size_t)(n - 1) * sizeof *encoded);
    int first;

    for (i = 0; i < n - 1; i++)
        encoded[i] = zeros ? 0 : value();
    first = value();
    put_ints(encoded, n - 1);
    put_int(first);
    free(encoded);
}
