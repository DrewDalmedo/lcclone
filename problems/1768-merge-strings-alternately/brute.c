/* Slow and obviously right, by another route: the result is the first
 * letter of word1 followed by word2 woven with the rest of word1, so take
 * one letter and swap the roles of the two words, again and again, until
 * the word whose turn it is has run out; then the other one follows whole. */
char *mergeAlternately(char *word1, char *word2)
{
    char *out = malloc(strlen(word1) + strlen(word2) + 1);
    const char *turn = word1, *other = word2, *swap;
    size_t k = 0;

    while (*turn) {
        out[k++] = *turn++;
        swap = turn;
        turn = other;
        other = swap;
    }
    while (*other)
        out[k++] = *other++;
    out[k] = '\0';
    return out;
}
