/* Slow and obviously right: write the word out in each of the three
 * patterns (all small, all capitals, first letter only) and see whether
 * one of them is the word itself. */
bool detectCapitalUse(char *word)
{
    size_t n = strlen(word), i;
    char *form = malloc(n + 1);
    int pattern;
    bool fits = false;

    for (pattern = 0; pattern < 3 && !fits; pattern++) {
        for (i = 0; i < n; i++) {
            int capital = pattern == 1 || (pattern == 2 && i == 0);

            form[i] = (char)(capital ? toupper((unsigned char)word[i])
                                     : tolower((unsigned char)word[i]));
        }
        form[n] = '\0';
        fits = strcmp(form, word) == 0;
    }
    free(form);
    return fits;
}
