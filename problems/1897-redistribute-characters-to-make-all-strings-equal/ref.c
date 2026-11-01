/* Reference solution: transfers never change how many of each letter there
 * are in all, and they can share the letters out among the strings in any
 * way and order. So the strings can be made equal exactly when every
 * letter's total count splits evenly n ways. */
bool makeEqual(char **words, int wordsSize)
{
    int count[26] = {0}, i, c;
    const char *p;

    for (i = 0; i < wordsSize; i++)
        for (p = words[i]; *p; p++)
            count[*p - 'a']++;
    for (c = 0; c < 26; c++)
        if (count[c] % wordsSize != 0)
            return false;
    return true;
}
