/* Words that share a prefix, so that the answer is seldom empty. Small
 * cases are up to five words of up to six letters from "ab". Full-size
 * ones are 200 words of up to 200 letters, in a shape the seed picks: all
 * the same word of 200 letters; words that agree up to the last letter of
 * the last word; words of 200 letters but the last, which is shorter and
 * is the answer; or a random common prefix with random tails. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 5) : 200, top = small ? 6 : 200;
    int shape = small ? 3 : (int)(gen_seed() % 4), i, k, len, common;
    const char *letters = small ? "ab" : "abcdefghijklmnopqrstuvwxyz";
    char **strs = malloc((size_t)n * sizeof *strs);
    char *first = malloc((size_t)top + 1);

    for (k = 0; k < top; k++)
        first[k] = rnd_char(letters);
    first[top] = '\0';
    common = shape == 3 ? (int)rnd(0, top) : top;
    for (i = 0; i < n; i++) {
        len = shape == 3 ? (int)rnd(common, top) : top;
        if (shape == 2 && i == n - 1)
            len = (int)rnd(0, top - 1);
        strs[i] = malloc((size_t)len + 1);
        memcpy(strs[i], first, (size_t)len);
        strs[i][len] = '\0';
        /* Past the shared part, a word's letters are its own. */
        for (k = common; k < len; k++)
            strs[i][k] = rnd_char(letters);
    }
    if (shape == 1) {
        /* Every word is first; the last differs in its last letter. */
        for (i = 0; i < n; i++)
            memcpy(strs[i], first, (size_t)top);
        do
            strs[n - 1][top - 1] = rnd_char(letters);
        while (strs[n - 1][top - 1] == first[top - 1]);
    }
    put_strings(strs, n);
    for (i = 0; i < n; i++)
        free(strs[i]);
    free(strs);
    free(first);
}
