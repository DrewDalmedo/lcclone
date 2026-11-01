/* Slow and obviously right: write out the signal of every word, looking
 * each letter up in a table written as one string, then count the signals
 * that differ from every signal before them. */
static const char table[] =
    "a.- b-... c-.-. d-.. e. f..-. g--. h.... i.. j.--- k-.- l.-.. m-- "
    "n-. o--- p.--. q--.- r.-. s... t- u..- v...- w.-- x-..- y-.-- z--.. ";

static char *signal_of(const char *word)
{
    char *out = malloc(4 * strlen(word) + 1), *o = out;
    const char *t;

    for (; *word; word++) {
        for (t = table; *t != *word; t++)
            ;
        for (t++; *t != ' '; t++)
            *o++ = *t;
    }
    *o = '\0';
    return out;
}

int uniqueMorseRepresentations(char **words, int wordsSize)
{
    char **signals = malloc((size_t)wordsSize * sizeof *signals);
    int count = 0, i, k, seen;

    for (i = 0; i < wordsSize; i++)
        signals[i] = signal_of(words[i]);
    for (i = 0; i < wordsSize; i++) {
        seen = 0;
        for (k = 0; k < i; k++)
            if (strcmp(signals[i], signals[k]) == 0)
                seen = 1;
        if (!seen)
            count++;
    }
    for (i = 0; i < wordsSize; i++)
        free(signals[i]);
    free(signals);
    return count;
}
