/* Reference solution: walk along s, matching the words against it in
 * order. A word that runs past the end of s, or differs from it, means
 * false; once s is used up exactly at the end of a word, the answer is
 * true. Running out of words first means false. */
bool isPrefixString(char *s, char **words, int wordsSize)
{
    size_t n = strlen(s), pos = 0, len;
    int i;

    for (i = 0; i < wordsSize && pos < n; i++) {
        len = strlen(words[i]);
        if (len > n - pos || memcmp(s + pos, words[i], len) != 0)
            return false;
        pos += len;
    }
    return pos == n;
}
