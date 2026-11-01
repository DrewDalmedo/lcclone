/* Reference solution: count each letter, then decide from the counts of the
 * letters present. One letter only, or every count 1, works. Otherwise the
 * erased copy must either be a letter's only copy, with all other letters
 * sharing one count, or come from the single letter whose count is one more
 * than the shared count of all the others. */
bool equalFrequency(char *word)
{
    int count[26] = {0};
    int letters = 0, lo = 0, hi = 0, at_lo = 0, at_hi = 0, i;

    for (i = 0; word[i]; i++)
        count[word[i] - 'a']++;
    for (i = 0; i < 26; i++) {
        if (count[i] == 0)
            continue;
        if (letters == 0 || count[i] < lo) {
            lo = count[i];
            at_lo = 0;
        }
        if (letters == 0 || count[i] > hi) {
            hi = count[i];
            at_hi = 0;
        }
        if (count[i] == lo)
            at_lo++;
        if (count[i] == hi)
            at_hi++;
        letters++;
    }
    if (letters == 1 || hi == 1)
        return true;
    if (lo == 1 && at_lo == 1 && at_hi == letters - 1)
        return true;
    return hi == lo + 1 && at_hi == 1;
}
