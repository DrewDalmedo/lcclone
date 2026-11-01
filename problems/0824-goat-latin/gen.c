/* Small cases: 1 to 5 words of 1 to 4 letters, from a set that holds
 * vowels and consonants in both cases, y and Y among them, so that every
 * kind of first letter comes up and words repeat.
 *
 * Full-size cases take a shape the seed picks:
 *   0. 75 one-letter words, or 74 and a two-letter one (the longest
 *      result, 3150 characters);
 *   1. words of 1 to 10 random letters, up to 150 characters in all;
 *   2. a single word of 150 letters;
 *   3. one short word over and over, up to 150 characters. */
static const char letters_small[] = "aeiouAEIOUyYbkZT";
static const char letters_all[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

static char sentence[151];
static int slen;

static void add_word(const char *w, int len)
{
    if (slen > 0)
        sentence[slen++] = ' ';
    memcpy(sentence + slen, w, (size_t)len);
    slen += len;
    sentence[slen] = '\0';
}

static void random_word(char *w, int len, const char *letters)
{
    int i;
    char c;

    for (i = 0; i < len; i++) {
        c = rnd_char(letters);
        w[i] = c;
    }
}

void gen(int small)
{
    char w[151];
    int shape = small ? -1 : (int)rnd(0, 3), words, len, i;

    slen = 0;
    sentence[0] = '\0';
    if (shape == -1) {
        words = (int)rnd(1, 5);
        for (i = 0; i < words; i++) {
            len = (int)rnd(1, 4);
            random_word(w, len, letters_small);
            add_word(w, len);
        }
    } else if (shape == 0) {
        words = rnd_chance(50) ? 75 : 74;
        for (i = 0; i < words; i++) {
            random_word(w, 1, rnd_chance(50) ? letters_small : letters_all);
            add_word(w, 1);
        }
        if (words == 74) {
            random_word(w, 2, letters_all);
            add_word(w, 2);
        }
    } else if (shape == 1 || shape == 3) {
        len = (int)rnd(1, shape == 1 ? 10 : 4);
        random_word(w, len, letters_all);
        while (slen + (slen > 0) + len <= 150) {
            add_word(w, len);
            if (shape == 1) {
                len = (int)rnd(1, 10);
                random_word(w, len, letters_all);
            }
        }
    } else {
        random_word(w, 150, letters_all);
        add_word(w, 150);
    }
    put_string(sentence);
}
