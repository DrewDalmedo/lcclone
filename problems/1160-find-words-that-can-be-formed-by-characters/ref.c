/* Reference solution: count the letters of chars once. For each word,
 * count its letters afresh and compare letter by letter; when no letter is
 * needed more often than chars has it, add the word's length. */
int countCharacters(char **words, int wordsSize, char *chars)
{
    int have[26] = {0}, need[26], total = 0, i, c, len, fits;
    const char *p;

    for (p = chars; *p; p++)
        have[*p - 'a']++;
    for (i = 0; i < wordsSize; i++) {
        memset(need, 0, sizeof need);
        for (p = words[i]; *p; p++)
            need[*p - 'a']++;
        len = (int)(p - words[i]);
        fits = 1;
        for (c = 0; c < 26; c++)
            if (need[c] > have[c]) {
                fits = 0;
                break;
            }
        if (fits)
            total += len;
    }
    return total;
}
