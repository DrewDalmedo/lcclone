/* Small cases: 1 to 4 words of 1 to 4 letters, drawn from a few letters
 * at both ends of the alphabet, in both cases. Full-size ones are 99 or
 * 100 characters long; the seed picks words of 1 to 6 letters (the last
 * one cut to fit exactly 100), a single word of 100 letters, or 50 words
 * of one letter. */
static const char letters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void gen(int small)
{
    char t[101];
    int len = 0, words, wl, room, i, k, shape;

    if (small) {
        words = (int)rnd(1, 4);
        for (i = 0; i < words; i++) {
            wl = (int)rnd(1, 4);
            if (i > 0)
                t[len++] = ' ';
            for (k = 0; k < wl; k++)
                t[len++] = rnd_char("aAbByYzZ");
        }
    } else {
        shape = (int)rnd(0, 3);
        if (shape == 1) {
            for (k = 0; k < 100; k++)
                t[len++] = rnd_char(letters);
        } else if (shape == 2) {
            for (i = 0; i < 50; i++) {
                if (i > 0)
                    t[len++] = ' ';
                t[len++] = rnd_char(letters);
            }
        } else {
            for (;;) {
                room = 100 - len - (len > 0);
                if (room < 1)
                    break;
                wl = (int)rnd(1, 6);
                if (wl > room)
                    wl = room;
                if (len > 0)
                    t[len++] = ' ';
                for (k = 0; k < wl; k++)
                    t[len++] = rnd_char(letters);
            }
        }
    }
    t[len] = '\0';
    put_string(t);
}
