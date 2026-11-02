/* Reference solution: one pass over s, failing at the first pair of
 * neighbouring digits whose difference is outside -2..2. */
bool isAdjacentDiffAtMostTwo(char *s)
{
    int i, d;

    for (i = 0; s[i + 1] != '\0'; i++) {
        d = s[i + 1] - s[i];
        if (d > 2 || d < -2)
            return false;
    }
    return true;
}
