/* Reference solution: for each entry from left to right, test its first
 * and its last letter. O(right - left) letter tests, plus the strlen
 * calls. */
static int is_vowel(char c)
{
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int vowelStrings(char **words, int wordsSize, int left, int right)
{
    int count = 0, i;
    size_t len;

    (void)wordsSize;
    for (i = left; i <= right; i++) {
        len = strlen(words[i]);
        if (is_vowel(words[i][0]) && is_vowel(words[i][len - 1]))
            count++;
    }
    return count;
}
