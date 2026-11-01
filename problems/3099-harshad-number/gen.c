/* One x from 1 to 100. The range is small, so small and full-size cases
 * are alike. */
void gen(int small)
{
    (void)small;
    put_int(rnd(1, 100));
}
