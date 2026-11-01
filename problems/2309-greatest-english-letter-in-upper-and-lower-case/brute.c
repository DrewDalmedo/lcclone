/* Slow and obviously right, by another method: try every letter from Z
 * down to A, and look through s for its capital and for its small form. */
char *greatestLetter(char *s)
{
    char *out = malloc(2);
    char c;

    out[0] = '\0';
    out[1] = '\0';
    for (c = 'Z'; c >= 'A'; c--)
        if (strchr(s, c) && strchr(s, c - 'A' + 'a')) {
            out[0] = c;
            break;
        }
    return out;
}
