/* Slow and obviously right: write out all 8n binary digits, padded to 8
 * per letter, and compare the string with its reverse digit by digit. */
bool isPalindromic(char *s)
{
    int n = (int)strlen(s), len = 8 * n, i, k;
    char *bits = malloc((size_t)len + 1);
    bool same = true;

    for (i = 0; i < n; i++)
        for (k = 0; k < 8; k++)
            bits[8 * i + k] = (((unsigned char)s[i] >> (7 - k)) & 1) ? '1' : '0';
    bits[len] = '\0';
    for (i = 0; i < len; i++)
        if (bits[i] != bits[len - 1 - i])
            same = false;
    free(bits);
    return same;
}
