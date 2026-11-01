/* Small cases: up to 6 letters from a tiny alphabet, each given a word
 * from a tiny vocabulary (one-to-one three times in four, else freely),
 * where some words are single letters that also occur in patterns and
 * others are prefixes of each other. The sentence follows the letters,
 * and then the seed may change one word, drop the last word, add one or
 * two words, or make every word random.
 *
 * Full-size cases take a shape the seed picks:
 *   0. 300 letters, all 26 of them used, each with a 9-letter word, the 26
 *      words alike but for their last letter: a match, 2999 characters;
 *   1. the same, but the last word breaks it: the word of another letter,
 *      or a new word under a letter seen before;
 *   2. the same with one word too few, or one letter too few;
 *   3. a single word of 3000 letters for a pattern of one letter, or two
 *      equal words of 1499 letters;
 *   4. 1500 one-letter words, all the same, for a pattern of 1 to 300
 *      copies of one letter;
 *   5. 300 letters from a random alphabet, with words of 1 to 9 letters,
 *      half the time with one word changed at a position from 256 on
 *      (which nearly always breaks the match). */
static const char *const alphabets[] = {"a", "ab", "abc", "xy", "xyz"};
static const char *const vocab[] = {
    "a", "b", "c", "x", "y", "z", "ab", "ba", "abb",
};
#define NVOCAB 9

static char sentence[3001];
static int slen;

static void add_word(const char *w, int len)
{
    if (slen > 0)
        sentence[slen++] = ' ';
    memcpy(sentence + slen, w, (size_t)len);
    slen += len;
    sentence[slen] = '\0';
}

static void small_case(void)
{
    const char *letters = alphabets[rnd(0, 4)];
    int m = (int)rnd(1, 6), k, i, c, v, pos, extra;
    int injective = rnd_chance(75), shape = (int)rnd(0, 9);
    int map[26], used[NVOCAB] = {0}, w[8];
    char pattern[7];

    for (i = 0; i < 26; i++)
        map[i] = -1;
    for (i = 0; i < m; i++) {
        pattern[i] = rnd_char(letters);
        c = pattern[i] - 'a';
        if (map[c] < 0) {
            do
                v = (int)rnd(0, NVOCAB - 1);
            while (injective && used[v]);
            map[c] = v;
            used[v] = 1;
        }
        w[i] = map[c];
    }
    pattern[m] = '\0';
    k = m;
    if (shape == 6) {
        pos = (int)rnd(0, m - 1);
        w[pos] = (int)rnd(0, NVOCAB - 1);
    } else if (shape == 7 && m > 1) {
        k = m - 1;
    } else if (shape == 7 || shape == 8) {
        extra = (int)rnd(1, 2);
        for (i = 0; i < extra; i++) {
            pos = (int)rnd(0, k - 1);
            w[k] = rnd_chance(50) ? w[pos] : (int)rnd(0, NVOCAB - 1);
            k++;
        }
    } else if (shape == 9) {
        for (i = 0; i < k; i++)
            w[i] = (int)rnd(0, NVOCAB - 1);
    }
    slen = 0;
    for (i = 0; i < k; i++)
        add_word(vocab[w[i]], (int)strlen(vocab[w[i]]));
    put_string(pattern);
    put_string(sentence);
}

/* 300 letters using all 26, each with a word of 9 letters: a shared
 * random prefix of 8 and a last letter of its own. */
static void alike_case(char *pattern, char words[26][10])
{
    int idx[300], last[26], i, c;
    char prefix[8];

    for (i = 0; i < 8; i++)
        prefix[i] = rnd_char("abcdefghijklmnopqrstuvwxyz");
    for (c = 0; c < 26; c++)
        last[c] = c;
    rnd_shuffle(last, 26);
    for (c = 0; c < 26; c++) {
        memcpy(words[c], prefix, 8);
        words[c][8] = (char)('a' + last[c]);
        words[c][9] = '\0';
    }
    for (i = 0; i < 300; i++)
        idx[i] = i < 26 ? i : (int)rnd(0, 25);
    rnd_shuffle(idx, 300);
    for (i = 0; i < 300; i++)
        pattern[i] = (char)('a' + idx[i]);
    pattern[300] = '\0';
}

static void full_case(int shape)
{
    static char big[3001];
    char pattern[301], words[26][10];
    int i, c, m, n, pos, len, cut;

    slen = 0;
    sentence[0] = '\0';
    if (shape <= 2) {
        alike_case(pattern, words);
        n = 300;
        cut = 0;
        if (shape == 1) {
            /* The last letter is pattern[0] again; its word breaks. */
            pattern[299] = pattern[0];
        } else if (shape == 2) {
            if (rnd_chance(50))
                n = 299;
            else
                cut = 1;
        }
        for (i = 0; i < n; i++) {
            c = pattern[i] - 'a';
            if (shape == 1 && i == 299) {
                if (rnd_chance(50)) {
                    /* Another letter's word. */
                    c = (int)rnd(0, 24);
                    if (c >= pattern[0] - 'a')
                        c++;
                    add_word(words[c], 9);
                } else {
                    /* A new word: a different first letter. */
                    memcpy(big, words[c], 10);
                    big[0] = big[0] == 'a' ? 'b' : 'a';
                    add_word(big, 9);
                }
            } else {
                add_word(words[c], 9);
            }
        }
        if (cut)
            pattern[299] = '\0';
    } else if (shape == 3) {
        if (rnd_chance(50)) {
            for (i = 0; i < 3000; i++)
                big[i] = rnd_char("ab");
            add_word(big, 3000);
            pattern[0] = rnd_char("abcdefghijklmnopqrstuvwxyz");
            pattern[1] = '\0';
        } else {
            for (i = 0; i < 1499; i++)
                big[i] = rnd_char("ab");
            add_word(big, 1499);
            add_word(big, 1499);
            pattern[0] = rnd_char("abcdefghijklmnopqrstuvwxyz");
            pattern[1] = '\0';
        }
    } else if (shape == 4) {
        big[0] = rnd_char("abcdefghijklmnopqrstuvwxyz");
        for (i = 0; i < 1500; i++)
            add_word(big, 1);
        m = rnd_chance(50) ? 300 : (int)rnd(1, 299);
        c = rnd_char("abcdefghijklmnopqrstuvwxyz");
        for (i = 0; i < m; i++)
            pattern[i] = (char)c;
        pattern[m] = '\0';
    } else {
        /* Word for letter c: 0 to 8 random letters, then 'a' + c, so the
         * words of different letters differ in their last letter. */
        int alpha = (int)rnd(1, 26), broken = rnd_chance(50);

        for (c = 0; c < 26; c++) {
            len = (int)rnd(0, 8);
            for (i = 0; i < len; i++)
                words[c][i] = rnd_char("abc");
            words[c][len] = (char)('a' + c);
            words[c][len + 1] = '\0';
        }
        for (i = 0; i < 300; i++)
            pattern[i] = (char)('a' + rnd(0, alpha - 1));
        pattern[300] = '\0';
        pos = broken ? (int)rnd(256, 299) : -1;
        if (broken && rnd_chance(50)) {
            /* pattern[pos] is a letter seen before; its word changes. */
            pattern[pos] = pattern[rnd(0, 255)];
        }
        for (i = 0; i < 300; i++) {
            c = pattern[i] - 'a';
            if (i == pos) {
                /* The word of the next letter (a new letter's word may
                 * then clash with a letter seen before), or a longer
                 * version of its own word: a 'z' in front. */
                if (rnd_chance(50)) {
                    c = (c + 1) % 26;
                    add_word(words[c], (int)strlen(words[c]));
                } else {
                    big[0] = 'z';
                    strcpy(big + 1, words[c]);
                    add_word(big, (int)strlen(big));
                }
            } else {
                add_word(words[c], (int)strlen(words[c]));
            }
        }
    }
    put_string(pattern);
    put_string(sentence);
}

void gen(int small)
{
    if (small)
        small_case();
    else
        full_case((int)(gen_seed() % 6));
}
