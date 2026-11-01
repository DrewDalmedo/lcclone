/* Reference solution: moves only shorten a string from the end, so each
 * string can only become one of its own non-empty prefixes. Keep the
 * longest prefix the three share, of length L, and chop everything after
 * it: len1 + len2 + len3 - 3 * L moves. With L = 0 the only common string
 * would be the empty one, which is not allowed. */
int findMinimumOperations(char *s1, char *s2, char *s3)
{
    int l1 = (int)strlen(s1), l2 = (int)strlen(s2), l3 = (int)strlen(s3);
    int len = 0;

    while (len < l1 && len < l2 && len < l3 && s1[len] == s2[len] &&
           s2[len] == s3[len])
        len++;
    return len == 0 ? -1 : l1 + l2 + l3 - 3 * len;
}
