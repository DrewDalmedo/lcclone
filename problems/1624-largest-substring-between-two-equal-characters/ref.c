/* Reference solution: remember where each letter first appears. Every
 * later copy of it ends a gap that starts there, and the widest gap for a
 * letter is the one from its first copy to its last. */
int maxLengthBetweenEqualCharacters(char *s)
{
    int first[26], best = -1, i, c;

    for (c = 0; c < 26; c++)
        first[c] = -1;
    for (i = 0; s[i] != '\0'; i++) {
        c = s[i] - 'a';
        if (first[c] < 0)
            first[c] = i;
        else if (i - first[c] - 1 > best)
            best = i - first[c] - 1;
    }
    return best;
}
