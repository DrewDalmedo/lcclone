/* Slow and obviously right: keep only the brackets, then peel off layers.
 * Each round deletes every "()" standing side by side in the string as it
 * was at the start of the round, which removes the innermost pairs; the
 * number of rounds until nothing is left is the depth. */
int maxDepth(char *s)
{
    char cur[101], next[101];
    int len = 0, rounds = 0, i, k;

    for (i = 0; s[i] != '\0'; i++)
        if (s[i] == '(' || s[i] == ')')
            cur[len++] = s[i];
    while (len > 0) {
        k = 0;
        for (i = 0; i < len; i++) {
            if (i + 1 < len && cur[i] == '(' && cur[i + 1] == ')')
                i++;
            else
                next[k++] = cur[i];
        }
        memcpy(cur, next, (size_t)k);
        len = k;
        rounds++;
    }
    return rounds;
}
