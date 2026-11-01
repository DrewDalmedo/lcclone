/* Reference solution: copy s, adding 32 (the distance from 'A' to 'a' in
 * ASCII) to each character from 'A' to 'Z'. */
char *toLowerCase(char *s)
{
    size_t n = strlen(s), i;
    char *out = malloc(n + 1);

    for (i = 0; i < n; i++)
        out[i] = s[i] >= 'A' && s[i] <= 'Z' ? (char)(s[i] - 'A' + 'a') : s[i];
    out[n] = '\0';
    return out;
}
