/* Ten digits. Small cases draw them from one to three random digits, so
 * that repeats, steps across the 9-0 seam and steps of 5 come up often.
 * Full-size cases (the same length) pick a shape by the seed: any digits,
 * digits around 0 and around 5 (the cheapest and the dearest from 0), or
 * one digit throughout. */
void gen(int small)
{
    char s[11], pool[3];
    int shape = small ? 0 : (int)rnd(1, 3), size, near, i;

    if (shape == 0) {
        size = (int)rnd(1, 3);
        for (i = 0; i < size; i++)
            pool[i] = rnd_char("0123456789");
        for (i = 0; i < 10; i++)
            s[i] = pool[rnd(0, size - 1)];
    } else if (shape == 1) {
        for (i = 0; i < 10; i++)
            s[i] = rnd_char("0123456789");
    } else if (shape == 2) {
        for (i = 0; i < 10; i++) {
            near = rnd_chance(50);
            s[i] = rnd_char(near ? "901" : "456");
        }
    } else {
        s[0] = rnd_char("0123456789");
        for (i = 1; i < 10; i++)
            s[i] = s[0];
    }
    s[10] = '\0';
    put_string(s);
}
