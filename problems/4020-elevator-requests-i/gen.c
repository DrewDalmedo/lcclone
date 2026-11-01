/* A building and a list of requests. Small cases have a few floors and a
 * few requests, so that repeats, floor 0 and back-and-forth trips come up
 * often. Full-size ones have 100 floors and 100 requests; the seed picks
 * random floors, a zigzag between floors near the top and floors near the
 * bottom (answers close to the largest), or long stretches of repeated
 * floors. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 10) : 100;
    int len = small ? (int)rnd(1, 8) : 100;
    int shape = small ? 0 : (int)rnd(0, 2);
    int *req = malloc((size_t)len * sizeof *req);
    int i, f;

    for (i = 0; i < len; i++) {
        if (shape == 1) {
            f = (int)rnd(0, 4);
            if (i % 2 == 0)
                f = n - 1 - f;
        } else if (shape == 2 && i > 0 && rnd_chance(70)) {
            f = req[i - 1];
        } else {
            f = (int)rnd(0, n - 1);
        }
        req[i] = f;
    }
    put_int(n);
    put_ints(req, len);
    free(req);
}
