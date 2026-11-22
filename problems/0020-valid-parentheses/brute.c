/* Slow and obviously right: a pair of matching brackets side by side can
 * be taken out without changing the answer, so take such pairs out until
 * there are none; the string was valid if nothing is left. */
bool isValid(char *s)
{
    size_t n = strlen(s), i, k;
    char *t = malloc(n + 1);
    bool changed = true;

    memcpy(t, s, n + 1);
    while (changed) {
        changed = false;
        for (i = 0, k = 0; i < n; i++) {
            if (i + 1 < n &&
                ((t[i] == '(' && t[i + 1] == ')') ||
                 (t[i] == '[' && t[i + 1] == ']') ||
                 (t[i] == '{' && t[i + 1] == '}'))) {
                i++;
                changed = true;
            } else {
                t[k++] = t[i];
            }
        }
        n = k;
    }
    free(t);
    return n == 0;
}
