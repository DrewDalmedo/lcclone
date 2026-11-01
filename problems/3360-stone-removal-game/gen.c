/* Any heap size for the small cases. A full-size case picks a size next to
 * one of the points where the winner changes, the easiest place to be off
 * by one. */
void gen(int small)
{
    static const int edge[] = {9, 10, 18, 19, 26, 27, 33, 34,
                               39, 40, 44, 45, 48, 49, 50};

    if (small)
        put_int(rnd(1, 50));
    else
        put_int(edge[rnd(0, 14)]);
}
