/* Reference solution: count each letter. Two equal tiles can always go on
 * opposite sides of the middle, so a letter that occurs c times gives c
 * rounded down to an even number; if any letter has a tile left over, one
 * such tile can stand in the middle. */
int longestPalindrome(char *s)
{
    int count[128] = {0}, len = 0, middle = 0, c;
    size_t i;

    for (i = 0; s[i] != '\0'; i++)
        count[(unsigned char)s[i]]++;
    for (c = 0; c < 128; c++) {
        len += count[c] - count[c] % 2;
        if (count[c] % 2 != 0)
            middle = 1;
    }
    return len + middle;
}
