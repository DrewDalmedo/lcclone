/* Reference solution: count the capitals. The word fits when there are
 * none, when every letter is one, or when there is exactly one and it is
 * the first letter. */
bool detectCapitalUse(char *word)
{
    size_t n = strlen(word), caps = 0, i;

    for (i = 0; i < n; i++)
        if (word[i] >= 'A' && word[i] <= 'Z')
            caps++;
    return caps == 0 || caps == n
        || (caps == 1 && word[0] >= 'A' && word[0] <= 'Z');
}
