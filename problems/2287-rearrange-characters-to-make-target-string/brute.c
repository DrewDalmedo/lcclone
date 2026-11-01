/* Slow and obviously right: build spellings one at a time. For each
 * letter of target, take the first position of s not used yet that
 * holds it; stop at the first letter that can't be found, and count the
 * spellings completed. */
int rearrangeCharacters(char *s, char *target)
{
    int n = (int)strlen(s), m = (int)strlen(target), used[100] = {0};
    int copies = 0, i, j;

    for (;;) {
        for (j = 0; j < m; j++) {
            for (i = 0; i < n; i++)
                if (!used[i] && s[i] == target[j])
                    break;
            if (i == n)
                return copies;
            used[i] = 1;
        }
        copies++;
    }
}
