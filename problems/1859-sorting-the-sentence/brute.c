/* Slow and obviously right: for each digit from 1 to 9 in turn, search
 * the whole of s for the word that ends in that digit, and if there is
 * one, append its letters (after a space, unless it is the first word). */
char *sortSentence(char *s)
{
    char *out = malloc(strlen(s) + 1);
    int n = 0, i, j, end, digit;

    for (digit = '1'; digit <= '9'; digit++)
        for (i = 0; s[i]; i = s[end] ? end + 1 : end) {
            for (end = i; s[end] && s[end] != ' '; end++)
                ;
            if (s[end - 1] != digit)
                continue;
            if (n > 0)
                out[n++] = ' ';
            for (j = i; j < end - 1; j++)
                out[n++] = s[j];
        }
    out[n] = '\0';
    return out;
}
