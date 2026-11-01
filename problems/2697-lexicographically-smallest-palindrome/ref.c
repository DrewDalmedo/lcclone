/* Reference solution: each mirror pair s[i], s[n-1-i] whose letters differ
 * needs one edit, and one is enough, so with the fewest edits every pair
 * ends as one of its own two letters. Giving each such pair the earlier of
 * its two letters makes the first palindrome in dictionary order; matched
 * pairs and the middle letter stay as they are. */
char *makeSmallestPalindrome(char *s)
{
    size_t n = strlen(s), i;
    char *out = malloc(n + 1);
    char c;

    memcpy(out, s, n + 1);
    for (i = 0; i < n / 2; i++) {
        c = out[i] < out[n - 1 - i] ? out[i] : out[n - 1 - i];
        out[i] = c;
        out[n - 1 - i] = c;
    }
    return out;
}
