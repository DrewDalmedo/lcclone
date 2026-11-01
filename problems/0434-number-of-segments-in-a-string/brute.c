/* Slow and obviously right: cut s at every single space into pieces
 * (empty pieces included), and count the pieces that are not empty. */
int countSegments(char *s)
{
    int n = (int)strlen(s), count = 0, start = 0, end;

    for (end = 0; end <= n; end++)
        if (end == n || s[end] == ' ') {
            /* the piece s[start..end) */
            if (end > start)
                count++;
            start = end + 1;
        }
    return count;
}
