/* Slow and obviously right: on a copy of s, find the first digit, cut it
 * out together with the character just before it (the closest letter on
 * its left, since every earlier digit is already gone), and repeat until
 * no digit is left. */
char *clearDigits(char *s)
{
    size_t len = strlen(s), i;
    char *t = malloc(len + 1);

    memcpy(t, s, len + 1);
    for (;;) {
        for (i = 0; t[i] && !isdigit((unsigned char)t[i]); i++)
            ;
        if (!t[i])
            return t;
        memmove(t + i - 1, t + i + 1, strlen(t + i + 1) + 1);
    }
}
