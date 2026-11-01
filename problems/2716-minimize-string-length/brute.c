/* Slow and obviously right: play the moves on a copy of s. Point at each
 * position in turn and erase the nearest copy of its letter to the right,
 * starting over after every erasure, until no move erases anything. No
 * move can erase the last copy of a letter, so the length left is the
 * smallest one reachable. */
int minimizedStringLength(char *s)
{
    char t[101];
    int n = (int)strlen(s), i, j, erased = 1;

    memcpy(t, s, (size_t)n + 1);
    while (erased) {
        erased = 0;
        for (i = 0; i < n && !erased; i++)
            for (j = i + 1; j < n; j++)
                if (t[j] == t[i]) {
                    memmove(t + j, t + j + 1, (size_t)(n - j));
                    n--;
                    erased = 1;
                    break;
                }
    }
    return n;
}
