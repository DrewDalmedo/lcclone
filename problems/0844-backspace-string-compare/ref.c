/* Reference solution: read both logs from the right end. Backspaces seen
 * but not yet used count up, and each one skips the next letter to its
 * left. Compare the letters that survive one pair at a time; the texts
 * are equal only if both logs run out of letters together. */
static int last_kept(const char *log, int i)
{
    int skip = 0;

    for (; i >= 0; i--) {
        if (log[i] == '#')
            skip++;
        else if (skip > 0)
            skip--;
        else
            break;
    }
    return i;
}

bool backspaceCompare(char *s, char *t)
{
    int i = (int)strlen(s) - 1, j = (int)strlen(t) - 1;

    for (;;) {
        i = last_kept(s, i);
        j = last_kept(t, j);
        if (i < 0 || j < 0)
            return i < 0 && j < 0;
        if (s[i] != t[j])
            return false;
        i--;
        j--;
    }
}
