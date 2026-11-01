/* Reference solution: xor every character of s and t together. Each letter
 * of s cancels its copy in t, so only the extra letter is left. */
char findTheDifference(char *s, char *t)
{
    unsigned char x = 0;
    size_t i;

    for (i = 0; s[i]; i++)
        x ^= (unsigned char)s[i];
    for (i = 0; t[i]; i++)
        x ^= (unsigned char)t[i];
    return (char)x;
}
