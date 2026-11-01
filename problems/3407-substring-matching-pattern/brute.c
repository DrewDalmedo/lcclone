/* Slow and obviously right: try every block s[i..j) of s and check that
 * the star can fill it out: the block is long enough for both parts, it
 * starts with the part before the star and ends with the part after it. */
bool hasMatch(char *s, char *p)
{
    int n = (int)strlen(s), plen = (int)strlen(p), star = 0, i, j, k, ok;
    int rlen;

    while (p[star] != '*')
        star++;
    rlen = plen - star - 1;
    for (i = 0; i <= n; i++)
        for (j = i; j <= n; j++) {
            if (j - i < star + rlen)
                continue;
            ok = 1;
            for (k = 0; k < star; k++)
                if (s[i + k] != p[k])
                    ok = 0;
            for (k = 0; k < rlen; k++)
                if (s[j - rlen + k] != p[star + 1 + k])
                    ok = 0;
            if (ok)
                return true;
        }
    return false;
}
