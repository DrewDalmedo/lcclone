/* Slow and obviously right, by another route: put all the letters in one
 * row and sort it. n equal strings hold every letter a multiple of n
 * times, so the sorted row must split into blocks of n letters, each
 * block one letter repeated. */
static int by_letter(const void *a, const void *b)
{
    return *(const char *)a - *(const char *)b;
}

bool makeEqual(char **words, int wordsSize)
{
    char row[10000];
    int total = 0, i;
    size_t len;
    bool ok = true;

    for (i = 0; i < wordsSize; i++) {
        len = strlen(words[i]);
        memcpy(row + total, words[i], len);
        total += (int)len;
    }
    qsort(row, (size_t)total, 1, by_letter);
    if (total % wordsSize != 0)
        return false;
    for (i = 0; i < total; i++)
        if (row[i] != row[i - i % wordsSize])
            ok = false;
    return ok;
}
