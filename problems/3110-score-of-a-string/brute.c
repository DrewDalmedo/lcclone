/* Slow and obviously right: walk each leg one letter at a time, from the
 * letter to the next one, and count the steps taken. */
int scoreOfString(char *s)
{
    int n = (int)strlen(s), steps = 0, i;
    char at;

    for (i = 0; i + 1 < n; i++) {
        at = s[i];
        while (at != s[i + 1]) {
            if (at < s[i + 1])
                at++;
            else
                at--;
            steps++;
        }
    }
    return steps;
}
