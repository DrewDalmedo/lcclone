/* Slow and obviously right: build one copy of the word at a time, taking
 * each of its seven letters from the first unused place of text that
 * holds it, until some letter can't be found. */
int maxNumberOfBalloons(char *text)
{
    static const char word[] = "balloon";
    size_t n = strlen(text), i;
    char *used = calloc(n + 1, 1);
    int copies = 0, k;

    for (;;) {
        for (k = 0; word[k]; k++) {
            for (i = 0; i < n; i++)
                if (!used[i] && text[i] == word[k])
                    break;
            if (i == n)
                break;
            used[i] = 1;
        }
        if (word[k])
            break;
        copies++;
    }
    free(used);
    return copies;
}
