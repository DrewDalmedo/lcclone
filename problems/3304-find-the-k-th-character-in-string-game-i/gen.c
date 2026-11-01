/* Small cases: k up to 64, often a power of two or next to one, where a
 * round's copy ends or begins. Full-size cases: the seed picks k next to a
 * power of two up to 256, in the copy made by the ninth round (257 to 500,
 * so the word must reach 512 letters), or anywhere up to 500. */
void gen(int small)
{
    int shape = (int)rnd(0, 2), k, m;

    if (shape == 0) {
        m = (int)rnd(0, small ? 5 : 8);
        k = 1 << m;
        k += (int)rnd(-1, 1);
        if (k < 1)
            k = 1;
    } else if (shape == 1 && !small) {
        k = (int)rnd(257, 500);
    } else {
        k = (int)rnd(1, small ? 64 : 500);
    }
    put_int(k);
}
