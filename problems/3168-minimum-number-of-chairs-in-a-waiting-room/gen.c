/* A log that never has anyone walk out of an empty room, so it starts with
 * 'E'. While someone is inside, each event is an arrival with a chance the
 * seed picks: about half, mostly arrivals (a high peak), or mostly
 * departures (many low peaks). Full-size cases can also be all 'E',
 * strictly alternating, or a mostly-departures walk that ends in a run of
 * 5 to 20 arrivals (the peak at the very last event). Small cases have 1
 * to 10 events; full-size ones 50. */
void gen(int small)
{
    static const int pcts[] = {50, 80, 25, 0, 0, 25};
    char s[51];
    int n = small ? (int)rnd(1, 10) : 50;
    int shape = small ? (int)rnd(0, 2) : (int)rnd(0, 5);
    int tail = shape == 5 ? (int)rnd(5, 20) : 0;
    int inside = 0, arrive, i;

    for (i = 0; i < n; i++) {
        if (shape == 3)
            arrive = 1;
        else if (shape == 4)
            arrive = i % 2 == 0;
        else if (inside == 0 || i >= n - tail)
            arrive = 1;
        else
            arrive = rnd_chance(pcts[shape]);
        s[i] = arrive ? 'E' : 'L';
        inside += arrive ? 1 : -1;
    }
    s[n] = '\0';
    put_string(s);
}
