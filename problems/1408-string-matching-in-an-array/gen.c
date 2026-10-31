/* Distinct random words over a small or a full alphabet. About half are
 * made from an earlier word, either a piece of it (so it hides there) or
 * the word with letters added around it (so the earlier one hides in it);
 * the words are shuffled at the end, so a word and the one it hides in
 * come in either order. */
static const char *letters;

static void random_word(char *w, int len)
{
    int i;

    for (i = 0; i < len; i++)
        w[i] = rnd_char(letters);
    w[len] = '\0';
}

void gen(int small)
{
    static const char *alphabets[] = {"ab", "abc",
                                      "abcdefghijklmnopqrstuvwxyz"};
    static char words[100][31];
    char cand[31], *ptrs[100];
    int n = small ? (int)rnd(1, 6) : 100, maxlen = small ? 5 : 30;
    int order[100], k, i, dup, slen, start, len, pre, post;
    const char *src;

    letters = alphabets[small ? rnd(0, 1) : rnd(0, 2)];
    for (k = 0; k < n; k++) {
        do {
            src = k > 0 ? words[rnd(0, k - 1)] : NULL;
            slen = src ? (int)strlen(src) : 0;
            if (src && rnd_chance(25)) {
                /* a piece of an earlier word */
                start = (int)rnd(0, slen - 1);
                len = (int)rnd(1, slen - start);
                memcpy(cand, src + start, (size_t)len);
                cand[len] = '\0';
            } else if (src && slen < maxlen && rnd_chance(33)) {
                /* an earlier word with letters around it */
                pre = (int)rnd(0, maxlen - slen);
                post = (int)rnd(pre ? 0 : 1, maxlen - slen - pre);
                random_word(cand, pre);
                memcpy(cand + pre, src, (size_t)slen);
                random_word(cand + pre + slen, post);
            } else {
                random_word(cand, (int)rnd(1, maxlen));
            }
            dup = 0;
            for (i = 0; i < k && !dup; i++)
                dup = strcmp(words[i], cand) == 0;
        } while (dup);
        strcpy(words[k], cand);
    }
    for (k = 0; k < n; k++)
        order[k] = k;
    rnd_shuffle(order, n);
    for (k = 0; k < n; k++)
        ptrs[k] = words[order[k]];
    put_strings(ptrs, n);
}
