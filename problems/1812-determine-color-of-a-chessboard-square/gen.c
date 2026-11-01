/* One random square of the board; there are only 64, so small and
 * full-size cases are alike. */
void gen(int small)
{
    char sq[3];
    int file = (int)rnd(0, 7);
    int rank = (int)rnd(0, 7);

    (void)small;
    sq[0] = (char)('a' + file);
    sq[1] = (char)('1' + rank);
    sq[2] = '\0';
    put_string(sq);
}
