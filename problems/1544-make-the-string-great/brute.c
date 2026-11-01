/* Slow and obviously right: find the leftmost clashing pair, erase it, and
 * search again from the start, until no pair clashes. */
static int clash(char a, char b)
{
    return a != b && tolower((unsigned char)a) == tolower((unsigned char)b);
}

char *makeGood(char *s)
{
    size_t len = strlen(s), i;
    char *out = malloc(len + 1);
    int found = 1;

    memcpy(out, s, len + 1);
    while (found) {
        found = 0;
        for (i = 0; i + 1 < len; i++)
            if (clash(out[i], out[i + 1])) {
                memmove(out + i, out + i + 2, len - i - 1);
                len -= 2;
                found = 1;
                break;
            }
    }
    return out;
}
