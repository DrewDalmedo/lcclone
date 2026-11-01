/* A length n and a digit sum s. Small cases favour the edges: s = 0, s
 * just below, at and just above 9 * n, and small s; the others take any s
 * from 0 to 100. Full-size cases use n = 5, the limit, with the sums a
 * downward scan from 99999 reaches last or never: s = 100, s from 0 to 2,
 * s too large for five digits, or any s up to 45. */
void gen(int small)
{
    int n, s, shape;

    if (!small) {
        n = 5;
        shape = (int)rnd(0, 3);
        if (shape == 0)
            s = 100;
        else if (shape == 1)
            s = (int)rnd(0, 2);
        else if (shape == 2)
            s = (int)rnd(46, 100);
        else
            s = (int)rnd(0, 45);
        put_int(n);
        put_int(s);
        return;
    }
    n = (int)rnd(1, 5);
    shape = (int)rnd(0, 5);
    if (shape == 0)
        s = 0;
    else if (shape == 1)
        s = 9 * n + (int)rnd(-1, 1);
    else if (shape == 2)
        s = (int)rnd(1, 9);
    else if (shape == 3)
        s = 9 * (int)rnd(1, n);
    else if (shape == 4)
        s = (int)rnd(0, 9 * n + 1);
    else
        s = (int)rnd(0, 100);
    put_int(n);
    put_int(s);
}
