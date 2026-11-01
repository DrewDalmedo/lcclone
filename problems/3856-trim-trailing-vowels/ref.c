/* Reference solution: walk back from the end of s past the vowels, stopping
 * at the start of the string, and copy what is left into a new string. */
char *trimTrailingVowels(char *s)
{
    size_t len = strlen(s);
    char *out;

    while (len > 0 && strchr("aeiou", s[len - 1]) != NULL)
        len--;
    out = malloc(len + 1);
    memcpy(out, s, len);
    out[len] = '\0';
    return out;
}
