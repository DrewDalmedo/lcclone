/* Reference solution: walk back from the end of s past the trailing
 * spaces, then count letters until a space or the start of s. */
int lengthOfLastWord(char *s)
{
    size_t end = strlen(s);
    int len = 0;

    while (end > 0 && s[end - 1] == ' ')
        end--;
    while (end > 0 && s[end - 1] != ' ') {
        end--;
        len++;
    }
    return len;
}
