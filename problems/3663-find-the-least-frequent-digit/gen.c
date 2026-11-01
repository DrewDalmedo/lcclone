/* One n of 1 to 10 digits, drawn from a few digit values so that tallies
 * tie and 0 is often one of them. Small cases have 2 to 8 digits (now and
 * then 1) from 2 or 3 values (now and then a single one). Full-size ones
 * have 10 digits from 1 to 10 values, starting with 1 or 2; one that
 * passes 2^31 - 1 is drawn again. */
void gen(int small)
{
    int pool[10], nonzero[10], kinds, count, len, first, i, k, tmp;
    long long n;

    for (;;) {
        for (i = 0; i < 10; i++)
            pool[i] = i;
        if (small) {
            len = rnd_chance(8) ? 1 : (int)rnd(2, 8);
            kinds = rnd_chance(8) ? 1 : (int)rnd(2, 3);
        } else {
            len = 10;
            kinds = (int)rnd(1, 10);
        }
        /* The first `kinds` entries of pool become distinct digit values. */
        for (i = 0; i < kinds; i++) {
            k = (int)rnd(i, 9);
            tmp = pool[i];
            pool[i] = pool[k];
            pool[k] = tmp;
        }
        if (kinds > 1 && rnd_chance(40)) {
            for (i = 0; i < kinds && pool[i] != 0; i++)
                ;
            if (i == kinds)
                pool[kinds - 1] = 0;
        }
        if (small) {
            count = 0;
            for (i = 0; i < kinds; i++)
                if (pool[i] != 0)
                    nonzero[count++] = pool[i];
            if (count == 0)
                continue;
            k = (int)rnd(0, count - 1);
            first = nonzero[k];
        } else {
            /* Ten digits fit only below 2^31, so the first is 1 or 2. */
            first = (int)rnd(1, 2);
            for (i = 0; i < kinds && pool[i] != first; i++)
                ;
            if (i == kinds)
                pool[0] = first;
        }
        n = first;
        for (i = 1; i < len; i++) {
            k = (int)rnd(0, kinds - 1);
            n = n * 10 + pool[k];
        }
        if (n <= 2147483647LL)
            break;
    }
    put_int(n);
}
