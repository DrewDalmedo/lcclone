/* Slow and obviously right: read s from the start, measuring every word,
 * and keep the length of the most recent one. */
int lengthOfLastWord(char *s)
{
    int last = 0, cur = 0;
    size_t i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == ' ') {
            if (cur > 0)
                last = cur;
            cur = 0;
        } else {
            cur++;
        }
    }
    if (cur > 0)
        last = cur;
    return last;
}
