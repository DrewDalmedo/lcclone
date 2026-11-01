/* Slow and obviously right: erase each position in turn, count the letters
 * of what is left from scratch, and test whether every letter present has
 * the same count. */
bool equalFrequency(char *word)
{
    int n = (int)strlen(word), skip, i, count[26], want;
    bool level;

    for (skip = 0; skip < n; skip++) {
        memset(count, 0, sizeof count);
        for (i = 0; i < n; i++)
            if (i != skip)
                count[word[i] - 'a']++;
        want = 0;
        level = true;
        for (i = 0; i < 26; i++) {
            if (count[i] == 0)
                continue;
            if (want == 0)
                want = count[i];
            else if (count[i] != want)
                level = false;
        }
        if (level)
            return true;
    }
    return false;
}
