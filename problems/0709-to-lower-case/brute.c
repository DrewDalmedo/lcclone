/* Slow and obviously right: look each character up in the list of the 26
 * capitals, and when it is there, take the small letter at the same place
 * in the list of small letters. */
char *toLowerCase(char *s)
{
    static const char capitals[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const char smalls[] = "abcdefghijklmnopqrstuvwxyz";
    size_t n = strlen(s), i;
    char *out = malloc(n + 1);
    int k;

    for (i = 0; i < n; i++) {
        out[i] = s[i];
        for (k = 0; k < 26; k++)
            if (s[i] == capitals[k])
                out[i] = smalls[k];
    }
    out[n] = '\0';
    return out;
}
