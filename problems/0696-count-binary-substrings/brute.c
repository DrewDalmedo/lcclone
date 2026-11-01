/* Slow and obviously right, straight from the definition: try every
 * substring of even length, and check that its first half repeats one
 * digit, its second half repeats one digit, and the two digits differ. */
int countBinarySubstrings(char *s)
{
    int n = (int)strlen(s), count = 0, start, len, k, ok;

    for (start = 0; start < n; start++)
        for (len = 2; start + len <= n; len += 2) {
            int half = len / 2;

            ok = s[start] != s[start + half];
            for (k = 0; ok && k < half; k++)
                if (s[start + k] != s[start] ||
                    s[start + half + k] != s[start + half])
                    ok = 0;
            count += ok;
        }
    return count;
}
