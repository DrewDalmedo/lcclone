/* Slow and obviously right: for each letter of the alphabet, find where it
 * occurs for the second time, and keep the letter for which that position
 * is smallest. */
char repeatedCharacter(char *s)
{
    int n = (int)strlen(s), best_pos = n, seen, i;
    char c, best = '\0';

    for (c = 'a'; c <= 'z'; c++) {
        seen = 0;
        for (i = 0; i < n; i++)
            if (s[i] == c && ++seen == 2) {
                if (i < best_pos) {
                    best_pos = i;
                    best = c;
                }
                break;
            }
    }
    return best;
}
