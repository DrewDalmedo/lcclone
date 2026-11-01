/* Reference solution: one pass that fails on any character other than a
 * letter or a digit, and notes whether a vowel and a consonant were seen. */
bool isValid(char *word)
{
    bool vowel = false, consonant = false;
    size_t len = 0;
    char c;

    for (; word[len] != '\0'; len++) {
        c = word[len];
        if (isdigit((unsigned char)c))
            continue;
        if (!isalpha((unsigned char)c))
            return false;
        if (strchr("aeiouAEIOU", c) != NULL)
            vowel = true;
        else
            consonant = true;
    }
    return len >= 3 && vowel && consonant;
}
