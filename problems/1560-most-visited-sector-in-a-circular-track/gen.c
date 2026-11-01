/* A ring of n rooms and a patrol of m stretches. Small cases have 2 to 6
 * rooms and 1 to 6 stretches. Full-size ones have 100 stretches. The seed
 * picks a shape: stretches of random lengths (on 100 rooms, or on a random
 * number of rooms in a full-size case); every stretch n - 1 steps long, so
 * that each one nearly goes round the ring; a patrol that ends where it
 * began; or one that ends one room before it began, so that every room
 * ties. */
static int forward(int room, int steps, int n)
{
    return (room - 1 + steps) % n + 1;
}

void gen(int small)
{
    int rounds[101];
    int n, m, shape, steps, want, i;

    if (small) {
        shape = (int)rnd(0, 5);
        n = (int)rnd(2, 6);
        m = (int)rnd(1, 6);
    } else {
        shape = 2 + (int)(gen_seed() % 4);
        n = shape == 2 ? (int)rnd(2, 100) : 100;
        m = 100;
    }
    rounds[0] = (int)rnd(1, n);
    for (i = 1; i <= m; i++) {
        steps = shape == 3 ? n - 1 : (int)rnd(1, n - 1);
        rounds[i] = forward(rounds[i - 1], steps, n);
    }
    if (shape >= 4) {
        want = shape == 4 ? rounds[0] : forward(rounds[0], n - 1, n);
        if (want != rounds[m - 1])
            rounds[m] = want;
    }
    put_int(n);
    put_ints(rounds, m + 1);
}
