/* Reference solution: two indices start at the two ends, swap their
 * characters and step towards each other until they meet. */
void reverseString(char *s, int sSize)
{
    int i = 0, j = sSize - 1;
    char t;

    while (i < j) {
        t = s[i];
        s[i++] = s[j];
        s[j--] = t;
    }
}
