/* Reference solution: a segment starts at every character that is not a
 * space and has a space, or the start of s, just before it. Count those
 * starts in one pass. */
int countSegments(char *s)
{
    int count = 0, i;

    for (i = 0; s[i] != '\0'; i++)
        if (s[i] != ' ' && (i == 0 || s[i - 1] == ' '))
            count++;
    return count;
}
