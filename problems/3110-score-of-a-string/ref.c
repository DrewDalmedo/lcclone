/* Reference solution: one pass over the neighbouring pairs, adding the
 * absolute difference of their character codes. */
int scoreOfString(char *s)
{
    int score = 0, i, d;

    for (i = 0; s[i] != '\0' && s[i + 1] != '\0'; i++) {
        d = s[i + 1] - s[i];
        score += d < 0 ? -d : d;
    }
    return score;
}
