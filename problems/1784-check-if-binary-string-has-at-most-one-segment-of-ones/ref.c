/* Reference solution: the first run of 1s starts at s[0], so a second run
 * exists exactly when some 1 comes right after a 0, that is when "01"
 * occurs somewhere in s. */
bool checkOnesSegment(char *s)
{
    int i;

    for (i = 1; s[i] != '\0'; i++)
        if (s[i - 1] == '0' && s[i] == '1')
            return false;
    return true;
}
