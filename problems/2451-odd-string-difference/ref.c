/* Reference solution: two of the first three words share the common steps.
 * If words 0 and 1 agree, their steps are the common ones and the odd word
 * is the first that disagrees with word 0; otherwise word 2 tells which of
 * the two is odd. The answer is returned as a fresh copy. */
static int same_steps(const char *a, const char *b)
{
    size_t j;

    for (j = 0; a[j + 1]; j++)
        if (a[j + 1] - a[j] != b[j + 1] - b[j])
            return 0;
    return 1;
}

char *oddString(char **words, int wordsSize)
{
    const char *odd = words[0];
    char *out;
    size_t len;
    int i;

    if (same_steps(words[0], words[1])) {
        for (i = 2; i < wordsSize; i++)
            if (!same_steps(words[0], words[i])) {
                odd = words[i];
                break;
            }
    } else if (same_steps(words[0], words[2])) {
        odd = words[1];
    }
    len = strlen(odd);
    out = malloc(len + 1);
    memcpy(out, odd, len + 1);
    return out;
}
