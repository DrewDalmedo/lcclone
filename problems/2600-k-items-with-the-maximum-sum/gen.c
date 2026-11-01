/* Three counts and a k. Small cases use counts from 0 to 5; full-size ones
 * counts from 0 to 50, each at 0 or 50 a quarter of the time. k is either
 * any legal value or one of the places where the answer changes course:
 * 0, numOnes, numOnes + 1, numOnes + numZeros, one more than that, or
 * every chip (kept within 0 and the number of chips). */
static int count(int small)
{
    int r;

    if (small)
        return (int)rnd(0, 5);
    r = (int)rnd(0, 7);
    return r == 0 ? 0 : r == 1 ? 50 : (int)rnd(0, 50);
}

void gen(int small)
{
    int p, z, m, total, k, pick;

    p = count(small);
    z = count(small);
    m = count(small);
    total = p + z + m;
    pick = (int)rnd(0, 6);

    if (pick == 0)
        k = 0;
    else if (pick == 1)
        k = p;
    else if (pick == 2)
        k = p + 1;
    else if (pick == 3)
        k = p + z;
    else if (pick == 4)
        k = p + z + 1;
    else if (pick == 5)
        k = total;
    else
        k = (int)rnd(0, total);
    if (k > total)
        k = total;
    put_int(p);
    put_int(z);
    put_int(m);
    put_int(k);
}
