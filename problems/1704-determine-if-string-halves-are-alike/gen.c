/* A string of letters of even length. Half the seeds give the two halves
 * the same number of vowels (in random places, of random case); the others
 * leave every letter random, or make the vowel counts differ by one. y and
 * Y turn up among the consonants. Small cases are a few letters long;
 * full-size ones have 1000 letters, sometimes with the left half all
 * vowels and the right half holding exactly 244 (counts that agree modulo
 * 256), or every letter a vowel. */
static char vowel(void)
{
    return rnd_char("aeiouAEIOU");
}

static char consonant(int small)
{
    return small ? rnd_char("bkyYTZ") : rnd_char("bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ");
}

/* Fills piece[0..h) with exactly v vowels in random places. */
static void fill(char *piece, int h, int v, int small)
{
    int *pos = malloc((size_t)h * sizeof *pos), i;

    for (i = 0; i < h; i++)
        pos[i] = i;
    rnd_shuffle(pos, h);
    for (i = 0; i < h; i++)
        piece[pos[i]] = i < v ? vowel() : consonant(small);
    free(pos);
}

void gen(int small)
{
    int h = small ? (int)rnd(1, 5) : 500, shape = (int)rnd(0, 5), i, v, w;
    char *s = malloc((size_t)(2 * h + 1));

    if (shape <= 2) {
        /* equal counts */
        v = (int)rnd(0, h);
        fill(s, h, v, small);
        fill(s + h, h, v, small);
    } else if (shape == 3) {
        /* counts one apart, either way round */
        v = (int)rnd(0, h - 1);
        w = (int)rnd(0, 1);
        fill(s, h, v + w, small);
        fill(s + h, h, v + 1 - w, small);
    } else if (shape == 4 || small) {
        /* every letter random */
        for (i = 0; i < 2 * h; i++) {
            w = rnd_chance(40);
            s[i] = w ? vowel() : consonant(small);
        }
    } else {
        /* full size only: 500 against 244, or all vowels */
        w = rnd_chance(50);
        fill(s, h, h, small);
        fill(s + h, h, w ? 244 : h, small);
    }
    s[2 * h] = '\0';
    put_string(s);
    free(s);
}
