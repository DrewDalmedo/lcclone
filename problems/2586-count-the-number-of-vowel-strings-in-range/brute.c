/* Slow and obviously right: go through every entry of words, skip those
 * outside [left, right], and look the first and last letters up in the
 * list of vowels one by one. */
static int in_list(char c)
{
    const char *vowels = "aeiou";
    int k;

    for (k = 0; k < 5; k++)
        if (vowels[k] == c)
            return 1;
    return 0;
}

int vowelStrings(char **words, int wordsSize, int left, int right)
{
    int count = 0, i, last;

    for (i = 0; i < wordsSize; i++) {
        if (i < left || i > right)
            continue;
        last = 0;
        while (words[i][last + 1] != '\0')
            last++;
        if (in_list(words[i][0]) && in_list(words[i][last]))
            count++;
    }
    return count;
}
