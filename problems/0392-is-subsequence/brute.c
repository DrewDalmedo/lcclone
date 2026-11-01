/* Slow and obviously right, straight from the definition: ok[i][j] says
 * whether s[i..] can be left over from t[j..]. Either s[i..] is empty, or
 * t[j] is crossed out, or t[j] is kept as s[i] when the two are equal. */
bool isSubsequence(char *s, char *t)
{
    size_t n = strlen(s), m = strlen(t), i, j, w = m + 1;
    bool *ok = malloc((n + 1) * w * sizeof *ok), result;

    for (i = n + 1; i-- > 0;)
        for (j = m + 1; j-- > 0;) {
            if (i == n)
                ok[i * w + j] = true;
            else if (j == m)
                ok[i * w + j] = false;
            else
                ok[i * w + j] = ok[i * w + j + 1] ||
                                (s[i] == t[j] && ok[(i + 1) * w + j + 1]);
        }
    result = ok[0];
    free(ok);
    return result;
}
