/* Words over a few letters, some of them copies of earlier words and some
 * respellings: other words with the same signal as an earlier word, made
 * by cutting its signal into codes at random. Small cases have up to 8
 * words of up to 4 letters, often over letters with short codes (such as
 * e, t, a, i, m and n), so that signals collide often. Full-size cases
 * have 100 words; the seed picks random words of 4 to 12 letters over the
 * whole alphabet with no copies (so nearly always 100 different signals),
 * a mix of random words of any length, copies and respellings, or 12-letter
 * words of letters with 4-symbol codes (the longest signals, 48 symbols). */
static const char *const code[26] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

static void signal_of(const char *w, char *out)
{
    out[0] = '\0';
    for (; *w; w++)
        strcat(out, code[*w - 'a']);
}

/* Writes to w a word whose signal is sig: at each step, the letter with
 * the longest code that starts the rest of sig, or half the time any
 * letter whose code does. Returns 0 if that takes more than 12 letters. */
static int respell(const char *sig, char *w)
{
    int opts[26], no, n = 0, i, longest;

    while (*sig) {
        if (n == 12)
            return 0;
        no = 0;
        longest = -1;
        for (i = 0; i < 26; i++)
            if (strncmp(sig, code[i], strlen(code[i])) == 0) {
                opts[no++] = i;
                if (longest < 0 || strlen(code[i]) > strlen(code[longest]))
                    longest = i;
            }
        i = rnd_chance(50) ? longest : opts[rnd(0, no - 1)];
        w[n++] = (char)('a' + i);
        sig += strlen(code[i]);
    }
    w[n] = '\0';
    return 1;
}

void gen(int small)
{
    static char words[100][13];
    static const char *const sets[] = {"etaimn", "eishtmo", "etaimnsdkgruwo"};
    char *ptrs[100], letters[27], sig[49];
    int idx[26], n, na, shape, i, k, len, r, from, copies, respells;

    shape = small ? (int)rnd(0, 3) : (int)rnd(0, 2);
    if (small && shape < 3) {
        strcpy(letters, sets[shape]);
    } else if (small) {
        na = (int)rnd(2, 5);
        for (i = 0; i < 26; i++)
            idx[i] = i;
        rnd_shuffle(idx, 26);
        for (i = 0; i < na; i++)
            letters[i] = (char)('a' + idx[i]);
        letters[na] = '\0';
    } else if (shape == 2) {
        strcpy(letters, "bcfhjlpqvxyz");
    } else {
        strcpy(letters, "abcdefghijklmnopqrstuvwxyz");
    }

    /* Out of 10: r < copies copies an earlier word, and copies <= r <
     * respells respells one. */
    copies = small || shape > 0 ? 2 : 0;
    respells = small ? 5 : shape == 1 ? 7 : copies;
    n = small ? (int)rnd(1, 8) : 100;
    for (k = 0; k < n; k++) {
        ptrs[k] = words[k];
        r = (int)rnd(0, 9);
        if (k > 0 && r < copies) {
            from = (int)rnd(0, k - 1);
            strcpy(words[k], words[from]);
            continue;
        }
        if (k > 0 && r < respells) {
            from = (int)rnd(0, k - 1);
            signal_of(words[from], sig);
            if (respell(sig, words[k]))
                continue;
        }
        if (small)
            len = (int)rnd(1, 4);
        else
            len = shape == 2 ? 12 : (int)rnd(shape == 0 ? 4 : 1, 12);
        for (i = 0; i < len; i++)
            words[k][i] = rnd_char(letters);
        words[k][len] = '\0';
    }
    put_strings(ptrs, n);
}
