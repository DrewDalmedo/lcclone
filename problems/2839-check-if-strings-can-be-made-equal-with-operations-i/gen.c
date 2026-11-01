/* Rows of four letters from a small alphabet (a and b, or a to c, in small
 * cases; a to z at full size, where repeats are rarer). s2 is s1 after
 * random jumps, and for some seeds it is then changed: two neighbouring
 * letters exchanged, the two end letters exchanged, or one letter
 * replaced; sometimes s2 is a random row. */
void gen(int small)
{
    const char *letters = small ? (rnd_chance(50) ? "ab" : "abc")
                                : "abcdefghijklmnopqrstuvwxyz";
    int shape = (int)rnd(0, 5), i, at;
    char s1[5], s2[5], c;

    for (i = 0; i < 4; i++)
        s1[i] = rnd_char(letters);
    s1[4] = '\0';
    memcpy(s2, s1, 5);
    if (rnd_chance(50)) {
        c = s2[0];
        s2[0] = s2[2];
        s2[2] = c;
    }
    if (rnd_chance(50)) {
        c = s2[1];
        s2[1] = s2[3];
        s2[3] = c;
    }
    if (shape == 1) {                   /* neighbours exchanged */
        at = (int)rnd(0, 2);
        c = s2[at];
        s2[at] = s2[at + 1];
        s2[at + 1] = c;
    } else if (shape == 2) {            /* the end letters exchanged */
        c = s2[0];
        s2[0] = s2[3];
        s2[3] = c;
    } else if (shape == 3) {            /* one letter replaced */
        at = (int)rnd(0, 3);
        do
            c = rnd_char(letters);
        while (c == s2[at]);
        s2[at] = c;
    } else if (shape == 4) {            /* anything */
        for (i = 0; i < 4; i++)
            s2[i] = rnd_char(letters);
    }
    put_string(s1);
    put_string(s2);
}
