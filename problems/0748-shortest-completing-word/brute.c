/* Slow and obviously right: try the lengths 1, 2, ... in turn, and the
 * words of each length in order. A word completes the plate when each
 * letter of the plate can be crossed out in a copy of the word, one
 * letter of the word per letter of the plate. */
static int completes(const char *plate, const char *word)
{
    char *copy = malloc(strlen(word) + 1), *hit, c;
    int ok = 1;

    strcpy(copy, word);
    for (; *plate && ok; plate++) {
        c = *plate;
        if (c >= 'A' && c <= 'Z')
            c = (char)(c - 'A' + 'a');
        if (c < 'a' || c > 'z')
            continue;
        hit = strchr(copy, c);
        if (hit)
            *hit = '#';
        else
            ok = 0;
    }
    free(copy);
    return ok;
}

char *shortestCompletingWord(char *licensePlate, char **words, int wordsSize)
{
    size_t len;
    int i;
    char *out;

    for (len = 1;; len++)
        for (i = 0; i < wordsSize; i++)
            if (strlen(words[i]) == len && completes(licensePlate, words[i])) {
                out = malloc(len + 1);
                strcpy(out, words[i]);
                return out;
            }
}
