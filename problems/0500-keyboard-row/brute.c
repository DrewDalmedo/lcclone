/* Slow and obviously right: try each of the three rows in turn, and keep a
 * word when some row has every one of its letters, each looked up by
 * scanning the row's keys. Kept words are copied. */
char **findWords(char **words, int wordsSize, int *returnSize)
{
    static const char *rows[] = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};
    char **out = malloc((size_t)wordsSize * sizeof *out);
    int i, r, n = 0, all, found;
    size_t j, k, len;

    for (i = 0; i < wordsSize; i++) {
        len = strlen(words[i]);
        for (r = 0; r < 3; r++) {
            all = 1;
            for (j = 0; j < len && all; j++) {
                char c = words[i][j];

                if (c >= 'A' && c <= 'Z')
                    c = (char)(c - 'A' + 'a');
                found = 0;
                for (k = 0; rows[r][k] != '\0'; k++)
                    if (rows[r][k] == c)
                        found = 1;
                all = found;
            }
            if (all)
                break;
        }
        if (r < 3) {
            out[n] = malloc(len + 1);
            memcpy(out[n], words[i], len + 1);
            n++;
        }
    }
    *returnSize = n;
    return out;
}
