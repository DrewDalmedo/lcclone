/* A script drawn from a mix of the three characters that the seed picks:
 * some lean left or right, some have no blanks or only blanks. For some
 * seeds the script runs one way first and then further back the other way,
 * so that the token strays far before it finishes. Small cases have up to
 * 8 steps; full-size ones 50, or a few less. */
void gen(int small)
{
    static const char *mix[] = {"LR_", "LLR_", "LRR_", "LR", "_",
                                "LLLR_", "LRRR_", "LR__", "L", "R_"};
    int n = small ? (int)rnd(1, 8) : (gen_seed() % 3 == 0 ? (int)rnd(45, 50) : 50);
    int shape = (int)rnd(0, 4), out, i;
    const char *letters = mix[rnd(0, 9)];
    char s[51], first, second;

    if (shape == 0 && n >= 3) {         /* out one way, back further */
        first = rnd_chance(50) ? 'L' : 'R';
        second = first == 'L' ? 'R' : 'L';
        out = (int)rnd(1, (n - 1) / 2);
        for (i = 0; i < n; i++)
            s[i] = i < out ? first : second;
        if (rnd_chance(50))
            s[rnd(0, n - 1)] = '_';
    } else {
        for (i = 0; i < n; i++)
            s[i] = rnd_char(letters);
    }
    s[n] = '\0';
    put_string(s);
}
