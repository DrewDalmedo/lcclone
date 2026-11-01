/* Slow and obviously right: cut a copy of the sentence into chunks with
 * strtok, and test each chunk against the rules one by one: no digit; at
 * most one hyphen, with a letter on each side; at most one closing mark,
 * and only as the last character. */
int countValidWords(char *sentence)
{
    char *copy = malloc(strlen(sentence) + 1), *tok;
    int count = 0, n, i, hyphens, marks, ok;

    strcpy(copy, sentence);
    for (tok = strtok(copy, " "); tok; tok = strtok(NULL, " ")) {
        n = (int)strlen(tok);
        hyphens = marks = 0;
        ok = 1;
        for (i = 0; i < n; i++) {
            if (isdigit((unsigned char)tok[i]))
                ok = 0;
            if (tok[i] == '-') {
                hyphens++;
                if (i == 0 || i == n - 1 || !islower((unsigned char)tok[i - 1]) ||
                    !islower((unsigned char)tok[i + 1]))
                    ok = 0;
            }
            if (strchr("!.,", tok[i])) {
                marks++;
                if (i != n - 1)
                    ok = 0;
            }
        }
        if (hyphens > 1 || marks > 1)
            ok = 0;
        count += ok;
    }
    free(copy);
    return count;
}
