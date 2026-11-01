/* Reference solution: if some b has an a anywhere to its right, then
 * somewhere between the two a b stands directly before an a. So it is
 * enough to look for the neighbouring pair "ba". */
bool checkString(char *s)
{
    size_t i;

    for (i = 0; s[i] != '\0' && s[i + 1] != '\0'; i++)
        if (s[i] == 'b' && s[i + 1] == 'a')
            return false;
    return true;
}
