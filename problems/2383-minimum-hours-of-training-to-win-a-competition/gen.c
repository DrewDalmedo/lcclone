/* Small cases have 1 to 6 challengers with stats of 1 to 10. The starting
 * energy is often the total of energy[] or one more, and the starting
 * experience often level with the first challenger's, so that ties come
 * up. Full-size cases have 100 challengers, in a shape the seed picks:
 * everything at the bounds against you (the largest answer, or near it);
 * all random; a strong start against weak challengers; or a weak start
 * against challengers that give little experience, with strong ones
 * scattered among them, so that the experience falls short later on. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 6) : 100, i, total = 0, shape, ie, ix;
    int energy[100], experience[100];

    if (small) {
        for (i = 0; i < n; i++) {
            energy[i] = (int)rnd(1, 10);
            total += energy[i];
            experience[i] = (int)rnd(1, 10);
        }
        shape = (int)rnd(0, 3);
        ie = shape == 0 ? total : shape == 1 ? total + 1 : (int)rnd(1, 40);
        if (ie > 100)
            ie = 100;
        ix = rnd_chance(30) ? experience[0] : (int)rnd(1, 12);
    } else {
        shape = (int)(gen_seed() % 4);
        for (i = 0; i < n; i++) {
            if (shape == 0) {
                energy[i] = (int)rnd(95, 100);
                experience[i] = (int)rnd(95, 100);
            } else if (shape == 1) {
                energy[i] = (int)rnd(1, 100);
                experience[i] = (int)rnd(1, 100);
            } else if (shape == 2) {
                energy[i] = (int)rnd(1, 3);
                experience[i] = (int)rnd(1, 3);
            } else {
                energy[i] = (int)rnd(1, 100);
                experience[i] = rnd_chance(10) ? (int)rnd(50, 100) : 1;
            }
        }
        if (shape == 0) {
            ie = (int)rnd(1, 3);
            ix = (int)rnd(1, 3);
        } else if (shape == 2) {
            ie = (int)rnd(90, 100);
            ix = (int)rnd(90, 100);
        } else if (shape == 3) {
            ie = (int)rnd(1, 100);
            ix = (int)rnd(1, 5);
        } else {
            ie = (int)rnd(1, 100);
            ix = (int)rnd(1, 100);
        }
    }
    put_int(ie);
    put_int(ix);
    put_ints(energy, n);
    put_ints(experience, n);
}
