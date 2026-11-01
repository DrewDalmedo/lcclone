/* Slow and obviously right: find each letter's key by looking it up in
 * both alphabets, cut the keystrokes into maximal runs on one key, and
 * answer the number of runs minus one. */
static int key_of(char c)
{
    const char *lower = "abcdefghijklmnopqrstuvwxyz";
    const char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int k;

    for (k = 0; k < 26; k++)
        if (lower[k] == c || upper[k] == c)
            return k;
    return -1;
}

int countKeyChanges(char *s)
{
    int n = (int)strlen(s), runs = 0, i = 0, j;

    while (i < n) {
        j = i;
        while (j < n && key_of(s[j]) == key_of(s[i]))
            j++;
        runs++;
        i = j;
    }
    return runs - 1;
}
